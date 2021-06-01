#include "Zombie.hpp"

#include <iostream>

#include "Managers.hpp"

Zombie::Zombie(glm::vec3 pos) : Entity(pos, "Resources/Zombie.bmp", glm::vec2(1.0f, 1.0f))
{
    
}

void Zombie::update(){
    auto player = &PLAYER;
    int x, y;
    bool seen = GameManager::instance->dda(
        position.x, position.z,
        player->pos.x, player->pos.z,
        &x, &y
    );
    if(seen){
        player->seen = true;
    } else{
        player->seen = false;
    }
    if(GameManager::instance->bfs((int16_t)position.x, (int16_t)position.z, (int16_t)PLAYER.pos.x, (int16_t)PLAYER.pos.z, path)){
    }
}