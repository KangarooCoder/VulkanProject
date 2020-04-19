//
//  componentConstructor.hpp
//  VulkanProject
//
//  Created by Keegan Bilodeau on 4/15/20.
//  Copyright © 2020 Keegan Bilodeau. All rights reserved.
//
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"

#ifndef componentConstructor_hpp
#define componentConstructor_hpp

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <vector>

#include "helper.hpp"

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

class ApplicationComponentConstructor
{
public:
    friend class GameApplication;
    
private:
    // Instance of application helper to assist in information retrieval
    ApplicationHelper helper;
    
    // Start of static helper functions
    using VkDUMessageSeverity = VkDebugUtilsMessageSeverityFlagBitsEXT;
    using VkDUMessageType = VkDebugUtilsMessageTypeFlagsEXT;
    using VkDUMCallBackData = VkDebugUtilsMessengerCallbackDataEXT;
    
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDUMessageSeverity messageSeverity, VkDUMessageType messageType, const VkDUMCallBackData* pCallBackData, void* pUserData);
    
    static std::vector<char> readFile(const std::string &fileName);
    // End of static helper functions
    
    // INSTANCE CREATION FUNCTIONS
    VkInstance* createInstance() const;
    
    // DEBUG MESSENGER CREATION FUNCTIONS
    VkResult CreateDebugeUtilsMessengerEXT(VkInstance* const instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) const;
    
    void DestroyDebugUtilsMessengerEXT(VkInstance* const instance, VkDebugUtilsMessengerEXT* const debugMessenger, const VkAllocationCallbacks* pAllocator) const;
    
    VkDebugUtilsMessengerEXT* setupDebugMessenger(VkInstance* const instance) const;
    
    VkDebugUtilsMessengerCreateInfoEXT generateDebugMessengerCreateInfo() const;
    
    // SURFACE CREATION FUNCTIONS
    VkSurfaceKHR* createSurface(VkInstance* const instance, GLFWwindow* const window) const;
    
    // LOGICAL DEVICE CREATION FUNCTIONS
    std::pair<VkDevice*, VkQueue*> createLogicalDevice(VkPhysicalDevice const &device, VkSurfaceKHR* const surface) const;
    
    // SWAPCHAIN CREATION FUNCTIONS
    std::pair<VkSwapchainKHR, std::pair<VkFormat, VkExtent2D>> createSwapChain(VkPhysicalDevice const &physicalDevice, VkDevice* const device, VkSurfaceKHR* const surface) const;
    
    std::vector<VkImage> getSwapChainImages(VkDevice* const device, VkSwapchainKHR const swapChain) const;
    
    // IMAGE VIEW CREATION FUNCTIONS
    std::vector<VkImageView> createImageViews(VkDevice* const device, std::vector<VkImage> &swapChainImages, VkFormat &format) const;
};

#endif /* componentConstructor_hpp */
