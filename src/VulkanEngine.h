#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include "implot.h"
#include "implot_internal.h"
#include <stdio.h>          // printf, fprintf
#include <stdlib.h>         // abort
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <memory>
#include "datatypes.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

//#define IMGUI_UNLIMITED_FRAME_RATE
#ifdef _DEBUG
#define IMGUI_VULKAN_DEBUG_REPORT
#endif

#ifdef IMGUI_VULKAN_DEBUG_REPORT
static VKAPI_ATTR VkBool32 VKAPI_CALL debug_report(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData)
{
	(void)flags; (void)object; (void)location; (void)messageCode; (void)pUserData; (void)pLayerPrefix; // Unused arguments
	fprintf(stderr, "[vulkan] Debug report from ObjectType: %i\nMessage: %s\n\n", objectType, pMessage);
	return VK_FALSE;
}
#endif // IMGUI_VULKAN_DEBUG_REPORT

class VulkanEngine
{
public:
	VulkanEngine();
	~VulkanEngine();

	static VkAllocationCallbacks* g_Allocator;
	static VkInstance               g_Instance;
	static VkPhysicalDevice         g_PhysicalDevice;
	static VkDevice                 g_Device;
	static uint32_t                 g_QueueFamily;
	static VkQueue                  g_Queue;
	static VkDebugReportCallbackEXT g_DebugReport;
	static VkPipelineCache          g_PipelineCache;
	static VkDescriptorPool         g_DescriptorPool;

	static ImGui_ImplVulkanH_Window g_MainWindowData;
	static int                      g_MinImageCount;
	static bool                     g_SwapChainRebuild;

	int Init();
	void RunLoop(std::shared_ptr<Args> args);
	void Cleanup(); 

private:
	GLFWwindow* window = nullptr;
	ImGui_ImplVulkanH_Window* wd = nullptr;

	static void check_vk_result(VkResult err);
	static void SetupVulkan(const char** extensions, uint32_t extensions_count);
	static void SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height);
	static void CleanupVulkan();
	static void CleanupVulkanWindow();
	static void FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data);
	static void FramePresent(ImGui_ImplVulkanH_Window* wd);
	static void glfw_error_callback(int error, const char* description);
};


