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
    vkDestroyInstance(*instance, nullptr);
}
