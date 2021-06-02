#pragma once

#include <map>
#include <queue>

#include "Entity.hpp"

class Zombie : public Entity{
public:
    Zombie(glm::vec3 pos);
    
    bool seesPlayer = false;
    
    std::map<uint32_t, uint32_t> path;
    glm::vec3 currentGoal;
    std::queue<glm::vec3> goals;
    
    glm::vec3 moveVec;
    glm::vec3 frontVec;
    void update();
};