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
    
    
    _listBulletCreations.clear();
#if (CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
#else
    _listBulletCreations[res::define::BULLET_TRIANGLE] = BulletCreation("TriangleBullet.png", 10, "VFX/polygonGreen.plist");
    _listBulletCreations[res::define::BULLET_RECT] = BulletCreation("RectBullet.png", 40, "VFX/rectFire.plist");
    _listBulletCreations[res::define::BULLET_CIRCLE] = BulletCreation("CircleBullet.png", 20, "VFX/circleBlue.plist");
#endif
    
    _listCharacterCreations.clear();
#if (CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
#else
    _listCharacterCreations.push_back(CharacterCreation("CircleCharacter.png",
                                                                                "CircleImage.jpg",
                                                                                "CircleLoad.png",
                                                                                100, 250, 30, 0.25,
                                                                                _listBulletCreations[res::define::BULLET_CIRCLE]));
    _listCharacterCreations.push_back(CharacterCreation("RectCharacter.png",
                                                                            "RectImage.jpg",
                                                                            "RectLoad.png",
                                                                            200, 200, 15, 0.35,
                                                                            _listBulletCreations[res::define::BULLET_RECT]));
    _listCharacterCreations.push_back(CharacterCreation("TriangleCharacter.png",
                                                                                "TriangleImage.jpg",
                                                                                "TriangleLoad.png",
                                                                                70, 350, 45, 0.15,
                                                                                _listBulletCreations[res::define::BULLET_TRIANGLE]));
#endif
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

map<res::define, BulletCreation>& ResourceManager::getListBulletCreation()
{
    return _listBulletCreations;
}
