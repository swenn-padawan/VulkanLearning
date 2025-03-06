#ifndef APP_HPP
# define APP_HPP

#define GLFW_INCLUDE_VULKAN
#define WIDTH 600
#define HEIGHT 800

# include <vulkan/vulkan_core.h>
# include <vector>
# include <iostream>
# include <GLFW/glfw3.h>

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
		App();
		~App();
		void	run( void );
		void	initWindow( void );
		void	createInstance( void );
		void	initVulkan( void );
		void	mainLoop( void );
		void	cleanup( void );
};

#endif //APP_HPP
