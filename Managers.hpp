#pragma once
#include "GameManager.hpp"
#include "GraphicsManager.hpp"
#include "SoundManager.hpp"

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
#define NOT_SOLID(t) ((!(t).isSolid) || (t).wallTexture == 0 || (IS_DOOR(t) && (t).isOpen))
#define SOLID(t) (!NOT_SOLID(t))
#define IN_BOUNDS(x, y) ((x) >= 0 && (x) < LEVEL->width && (y) >= 0 && (y) < LEVEL->height)
#define NOT_IN_BOUNDS(x, y) (!IN_BOUNDS(x, y))

#define PLAY_S(s, p) (SoundManager::instance->playSound((s), (p)))
#define PLAY_I(i, p) (SoundManager::instance->playSound((i), (p)))

#define PRINT(str, x, y, siz) (GameManager::instance->print((str), (x), (y), (siz)))

#define PACK_COORDS(x, y) ((((uint32_t)(x)) << 16) | (y))
#define UNPACK_X(x) (((x) & 0xffff0000) >> 16)
#define UNPACK_Y(y) ((y) & 0x0000ffff)

#define PRINT_VEC3(v) (v.x) << ", " << (v.y) << ", " << (v.z)

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define M_PI 3.14159
#define M_PI_2 (M_PI/2.0)
#endif