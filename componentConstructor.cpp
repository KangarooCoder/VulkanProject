//
//  componentConstructor.cpp
//  VulkanProject
//
//  Created by Keegan Bilodeau on 4/15/20.
//  Copyright © 2020 Keegan Bilodeau. All rights reserved.
//

#include "componentConstructor.hpp"

#include <iostream>

#include "helper.hpp"

// STATIC FUNCTIONS MEMBERS START
VKAPI_ATTR VkBool32 VKAPI_CALL ApplicationComponentConstructor::debugCallback(VkDUMessageSeverity messageSeverity, VkDUMessageType messageType, const VkDUMCallBackData *pCallbackData, void *pUserData)
{
    if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        std::cerr << "Validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}
// STATIC FUNCTION MEMBERS END

// INSTANCE CREATION FUNCTIONS START
VkInstance* ApplicationComponentConstructor::createInstance() const
{
    VkInstance* newInstance = new VkInstance;
    
    // Make sure that if validation layers are enabled, they are supported
    if (enableValidationLayers && !helper.checkLayerValidationSupport())
        throw std::runtime_error("Validation layers requested, but not avaialable!");
    
    // Info regarding the application itself
    VkApplicationInfo appInfo {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello, Triangle, Application";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_MAKE_VERSION(1, 0, 0);
    
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    
    auto extensions = helper.getRequiredExtensions(enableValidationLayers);
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();
    
    VkDebugUtilsMessengerCreateInfoEXT instanceDebugCreateInfo;
    if (enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
        
        instanceDebugCreateInfo = generateDebugMessengerCreateInfo();
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &instanceDebugCreateInfo;
    }
    else
        createInfo.enabledLayerCount = 0;
    
    if (vkCreateInstance(&createInfo, nullptr, newInstance) != VK_SUCCESS)
        throw std::runtime_error("Failed to create instance!");
    
    return newInstance;
}
// INSTANCE CREATION FUNCTIONS END

// DEBUG MESSENGER CREATION FUNCTIONS START
VkResult ApplicationComponentConstructor::CreateDebugeUtilsMessengerEXT(VkInstance* const instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) const
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr((*instance), "vkCreateDebugUtilsMessengerEXT");
    
    if (func != nullptr)
        return func((*instance), pCreateInfo, pAllocator, pDebugMessenger);
    else
        return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void ApplicationComponentConstructor::DestroyDebugUtilsMessengerEXT(VkInstance* const instance, VkDebugUtilsMessengerEXT* const debugMessenger, const VkAllocationCallbacks* pAllocator) const
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr((*instance), "vkDestroyDebugUtilsMessengerEXT");
    
    if (func != nullptr)
        func((*instance), *debugMessenger, pAllocator);
}

VkDebugUtilsMessengerEXT* ApplicationComponentConstructor::setupDebugMessenger(VkInstance* const instance) const
{
    if (!enableValidationLayers) return nullptr;
    
    VkDebugUtilsMessengerEXT* newDebugMessenger = new VkDebugUtilsMessengerEXT;
    
    VkDebugUtilsMessengerCreateInfoEXT createInfo {};
    createInfo = generateDebugMessengerCreateInfo();
    
    if (CreateDebugeUtilsMessengerEXT(instance, &createInfo, nullptr, newDebugMessenger) != VK_SUCCESS)
        throw std::runtime_error("Failed to setup debug messenger!");
    
    return newDebugMessenger;
}

VkDebugUtilsMessengerCreateInfoEXT ApplicationComponentConstructor::generateDebugMessengerCreateInfo() const
{
    VkDebugUtilsMessengerCreateInfoEXT createInfo {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    
    return createInfo;
}
// DEBUG MESSENGER CREATION FUNCTIONS END

// SURFACE CREATION FUNCTIONS START
VkSurfaceKHR* ApplicationComponentConstructor::createSurface(VkInstance* const instance, GLFWwindow* const window) const
{
    VkSurfaceKHR* newSurface = new VkSurfaceKHR;
    
    if (glfwCreateWindowSurface((*instance), window, nullptr, newSurface) != VK_SUCCESS)
        throw std::runtime_error("Failed to create window surface!");
    
    return newSurface;
}
// SURFACE CREATION FUNCTIONS END

// LOGICAL DEVICE CREATION FUNCTIONS START
std::pair<VkDevice*, VkQueue*> ApplicationComponentConstructor::createLogicalDevice(VkPhysicalDevice const &device, VkSurfaceKHR* const surface) const
{
    VkDevice* newDevice = new VkDevice;
    VkQueue* newGraphicsQueue = new VkQueue;
    
    QueueFamilyIndices indices = helper.findQueueFamilies(device, (*surface));
    
    VkDeviceQueueCreateInfo queueCreateInfo {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
    queueCreateInfo.queueCount = 1;
    
    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;
    
    VkPhysicalDeviceFeatures deviceFeatures {};
    
    VkDeviceCreateInfo createInfo {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;
    
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();
    
    if (enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }
    
    if (vkCreateDevice(device, &createInfo, nullptr, newDevice) != VK_SUCCESS)
        throw std::runtime_error("Failed to create logical device");
    
    vkGetDeviceQueue((*newDevice), indices.graphicsFamily.value(), 0, newGraphicsQueue);
    
    return std::make_pair(newDevice, newGraphicsQueue);
}

// SWAPCHAIN CREATION FUNCTIONS START
std::pair<VkSwapchainKHR, std::pair<VkFormat, VkExtent2D>> ApplicationComponentConstructor::createSwapChain(VkPhysicalDevice const &physicalDevice, VkDevice* const device, VkSurfaceKHR* const surface) const
{
    VkSwapchainKHR newSwapChain;
    
    SwapChainSupportDetails swapChainSupport = helper.querySwapChainSupport(physicalDevice, (*surface));
    
    VkSurfaceFormatKHR surfaceFormat = helper.chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = helper.chooseSwapSurfacePresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = helper.chooseSwapSurfaceExtent(swapChainSupport.capabilities);
    
    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
        imageCount = swapChainSupport.capabilities.maxImageCount;
    
    VkSwapchainCreateInfoKHR createInfo {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = (*surface);
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    
    QueueFamilyIndices indices = helper.findQueueFamilies(physicalDevice, (*surface));
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};
    
    if (indices.graphicsFamily != indices.presentFamily)
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }
    
    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;
    
    if (vkCreateSwapchainKHR((*device), &createInfo, nullptr, &newSwapChain) != VK_SUCCESS)
        throw std::runtime_error("Failed to create swapchain!");
    
    return std::make_pair(newSwapChain, std::make_pair(surfaceFormat.format, extent));
}

std::vector<VkImage> ApplicationComponentConstructor::getSwapChainImages(VkDevice* const device, VkSwapchainKHR const swapChain) const
{
    std::vector<VkImage> swapChainImages;
    uint32_t imageCount = 0;
    
    vkGetSwapchainImagesKHR((*device), swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR((*device), swapChain, &imageCount, swapChainImages.data());
    
    return swapChainImages;
}
// SWAPCHAIN CREATION FUNCTIONS END

// IMAGE VIEW CREATION FUNCTIONS START
std::vector<VkImageView> ApplicationComponentConstructor::createImageViews(VkDevice* const device, std::vector<VkImage> &swapChainImages, VkFormat &format) const
{
    std::vector<VkImageView> newSwapChainImageViews;
    newSwapChainImageViews.resize(swapChainImages.size());
    
    for (size_t i = 0; i < newSwapChainImageViews.size(); i++)
    {
        VkImageViewCreateInfo createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = format;
        
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;
        
        if (vkCreateImageView((*device), &createInfo, nullptr, &newSwapChainImageViews[i]) != VK_SUCCESS)
            throw std::runtime_error("Failed to create image views!");
    }
    
    return newSwapChainImageViews;
}
