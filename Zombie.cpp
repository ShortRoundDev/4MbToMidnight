#include "Zombie.hpp"

#include <iostream>

#include "glm/gtx/rotate_vector.hpp"

#include "Managers.hpp"

Zombie::Zombie(glm::vec3 pos) :
    Entity(pos + glm::vec3(1.0f, 0.0f, 0.0f), "Resources/Zombie.bmp", glm::vec2(1.0f, 1.0f), 0.1f),
    frontVec(sqrtf(2.0f)/2.0f, 0.0f, sqrt(2.0f)/2.0f),
    currentGoal(-1.0f, -1.0f, -1.0f)
{
}

void Zombie::update(){
    int x, y;
    bool seen = GameManager::instance->dda(
        position.x, position.z,
        PLAYER.pos.x, PLAYER.pos.z,
        &x, &y
    );
    //see player, attack
    if(seen){
        std::cout << "I see you!" << std::endl;
        if(!goals.empty()){
            std::queue<glm::vec3> empty;
            std::swap(goals, empty);
        }
        if(!path.empty())
            path.clear();
        attackPlayer();
        seesPlayer = true;
        lastSawPlayer = glm::vec3(PLAYER.pos.x, 0, PLAYER.pos.z);
        std::cout << "Last seen at " << lastSawPlayer.x << ", " <<  lastSawPlayer.z << std::endl;

    } else {
        // just lost sight of player, hunt
        if(seesPlayer) {
            std::cout << "Hey whered you go!" << std::endl;
            generatePath();
            seesPlayer = false;
            std::cout << "I'm at " << (int)position.x << ", " << (int)position.z << std::endl;
            GameManager::printPath(path, PACK_COORDS((int)lastSawPlayer.x, (int)lastSawPlayer.z), PACK_COORDS((int)position.x, (int)position.z));
            if(!goals.empty()){
                currentGoal = goals.front();
                goals.pop();
                std::cout << "I'll check out " << (int)currentGoal.x << ", " << (int)currentGoal.z << std::endl;
            }else {
                std::cout << "I can't get there" << std::endl;
            }
        }
        // already hunting, follow goal
        else if(currentGoal.x != -1 && currentGoal.y != -1) {
            auto dist = glm::distance(position, currentGoal);
            //reached goal, get new
            if(dist < 0.1){
                if(!goals.empty()){
                    currentGoal = goals.front();
                    goals.pop();
                    std::cout << "I'll check out " << (int)currentGoal.x << ", " << (int)currentGoal.z << std::endl;
                } else {
                    // out of goals
                    std::cout << "Is he around here?" << std::endl;
                    currentGoal = glm::vec3(-1, 0, -1);
                }
            }
        }
    }
    if(currentGoal.x != -1 && currentGoal.z != -1)
    {
        goToGoal(currentGoal);
        position = pushWall(position + moveVec);
    } else {
    }
}

void Zombie::wander() {
    return; // sit still for now
}

void Zombie::attackPlayer(){
    currentGoal = PLAYER.pos;
}

void Zombie::goToGoal(glm::vec3 goal) {
    
    auto diffVec = goal - position;
    
    auto diff2 = glm::vec2(diffVec.x, diffVec.z);
    auto front2 = glm::vec2(frontVec.x, frontVec.z);
    
    auto dp = glm::dot(diff2, front2);
    float s = (2 * std::signbit(glm::dot(diff2, glm::rotate(front2, (float)M_PI_2)))) - 1;
    
    float angle = s * acos(
        dp/
        (glm::length(diff2) * glm::length(front2))
    );
    
    if(fabs(angle) > 0.1)
        frontVec = glm::rotateY(frontVec, angle * 0.04f);
    moveVec = frontVec * 0.01f;
    //position += moveVec;
}

void Zombie::seekPlayer() {
}

void Zombie::generatePath() {
    bool found = GameManager::instance->bfs((int16_t)position.x, (int16_t)position.z, (int16_t)lastSawPlayer.x, (int16_t)lastSawPlayer.z, path);
    if(goals.empty() && found){
        //Start backwards
        glm::vec3 last = position;

        float itX = (float)((int)PLAYER.pos.x) + 0.5f;
        float itY = (float)((int)PLAYER.pos.z) + 0.5f;
        uint32_t it = PACK_COORDS((int)itX, (int)itY);
        
        bool quit = (itX == (int)last.x) && (itY == (int)last.z);
        //safety valve
        int counter = 0;
        while(!quit && counter < 512){
            int x, y;
            counter++;
            if(GameManager::instance->dda(
                last.x,
                last.z,
                itX,
                itY,
                &x, &y
            )){
                last = glm::vec3(itX, 0, itY);
                goals.push(last);
                std::cout << "Adding goal " << last.x << ", " << last.z << std::endl;
                it = PACK_COORDS((int)PLAYER.pos.x, (int)PLAYER.pos.z);
                itX = (float)((int)PLAYER.pos.x) + 0.5f;
                itY = (float)((int)PLAYER.pos.z) + 0.5f;
            } else {
                it = path[it];
                itX = (float)(UNPACK_X(it)) + 0.5f;
                itY = (float)(UNPACK_Y(it)) + 0.5f;
            }
            quit = ((int)itX == (int)last.x) && ((int)itY == (int)last.z);
        }
    }
    int counter = 0;
    if(goals.empty() && found) { // raycasting failed
        std::cout << "Adding linear path" << std::endl;
        uint32_t goal = path[PACK_COORDS((int16_t)lastSawPlayer.x, (int16_t)lastSawPlayer.z)];
        std::cout << "Last saw at " << lastSawPlayer.x << ", " << lastSawPlayer.z << std::endl;
        uint32_t start = PACK_COORDS((int16_t)position.x, (int16_t)position.z);
        while(goal != start && counter < 512) {
            auto x = UNPACK_X(goal);
            auto y = UNPACK_Y(goal);
            goals.push(glm::vec3(x, 0, y));
            goal = path[goal];
            counter++;
        }
    }
    if(goals.empty()){
        std::cout << "Catastrophic failure" << std::endl;
    }
}