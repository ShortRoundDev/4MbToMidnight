#include "GameManager.hpp"
#include "GraphicsManager.hpp"

#include "Managers.hpp"

#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <time.h>
#include <cstring>

#include <cxxmidi/file.hpp>
#include <cxxmidi/output/default.hpp>
#include <cxxmidi/player/player_sync.hpp>

#define TIMESPEED 16666666

float alpha = 1.0f;

// Singleton stuff

std::unique_ptr<GameManager> GameManager::instance = nullptr;
int64_t GameManager::accumulator = 0;
std::chrono::high_resolution_clock::time_point GameManager::lastFrame = std::chrono::high_resolution_clock::now();

bool GameManager::keyMap[GLFW_KEY_LAST];


int GameManager::init(GLFWwindow* window, const uint16_t width, const uint16_t height) {
    GameManager::instance = std::make_unique<GameManager>(window, width, height);
    instance->player.pos = instance->currentLevel->playerPos;
    instance->camera.cameraFront = instance->currentLevel->cameraFront;
    return 0;
}

void GameManager::deleteEntity(Entity* entity) {
    instance->currentLevel->removeEntities.push_back(entity);
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
    
    // Audio
    cxxmidi::output::Default output;
    for(size_t i = 0; i < output.GetPortCount(); i++){
        std::cout << i << ": " << output.GetPortName(i) << std::endl;
    }
    output.OpenVirtualPort();
    //output.Op
    cxxmidi::File file("Resources/Audio/chopin.mid");
    
    cxxmidi::player::PlayerSync player(&output);
    player.SetFile(&file);

    player.Play();
}

GameManager::~GameManager() {

}

void GameManager::_update() {
    camera.update();
    player.update(GraphicsManager::instance->window);    
    currentLevel->update();
} 

void GameManager::_draw() {
    currentLevel->draw();
    auto playerStr = std::string("P ") + std::to_string((int)player.pos.x) + " " + std::to_string((int)player.pos.z);
    print(playerStr.c_str(), SCREEN_X(64), SCREEN_Y(64), 0.05f);
    player.draw();
}

void GameManager::print(const char* message, float xPos, float yPos, float size) {
    fontShader->use();
    glBindVertexArray(fontVao);
    glBindTexture(GL_TEXTURE_2D, fontTex);
    
    for(uint64_t i = 0; i < strlen(message); i++) {
        char c = message[i];
        auto cDiff = c - 'A';
        if(!(cDiff >= 0 && cDiff <= 25)){
            cDiff = c - '0';
            if(cDiff >= 0 && cDiff <= 9){
                cDiff += 26;
            } else {
                continue;
            }
        }
        fontShader->setInt("c", cDiff);
        fontShader->setVec3("trans", glm::vec3(xPos + ((float)i * size), yPos, size));
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}

void GameManager::_processInput(GLFWwindow* window){
}

void GameManager::initEventHandlers(GLFWwindow* window) {
    /*if(glfwRawMouseMotionSupported()){
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }*/
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
    GameManager::instance->player.keyHandler(window, key, scancode, action, mods);
}

void GameManager::initFont() {
    fontTex = GraphicsManager::loadTex(2000, GL_BGRA);
    
    float square[] = {
        -0.5f, -0.5f, 0.0f, 0.0f,       0.0f, // Top left
         0.5f, -0.5f, 0.0f, 1.0f/36.0f, 0.0f, // Top Right
        -0.5f,  0.5f, 0.0f, 0.0f,       1.0f,
        
         0.5f, -0.5f, 0.0f, 1.0f/36.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 1.0f/36.0f, 1.0f,
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


void GameManager::dda(float endX, float endY, int* x, int* y){
    *x = -1;
    *y = -1;
    
    float x1 = camera.cameraPos.x;
    float y1 = camera.cameraPos.z;
    
    auto x2 = endX;
    auto y2 = endY;
    
    int gridPosX = (int)x1;
    int gridPosY = (int)y1;
     
    float dirX = x2 - x1;
    float dirY = y2 - y1;
    float distSqr = dirX * dirX + dirY * dirY;
    if (distSqr < 0.00000001)
        return;
     
    float nf = 1 / sqrt(distSqr);
    dirX *= nf;
    dirY *= nf;
     
    float deltaX = 1.0f / std::fabs(dirX);
    float deltaY = 1.0f / std::fabs(dirY);
     
    float maxX = gridPosX - x1;
    float maxY = gridPosY - y1;
    if (dirX >= 0) maxX += 1.0f;
    if (dirY >= 0) maxY += 1.0f;
    maxX /= dirX;
    maxY /= dirY;
     
    int stepX = dirX < 0 ? -1 : 1;
    int stepY = dirY < 0 ? -1 : 1;
    int gridGoalX = x2;
    int gridGoalY = y2;
    int currentDirX = gridGoalX - gridPosX;
    int currentDirY = gridGoalY - gridPosY;
        
    while ((currentDirX * stepX > 0 || currentDirY * stepY > 0) && gridPosX >= 0 && gridPosY >= 0)
    {
        if (maxX < maxY)
        {
            maxX += deltaX;
            gridPosX += stepX;
            currentDirX = gridGoalX - gridPosX;
        }
        else
        {
            maxY += deltaY;
            gridPosY += stepY;
            currentDirY = gridGoalY - gridPosY;
        }
        auto wall = WALLS[COORDS(gridPosX, gridPosY)];
        if(wall.wallTexture != 0 || (wall.isOpen)) {
            *x = gridPosX;
            *y = gridPosY;
            return;
        }
    }
    *x = -1;
    *y = -1;
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