#pragma once

#include <string>
#include <stdint.h>
#include <vector>

#include "glad/glad.h"

#include "Wall.hpp"
#include "Shader.hpp"
#include "Entity.hpp"
// File format:
// Header:
// numberOfTextures uint8_t     1 // total textures in map. Each texture must be named 0.png, 1.png, 2.png, etc.
// numberOfWalls    uint16_t    2 // total walls in map
// Wall area

// Wall:
// A                glm::vec2   8
// B                glm::vec2   8
// textureNumber    uint8_t     1
// bitArea          uint8_t     1 // bit 1      -> door
//                                // bits 2-3   -> keyNumber


class Level{
public:
    Level(std::string path);
    ~Level();
    
    uint8_t numberOfTextures;
    uint8_t width;
    uint8_t height;
    uint8_t* walls;
    
    std::vector<Entity*> entities;
    
    GLuint wallsVao;
    GLuint ceilingVao;
    GLuint ceilingTexture;
    
    Shader* wallShader;
    
    GLuint floorVao;
    GLuint floorTexture;
    
    uint8_t* loadFile(std::string path);
    
    void draw();
private:
    void uploadWall();
    void uploadHorizontalPlane(float h, GLuint* which);
    void uploadFloor();
    void uploadCeiling();
};