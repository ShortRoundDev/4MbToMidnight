#include "Player.hpp"

#include <iostream>
#include <tuple>

#include "GameManager.hpp"
#include "Camera.hpp"
#include "GLFW/glfw3.h"

#include "Managers.hpp"
#include "BulletHole.hpp"
#include "ZombieGib.hpp"

Player::Player(glm::vec3 startPos):
    pos(startPos),
    moveVec(0, 0, 0),
    moveDir(0, 0, 0){
    
    gun = GraphicsManager::loadTex("Resources/gun.png", GL_BGRA);
    GraphicsManager::loadTex("Resources/crosshair.bmp", GL_BGRA);
}

Player::~Player(){
    
}

void Player::update(GLFWwindow* window){
    Camera* camera = &(GameManager::instance->camera);
    move(window);
    collide();
    camera->cameraPos = this->pos;
    gunTheta += 0.1;
    
    if(gunFrame > 0.0f){
        gunFrame += 0.3f;
    }
    if(gunFrame >= 5.0f){
        gunFrame = 0.0f;
    }
}

void Player::move(GLFWwindow* window) {
    bool moving = false;
    Camera* camera = &(GameManager::instance->camera);
    moveDir = glm::vec3(0, 0, 0);
    
    if(GameManager::keyMap[GLFW_KEY_W]){
        moving = true;
        moveDir += glm::normalize(glm::vec3(camera->cameraFront.x, 0.0f, camera->cameraFront.z));
        //camera.cameraPos += cameraSpeed * camera.cameraFront;
    }
    if(GameManager::keyMap[GLFW_KEY_S]){
        //camera.cameraPos -= cameraSpeed * camera.cameraFront;
        moving = true;
        moveDir -= glm::normalize(glm::vec3(camera->cameraFront.x, 0.0f, camera->cameraFront.z));
    }
    if(GameManager::keyMap[GLFW_KEY_A]){
        moving = true;
        moveDir -= glm::normalize(glm::cross(camera->cameraFront, camera->cameraUp));
        //camera.cameraPos -= glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp)) * cameraSpeed;
    }
    if(GameManager::keyMap[GLFW_KEY_D]){
        moving = true;
        moveDir += glm::normalize(glm::cross(camera->cameraFront, camera->cameraUp));
        //camera.cameraPos += glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp)) * cameraSpeed;
    }
    if(!moving){
        this->accel *= 0.7;
    } else {
        accel += PLAYER_ACCEL;
        accel = glm::clamp(accel, -0.03f, 0.03f);
    }
    
    if(moving && (abs(moveDir.x) > 0 || abs(moveDir.z) > 0)){
        moveDir = glm::normalize(moveDir);
        moveVec = moveDir * accel;
    } else {
        moveVec *= 0.7;
    }
}
    
void Player::collide() {
    auto nextStep = (pos + moveVec);
    pos = pushWall(nextStep);
}

glm::vec3 Player::pushWall(glm::vec3 newPos) {
    
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

void Player::keyHandler(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(key == GLFW_KEY_E && action == GLFW_PRESS) {
        auto look = glm::normalize(glm::vec3(CAMERA.cameraFront.x, 0.0f, CAMERA.cameraFront.z));
        auto lookX = look.x;
        auto lookY = look.z;
        for(int i = -1; i < 2; i++){
            int x = (int)pos.x;
            int y = (int)pos.z;
            
            if(lookX >= 0.5){
                y += i;
                x += 1;
            } else if(lookX <= -0.5){
                y += i;
                x -= 1;
            } else if(lookY >= 0.5) {
                x += i;
                y += 1;
            } else {
                x += i;
                y -= 1;
            }
            if(x >= 0 && y >= 0 && x < LEVEL->width && y < LEVEL->height){
                auto wall = &(WALLS[COORDS(x, y)]);
                if(IS_DOOR((*wall))) {
                    wall->open();
                    PLAY_S("Resources/Audio/DoorClose.ogg", glm::vec3((float)x + 0.5f, 0.5f, (float)y + 0.5f));
                    break;
                }
            }
        }
    }
}

void Player::draw() {
    if(seen)
        PRINT("I SEE YOU", SCREEN_X(300.0f), SCREEN_Y(64.0f), 0.1f);
    
    auto shader = SHADERS["UI"];
    shader->use();
    shader->setInt("frame", 0);
    shader->setInt("maxFrame", 1);
    glBindVertexArray(Entity::vao);
    if(hasRedKey) {
        glBindTexture(GL_TEXTURE_2D, GraphicsManager::findTex(1000));
        shader->setVec3("scale", glm::vec3(
            SCREEN_W(64.0f),
            SCREEN_H(64.0f),
            1.0f
        ));
        shader->setVec3("offset", glm::vec3(
            SCREEN_X(1024.0f - 48.0f),
            SCREEN_Y(768.0f - 48.0f),
            0.0f
        ));
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    if(hasBlueKey) {
        glBindTexture(GL_TEXTURE_2D, GraphicsManager::findTex(1001));
        shader->setVec3("scale", glm::vec3(
            SCREEN_W(64.0f),
            SCREEN_H(64.0f),
            1.0f
        ));
        shader->setVec3("offset", glm::vec3(
            SCREEN_X(1024.0f - 48.0f),
            SCREEN_Y(768.0f - 96.0f),
            0.0f
        ));
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    if(hasYellowKey) {
        glBindTexture(GL_TEXTURE_2D, GraphicsManager::findTex(1002));
        shader->setVec3("scale", glm::vec3(
            SCREEN_W(64.0f),
            SCREEN_H(64.0f),
            1.0f
        ));
        shader->setVec3("offset", glm::vec3(
            SCREEN_X(1024.0f - 48.0f),
            SCREEN_Y(768.0f - 144.0f),
            0.0f
        ));
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    shader->setFloat("frame", floor(gunFrame));
    shader->setFloat("maxFrame", 5.0f);
    glBindTexture(GL_TEXTURE_2D, gun);
    shader->setVec3("scale", glm::vec3(
        SCREEN_W(512.0f),
        SCREEN_H(1024.0f),
        1.0f
    ));
    shader->setVec3("offset", glm::vec3(
        cos(gunTheta) * glm::length(moveVec),
        SCREEN_Y(768.0f) - (1 + (sin(gunTheta * 2))) *  glm::length(moveVec),
        0.0f
    ));
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    if(GameManager::instance->bright){
        glBindTexture(GL_TEXTURE_2D, GraphicsManager::findTex("Resources/crosshair.bmp"));
        shader->setFloat("frame", 0);
        shader->setFloat("maxFrame", 1.0f);
        shader->setVec3("scale", glm::vec3(
            SCREEN_W(32.0f),
            SCREEN_H(32.0f),
            1.0f
        ));
        shader->setVec3("offset", glm::vec3(
            SCREEN_X(512.0f - 16.0f),
            SCREEN_Y(768.0f / 2.0f - 16.0f),
            0.0f
        ));
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}

void Player::shoot() {
    gunFrame = 1.0f;
    auto c = cosf(CAMERA.cameraFront.z);
    auto s = sinf(CAMERA.cameraFront.y);
    auto angle = atanf(s/c);
    
    auto end = pos + (CAMERA.cameraFront * 50.0f);
    int hitType = 0; // 0 = none, 1 = wall, 2 = ent;
    
    Entity* hitEnt;
    glm::vec3 hitPos(0.0f, 0.0f, 0.0f);
    
    SoundManager::instance->playSound("Resources/Audio/pistol.ogg", pos + CAMERA.cameraFront * 0.3f);
    
    int x, y;
    bool pass = GameManager::instance->dda(
        pos.x,
        pos.z,
        end.x,
        end.z,
        &x, &y
    );
    if(!pass) {
        if(GameManager::instance->castRayToWall(pos, x, y, &hitPos)){
            hitType = 1;
        }
    }

    glm::vec3 entHitPos(0.0f, 0.0f, 0.0f);
    if(GameManager::instance->castRayToEntities(pos, CAMERA.cameraFront, &entHitPos, &hitEnt)){
        if(hitType != 1)
            hitType = 2;
        else if(glm::length(pos - entHitPos) < glm::length(pos - hitPos))
            hitType = 2;
    }
    
    if(hitType == 1) {
        GameManager::addEntity(new BulletHole((pos + (0.99f * (hitPos - pos)))));
    }
    else if(hitType == 2) {
        GameManager::addEntity(new ZombieGib(entHitPos, glm::vec2(0.2, 0.2)));
        hitEnt->hurt(1);
        std::cout << hitEnt->health << std::endl;
    }
}