#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "Shader.hpp"

class Entity {
public:
    static void init(Shader* shader);

    Entity(glm::vec3, std::string texture, glm::vec2 scale, float radius);
    Entity(glm::vec3, GLuint texture, glm::vec2 scale, float radius);
    virtual ~Entity();
    float radius;
    
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 moveVec;
    float accel;
    
    void draw();
    virtual void update();
    glm::vec2 scale;
    
    static GLuint vao;
    static Shader* shader;
protected:
    glm::vec3 pushWall(glm::vec3 newPos);
    
private:
    GLuint texture;
    
    static void uploadVertices();
};