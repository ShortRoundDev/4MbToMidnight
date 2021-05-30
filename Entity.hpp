#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "Shader.hpp"

class Entity {
public:
    static void init(Shader* shader);

    Entity(glm::vec3, uint16_t texture, glm::vec2 scale);
    virtual ~Entity();
    
    glm::vec3 position;
    
    void draw();
    virtual void update();
    glm::vec2 scale;
    
    static GLuint vao;
    static Shader* shader;
private:
    GLuint texture;
    
    static void uploadVertices();
};