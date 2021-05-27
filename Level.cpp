#include "Level.hpp"
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "glm/glm.hpp"
#include "GraphicsManager.hpp"
#include "GameManager.hpp"

#define TILE_TOP_LEFT       0.0f, 0.0f
#define TILE_TOP_RIGHT      1.0f, 0.0f
#define TILE_BOTTOM_RIGHT   1.0f, 1.0f
#define TILE_BOTTOM_LEFT    0.0f, 1.0f

Level::Level(std::string path){
    auto buffer = loadFile(path);
    if(buffer == NULL)
        return;
    this->numberOfTextures = buffer[0];
    this->width = buffer[1];
    this->height = buffer[2];
    this->walls = buffer + 3;
    this->entities = std::vector<Entity*>();
    
    entities.push_back(new Entity(
        glm::vec3(2.5f, 0.0f, 2.5f),
        1001,
        glm::vec2(0.2f, 0.2f)
    ));
    uploadWall();
    uploadFloor();
    uploadCeiling();
    
    wallShader = GraphicsManager::shaders["Walls"];
}

uint8_t* Level::loadFile(std::string path){
    FILE* fp = fopen(path.c_str(), "rb");
    if(fp == NULL) {
        std::cerr << "Failed to open map! " << strerror(errno) << std::endl;
        return NULL;
    }
    fseek(fp, 0, SEEK_END);
    size_t t = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    uint8_t* buffer = (uint8_t*)calloc(t, sizeof(uint8_t));
    fread(buffer, t, sizeof(uint8_t), fp);
    fclose(fp);
    return buffer;
}

void Level::draw() {
    wallShader->use();
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    Camera* camera = &(GameManager::instance->camera);
    wallShader->setMat4("projection", GameManager::instance->projection);
    wallShader->setVec3("playerPos", camera->cameraPos);
    wallShader->setMat4("view", camera->view);

    glBindVertexArray(floorVao);
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glBindVertexArray(ceilingVao);
    glBindTexture(GL_TEXTURE_2D, ceilingTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(wallsVao);
    for(int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++) {
            auto tile = (j + (i * height));
            auto tileNum = walls[tile];
            if(tileNum == 0)
                continue;
            auto texture = GraphicsManager::textures[tileNum];
            glBindTexture(GL_TEXTURE_2D, texture);
            wallShader->setVec3("offset", glm::vec3((float)i, 0.0f, (float)j));
            
            if(tileNum >= 100 && tileNum < 104) {
                auto top = ((j - 1) + (i * height));
                auto bottom = ((j + 1) + (i * height));
                
                if(top >= 0 && bottom < width * height && (walls[top] != 0 && walls[bottom] != 0)){
                    wallShader->setVec3("scale", glm::vec3(0.5f, 1.0f, 1.0f));
                } else {
                    wallShader->setVec3("scale", glm::vec3(1.0f, 1.0f, 0.5f));
                }
            } else {
                wallShader->setVec3("scale", glm::vec3(1.0f, 1.0f, 1.0f));
            }
            glDrawArrays(GL_TRIANGLES, 0, 24);
        }
    }
    
    for(auto e : entities){
        e->draw();
    }
}

void Level::uploadWall(){
    const float X = -0.5,
                Y = -0.5,
                W = 0.5,
                H = 1.0f,
                B = 0.0f,
                D = 0.5f;
    
    float cube[] = {
        // Right Face
        W, H, D, TILE_TOP_RIGHT,
        W, H, Y, TILE_TOP_LEFT,
        W, B, Y, TILE_BOTTOM_LEFT,
        
        W, H, D, TILE_TOP_RIGHT,
        W, B, D, TILE_BOTTOM_RIGHT,
        W, B, Y, TILE_BOTTOM_LEFT,
        
        // Front Face
        X, H, Y, TILE_TOP_LEFT,
        W, H, Y, TILE_TOP_RIGHT,
        W, B, Y, TILE_BOTTOM_RIGHT,
        
        X, H, Y, TILE_TOP_LEFT,
        W, B, Y, TILE_BOTTOM_RIGHT,
        X, B, Y, TILE_BOTTOM_LEFT,
        
        // Left Face
        X, H, Y, TILE_TOP_RIGHT,
        X, B, D, TILE_BOTTOM_LEFT,
        X, H, D, TILE_TOP_LEFT,
        
        X, H, Y, TILE_TOP_RIGHT,
        X, B, Y, TILE_BOTTOM_RIGHT,
        X, B, D, TILE_BOTTOM_LEFT,
        
        // Back Face
        X, H, D, TILE_TOP_RIGHT,
        X, B, D, TILE_BOTTOM_RIGHT,
        W, B, D, TILE_BOTTOM_LEFT,
        
        X, H, D, TILE_TOP_RIGHT,
        W, H, D, TILE_TOP_LEFT,
        W, B, D, TILE_BOTTOM_LEFT,
    };
    
    unsigned int vbo, vao;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 120 * sizeof(float), cube, GL_STATIC_DRAW);
    
    // Vertex Data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // UV Mapping
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);
    this->wallsVao = vao;
}

void Level::uploadHorizontalPlane(float h, GLuint* which) {
    float texw = (float)(width)/1.0f;
    
    float planeH = -((float)height - 0.5);
    float planeW = -((float)width - 0.5);
    float O = 0.5f;
    
    float plane[] = {
        O,      h, O,       0.0f, 0.0f,
        O,      h, planeH,  0.0f, texw,
        planeW, h, planeH,  texw, texw,
        
        planeW, h, planeH,  texw, texw,
        planeW, h, O,       texw, 0.0f,
        O,      h, O,       0.0f, 0.0f
    };
    
    unsigned int vbo, vao;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plane), plane, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (sizeof(float) * 3));
    glEnableVertexAttribArray(1);
    *which = vao;
}

void Level::uploadCeiling() {
    uploadHorizontalPlane(1.0f, &(this->ceilingVao));
}

void Level::uploadFloor() {
    uploadHorizontalPlane(0.0f, &(this->floorVao));
}