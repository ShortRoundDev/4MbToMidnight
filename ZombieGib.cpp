#include "ZombieGib.hpp"

#include "Managers.hpp"

ZombieGib::ZombieGib(glm::vec3 pos, glm::vec2 scale) : Entity(pos, "Resources/pop.bmp", scale, glm::vec2(0, 0)) {
    totalFrames = 14;
    frame = 0;
}

ZombieGib::~ZombieGib(){
    
}

void ZombieGib::update(){
    lifetime++;
    if(frame >= totalFrames){
        GameManager::deleteEntity(this);
    }
    frame = lifetime/2;
}