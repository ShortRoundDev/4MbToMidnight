#include "Player.hpp"

#include <iostream>

#include "GameManager.hpp"
#include "Camera.hpp"
#include "GLFW/glfw3.h"

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
    /*auto nextStep = (pos + moveVec);
    int x = (int)nextStep.x,
        y = (int)nextStep.z;
        
    Level* level = GameManager::instance->currentLevel;
    if(level->walls[x + (y * level->height)] != 0)
        return;
        
    //std::cout << moveVec.x << ", " << moveVec.y << std::endl;*/
    pos += moveVec;
    pos.y = 0.5;
}


void Player::draw() {
}

