//
//  gameApplication.cpp
//  VulkanProject
//
//  Created by Keegan Bilodeau on 4/15/20.
//  Copyright © 2020 Keegan Bilodeau. All rights reserved.
//

#include "gameApplication.hpp"

void GameApplication::run()
{
    initWindow();
    initVulkan();
    
    mainLoop();
    
    cleanup();
}

void GameApplication::initWindow()
{
    glfwInit();
    
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Vulkan Window", nullptr, nullptr);
}

void GameApplication::initVulkan()
{
    glfwSetErrorCallback(ErrorCallback);
    
    instance = componentConstructor.createInstance();
    debugMessenger = componentConstructor.setupDebugMessenger(instance);
    surface = componentConstructor.createSurface(instance, window);
    physicalDevice = std::move(componentConstructor.helper.pickPhysicalDevice(instance, surface));

    std::pair<VkDevice*, VkQueue*> deviceAndQueue = componentConstructor.createLogicalDevice(physicalDevice, surface);

    device = deviceAndQueue.first;
    graphicsQueue = deviceAndQueue.second;

    std::pair<VkSwapchainKHR, std::pair<VkFormat, VkExtent2D>> swapChainAndInfo = componentConstructor.createSwapChain(physicalDevice, device, surface);

    swapChain = swapChainAndInfo.first;
    swapChainFormat = std::move(swapChainAndInfo.second.first);
    swapChainExtent = std::move(swapChainAndInfo.second.second);

    swapChainImages = componentConstructor.getSwapChainImages(device, swapChain);
    swapChainImageViews = componentConstructor.createImageViews(device, swapChainImages, swapChainFormat);
}

void GameApplication::mainLoop() const
{
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        drawFrame();
    }
}

void GameApplication::drawFrame() const
{
    
}

void GameApplication::cleanup()
{
    for (auto &imageView : swapChainImageViews)
        vkDestroyImageView((*device), imageView, nullptr);
    
    std::vector<VkImageView>().swap(swapChainImageViews);
    
    vkDestroySwapchainKHR((*device), swapChain, nullptr);
    
    vkDestroyDevice((*device), nullptr);
    delete device;
    device = nullptr;
    
    if (enableValidationLayers)
    {
        componentConstructor.DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
        delete debugMessenger;
        debugMessenger = nullptr;
    }
    
    vkDestroySurfaceKHR((*instance), (*surface), nullptr);
    delete surface;
    surface = nullptr;
    
    vkDestroyInstance((*instance), nullptr);
    delete instance;
    instance = nullptr;
    
    glfwDestroyWindow(window);
    glfwTerminate();
}
