#include "GameManager.hpp"
#include "GraphicsManager.hpp"

#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <time.h>

#define TIMESPEED 16666666

float alpha = 1.0f;

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
    initFont();

    Entity::init(GraphicsManager::shaders["Entities"]);
    fontShader = GraphicsManager::shaders["Font"];

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
    char text[] = "GOT BLUE KEY";
    fontShader->use();
    glClear(GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(fontVao);
    glBindTexture(GL_TEXTURE_2D, fontTex);
    for(int i = 0; i < sizeof(text) - 1; i++) {
        int c = text[i] - 'A';
        if(c < 0 || c > 25)
            continue;
        fontShader->setVec3("trans", glm::vec3(
            GraphicsManager::px2scrnX(64 + i * 28),
            GraphicsManager::px2scrnY(128),
            0.1f));
        fontShader->setInt("c", c);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    Shader* shader = GraphicsManager::shaders["UI"];
    shader->use();
    glClear(GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(GraphicsManager::instance->squareVao);
    glBindTexture(GL_TEXTURE_2D, GraphicsManager::textures[1001]);
    alpha -= 0.001;
    shader->setFloat("alpha", alpha);
    shader->setVec3("offset", glm::vec3(
        GraphicsManager::px2scrnX(64),
        GraphicsManager::px2scrnY(64),
        0.0f
    ));
    shader->setVec3("scale", glm::vec3(
        2 * GraphicsManager::scrnscaleX(64), 2 * GraphicsManager::scrnscaleY(64), 1.0f
    ));
    glDrawArrays(GL_TRIANGLES, 0, 6);
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
    if(key == GLFW_KEY_T && action == GLFW_PRESS) {
        if(GameManager::instance->wireframe){
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        GameManager::instance->wireframe ^= true;
    }
}

void GameManager::initFont() {
    fontTex = GraphicsManager::loadTex(2000, GL_BGRA);
    
    float square[] = {
        -0.5f, -0.5f, 0.0f, 0.0f,       0.0f, // Top left
         0.5f, -0.5f, 0.0f, 1.0f/26.0f, 0.0f, // Top Right
        -0.5f,  0.5f, 0.0f, 0.0f,       1.0f,
        
         0.5f, -0.5f, 0.0f, 1.0f/26.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 1.0f/26.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 0.0f,       1.0f
    };
    
    unsigned int vbo, vao;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 30 * sizeof(float), square, GL_STATIC_DRAW);
    
    // Vertex Data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // UV Mapping
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);
    this->fontVao = vao;
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