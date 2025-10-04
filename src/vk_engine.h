// vulkan_guide.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <vk_types.h>

//> framedata
struct FrameData {
	// Semaphore is used for GPU -> GPU sync
	// Allows the GPU to wait for a GPU operation to complete before continuing to another operation
	VkSemaphore _swapchainSemaphore, _renderSemaphore;
	// Fence is used for GPU -> CPU sync
	// CPU can wait on a fence to be signaled from the GPU
	VkFence _renderFence;
	
	// You first create a command pool, you then allocate command buffers from the pool
	// If we want to multithread command buffer recording, we need a different command pool for each thread
	VkCommandPool _commandPool;
	VkCommandBuffer _mainCommandBuffer;
};

constexpr unsigned int FRAME_OVERLAP = 2;
//< framedata

class VulkanEngine {
public:
	FrameData _frames[FRAME_OVERLAP];

	FrameData& get_current_frame() {
		return _frames[_frameNumber % FRAME_OVERLAP];
	}

	VkQueue _graphicsQueue;
	uint32_t _graphicsQueueFamily;

	bool _isInitialized{ false };
	int _frameNumber {0};
	bool stop_rendering{ false };
	VkExtent2D _windowExtent{ 1700 , 900 };

	struct SDL_Window* _window{ nullptr };

	VkInstance _instance; // Vulkan library handle
	VkDebugUtilsMessengerEXT _debug_messenger; // Debug output handle
	VkPhysicalDevice _chosenGPU; // GPU chosen as the default device
	VkDevice _device; // Vulkan device for commands
	VkSurfaceKHR _surface; // Vulkan window surface
	VkSwapchainKHR _swapchain;
	VkFormat _swapchainImageFormat;

	std::vector<VkImage> _swapchainImages;
	std::vector<VkImageView> _swapchainImageViews;
	VkExtent2D _swapchainExtent;

	static VulkanEngine& Get();

	//initializes everything in the engine
	void init();

	//shuts down the engine
	void cleanup();

	//draw loop
	void draw();

	//run main loop
	void run();

private:
	void init_vulkan();
	void init_swapchain();
	void init_commands();
	void init_sync_structures();
	void create_swapchain(uint32_t width, uint32_t height);
	void destroy_swapchain();
};
