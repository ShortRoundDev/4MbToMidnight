#pragma once

#include "glm/glm.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#define PLAYER_ACCEL (0.01f)

class Player{
public:
    Player(glm::vec3 startPos);
    ~Player();
    
    uint8_t health = 5;
    
    float radius = 0.1f;
    glm::vec3 pos;
    glm::vec3 moveVec;
    glm::vec3 moveDir;
    float accel = 0;
    
    void update(GLFWwindow* window);
    void move(GLFWwindow* window);
    
    void collide();
    
    void draw();
};