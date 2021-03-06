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
#define TITLEMAP_PATH "Map/Map1.tmx"

#define PLAYER_SQUARE_PATH "sprite.png"
#define BOT_SQUARE_PATH "sprite.png"

#define AIM1 "aim/aim1.png"

#define PLAYER_SPEED_NORMAL 5.f
#define BOT_HANDLE_INTERVAL 0.f

#define BULLET1 "bullet/bullet1.png"
#define BULLET1_INTERVAL 0.75f
#define BULLET1_SPEED 1000.f


//constants
namespace constants
{
    const string nameEnemyGroup = "Enemy";
    const string preNameEnemy = "enemyPos";
    const string nameEnemyMoveMap = "EnemyMoveMap";
    const string preMoveEnemy = "enemystep";
}

enum objecttag
{
	BULLET,
	PLAYER,
	ENEMY
};


//physics define in game
#define PHYSICS_PLAYER 0x00000001
#define PHYSICS_ENEMY 0x00000002
#define PHYSICS_EDGE 0x00000003
#define PHYSICS_BULLET_PLAYER 0x00000006
#define PHYSICS_BULLET_BOT 0x00000004

//template
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
#endif

template<class T>
class Singleton
{
public:
	static T* getInstance()
	{
		static T* instance{ new T() };
		return instance;
	}
};
