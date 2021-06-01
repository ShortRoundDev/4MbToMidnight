#include "Entity.hpp"

#include "GraphicsManager.hpp"
#include "GameManager.hpp"

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

Entity::Entity(glm::vec3 position, std::string texture, glm::vec2 scale) {
    this->position = position;
    this->texture = GraphicsManager::loadTex(texture, GL_BGRA);
    this->front = glm::vec3(0.0f, 0.0f, 1.0f);
    this->scale = scale;
}

Entity::Entity(glm::vec3, GLuint texture, glm::vec2 scale) {
    this->position = position;
    this->texture = GraphicsManager::loadTex(texture, GL_BGRA);
    this->front = glm::vec3(0.0f, 0.0f, 1.0f);
    this->scale = scale;
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