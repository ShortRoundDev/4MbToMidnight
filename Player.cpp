#include "Player.hpp"

#include <iostream>
#include <tuple>

#include "GameManager.hpp"
#include "Camera.hpp"
#include "GLFW/glfw3.h"

#include "Managers.hpp"

Player::Player(glm::vec3 startPos):
    pos(startPos),
    moveVec(0, 0, 0),
    moveDir(0, 0, 0){
    
}

Player::~Player(){
    
}

void Player::update(GLFWwindow* window){
    Camera* camera = &(GameManager::instance->camera);
    move(window);
    collide();
    camera->cameraPos = this->pos;
    
}

void Player::move(GLFWwindow* window) {
    bool moving = false;
    Camera* camera = &(GameManager::instance->camera);
    moveDir = glm::vec3(0, 0, 0);
    
    if(GameManager::keyMap[GLFW_KEY_W]){
        moving = true;
        moveDir += glm::normalize(glm::vec3(camera->cameraFront.x, 0.0f, camera->cameraFront.z));
        //camera.cameraPos += cameraSpeed * camera.cameraFront;
    }
    if(GameManager::keyMap[GLFW_KEY_S]){
        //camera.cameraPos -= cameraSpeed * camera.cameraFront;
        moving = true;
        moveDir -= glm::normalize(glm::vec3(camera->cameraFront.x, 0.0f, camera->cameraFront.z));
    }
    if(GameManager::keyMap[GLFW_KEY_A]){
        moving = true;
        moveDir -= glm::normalize(glm::cross(camera->cameraFront, camera->cameraUp));
        //camera.cameraPos -= glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp)) * cameraSpeed;
    }
    if(GameManager::keyMap[GLFW_KEY_D]){
        moving = true;
        moveDir += glm::normalize(glm::cross(camera->cameraFront, camera->cameraUp));
        //camera.cameraPos += glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp)) * cameraSpeed;
    }
    if(!moving){
        this->accel *= 0.7;
    } else {
        accel += PLAYER_ACCEL;
        accel = glm::clamp(accel, -0.03f, 0.03f);
    }
    
    if(moving && (abs(moveDir.x) > 0 || abs(moveDir.z) > 0)){
        moveDir = glm::normalize(moveDir);
        moveVec = moveDir * accel;
    } else {
        moveVec *= 0.7;
    }
}
    
void Player::collide() {
    auto nextStep = (pos + moveVec);
    pos = pushWall(nextStep);
}

glm::vec3 Player::pushWall(glm::vec3 newPos) {
    
    //check if inside wall after newPos
    
    int x = (int)newPos.x,
        y = (int)newPos.z;
        
    float xDir = newPos.x - pos.x,
          yDir = newPos.z - pos.z;
    
    float xDiff = 0.0f,
          yDiff = 0.0f;
    
    auto w = WALLS[COORDS(x, y)];
    
    
    
    if(IN_BOUNDS(x + 1, y)) {
        // Check right wall
        auto nextWall = WALLS[COORDS(x + 1, y)];
        auto distX = (((float)x + 1.0f) - newPos.x);
        if(distX < radius && SOLID(nextWall)){
            xDiff = -(radius - distX);
        }
    }if(IN_BOUNDS(x - 1, y)){
        auto nextWall = WALLS[COORDS(x - 1, y)];
        auto distX = (newPos.x - (float)x);
        if(distX < radius && SOLID(nextWall)){
            xDiff = radius - distX;
        }
    }
    
    if(IN_BOUNDS(x, y + 1)) {
        auto nextWall = WALLS[COORDS(x, y + 1)];
        auto distY = ((float)y + 1.0f) - newPos.z;
        if(distY < radius && SOLID(nextWall)){
            yDiff = -(radius - distY);
        }
    } if(IN_BOUNDS(x, y - 1)){
        auto nextWall = WALLS[COORDS(x, y - 1)];
        auto distY = (newPos.z - (float)y);
        if(distY < radius && SOLID(nextWall)){
            yDiff = radius - distY;
        }
    }
    
    if(xDiff == 0.0f && yDiff == 0.0f){
        //time to check the corners...
        //lets just push X for simplicity. Always lean left/right I guess
        
        //top right
        if(IN_BOUNDS(x + 1, y + 1)){
            auto nextWall = WALLS[COORDS(x + 1, y + 1)];
            
            auto dist = glm::length(glm::vec3(((float)x + 1), newPos.y, ((float)y + 1)) - newPos);
            if(dist < radius && SOLID(nextWall)){
                auto pushBack = glm::normalize(newPos - glm::vec3(x + 1, 0, y + 1)) * ((radius) - dist);
                xDiff = pushBack.x;
                yDiff = pushBack.z;
                accel = 0;
                moveVec.x = 0;
                moveVec.z = 0;
            }
        }
        //bottom right
        if(IN_BOUNDS(x + 1, y - 1)){
            auto nextWall = WALLS[COORDS(x + 1, y - 1)];
            
            auto dist = glm::length(glm::vec3(((float)x + 1), newPos.y, ((float)y)) - newPos);
            if(dist < radius && SOLID(nextWall)){
                auto pushBack = glm::normalize(glm::vec3(newPos - glm::vec3(x + 1, 0, y))) * ((radius) - dist);
                xDiff = pushBack.x;
                yDiff = pushBack.z;
                accel = 0;
                moveVec.x = 0;
                moveVec.z = 0;
            }
        }
        // Bottom left
        if(IN_BOUNDS(x - 1, y - 1)) {
            auto nextWall = WALLS[COORDS(x - 1, y - 1)];
            
            auto dist = glm::length(glm::vec3(((float)x), newPos.y, ((float)y)) - newPos);

            if(dist < radius && SOLID(nextWall)){
                auto pushBack = glm::normalize(glm::vec3(newPos - glm::vec3(x, 0, y))) * ((radius) - dist);
                xDiff = pushBack.x;
                yDiff = pushBack.z;
                accel = 0;
                moveVec.x = 0;
                moveVec.z = 0;
            }
        }
        // top left
        if(IN_BOUNDS(x - 1, y + 1)){
            auto nextWall = WALLS[COORDS(x - 1, y + 1)];
            auto dist = glm::length(glm::vec3(((float)x), newPos.y, ((float)y + 1)) - newPos);

            
            if(dist < radius && SOLID(nextWall)){
                auto pushBack = glm::normalize(glm::vec3(newPos - glm::vec3(x, 0, y + 1))) * ((radius) - dist);
                xDiff = pushBack.x;
                yDiff = pushBack.z;
                accel = 0;
                moveVec.x = 0;
                moveVec.z = 0;
            }
        }
    }
    
    return newPos + glm::vec3(xDiff, 0.0f, yDiff);
}

void Player::keyHandler(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(key == GLFW_KEY_E && action == GLFW_PRESS) {
        auto look = glm::normalize(glm::vec3(CAMERA.cameraFront.x, 0.0f, CAMERA.cameraFront.z));
        auto lookX = look.x;
        auto lookY = look.z;
        for(int i = -1; i < 2; i++){
            int x = (int)pos.x;
            int y = (int)pos.z;
            
            if(lookX >= 0.5){
                y += i;
                x += 1;
            } else if(lookX <= -0.5){
                y += i;
                x -= 1;
            } else if(lookY >= 0.5) {
                x += i;
                y += 1;
            } else {
                x += i;
                y -= 1;
            }
            if(x >= 0 && y >= 0 && x < LEVEL->width && y < LEVEL->height){
                auto wall = &(WALLS[COORDS(x, y)]);
                if(IS_DOOR((*wall))) {
                    wall->open();
                    break;
                }
            }
        }
    }
}

void Player::draw() {
}

