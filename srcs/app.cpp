#include "app.hpp"
#include <stdexcept>
#include <vulkan/vulkan_core.h>

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
	const bool enabledValidationLayers = false;
#else
	const bool enabledValidationLayers = true;
#endif

bool	checkValidationLayerSupport()
{
	uint32_t	layerCount;

	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
	for (const char* layerName : validationLayers){
		bool	layerFound = false;

		for (const auto& layerProperties : availableLayers){
			if (strcmp(layerName, layerProperties.layerName) == 0){
				layerFound = true;
				break;
			}
		}
		if (!layerFound)
			return (false);
	}
	return (true);
}

std::vector<const char*> getRequiredExtensions(){
	uint32_t		glfwExtensionsCount = 0;
	const char**	glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);

	std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionsCount);

	if (enabledValidationLayers){
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
	return (extensions);
}


void	App::initWindow( void )
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "VulkanTest", NULL, NULL);
}

static bool	isDeviceSuitable(VkPhysicalDevice device){
	queueFamilyIndices indices = findQueueFamilies(device);

	return (indices.isComplete());
}

void	App::pickPhysicaldevice( void )
{
	uint32_t	deviceCount = 0;
	
	physicalDevice = VK_NULL_HANDLE;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if (deviceCount == 0){
		throw std::runtime_error("failed to find  GPUs with Vulkan support!");
	}
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	for (const auto& device : devices){
		if (isDeviceSuitable(device)){
			physicalDevice = device;
			break ;
		}
	}
	if (physicalDevice == VK_NULL_HANDLE){
		throw std::runtime_error("failed to find a suitable GPU!");
	}
}

void	App::createInstance( void )
{
	VkApplicationInfo		appInfo;
	VkInstanceCreateInfo	createInfo{};
	uint32_t				glfwExtensionCount = 0;
	const char**			glfwExtensions;

	if (enabledValidationLayers && !checkValidationLayerSupport()){
		throw std::runtime_error("validation layers requested, but not available!");
	}
	std::memset(&appInfo, 0, sizeof(VkApplicationInfo));
	std::memset(&createInfo, 0, sizeof(VkInstanceCreateInfo));
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "VulkanTest";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;
	if (vkCreateInstance(&createInfo, nullptr, &this->instance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create instance!");
	}
}

void	App::createLogicalDevice( void ){
	queueFamilyIndices	indices = findQueueFamilies(physicalDevice);

	VkDeviceQueueCreateInfo	queueCreateInfo{};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
	queueCreateInfo.queueCount = 1;

	float	queuePriority = 1.0f;
	queueCreateInfo.pQueuePriorities = &queuePriority;
	VkPhysicalDeviceFeatures	deviceFeatures{};
	VkDeviceCreateInfo		createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = &queueCreateInfo;

	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = 0;
	if (enabledValidationLayers){
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else
		createInfo.enabledLayerCount = 0;
	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS){
		throw std::runtime_error("failed to create logical device!");
	}
	VkQueue	graphicsQueue;
	vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
}

void	App::initVulkan( void )
{
	try
	{
		createInstance();
	}
	catch (std::exception &e)
	{
		std::cout << e.what();
	}
	pickPhysicaldevice();
	createLogicalDevice();
}

void	App::mainLoop( void )
{
	while (!glfwWindowShouldClose(window))
		glfwPollEvents();
}

void	App::cleanup( void )
{
	vkDestroyDevice(device, nullptr);
    if (instance == VK_NULL_HANDLE) {
        std::cerr << "ERROR: Instance is NULL before destruction!" << std::endl;
    } else {
        vkDestroyInstance(instance, nullptr);
        std::cout << "Instance destroyed successfully." << std::endl;
    }
	glfwDestroyWindow(window);
	glfwTerminate();
}

void	App::run( void )
{
	initWindow();
	initVulkan();
	mainLoop();
	cleanup();
}


