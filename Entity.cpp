#include "Entity.hpp"

#include "GraphicsManager.hpp"
#include "GameManager.hpp"

#include "Managers.hpp"

//static stuff

#define TILE_TOP_LEFT       0.0f, 0.0f
#define TILE_TOP_RIGHT      1.0f, 0.0f
#define TILE_BOTTOM_RIGHT   1.0f, 1.0f
#define TILE_BOTTOM_LEFT    0.0f, 1.0f

Shader* Entity::shader = NULL;
GLuint Entity::vao = 0;

void Entity::init(Shader* shader) {
    Entity::shader = shader;
    uploadVertices();
}

void Entity::uploadVertices(){
    float plane[] = {
        -0.5f,  1.0f, 0.0f, TILE_TOP_LEFT,
         0.5f,  1.0f, 0.0f, TILE_TOP_RIGHT,
        -0.5f,  0.0f, 0.0f, TILE_BOTTOM_LEFT,
        
         0.5f,  1.0f, 0.0f, TILE_TOP_RIGHT,
         0.5f,  0.0f, 0.0f, TILE_BOTTOM_RIGHT,
        -0.5f,  0.0f, 0.0f, TILE_BOTTOM_LEFT
    };
    Entity::vao = GraphicsManager::generateVao(plane, sizeof(plane));
}

Entity::Entity(glm::vec3 position, std::string texture, glm::vec2 scale, float radius) {
    this->position = position;
    this->texture = GraphicsManager::loadTex(texture, GL_BGRA);
    this->front = glm::vec3(0.0f, 0.0f, 1.0f);
    this->scale = scale;
    this->radius = radius;
}

Entity::Entity(glm::vec3, GLuint texture, glm::vec2 scale, float radius) {
    this->position = position;
    this->texture = GraphicsManager::loadTex(texture, GL_BGRA);
    this->front = glm::vec3(0.0f, 0.0f, 1.0f);
    this->scale = scale;
    this->radius = radius;
}

Entity::~Entity() {
    
}

void Entity::draw() {
    shader->use();
    
    shader->setVec3("entityPoint", position);
    shader->setVec2("scale", scale);
    
    glBindVertexArray(Entity::vao);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Entity::update() {
    
}

glm::vec3 Entity::pushWall(glm::vec3 newPos) {
    
    //check if inside wall after newPos
    
    int x = (int)newPos.x,
        y = (int)newPos.z;
        
    float xDiff = 0.0f,
          yDiff = 0.0f;
    
    auto w = WALLS[COORDS(x, y)];
    
    if(IN_BOUNDS(x + 1, y)) {
        // Check right wall
        auto nextWall = WALLS[COORDS(x + 1, y)];
        auto distX = (((float)x + 1.0f) - newPos.x);
        if(distX < radius && SOLID(nextWall)){
            xDiff = -(radius - distX);
        }
    }if(IN_BOUNDS(x - 1, y)){
        auto nextWall = WALLS[COORDS(x - 1, y)];
        auto distX = (newPos.x - (float)x);
        if(distX < radius && SOLID(nextWall)){
            xDiff = radius - distX;
        }
    }
    
    if(IN_BOUNDS(x, y + 1)) {
        auto nextWall = WALLS[COORDS(x, y + 1)];
        auto distY = ((float)y + 1.0f) - newPos.z;
        if(distY < radius && SOLID(nextWall)){
            yDiff = -(radius - distY);
        }
    } if(IN_BOUNDS(x, y - 1)){
        auto nextWall = WALLS[COORDS(x, y - 1)];
        auto distY = (newPos.z - (float)y);
        if(distY < radius && SOLID(nextWall)){
            yDiff = radius - distY;
        }
    }
    
    if(xDiff == 0.0f && yDiff == 0.0f){
        //time to check the corners...
        //lets just push X for simplicity. Always lean left/right I guess
        
        //top right
        if(IN_BOUNDS(x + 1, y + 1)){
            auto nextWall = WALLS[COORDS(x + 1, y + 1)];
            
            auto dist = glm::length(glm::vec3(((float)x + 1), newPos.y, ((float)y + 1)) - newPos);
            if(dist < radius && SOLID(nextWall)){
                auto pushBack = glm::normalize(newPos - glm::vec3(x + 1, 0, y + 1)) * ((radius) - dist);
                xDiff = pushBack.x;
                yDiff = pushBack.z;
                accel = 0;
                moveVec.x = 0;
                moveVec.z = 0;
            }
        }
        //bottom right
        if(IN_BOUNDS(x + 1, y - 1)){
            auto nextWall = WALLS[COORDS(x + 1, y - 1)];
            
            auto dist = glm::length(glm::vec3(((float)x + 1), newPos.y, ((float)y)) - newPos);
            if(dist < radius && SOLID(nextWall)){
                auto pushBack = glm::normalize(glm::vec3(newPos - glm::vec3(x + 1, 0, y))) * ((radius) - dist);
                xDiff = pushBack.x;
                yDiff = pushBack.z;
                accel = 0;
                moveVec.x = 0;
                moveVec.z = 0;
            }
        }
        // Bottom left
        if(IN_BOUNDS(x - 1, y - 1)) {
            auto nextWall = WALLS[COORDS(x - 1, y - 1)];
            
            auto dist = glm::length(glm::vec3(((float)x), newPos.y, ((float)y)) - newPos);

            if(dist < radius && SOLID(nextWall)){
                auto pushBack = glm::normalize(glm::vec3(newPos - glm::vec3(x, 0, y))) * ((radius) - dist);
                xDiff = pushBack.x;
                yDiff = pushBack.z;
                accel = 0;
                moveVec.x = 0;
                moveVec.z = 0;
            }
        }
        // top left
        if(IN_BOUNDS(x - 1, y + 1)){
            auto nextWall = WALLS[COORDS(x - 1, y + 1)];
            auto dist = glm::length(glm::vec3(((float)x), newPos.y, ((float)y + 1)) - newPos);

            
            if(dist < radius && SOLID(nextWall)){
                auto pushBack = glm::normalize(glm::vec3(newPos - glm::vec3(x, 0, y + 1))) * ((radius) - dist);
                xDiff = pushBack.x;
                yDiff = pushBack.z;
                accel = 0;
                moveVec.x = 0;
                moveVec.z = 0;
            }
        }
    }
    
    return newPos + glm::vec3(xDiff, 0.0f, yDiff);
}