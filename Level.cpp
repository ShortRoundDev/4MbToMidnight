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
    this->numberOfTextures = ((uint16_t*)buffer)[0];
    this->width = buffer[2];
    this->height = buffer[4];
    this->walls = (Wall*) calloc(width * height, sizeof(Wall));
    this->entities = std::vector<Entity*>();
    
    auto playerX = *((uint16_t*)(buffer + 6));
    auto playerY = *((uint16_t*)(buffer + 8));
    
    playerPos = glm::vec3(
        ((float)playerX),
        0.5f,
        ((float)playerY)
    );
    uint16_t lookDir = *((uint16_t*)(buffer + 10));
    switch(lookDir){
        case 0: {
            cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
            break;
        }
        case 1: {
            cameraFront = glm::vec3(1.0f, 0.0f, 0.0f);
            break;
        }
        case 2: {
            cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
            break;
        }
        case 3: {
            cameraFront = glm::vec3(-1.0f, 0.0f, 0.0f);
            break;
        }
    }
    
    this->wallsLocation = buffer + 12;
    this->entitiesLocation = wallsLocation + (8 * width * height);
    auto diff = entitiesLocation - buffer;
    
    loadWalls();
    loadEntities();
    uploadWall();
    uploadFloor();
    uploadCeiling();
    
    wallShader = GraphicsManager::shaders["Walls"];
    free(buffer);
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
    
    wallShader->setVec3("scale", glm::vec3(1.0, 1.0, 1.0));
    wallShader->setVec3("offset", glm::vec3(0, 0, 0));

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
            auto tileNum = walls[tile].wallTexture;
            if(tileNum == 0)
                continue;
            auto texture = GraphicsManager::textures[tileNum];
            glBindTexture(GL_TEXTURE_2D, texture);
            wallShader->setVec3("offset", glm::vec3((float)i, 0.0f, (float)j));
            
            if(tileNum >= 100 && tileNum <= 104) {
                auto top = ((j - 1) + (i * height));
                auto bottom = ((j + 1) + (i * height));
                
                if(top >= 0 && bottom < width * height && (walls[top].wallTexture != 0 && walls[bottom].wallTexture != 0)){
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
    
    Entity::shader->use();
    Entity::shader->setVec3("playerPos", camera->cameraPos);
    Entity::shader->setMat4("view", camera->view);
    Entity::shader->setMat4("projection", GameManager::instance->projection);

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
        W, H, D, TILE_TOP_LEFT,
        W, H, Y, TILE_TOP_RIGHT,
        W, B, Y, TILE_BOTTOM_RIGHT,
        
        W, H, D, TILE_TOP_LEFT,
        W, B, D, TILE_BOTTOM_LEFT,
        W, B, Y, TILE_BOTTOM_RIGHT,
        
        // Front Face
        X, H, Y, TILE_TOP_RIGHT,
        W, H, Y, TILE_TOP_LEFT,
        W, B, Y, TILE_BOTTOM_LEFT,
        
        X, H, Y, TILE_TOP_RIGHT,
        W, B, Y, TILE_BOTTOM_LEFT,
        X, B, Y, TILE_BOTTOM_RIGHT,
        
        // Left Face
        X, H, Y, TILE_TOP_LEFT,
        X, B, D, TILE_BOTTOM_RIGHT,
        X, H, D, TILE_TOP_RIGHT,
        
        X, H, Y, TILE_TOP_LEFT,
        X, B, Y, TILE_BOTTOM_LEFT,
        X, B, D, TILE_BOTTOM_RIGHT,
        
        // Back Face
        X, H, D, TILE_TOP_LEFT,
        X, B, D, TILE_BOTTOM_LEFT,
        W, B, D, TILE_BOTTOM_RIGHT,
        
        X, H, D, TILE_TOP_LEFT,
        W, H, D, TILE_TOP_RIGHT,
        W, B, D, TILE_BOTTOM_RIGHT,
    };
    
    this->wallsVao = GraphicsManager::generateVao(cube, sizeof(cube));
}

void Level::uploadHorizontalPlane(float h, GLuint* which) {
    float texw = (float)(width)/1.0f;
    
    float planeH = ((float)height - 1.5f);
    float planeW = ((float)width - 1.5f);
    float O = -0.5f;
    
    float plane[] = {
        O,      h, O,       0.0f, 0.0f,
        O,      h, planeH,  0.0f, texw,
        planeW, h, planeH,  texw, texw,
        
        planeW, h, planeH,  texw, texw,
        planeW, h, O,       texw, 0.0f,
        O,      h, O,       0.0f, 0.0f
    };
    *which = GraphicsManager::generateVao(plane, sizeof(plane));
}

void Level::uploadCeiling() {
    uploadHorizontalPlane(1.0f, &(this->ceilingVao));
}

void Level::uploadFloor() {
    uploadHorizontalPlane(0.0f, &(this->floorVao));
}

void Level::loadWalls() {
    for(int i = 0; i < width * height; i++){
        auto offset = i * sizeof(uint64_t);
        walls[i].wallTexture = *((uint16_t*)(wallsLocation + offset));
        walls[i].ceilingTexture = *((uint16_t*)(wallsLocation + offset + 2));
        if(walls[i].ceilingTexture == 0 && i != 0)
            walls[i].ceilingTexture = walls[0].ceilingTexture;
        walls[i].floorTexture = *((uint16_t*)(wallsLocation + offset + 4));
        if(walls[i].floorTexture == 0 && i != 0)
            walls[i].floorTexture = walls[0].floorTexture;
        walls[i].zone = *(wallsLocation + offset + 6);
        uint8_t bitMask = *(wallsLocation + offset + 7);
        walls[i].isDoor = (bitMask & 1) == 1;
        walls[i].key = (bitMask >> 1) & 0b11;
    }
}

void Level::loadEntities() {
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            auto tile = (j + (i * height));
            auto offset = tile * sizeof(uint16_t);
            auto entNum = *((uint16_t*)(entitiesLocation + offset));
            if(entNum == 0)
                continue;
            entities.push_back(createEntity(entNum, i, j));
        }
    }
}

Entity* Level::createEntity(uint16_t entNum, int x, int y) {
    return new Entity(
        glm::vec3((float)x, 0, (float)y),
        entNum,
        glm::vec2(0.25f, 0.25f)
    );
}