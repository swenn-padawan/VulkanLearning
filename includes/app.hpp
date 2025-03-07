#ifndef APP_HPP
# define APP_HPP

#define GLFW_INCLUDE_VULKAN
#define WIDTH 600
#define HEIGHT 800

#include <cstdint>
#include <vulkan/vulkan_core.h>
# include <iostream>
#include <cstring>
# include <vulkan/vulkan_core.h>
# include <vector>
# include <optional>
# include <GLFW/glfw3.h>

/* * * * * * * * * * *
 *	   App Class     *
 * * * * * * * * * * */

class	App
{
	private:
		VkInstance				instance;
		VkDevice				device;
		VkPhysicalDevice		physicalDevice;
		VkQueue					graphicsQueue;
		VkSurfaceKHR			surface;
		VkSwapchainKHR			swapChain;
		std::vector<VkImage>	swapChainImages;
		VkCommandPool			commandPool;
		GLFWwindow*				window;
	public:
		void	run( void );
		void	initWindow( void );
		void	pickPhysicaldevice( void );
		void	createLogicalDevice( void );
		void	createInstance( void );
		void	initVulkan( void );
		void	mainLoop( void );
		void	cleanup( void );
};

/* * * * * * * * * * *
 * Queues Fonctions  *
 * * * * * * * * * * */

struct	queueFamilyIndices{
	std::optional<uint32_t>	graphicsFamily;
	bool isComplete( void ){
		return (graphicsFamily.has_value());
	}
};

queueFamilyIndices	findQueueFamilies(VkPhysicalDevice device);

#endif //APP_HPP
