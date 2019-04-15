//
//  ResoureManager.cpp
//  Shooter
//
//  Created by Khanh on 3/16/19.
//

#include "ResourceManager.h"


ResourceManager::ResourceManager()
{
    //image
    _resMap[res::define::SPLASH] = "splashKW.jpg";
    _resMap[res::define::IMG_BG_1] = "BG.jpg";
    _resMap[res::define::IMG_FOG] = "Fog.jpg";
    
    //UI
    _resMap[res::define::BTN_BLUE] = "Assets/PNG/UI/buttonBlue.png";
    _resMap[res::define::BTN_GREEN] = "Assets/PNG/UI/buttonGreen.png";
    _resMap[res::define::BTN_RED] = "Assets/PNG/UI/buttonRed.png";
    _resMap[res::define::BTN_YELLOW] = "Assets/PNG/UI/buttonYellow.png";
    _resMap[res::define::CURSOR_UI] = "Assets/PNG/UI/cursor.png";
    _resMap[res::define::FONT_KENVECTOR_FUTURE_THIN] = "Assets/Bonus/kenvector_future_thin.ttf";
    _resMap[res::define::FONT_ARIAL] = "fonts/arial.ttf";
    
    //player
    _resMap[res::define::PLAYER_1_BLUE] = "Assets/PNG/playerShip1_blue.png";
    _resMap[res::define::PLAYER_1_GREEN] = "Assets/PNG/playerShip1_green.png";
    _resMap[res::define::PLAYER_1_ORANGE] = "Assets/PNG/playerShip1_orange.png";
    _resMap[res::define::PLAYER_1_RED] = "Assets/PNG/playerShip1_red.png";
    
    _resMap[res::define::PLAYER_2_BLUE] = "Assets/PNG/playerShip2_blue.png";
    _resMap[res::define::PLAYER_2_GREEN] = "Assets/PNG/playerShip2_green.png";
    _resMap[res::define::PLAYER_2_ORANGE] = "Assets/PNG/playerShip2_orange.png";
    _resMap[res::define::PLAYER_2_RED] = "Assets/PNG/playerShip2_red.png";
    
    _resMap[res::define::PLAYER_3_BLUE] = "Assets/PNG/playerShip3_blue.png";
    _resMap[res::define::PLAYER_3_GREEN] = "Assets/PNG/playerShip3_green.png";
    _resMap[res::define::PLAYER_3_ORANGE] = "Assets/PNG/playerShip3_orange.png";
    _resMap[res::define::PLAYER_3_RED] = "Assets/PNG/playerShip3_red.png";
    
    //enemy
    _resMap[res::define::ENEMY_1_BLUE] = "Assets/PNG/ufoBlue.png";
    _resMap[res::define::ENEMY_1_GREEN] = "Assets/PNG/ufoGreen.png";
    _resMap[res::define::ENEMY_1_RED] = "Assets/PNG/ufoRed.png";
    //_resMap[res::define::ENEMY_1_YELLOW] = "Assets/PNG/ufoYellow.png";
    _resMap[res::define::ENEMY_1_YELLOW] = "Assets/PNG/playerShip2_orange.png";
    
    //bullet
    _resMap[res::define::BULLET_BLUE_1] = "Assets/PNG/Lasers/laserBlue01.png";
    _resMap[res::define::BULLET_GREEN_1] = "Assets/PNG/Lasers/laserGreen01.png";
    _resMap[res::define::BULLET_RED_1] = "Assets/PNG/Lasers/laserRed01.png";
}

ResourceManager::~ResourceManager()
{
    _resMap.clear();
}

const std::string& ResourceManager::at(const res::define& name) const
{
    return _resMap.at(name);
}
