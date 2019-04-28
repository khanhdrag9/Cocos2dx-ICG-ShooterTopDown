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
    _resMap[res::define::IMG_HOME_SCREEN_1] = "HomeScene.jpg";
    _resMap[res::define::IMG_HOME_SCREEN_1_NO_TITLE] = "HomeSceneWithoutTitle.jpg";
    _resMap[res::define::IMG_BG_1] = "BG.jpg";
    _resMap[res::define::IMG_FOG] = "Fog.jpg";
	_resMap[res::define::IMG_BLACK] = "Black.jpg";
    

	//image Map
	_resMap[res::define::IMG_MAP1] = "Map/MapImage/MapImage1.jpg";
	_resMap[res::define::IMG_MAP2] = "Map/MapImage/MapImage2.jpg";
	_resMap[res::define::IMG_MAP3] = "Map/MapImage/MapImage3.jpg";
    
    //Map link
    _resMap[res::define::MAP1] = "Map/Map1.tmx";
    _resMap[res::define::MAP2] = "Map/Map2.tmx";
    _resMap[res::define::MAP3] = "Map/Map3.tmx";
    
    //UI
    _resMap[res::define::BTN_PLAY] = "Map/MapImage/ButtonPlay.png";
    _resMap[res::define::BTN_BLUE] = "Assets/PNG/UI/buttonBlue.png";
    _resMap[res::define::BTN_GREEN] = "Assets/PNG/UI/buttonGreen.png";
    _resMap[res::define::BTN_RED] = "Assets/PNG/UI/buttonRed.png";
    _resMap[res::define::BTN_YELLOW] = "Assets/PNG/UI/buttonYellow.png";
	_resMap[res::define::BTN_OPTION] = "";
    _resMap[res::define::CURSOR_UI] = "Assets/PNG/UI/cursor.png";
    _resMap[res::define::FONT_KENVECTOR_FUTURE_THIN] = "Assets/Bonus/kenvector_future_thin.ttf";
    _resMap[res::define::FONT_ARIAL] = "fonts/arial.ttf";
    
    //musics
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    _resMap[res::define::MUSIC_ACTION_FIGHT] = "SFX/action_fight.mp3";
    _resMap[res::define::MUSIC_READY_TO_FIGHT] = "SFX/ready_to_fight.mp3";
    _resMap[res::define::MUSIC_BOSS_FIGHT] = "SFX/boss_fight.mp3";
#else
    _resMap[res::define::MUSIC_ACTION_FIGHT] = "action_fight.mp3";
    _resMap[res::define::MUSIC_READY_TO_FIGHT] = "ready_to_fight.mp3";
    _resMap[res::define::MUSIC_BOSS_FIGHT] = "boss_fight.mp3";
#endif
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic(at(res::define::MUSIC_ACTION_FIGHT).c_str());
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic(at(res::define::MUSIC_READY_TO_FIGHT).c_str());
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic(at(res::define::MUSIC_BOSS_FIGHT).c_str());
    
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    
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

	//character
	_resMap[res::define::CHARACTER_TRIANGLE] = "";
	_resMap[res::define::CHARACTER_SQUARE] = "";
	_resMap[res::define::CHARACTER_CIRCLE] = "";
    
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
    
    
    _listCharacterCreations.clear();
    
    _listCharacterCreations = vector<CharacterCreation>{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
        CharacterCreation("CharacterAssets/CircleCharacter.png",
                          "CharacterAssets/CircleImage.jpg",
                          "",
                          "CharacterAssets/CircleBullet.png",
                          100, 250, 30, 0.25, 20),
        CharacterCreation("CharacterAssets/TriangleCharacter.png",
                          "CharacterAssets/TriangleImage.jpg",
                          "",
                          "CharacterAssets/TriangleBullet.png",
                          70, 350, 45, 0.15, 10),
        CharacterCreation("CharacterAssets/RectCharacter.png",
                          "CharacterAssets/RectImage.jpg",
                          "",
                          "CharacterAssets/RectBullet.png",
                          200, 200, 15, 0.35, 40),
#else
        CharacterCreation("CircleCharacter.png",
                          "CircleImage.jpg",
                          "CircleLoad.png",
                          "CircleBullet.png",
                          100, 250, 30, 0.25, 20),
        CharacterCreation("TriangleCharacter.png",
                          "TriangleImage.jpg",
                          "TriangleLoad.png",
                          "TriangleBullet.png",
                          70, 350, 45, 0.15, 10),
        CharacterCreation("RectCharacter.png",
                          "RectImage.jpg",
                          "RectLoad.png",
                          "RectBullet.png",
                          200, 200, 15, 0.35, 40),
#endif
    };
}

ResourceManager::~ResourceManager()
{
    _resMap.clear();
}

const std::string& ResourceManager::at(const res::define& name) const
{
    return _resMap.at(name);
}

vector<CharacterCreation>& ResourceManager::getListCharacterCreation()
{
    return _listCharacterCreations;
}
