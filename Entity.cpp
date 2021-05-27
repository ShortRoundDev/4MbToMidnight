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
    Entity::vao = vao;
}

Entity::Entity(glm::vec3 position, uint16_t texture, glm::vec2 scale) {
    this->position = position;
    this->texture = GraphicsManager::loadTex(texture, GL_BGRA);
    this->scale = scale;
}

Entity::~Entity() {
    
}

void Entity::draw() {
    Camera* camera = &(GameManager::instance->camera);
    
    shader->use();
    
    shader->setMat4("projection", GameManager::instance->projection);
    shader->setVec3("entityPoint", position);
    shader->setVec3("playerPos", camera->cameraPos);
    shader->setMat4("view", camera->view);
    shader->setVec2("scale", scale);
    
    glBindVertexArray(Entity::vao);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
}

void Entity::update() {
    
}