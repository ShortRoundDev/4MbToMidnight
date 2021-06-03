#include "Level.hpp"
#include "Level.hpp"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>

#include "glm/glm.hpp"
#include "GraphicsManager.hpp"
#include "GameManager.hpp"

#include "EntDef.h"

#define TILE_TOP_LEFT       0.0f, 0.0f
#define TILE_TOP_RIGHT      1.0f, 0.0f
#define TILE_BOTTOM_RIGHT   1.0f, 1.0f
#define TILE_BOTTOM_LEFT    0.0f, 1.0f

//25px
#define DOOR_MAX_Y          0.78125f

#define DOOR_TOP_RIGHT      1.0f, DOOR_MAX_Y
#define DOOR_BOTTOM_RIGHT   1.0f, 1.0f
#define DOOR_BOTTOM_LEFT    0.0f, 1.0f
#define DOOR_TOP_LEFT       0.0f, DOOR_MAX_Y

#define COORD(x, y) (x + (y * width))

Level::Level(std::string path){
    auto buffer = loadFile(path);
    if(buffer == NULL)
        return;
    this->numberOfTextures = ((uint16_t*)buffer)[0];
    this->width = buffer[2];
    this->height = buffer[4];
    this->walls = (Wall*) calloc(width * height, sizeof(Wall));
    this->entities = std::vector<Entity*>();
    this->removeEntities = std::vector<Entity*>();
    
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
    auto eDiff = entitiesLocation - buffer;
    
    loadWalls();
    loadEntities();
    uploadWall();
    uploadFloor();
    uploadCeiling();
    
    wallShader = GraphicsManager::shaders["Walls"];
    free(buffer);
}

uint8_t* Level::loadFile(std::string path){

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    FILE* fp;
    fopen_s(&fp, path.c_str(), "rb");
#else
    FILE* fp = fopen(path.c_str(), "rb");
#endif
    if(fp == NULL) {

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
        char errmsg[1024];
        strerror_s(errmsg, errno);
        std::cerr << "Failed to open map! " << errmsg << std::endl;
#else
        std::cerr << "Failed to open map! " << strerror(errno) << std::endl;
#endif
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

    if(GameManager::instance->bright){
        wallShader->setFloat("minBright", 0.9f);
    } else {
        wallShader->setFloat("minBright", 0.0f);
    }

    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            auto tile = COORD(j, i);
            auto w = walls + tile;
            auto tileNum = walls[tile].wallTexture;
            auto offset = glm::vec3((float)j, 0.0f, (float)i);
            
            wallShader->setVec4("tint", w->tint);
            wallShader->setVec3("offset", offset);
            wallShader->setVec3("scale", glm::vec3(1.0f, 1.0f, 1.0f));
            //floor
            glBindVertexArray(floorVao);
            glBindTexture(GL_TEXTURE_2D, floorTexture);
            glDrawArrays(GL_TRIANGLES, 0, 6);
                
            //ceiling
            glBindVertexArray(ceilingVao);
            glBindTexture(GL_TEXTURE_2D, ceilingTexture);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            

            auto texture = GraphicsManager::findTex(tileNum);
            if(tileNum == 0)
                goto reset;
                
            offset += glm::vec3(0, walls[tile].displacement.y, 0);

            glBindVertexArray(wallsVao);
            wallShader->setVec4("tint", w->tint);
            glBindTexture(GL_TEXTURE_2D, texture);

            if(tileNum >= 100 && tileNum <= 104) {
                auto left = COORD(j - 1, i);
                auto right = COORD(j + 1, i);
                
                if(left >= 0 && right < width * height && (walls[left].wallTexture != 0 && walls[right].wallTexture != 0)){
                    wallShader->setVec3("scale", glm::vec3(1.0f, 1.0f, 0.5f));
                    offset.z += 0.25f;
                } else {
                    wallShader->setVec3("scale", glm::vec3(0.5f, 1.0f, 1.0f));
                    offset.x += 0.25f;
                }
            } else {
                wallShader->setVec3("scale", glm::vec3(1.0f, 1.0f, 1.0f));
            }
            
            wallShader->setVec3("offset", offset);
            glDrawArrays(GL_TRIANGLES, 0, 30);
        reset:
            w->tint = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        }
    }
    
    Entity::shader->use();
    Entity::shader->setVec3("playerPos", camera->cameraPos);
    Entity::shader->setVec4("tint", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    Entity::shader->setMat4("view", camera->view);
    Entity::shader->setMat4("projection", GameManager::instance->projection);
    if(GameManager::instance->bright){
        Entity::shader->setFloat("minBright", 0.9f);
    } else {
        Entity::shader->setFloat("minBright", 0.0f);
    }


    for(auto e : entities){
        e->draw();
    }
}

void Level::update() {
    for(const auto &e : entities) {
        e->update();
    }
    
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            auto wall = walls[COORD(j, i)];
            walls[COORD(j, i)].update();
        }
    }    
    for(auto e : removeEntities) {
        auto idx = std::find(entities.begin(), entities.end(), e);
        entities.erase(idx);
        delete e;
    }
    removeEntities.clear();
}

void Level::uploadWall(){
    const float X = 0.0f,
                Y = 0.0f,
                B = 0.0f,
                W = 1.0f,
                H = 1.0f,
                D = 1.0f;
    
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
        
        // Include bottom face for slidingdoors
        W, B, D, DOOR_BOTTOM_RIGHT,//DOOR_TOP_RIGHT,    // far bottom right
        X, B, D, DOOR_TOP_RIGHT,     // far bottom left
        X, B, Y, DOOR_TOP_LEFT,  // near bottom left
        
        W, B, D, DOOR_BOTTOM_RIGHT,    // far bottom right
        W, B, Y, DOOR_BOTTOM_LEFT, // near bottom right
        X, B, Y, DOOR_TOP_LEFT  // near bottom left
    };
    
    this->wallsVao = GraphicsManager::generateVao(cube, sizeof(cube));
}

void Level::uploadHorizontalPlane(float h, GLuint* which) {
    float texw = 1.0f;
    
    float planeH = 1.0f;
    float planeW = 1.0f;
    float O = 0.0f;
    
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
        walls[i].y = (i/width);
        walls[i].x = i%width;
        auto tex = *((uint16_t*)(wallsLocation + offset));;
        walls[i].wallTexture = tex;
        walls[i].ceilingTexture = *((uint16_t*)(wallsLocation + offset + 2));
        if(walls[i].ceilingTexture == 0 && i != 0)
            walls[i].ceilingTexture = walls[0].ceilingTexture;
        walls[i].floorTexture = *((uint16_t*)(wallsLocation + offset + 4));
        if(walls[i].floorTexture == 0 && i != 0)
            walls[i].floorTexture = walls[0].floorTexture;
        uint8_t bitMask = *(wallsLocation + offset + 6);
        walls[i].zone = ((bitMask & 0xff00) >> 8);
        walls[i].isDoor = (bitMask & 1) == 1;
        walls[i].key = (bitMask >> 1) & 0b11;
        if(bitMask != 0)
            std::cout << std::to_string(bitMask) << std::endl;
        walls[i].tint = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }
}

void Level::loadEntities() {
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            auto tile = (j + (i * width));
            //auto offset = tile * sizeof(uint16_t);
            auto entNum = *(((uint16_t*)entitiesLocation) + tile);
            
            if(entNum == 0)
                continue;
            std::cout << "Found at " << j << ", " << i << std::endl;
            entities.push_back(createEntity(entNum, j, i));
        }
    }
}

Entity* Level::createEntity(uint16_t entNum, int x, int y) {
    auto start = glm::vec3((float)x + 0.5f, 0.0f, (float)y + 0.5f);
    switch(entNum){
        case RED_KEY:
            return new RedKey(start);
        case BLUE_KEY:
            return new BlueKey(start);
        case YELLOW_KEY:
            return new YellowKey(start);
        case ZOMBIE:
            return new Zombie(start);
        
    }
    return new Entity(
        glm::vec3((float)x + 0.5f, 0, (float)y + 0.5f),
        entNum,
        glm::vec2(1.0f, 1.0f),
        0.05f
    );
}