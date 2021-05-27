#include "GameManager.hpp"
#include "GraphicsManager.hpp"

#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <time.h>

#define TIMESPEED 16666666

// Singleton stuff

std::unique_ptr<GameManager> GameManager::instance = nullptr;
int64_t GameManager::accumulator = 0;
std::chrono::system_clock::time_point GameManager::lastFrame = std::chrono::high_resolution_clock::now();

bool GameManager::keyMap[GLFW_KEY_LAST];


int GameManager::init(GLFWwindow* window, const uint16_t width, const uint16_t height) {
    GameManager::instance = std::make_unique<GameManager>(window, width, height);
    
    return 0;
}

void GameManager::update() {

    auto newTime = std::chrono::high_resolution_clock::now();
    auto frameTime = std::chrono::duration_cast<std::chrono::nanoseconds>(newTime - lastFrame).count();
    lastFrame = newTime;

    if(frameTime >= 250000000){
        frameTime = 250000000;
    }
    accumulator += frameTime;
    while(accumulator >= 16129000) { // 16ms
        GameManager::instance->_update();
        accumulator -= TIMESPEED;
    }

}

void GameManager::draw() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    GameManager::instance->_draw();

    GraphicsManager::draw();
}

void GameManager::processInput(GLFWwindow* window) {
    GameManager::instance->_processInput(window);
}

// Instance Stuff

GameManager::GameManager(GLFWwindow* window, const uint16_t width, const uint16_t height):
    camera(width, height),
    player(glm::vec3(2.5f, 0.5, 2.5f)){
    accumulator = 0;
    initEventHandlers(window);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Entity::init(GraphicsManager::shaders["Entities"]);

    currentLevel = new Level("Resources/map.bin");
    for(int i = 1; i < currentLevel->numberOfTextures + 1; i++){
        GraphicsManager::loadTex(i, GL_BGR);
    }
    for(int i = 0; i < 3; i++)
        GraphicsManager::loadTex(100 + i, GL_BGR);
    currentLevel->ceilingTexture = GraphicsManager::loadTex(200, GL_BGR);
    currentLevel->floorTexture = GraphicsManager::loadTex(201, GL_BGR);
    
    this->projection = glm::perspective(
        glm::radians(45.0f),
        (float)GraphicsManager::instance->getWidth() / (float)GraphicsManager::instance->getHeight(),
        0.1f,
        100.0f
    );    
    this->camera.view = glm::translate(glm::mat4(1.0f), this->camera.cameraPos);
    
    //auto cameraTarget = glm::vec3(2.5f, 0.5f, 2.5f);
}

GameManager::~GameManager() {
    
}

void GameManager::_update() {
    camera.update();
    player.update(GraphicsManager::instance->window);    
} 

void GameManager::_draw() {
    currentLevel->draw();
}

void GameManager::_processInput(GLFWwindow* window){
}

void GameManager::initEventHandlers(GLFWwindow* window) {
    if(glfwRawMouseMotionSupported()){
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(action == GLFW_PRESS)
        GameManager::keyMap[key] = true;
    else if(action == GLFW_RELEASE)
        GameManager::keyMap[key] = false;
    
    if(key == GLFW_KEY_M && action == GLFW_PRESS) {
        if(glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED){
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        }
    }
}

void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos) {
    
    auto width = GraphicsManager::instance->getWidth();
    auto height = GraphicsManager::instance->getHeight();
    
    Camera* camera = &(GameManager::instance->camera);
    
    float sensitivity = 0.1f;
    float x = (xPos - width/2) * sensitivity;
    float y = (yPos - height/2) * sensitivity;
    
    camera->yaw += x;
    camera->pitch -= y;
    
    if(camera->pitch > 89.0f)
        camera->pitch = 89.0f;
    if(camera->pitch < -89.0f)
        camera->pitch = -89.0f;
        
    glm::vec3 front;
    front.x = cos(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
    front.y = sin(glm::radians(camera->pitch));
    front.z = sin(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
    camera->cameraFront = glm::normalize(front);
    if(glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
        glfwSetCursorPos(window, width/2, height/2); 
}