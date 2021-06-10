#include "Ammo.hpp"

#include "Managers.hpp"

Ammo::Ammo(glm::vec3 pos) : Entity(pos, "Resources/ammo.png", glm::vec2(0.2, 0.2), glm::vec2(0.1, 0.1)){
}

Ammo::~Ammo(){
    
}

void Ammo::update(){
    if(glm::length(glm::vec2(position.x, position.z) - glm::vec2(PLAYER.pos.x, PLAYER.pos.z)) < 0.1f){
        SoundManager::instance->playSound("Resources/Audio/ammo_pickup.ogg", position);
    }
}