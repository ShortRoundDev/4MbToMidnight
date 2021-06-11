#include "YellowKey.hpp"

#include <iostream>
#include "Managers.hpp"

YellowKey::YellowKey(glm::vec3 pos): Entity(pos, 1002, glm::vec2(0.25f, 0.25f), glm::vec2(0.1f, 0.1f)) {
}

YellowKey::~YellowKey() {
    
}

void YellowKey::update(){
    auto player = &GameManager::instance->player;
    if(glm::length(
        glm::vec2(position.x, position.z) -
        glm::vec2(player->pos.x, player->pos.z)
    ) < 0.5f) {
        player->hasYellowKey = true;
        SoundManager::instance->playSound("Resources/Audio/terminal_05.ogg", glm::vec3(0));
        GameManager::deleteEntity(this);
    }
}