#include "GameManager.hpp"
#include "GraphicsManager.hpp"

#include "Managers.hpp"

#include <iostream>
#include <time.h>
#include <cstring>
#include <queue>

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/intersect.hpp>

#define TIMESPEED 16666666

float alpha = 1.0f;

// Singleton stuff

std::unique_ptr<GameManager> GameManager::instance = nullptr;
int64_t GameManager::accumulator = 0;
std::chrono::high_resolution_clock::time_point GameManager::lastFrame = std::chrono::high_resolution_clock::now();

bool GameManager::keyMap[GLFW_KEY_LAST];


int GameManager::init(GLFWwindow* window, const uint16_t width, const uint16_t height) {
    GameManager::instance = std::make_unique<GameManager>(window, width, height);
    instance->player.pos = instance->currentLevel->playerPos;
    instance->camera.cameraFront = instance->currentLevel->cameraFront;
    return 0;
}

void GameManager::deleteEntity(Entity* entity) {
    instance->currentLevel->removeEntities.push_back(entity);
}

void GameManager::addEntity(Entity* entity) {
    instance->currentLevel->addEntities.push_back(entity);
}

void GameManager::update() {
    auto newTime = std::chrono::high_resolution_clock::now();
    auto frameTime = std::chrono::duration_cast<std::chrono::nanoseconds>(newTime - lastFrame).count();
    lastFrame = newTime;

    if(frameTime >= 250000000){
        frameTime = 250000000;
    }
    accumulator += frameTime;
    while(accumulator >= 16129000) { // 16ms
        SoundManager::instance->update();
        GameManager::instance->_update();
        accumulator -= TIMESPEED;
    }

    
}

void GameManager::draw() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    GameManager::instance->_draw();
    GraphicsManager::draw();
}

void GameManager::processInput(GLFWwindow* window) {
    GameManager::instance->_processInput(window);
}

// Instance Stuff

GameManager::GameManager(GLFWwindow* window, const uint16_t width, const uint16_t height):
    camera(width, height),
    player(glm::vec3(2.5f, 0.5, 2.5f)){
    accumulator = 0;
    initEventHandlers(window);
    initFont();

    Entity::init(GraphicsManager::shaders["Entities"]);
    fontShader = GraphicsManager::shaders["Font"];

    currentLevel = new Level("Resources/map.bin");
    for(int i = 1; i < currentLevel->numberOfTextures + 1; i++){
        GraphicsManager::loadTex(i, GL_BGR);
    }
    for(int i = 0; i < 3; i++)
        GraphicsManager::loadTex(100 + i, GL_BGR);
    currentLevel->ceilingTexture = GraphicsManager::loadTex(200, GL_BGR);
    currentLevel->floorTexture = GraphicsManager::loadTex(201, GL_BGR);
    
    this->projection = glm::perspective(
        glm::radians(45.0f),
        (float)GraphicsManager::instance->getWidth() / (float)GraphicsManager::instance->getHeight(),
        0.1f,
        100.0f
    );    
    this->camera.view = glm::translate(glm::mat4(1.0f), this->camera.cameraPos);
    
    // Audio
}

GameManager::~GameManager() {

}

void GameManager::_update() {
    camera.update();
    player.update(GraphicsManager::instance->window);    
    currentLevel->update();
} 

void GameManager::_draw() {
    currentLevel->draw();
    auto playerStr = std::string("P ") + std::to_string((int)player.pos.x) + " " + std::to_string((int)player.pos.z);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    player.draw();
    print(playerStr.c_str(), SCREEN_X(64), SCREEN_Y(64), 0.05f);
}

void GameManager::print(const char* message, float xPos, float yPos, float size) {
    fontShader->use();
    glBindVertexArray(fontVao);
    glBindTexture(GL_TEXTURE_2D, fontTex);
    
    for(uint64_t i = 0; i < strlen(message); i++) {
        char c = message[i];
        auto cDiff = c - 'A';
        if(!(cDiff >= 0 && cDiff <= 25)){
            cDiff = c - '0';
            if(cDiff >= 0 && cDiff <= 9){
                cDiff += 26;
            } else {
                continue;
            }
        }
        fontShader->setInt("c", cDiff);
        fontShader->setVec3("trans", glm::vec3(xPos + ((float)i * size), yPos, size));
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}

void GameManager::_processInput(GLFWwindow* window){
}

void GameManager::initEventHandlers(GLFWwindow* window) {
    /*if(glfwRawMouseMotionSupported()){
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }*/
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(action == GLFW_PRESS)
        GameManager::keyMap[key] = true;
    else if(action == GLFW_RELEASE)
        GameManager::keyMap[key] = false;
    
    if(key == GLFW_KEY_M && action == GLFW_PRESS) {
        if(glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED){
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        }
    }
    if(key == GLFW_KEY_1 && action == GLFW_PRESS) {
        (&CAMERA)->cameraFront = glm::vec3(0.0f, 0.0f, 1.0f); // Looking down
    }
    if(key == GLFW_KEY_2 && action == GLFW_PRESS) {
        (&CAMERA)->cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); // Looking up
    }
    if(key == GLFW_KEY_3 && action == GLFW_PRESS) {
        (&CAMERA)->cameraFront = glm::vec3(1.0f, 0.0f, 0.0f); // Looking to the right
    }
    if(key == GLFW_KEY_4 && action == GLFW_PRESS) {
        (&CAMERA)->cameraFront = glm::vec3(-1.0f, 0.0f, 0.0f); // Looking to the left
    }
    if(key == GLFW_KEY_B && action == GLFW_PRESS){
        GameManager::instance->bright ^= true;
    }
    if(key == GLFW_KEY_T && action == GLFW_PRESS) {
        if(GameManager::instance->wireframe){
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        GameManager::instance->wireframe ^= true;
    }
    GameManager::instance->player.keyHandler(window, key, scancode, action, mods);
}

void GameManager::initFont() {
    fontTex = GraphicsManager::loadTex(2000, GL_BGRA);
    
    float square[] = {
        -0.5f, -0.5f, 0.0f, 0.0f,       0.0f, // Top left
         0.5f, -0.5f, 0.0f, 1.0f/36.0f, 0.0f, // Top Right
        -0.5f,  0.5f, 0.0f, 0.0f,       1.0f,
        
         0.5f, -0.5f, 0.0f, 1.0f/36.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 1.0f/36.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 0.0f,       1.0f
    };
    
    unsigned int vbo, vao;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 30 * sizeof(float), square, GL_STATIC_DRAW);
    
    // Vertex Data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // UV Mapping
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);
    this->fontVao = vao;
}


bool GameManager::dda(float startX, float startY, float endX, float endY, int* x, int* y){
    *x = -1;
    *y = -1;
    
    if(SOLID(WALLS[COORDS((int)startX, (int)startY)])){
        return false;
    }
    
    float x1 = startX;
    float y1 = startY;
    
    auto x2 = endX;
    auto y2 = endY;
    
    int gridPosX = (int)x1;
    int gridPosY = (int)y1;
     
    float dirX = x2 - x1;
    float dirY = y2 - y1;
    float distSqr = dirX * dirX + dirY * dirY;
    if (distSqr < 0.00000001)
        return false;
     
    float nf = 1 / sqrt(distSqr);
    dirX *= nf;
    dirY *= nf;
     
    float deltaX = 1.0f / std::fabs(dirX);
    float deltaY = 1.0f / std::fabs(dirY);
     
    float maxX = gridPosX - x1;
    float maxY = gridPosY - y1;
    if (dirX >= 0) maxX += 1.0f;
    if (dirY >= 0) maxY += 1.0f;
    maxX /= dirX;
    maxY /= dirY;
     
    int stepX = dirX < 0 ? -1 : 1;
    int stepY = dirY < 0 ? -1 : 1;
    int gridGoalX = x2;
    int gridGoalY = y2;
    int currentDirX = gridGoalX - gridPosX;
    int currentDirY = gridGoalY - gridPosY;
        
    while ((currentDirX * stepX > 0 || currentDirY * stepY > 0) && gridPosX >= 0 && gridPosY >= 0)
    {
        if (maxX < maxY)
        {
            maxX += deltaX;
            gridPosX += stepX;
            currentDirX = gridGoalX - gridPosX;
        }
        else
        {
            maxY += deltaY;
            gridPosY += stepY;
            currentDirY = gridGoalY - gridPosY;
        }
                      
        auto wall = WALLS[COORDS(gridPosX, gridPosY)];
        if(wall.wallTexture != 0 || (wall.isOpen)) {
            *x = gridPosX;
            *y = gridPosY;            
            return false;
        }
    }
    *x = endY;
    *y = endX;
    return true;
}

bool GameManager::castRayToWall(glm::vec3 start, int wallX, int wallY, glm::vec3* out) {
    float fWallX = (float)wallX,
          fWallY = (float)wallY;
    std::cout << "Casting at (" << fWallX << "," << fWallY <<  ")" << std::endl;
    std::cout << "Towards    (" << CAMERA.cameraFront.x << ", " << CAMERA.cameraFront.y << ", " << CAMERA.cameraFront.z << ")" << std::endl;
    
    const float X = wallX,
                B = 0.0f,
                Y = wallY,
                
                W = X + 1.0f,
                H =     1.0f,
                D = Y + 1.0f;
    
    /*float cube[] = {
        // Right Face
        W, H, D,
        W, B, Y,
        W, H, Y,
        
        W, H, D,
        W, B, D,
        W, B, Y,
        
        // Front Face
        X, H, Y,
        W, H, Y,
        W, B, Y,
        
        X, H, Y,
        W, B, Y,
        X, B, Y,
        
        // Left Face
        X, H, Y,
        X, B, D,
        X, H, D,
        
        X, H, Y,
        X, B, Y,
        X, B, D,
        
        // Back Face
        X, H, D,
        X, B, D,
        W, B, D,
        
        X, H, D,
        W, B, D,
        W, H, D
    };*/
    
    glm::vec3 cube[] = {
        glm::vec3(W, H, D), glm::vec3( 1.0f, 0.0f,  0.0f), glm::vec3(W, B, Y), // right  side, top left corner  | i % 2 -> 0
        glm::vec3(W, H, Y), glm::vec3( 0.0f, 0.0f, -1.0f), glm::vec3(X, B, Y), // front  face, top left corner  | i % 2 -> 1
        glm::vec3(X, H, D), glm::vec3(-1.0f, 0.0f,  0.0f), glm::vec3(X, B, Y), // left   side, top right corner | i % 2 -> 0
        glm::vec3(W, H, D), glm::vec3( 0.0f, 0.0f,  1.0f), glm::vec3(X, B, D)  // bottom face, top right corner | i % 2 -> 1
    };
    bool hit = false;
    float finalDist = 999999.9f;
    int _i = 0;
    for(size_t i = 0; i < sizeof(cube)/sizeof(glm::vec3); i += 3){
        auto plane = cube[i];
        float dist = 0.0f;
        if(glm::intersectRayPlane(
            start,
            CAMERA.cameraFront,
            plane,
            cube[i + 1],
            dist
        )){
            dist = fabs(dist);
            auto worldPos = start + (CAMERA.cameraFront * dist);
            auto bounds = cube[i + 2];
            if(i % 2 == 0) { // check left and right, z and y
                if(!(
                    worldPos.z >= bounds.z && worldPos.z <= plane.z &&
                    worldPos.y >= bounds.y && worldPos.y <= plane.y
                )){
                    continue;
                }
            } else { // check north and south, x and y
                if(!(
                    worldPos.x >= bounds.x && worldPos.x <= plane.x &&
                    worldPos.y >= bounds.y && worldPos.y <= plane.y
                )){
                    continue;
                }
            }
            hit = true;
            if(finalDist > dist && dist != -INFINITY){
                finalDist = dist;
                _i = i;
            }
        }
    }
    std::cout << "--> " << cube[_i].x << ", " << cube[_i].y << "," << cube[_i].z << " - " << cube[_i + 1].x << ", " << cube[_i + 1].y << ", " << cube[_i + 1].z << std::endl;
    *out = start + (CAMERA.cameraFront * finalDist);
    return hit;
}

bool GameManager::castRayToEntities(glm::vec3 start, glm::vec3 dir, glm::vec3* worldPos, glm::vec3* intersectNormal, Entity** e) {
    float shortestDist = INFINITY;
    glm::vec3 closestPos(0.0f, 0.0f, 0.0f);
    glm::vec3 closestNormal(0.0f, 0.0f, 0.0f);
    Entity* closestEnt = nullptr;
    bool hit = false;
    for(auto e : currentLevel->entities){
        if(!e->shootable)
            continue;
        auto _start = start;
        //_start *= glm::vec3(e->radiusX, e->radiusY, e->radiusX);
        auto _dir = dir;
        //_dir *= glm::vec3(e->radiusX, e->radiusY, e->radiusX);
        auto _pos = e->position;
        //_pos *= glm::vec3(e->radiusX, e->radiusY, e->radiusX);
        glm::vec3 intersectPosition(0.0f, 0.0f, 0.0f);
        glm::vec3 intersectNormal(0.0f, 0.0f, 0.0f);
        if(glm::intersectRaySphere(
            _start,
            _dir,
            _pos,
            0.5f,
            intersectPosition,
            intersectNormal
        )){
            hit = true;
            auto dist = glm::length(intersectPosition - start);
            if(dist < shortestDist){
                shortestDist = dist;
                closestEnt = e;
                closestPos = intersectPosition;
                closestNormal = intersectNormal;
            }
        }
    }
    if(hit){
        *worldPos = closestPos;
        *intersectNormal = closestNormal;
        *e = closestEnt;
    }
    return hit;
}

bool GameManager::bfs(float startX, float startY, float endX, float endY, std::map<uint32_t, uint32_t> &cameFrom)
{
    int16_t x1 = (int16_t)startX,
            y1 = (int16_t)startY,
            x2 = (int16_t)endX,
            y2 = (int16_t)endY;
    
    std::queue<uint32_t> frontier;
    
    auto end = PACK_COORDS(x2, y2);
    auto start = PACK_COORDS(x1, y1);
    cameFrom[start] = PACK_COORDS(-1, -1);
    
    frontier.push(start);
    
    bool foundPath = false;
    
    while(!frontier.empty()){
        uint32_t current = frontier.front();
        frontier.pop();
        
        int16_t cX = UNPACK_X(current),
                cY = UNPACK_Y(current);

        if(current == end) {
            foundPath = true;
            break;
        }
        
        //top
        int16_t neighbors[] = {
            cX, cY - 1, // top
            cX + 1, cY, // right
            cX, cY + 1, // bottom
            cX - 1, cY  // left
        };
        
        for(size_t i = 0; i < sizeof(neighbors)/sizeof(int16_t); i += 2) {
            auto x = neighbors[i],
                 y = neighbors[i + 1];
            
            auto next = PACK_COORDS(x, y);
            bool notVisited = cameFrom.find(next) == cameFrom.end();
            if(notVisited && IN_BOUNDS(x, y) && NOT_SOLID(WALLS[COORDS(x, y)])){
                frontier.push(next);
                cameFrom[next] = current;
            }
        }
    }
    
    return foundPath;
}

void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos) {
    
    auto width = GraphicsManager::instance->getWidth();
    auto height = GraphicsManager::instance->getHeight();
    
    Camera* camera = &(GameManager::instance->camera);
    
    float sensitivity = 0.1f;
    float x = (xPos - width/2) * sensitivity;
    float y = (yPos - height/2) * sensitivity;
    
    camera->yaw += x;
    camera->pitch -= y;
    
    if(camera->pitch > 89.0f)
        camera->pitch = 89.0f;
    if(camera->pitch < -89.0f)
        camera->pitch = -89.0f;
        
    glm::vec3 front;
    front.x = cos(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
    front.y = sin(glm::radians(camera->pitch));
    front.z = sin(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
    camera->cameraFront = glm::normalize(front);
    if(glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
        glfwSetCursorPos(window, width/2, height/2);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if(button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
        (&PLAYER)->shoot();
    }
}

void GameManager::printPath(std::map<uint32_t, uint32_t> &path, uint32_t start, uint32_t end) {
    auto node = start;
    std::cout << "==========" << std::endl;
    int x = UNPACK_X(node),
        y = UNPACK_Y(node);
    while(node != end && node != 0) {
        std::cout << x << ", " << y << std::endl;
        node = path[node];
        x = UNPACK_X(node);
        y = UNPACK_Y(node);
    }
    std::cout << x << ", " << y << std::endl;
}