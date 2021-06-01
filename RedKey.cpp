#include "RedKey.hpp"

#include <iostream>
#include "GameManager.hpp"

RedKey::RedKey(glm::vec3 pos): Entity(pos, 1000, glm::vec2(0.25f, 0.25f)) {
}

RedKey::~RedKey() {
    
}

void RedKey::update(){
    auto player = &(GameManager::instance->player);
    if(glm::length(
        glm::vec2(position.x, position.z) -
        glm::vec2(player->pos.x, player->pos.z)
    ) < 0.25f) {
        player->hasRedKey = true;
        GameManager::deleteEntity(this);
    }
}