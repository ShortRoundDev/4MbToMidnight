#include "BlueKey.hpp"

#include <iostream>
#include "GameManager.hpp"

BlueKey::BlueKey(glm::vec3 pos): Entity(pos, 1001, glm::vec2(0.25f, 0.25f), glm::vec2(0.1f, 0.1f)) {
}

BlueKey::~BlueKey() {
    
}

void BlueKey::update(){
    auto player = &(GameManager::instance->player);
    if(glm::length(
        glm::vec2(position.x, position.z) -
        glm::vec2(player->pos.x, player->pos.z)
    ) < 0.25f) {
        player->hasBlueKey = true;
        GameManager::deleteEntity(this);
    }
}