//
//  gameApplication.hpp
//  VulkanProject
//
//  Created by Keegan Bilodeau on 4/15/20.
//  Copyright © 2020 Keegan Bilodeau. All rights reserved.
//
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"

#ifndef gameApplication_hpp
#define gameApplication_hpp

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdio.h>
#include <string>

#include "componentConstructor.hpp"

class GameApplication
{
public:
    void run();
    
private:
    // Instance of component constructor for VK object generation
    ApplicationComponentConstructor componentConstructor;
    
    GLFWwindow* window;
    VkSurfaceKHR* surface;
    
    VkInstance* instance;
    VkDebugUtilsMessengerEXT* debugMessenger;
    
    VkPhysicalDevice physicalDevice;
    VkDevice* device;
    VkQueue* graphicsQueue;
    
    VkSwapchainKHR swapChain;
    VkFormat swapChainFormat;
    VkExtent2D swapChainExtent;
    
    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;
    
    // Start of init functions
    void initWindow();
    
    void initVulkan();
    // End of init functions
    
    // Start of main functions
    void mainLoop() const;
    
    void drawFrame() const;
    
    void cleanup();
    // End of main functions
    
    static void ErrorCallback(int, const char* err_str)
    {
        std::cout << "GLFW Error: " << err_str << std::endl;
    }
};

#endif /* gameApplication_hpp */
