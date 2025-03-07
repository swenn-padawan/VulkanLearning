#include "app.hpp"
#include <ios>
#include <optional>
#include <vector>
#include <vulkan/vulkan_core.h>

queueFamilyIndices	findQueueFamilies(VkPhysicalDevice device)
{
	queueFamilyIndices	indices;
	uint32_t			queueFamilyCount = 0;

	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
	
	int	i = 0;
	for (const auto& queueFamily : queueFamilies){
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT){
			indices.graphicsFamily = i;
		}
		if (indices.isComplete())
			break ;
		i++;
	}


	return (indices);
}
