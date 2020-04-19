//
//  helper.hpp
//  VulkanProject
//
//  Created by Keegan Bilodeau on 4/15/20.
//  Copyright © 2020 Keegan Bilodeau. All rights reserved.
//
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"

#ifndef helper_hpp
#define helper_hpp

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <optional>
#include <stdio.h>
#include <vector>

const int WINDOW_WIDTH = 750;
const int WINDOW_HEIGHT = 750;

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

// Start of helper struct definitions
struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
    
    bool isComplete()
    {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};
// End of helper struct definitions

class ApplicationHelper
{
public:
    friend class GameApplication;
    friend class ApplicationComponentConstructor;
    
private:
    bool checkDeviceExtensionSupport(const VkPhysicalDevice* device) const;
    
    bool checkLayerValidationSupport() const;
    
    bool isDeviceSuitable(VkPhysicalDevice const device, VkSurfaceKHR* const surface) const;
    
    std::vector<const char*> getRequiredExtensions(const bool &enableValidationLayers) const;
    
    QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice &device, const VkSurfaceKHR &surface) const;
    
    SwapChainSupportDetails querySwapChainSupport(const VkPhysicalDevice &device, const VkSurfaceKHR &surface) const;
    
    VkPhysicalDevice pickPhysicalDevice(VkInstance* const instance, VkSurfaceKHR* const surface) const;
    
    VkExtent2D chooseSwapSurfaceExtent(const VkSurfaceCapabilitiesKHR &capabilities) const;
    
    VkPresentModeKHR chooseSwapSurfacePresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes) const;
    
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) const;
};

#endif /* helper_hpp */
