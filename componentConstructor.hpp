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

#include <vulkan/vulkan.h>

#include <stdio.h>
#include <vector>

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

const int WINDOW_WIDTH = 750;
const int WINDOW_HEIGHT = 750;

class ApplicationComponentConstructor
{
public:
    friend class GameApplication;
    
private:
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
    inline VkDebugUtilsMessengerEXT* setupDebugMessenger(VkInstance* const instance) const
    {
        if (!enableValidationLayers) return nullptr;
        
        VkDebugUtilsMessengerEXT* newDebugMessenger = new VkDebugUtilsMessengerEXT;
        
        VkDebugUtilsMessengerCreateInfoEXT createInfo {};
        createInfo = generateDebugMessengerCreateInfo();
        
        if (vkCreateDebugUtilsMessengerEXT(*instance, &createInfo, nullptr, newDebugMessenger) != VK_SUCCESS)
            throw std::runtime_error("Failed to setup debug messenger!");
        
        return newDebugMessenger;
    };
    
    VkDebugUtilsMessengerCreateInfoEXT generateDebugMessengerCreateInfo() const;
    
    // SURFACE CREATION FUNCTIONS
    VkSurfaceKHR* createSurface();
    
    // LOGICAL DEVICE CREATION FUNCTIONS
    VkDevice* createLogicalDevice();
};

#endif /* componentConstructor_hpp */
