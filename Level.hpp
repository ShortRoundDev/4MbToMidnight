#pragma once

#include <string>
#include <stdint.h>
#include <vector>

#include "glad/glad.h"

#include "Wall.hpp"
#include "Shader.hpp"
#include "Entity.hpp"

class Level{
public:
    Level(std::string path);
    ~Level();
    
    uint16_t numberOfTextures;
    uint8_t width;
    uint8_t height;
    Wall* walls;
    uint8_t* wallsLocation;
    
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
    
    void loadWalls();
};