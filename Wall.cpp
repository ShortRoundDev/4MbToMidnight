#include "Wall.hpp"

#include <iostream>
#include "GameManager.hpp"

Wall::Wall() {
    this->displacement = glm::vec3(0.0f, 0.0f, 0.0f);
    openProgress = 0.0f;
    isOpen = false;
    isOpening = false;
    tint = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

Wall::~Wall() {
    
}

void Wall::update(){    
    if(isOpening){
        if(openProgress < 1.0f) {
            openProgress += 0.03f;
        }
        if(openProgress >= 1.0f) {
            openProgress = 1.0f;
            isOpen = true;
            isOpening = false;
        }
        this->displacement = glm::vec3(0.0f, openProgress * 0.9f, 0.0f);
    } else if(isOpen) {
        this->displacement = glm::vec3(0.0f, 0.9, 0.0f);
    } else {
        this->displacement = glm::vec3(0.0f, 0.0f, 0.0f);
    }
}

void Wall::open(){
    this->isOpening = true;
    openProgress = 0.0f;
}

void Wall::draw() {
    
}