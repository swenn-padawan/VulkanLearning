#include "app.hpp"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <stdexcept>
#include <vector>
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_extension_inspection.hpp>
#include <vulkan/vulkan_core.h>
#include <cstring>

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
	return (false);
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

App::App( void )
{
	
}

App::~App( void )
{
	
}

void	App::initWindow( void )
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "VulkanTest", NULL, NULL);
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

	auto	extensions = getRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();
	if (enabledValidationLayers){
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
	}
	else
		createInfo.enabledLayerCount = 0;
	
	std::vector<const char*> requiredExtensions;

	for (uint32_t i = 0; i < glfwExtensionCount; i++) {
		requiredExtensions.emplace_back(glfwExtensions[i]);
	}
	requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

	createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

	createInfo.enabledExtensionCount = (uint32_t) requiredExtensions.size();
	createInfo.ppEnabledExtensionNames = requiredExtensions.data();

	if (vkCreateInstance(&createInfo, nullptr, &this->instance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create instance!");
	}
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
}

void	App::mainLoop( void )
{
	while (!glfwWindowShouldClose(window))
		glfwPollEvents();
}

void	App::cleanup( void )
{
	vkDestroyInstance(this->instance, nullptr);
	glfwDestroyWindow(window);
	glfwTerminate();
}

void	App::run( void )
{
	initWindow();
	mainLoop();
	cleanup();
}


