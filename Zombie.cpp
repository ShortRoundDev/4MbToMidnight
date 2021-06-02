#include "Zombie.hpp"

#include <iostream>

#include "Managers.hpp"

Zombie::Zombie(glm::vec3 pos) : Entity(pos, "Resources/Zombie.bmp", glm::vec2(1.0f, 1.0f))
{
    
}

void Zombie::update(){
    
    /*bool seen = GameManager::instance->dda(
        position.x, position.z,
        PLAYER.pos.x, PLAYER.pos.z,
        &x, &y
    );*/
    /*if(seen){
        player->seen = true;
    } else{
        player->seen = false;
    }*/
    glm::vec3 last = position;
    if(path.empty() && GameManager::instance->bfs((int16_t)position.x, (int16_t)position.z, (int16_t)PLAYER.pos.x, (int16_t)PLAYER.pos.z, path)){
        //Start backwards
        float itX = (float)((int)PLAYER.pos.x) + 0.5f;
        float itY = (float)((int)PLAYER.pos.z) + 0.5f;
        uint32_t it = PACK_COORDS((int)itX, (int)itY);
        
        bool quit = (itX == (int)last.x) && (itY == (int)last.z);
        
        while(!quit){
            int x, y;
            if(GameManager::instance->dda(
                last.x,
                last.z,
                itX,
                itY,
                &x, &y
            )){
                last = glm::vec3(itX, 0, itY);
                goals.push(last);
                it = PACK_COORDS((int)PLAYER.pos.x, (int)PLAYER.pos.z);
                itX = (float)((int)PLAYER.pos.x) + 0.5f;
                itY = (float)((int)PLAYER.pos.z) + 0.5f;
                std::cout << "Pushed " << last.x << ", " << last.z << std::endl;
            } else {
                it = path[it];
                itX = (float)(UNPACK_X(it)) + 0.5f;
                itY = (float)(UNPACK_Y(it)) + 0.5f;
            }
            quit = ((int)itX == (int)last.x) && ((int)itY == (int)last.z);
        }
    }
}