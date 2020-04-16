//
//  componentConstructor.cpp
//  VulkanProject
//
//  Created by Keegan Bilodeau on 4/15/20.
//  Copyright © 2020 Keegan Bilodeau. All rights reserved.
//

#include "componentConstructor.hpp"
#include "helper.hpp"

#include <iostream>

VKAPI_ATTR VkBool32 VKAPI_CALL ApplicationComponentConstructor::debugCallback(VkDUMessageSeverity messageSeverity, VkDUMessageType messageType, const VkDUMCallBackData *pCallbackData, void *pUserData)
{
    if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        std::cerr << "Validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}

VkInstance* ApplicationComponentConstructor::createInstance() const
{
    ApplicationHelper helper;
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

VkDebugUtilsMessengerEXT* ApplicationComponentConstructor::setupDebugMessenger(VkInstance* const instance) const
{
    if (!enableValidationLayers) return nullptr;
    
    VkDebugUtilsMessengerEXT* newDebugMessenger = new VkDebugUtilsMessengerEXT;
    
    VkDebugUtilsMessengerCreateInfoEXT createInfo {};
    createInfo = generateDebugMessengerCreateInfo();
    
    if (vkCreateDebugUtilsMessengerEXT(*instance, &createInfo, nullptr, newDebugMessenger) != VK_SUCCESS)
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
