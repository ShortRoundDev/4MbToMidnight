#include <stdio.h>
#include <iostream>

#include "GraphicsManager.hpp"
#include "GameManager.hpp"
#include "SoundManager.hpp"

#include <GLFW/glfw3.h>

int main(int argc, char **argv)
{
    std::string title = "Game";
    
    int error = GraphicsManager::init(
        title,
        1024,
        768
    );
    
    GameManager::init(GraphicsManager::instance->window, 1024, 768);
    SoundManager::init();

    
    if(error){
        std::cerr << "Failed to launch" << std::endl;
        return error;
    }
    auto window = GraphicsManager::instance->window;
    while(!glfwWindowShouldClose(window)) {
        GameManager::processInput(window);
        GameManager::update();
        SoundManager::update();
        GameManager::draw();
    }
    
    return 0;
}
