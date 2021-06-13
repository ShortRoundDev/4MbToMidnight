#pragma once

#include "glm/glm.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#define WEP_PISTOL 1
#define WEP_RIFLE  2

#define PLAYER_ACCEL (0.01f)

class Player{
public:
    Player(glm::vec3 startPos);
    ~Player();
    
    float radius = 0.2f;
    uint8_t health = 5;
    
    glm::vec3 pos;
    glm::vec3 moveVec;
    glm::vec3 moveDir;
    float accel = 0;
    
    void update(GLFWwindow* window);
    void move(GLFWwindow* window);
    
    bool hasBlueKey = false;
    bool hasRedKey = false;
    bool hasYellowKey = false;
    bool seen = false;
    
    float gunTheta = 0.0f;
    
    bool hasPistol = false;
    bool hasRifle = false;
    int activeWeapon = 0;
    
    GLuint gun;
    GLuint pistolAmmoIndicator;
    GLuint rifle;
    GLuint rifleAmmoIndicator;

    int ammo = 0;
    int rifleAmmo = 0;
    float gunFrame = 0;
    float rifleFrame = 0;
    
    GLuint popupSign;
    
    glm::vec3 pushWall(glm::vec3 newPos);
    
    void keyHandler(GLFWwindow* window, int key, int scancode, int action, int mods);
    void collide();
    
    void draw();
    void shoot();
    
private:
    void drawPistol();
    void drawRifle();
    
    bool shootPistol();
    bool shootRifle();
};