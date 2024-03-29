#pragma once

namespace Core
{
	struct SwapChainSupportDetails 
	{
		VkSurfaceCapabilitiesKHR Capabilities;
		vector<VkSurfaceFormatKHR> Formats;
		vector<VkPresentModeKHR> PresentModes;
	};

	struct QueueFamilyIndices
	{
		optional<uint32_t> GraphicsAndComputeFamily;
		optional<uint32_t> PresentFamily;

		bool IsComplete() { return GraphicsAndComputeFamily.has_value() && PresentFamily.has_value(); }
	};

	class Window;
	class CommandPool;
	class CommandBuffer;
	class Device
	{
	public:
		Device(Window& window);
		~Device();

		VkDevice GetDevice() { return _device; }
		SwapChainSupportDetails GetSwapChainSupport() 
		{ 
			return QuerySwapChainSupport(_physicalDevice); 
		}
		VkSurfaceKHR GetSurface() { return _surface; }
		QueueFamilyIndices FindQueueFamilies()
		{
			return FindQueueFamilies(_physicalDevice);
		}

		VkQueue GetGraphicsQueue() { return _graphicsQueue; }
		VkQueue GetPresentQueue() { return _presentQueue; }
		VkPhysicalDevice GetPhysicalDevice() { return _physicalDevice; }
		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		CommandBuffer& BeginSingleTimeCommands() const;
		void EndSingleTimeCommands(CommandBuffer& commandBuffer) const;

		VkFormat FindSupportedFormat(
			const vector<VkFormat>& candidates,
			VkImageTiling tiling,
			VkFormatFeatureFlags features);

		const VkInstance& GetInstance() const { return _instance; }
	private:
		void CreateInstance();
		bool CheckValidationLayerSupport();
		vector<const char*> GetRequiredExtensions();
		void SetupDebugMessenger();
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void PickPhysicalDevice();
		int RateDeviceSuitability(VkPhysicalDevice device);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
		bool IsDeviceSuitable(VkPhysicalDevice device);
		bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
		void CreateLogicalDevice();
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData)
		{
			std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
			return VK_FALSE;
		}
	private:
		VkInstance _instance;
		VkDebugUtilsMessengerEXT _debugMessenger;
		VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
		VkDevice _device;
		VkQueue _graphicsQueue;
		VkQueue _computeQueue;
		VkQueue _presentQueue;
		VkSurfaceKHR _surface;

		CommandPool* _commandPool;

		const vector<const char*> _validationLayers = 
		{
			"VK_LAYER_KHRONOS_validation"
		};

		const vector<const char*> _deviceExtensions =
		{
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

#ifdef NDEBUG
		const bool _enableValidationLayers = false;
#else
		const bool _enableValidationLayers = true;
#endif
	};
}

