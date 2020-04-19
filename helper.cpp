//
//  helper.cpp
//  VulkanProject
//
//  Created by Keegan Bilodeau on 4/15/20.
//  Copyright © 2020 Keegan Bilodeau. All rights reserved.
//
#include <cstring>
#include <set>
#include <string>
#include <vector>

#include "helper.hpp"

bool ApplicationHelper::checkDeviceExtensionSupport(const VkPhysicalDevice* device) const
{
    uint32_t extensionCount = 0;
    vkEnumerateDeviceExtensionProperties((*device), nullptr, &extensionCount, nullptr);
    
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties((*device), nullptr, &extensionCount, availableExtensions.data());
    
    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
    
    for (const auto &extension : availableExtensions)
        requiredExtensions.erase(extension.extensionName);
    
    return requiredExtensions.empty();
}

bool ApplicationHelper::checkLayerValidationSupport() const
{
    uint32_t layerCount = 0;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    
    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
        
    for (const char* layerName : validationLayers)
    {
        bool layerFound = false;
        
        for (const auto &layerProperties : availableLayers)
        {
            if (strcmp(layerName, layerProperties.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }
        
        if (!layerFound)
            return false;
    }
    
    return true;
}

bool ApplicationHelper::isDeviceSuitable(VkPhysicalDevice const device, VkSurfaceKHR* const surface) const
{
    QueueFamilyIndices indices = findQueueFamilies((device), (*surface));
    
    bool extensionsSupported = checkDeviceExtensionSupport(&device);
    
    bool swapChainAdequate = false;
    if (extensionsSupported)
    {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport((device), (*surface));
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }
    
    return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

std::vector<const char*> ApplicationHelper::getRequiredExtensions(const bool &enableValidationLayers) const
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    
    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    if (enableValidationLayers)
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    
    return extensions;
}

QueueFamilyIndices ApplicationHelper::findQueueFamilies(const VkPhysicalDevice &device, const VkSurfaceKHR &surface) const
{
    QueueFamilyIndices indices;
    
    uint32_t queueCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, nullptr);
    
    std::vector<VkQueueFamilyProperties> queueFamilies(queueCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, queueFamilies.data());
    
    int i = 0;
    for (const auto &queueFamily : queueFamilies)
    {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            indices.graphicsFamily = i;
        
        VkBool32 presentSupport;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
        
        if (presentSupport)
            indices.presentFamily = i;
        
        if (indices.isComplete())
            break;
        
        i++;
    }
    
    return indices;
}

SwapChainSupportDetails ApplicationHelper::querySwapChainSupport(const VkPhysicalDevice &device, const VkSurfaceKHR &surface) const
{
    SwapChainSupportDetails details;
    
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);
    
    uint32_t formatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
    
    if (formatCount != 0)
    {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }
    
    uint32_t presentModesCount = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModesCount, nullptr);
    
    if (presentModesCount != 0)
    {
        details.presentModes.resize(presentModesCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModesCount, details.presentModes.data());
    }
    
    return details;
}

VkPhysicalDevice ApplicationHelper::pickPhysicalDevice(VkInstance* const instance, VkSurfaceKHR* const surface) const
{
    VkPhysicalDevice newDevice;
    newDevice = VK_NULL_HANDLE;
    
    uint32_t deviceCount;
    vkEnumeratePhysicalDevices((*instance), &deviceCount, nullptr);
    
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices((*instance), &deviceCount, devices.data());
    
    for (const auto &device : devices)
    {
        if (isDeviceSuitable(device, surface))
        {
            newDevice = device;
            break;
        }
    }
    
    if (newDevice == VK_NULL_HANDLE)
        throw std::runtime_error("Failed to find a suitable GPU!");
    
    return newDevice;
}

VkExtent2D ApplicationHelper::chooseSwapSurfaceExtent(const VkSurfaceCapabilitiesKHR &capabilities) const
{
    if (capabilities.currentExtent.width != UINT32_MAX)
        return capabilities.currentExtent;
    else
    {
        VkExtent2D actualExtent {WINDOW_WIDTH, WINDOW_HEIGHT};
        
        actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
        
        return actualExtent;
    }
}

 VkPresentModeKHR ApplicationHelper::chooseSwapSurfacePresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes) const
{
    for (const auto &availablePresentMode : availablePresentModes)
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            return availablePresentMode;
    
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkSurfaceFormatKHR ApplicationHelper::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) const
{
    for (const auto &availableFormat : availableFormats)
        if (availableFormat.format == VK_FORMAT_R8G8B8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            return availableFormat;
    
    return availableFormats[0];
}
