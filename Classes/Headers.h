#pragma once

//system
#include <iostream>
#include <memory>
#include <map>
#include <queue>
#include <array>

using namespace std;
//cocos
#include "cocos2d.h"

USING_NS_CC;

//game
#define PLAYER_PATH "sprite.png"

#define AIM1 "aim/aim1.png"

#define BULLET1 "bullet/bullet1.png"
#define BULLET1_INTERVAL 0.3f
#define BULLET_SPEED 0.7f


//physics define in game

#define PHYSICS_PLAYER 0x00000001
#define PHYSICS_EDGE 0x00000002
#define PHYSICS_BULLET_PLAYER 0x00000001
#define PHYSICS_BULLET_BOT 0x00000004