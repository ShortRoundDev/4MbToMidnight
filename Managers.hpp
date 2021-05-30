#pragma once
#include "GameManager.hpp"
#include "GraphicsManager.hpp"

#define CAMERA (GameManager::instance->camera)
#define WALLS (GameManager::instance->currentLevel->walls)
#define LEVEL (GameManager::instance->currentLevel)
#define PLAYER (GameManager::instance->player)

#define SHADERS (GraphicsManager::shaders)
#define SCREEN_X(x) (GraphicsManager::px2scrnX(x))
#define SCREEN_Y(y) (GraphicsManager::px2scrnY(y))
#define SCREEN_W(w) (GraphicsManager::scrnscaleX(w))
#define SCREEN_H(h) (GraphicsManager::scrnscaleY(h))

#define COORDS(x, y) ((x) + ((y) * LEVEL->width))

#define IS_DOOR(t) ((t).wallTexture >= 100 && (t).wallTexture <= 104)
#define NOT_SOLID(t) ((t).wallTexture == 0 || (IS_DOOR(t) && (t).isOpen))
#define SOLID(t) (!NOT_SOLID(t))
#define IN_BOUNDS(x, y) ((x) >= 0 && (x) < LEVEL->width && (y) >= 0 && (y) < LEVEL->height)
#define NOT_IN_BOUNDS(x, y) (!IN_BOUNDS(x, y))