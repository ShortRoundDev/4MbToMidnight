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
    uint8_t* entitiesLocation;
    
    std::vector<Entity*> entities;
    std::vector<Entity*> removeEntities;
    std::vector<Entity*> addEntities;
    
    GLuint wallsVao;
    GLuint ceilingVao;
    GLuint ceilingTexture;
    
    Shader* wallShader;

    glm::vec3 playerPos;
    glm::vec3 cameraFront;
    
    GLuint floorVao;
    GLuint floorTexture;
    
    uint8_t* loadFile(std::string path);
    
    void draw();
    void update();
private:
    void uploadWall();
    void uploadHorizontalPlane(float h, GLuint* which);
    void uploadFloor();
    void uploadCeiling();
    
    void loadWalls();
    void loadEntities();
    
    Entity* createEntity(uint16_t entNum, int x, int y);
};