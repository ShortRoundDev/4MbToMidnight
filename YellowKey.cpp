#include "YellowKey.hpp"

#include <iostream>
#include "GameManager.hpp"

YellowKey::YellowKey(glm::vec3 pos): Entity(pos, 1002, glm::vec2(0.25f, 0.25f)) {
}

YellowKey::~YellowKey() {
    
}

void YellowKey::update(){
    auto player = &GameManager::instance->player;
    if(glm::length(
        glm::vec2(position.x, position.z) -
        glm::vec2(player->pos.x, player->pos.z)
    ) < 0.25f) {
        player->hasYellowKey = true;
        GameManager::deleteEntity(this);
    }
}