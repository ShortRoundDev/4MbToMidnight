#pragma once
#include <stdint.h>

#include "glm/glm.hpp"

class Wall {
public:
    Wall();
    ~Wall();
    
    glm::vec2 a;
    glm::vec2 b;
    uint8_t wallTexture;
    bool door;
    int key;
};