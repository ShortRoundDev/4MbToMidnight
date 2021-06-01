#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "Shader.hpp"

class Entity {
public:
    static void init(Shader* shader);

    Entity(glm::vec3, std::string texture, glm::vec2 scale);
    Entity(glm::vec3, GLuint texture, glm::vec2 scale);
    virtual ~Entity();
    
    glm::vec3 position;
    glm::vec3 front;
    
    void draw();
    virtual void update();
    glm::vec2 scale;
    
    static GLuint vao;
    static Shader* shader;
private:
    GLuint texture;
    
    static void uploadVertices();
};