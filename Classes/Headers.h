#pragma once

//C++
#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>
#include <iterator>
#include <future>

using namespace std;

//Cocos
#include "cocos2d.h"
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#include "ui/CocosGUI.h"

using namespace NS_CC;

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    #define USE_TOUCH 1
    #define USE_JOYSTICK 1
#else
    #define USE_TOUCH 0
    #define USE_JOYSTICK 1
#endif
#define DEBUG_SIGHT 0
#define DEBUG_ENEMY 0
#define DEBUG_GRAHP 0
#define DISIBLE_AI 0
#define ENABLE_FOG 1
#define VISIBLE_ENEMY 1
#define CHEAT 1
