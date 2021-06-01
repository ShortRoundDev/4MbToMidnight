#pragma once

#include <map>
#include "Entity.hpp"

class Zombie : public Entity{
public:
    Zombie(glm::vec3 pos);
    
    std::map<uint32_t, uint32_t> path;
    
    void update();
};