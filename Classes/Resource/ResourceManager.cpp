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
    _resMap[res::define::IMG_WIN_TITLE] = "WinTitle.png";
    _resMap[res::define::IMG_LOSE_TITLE] = "LoseTitle.png";
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Titles.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("OptionAssets/Options.plist");

	//image Map
	_resMap[res::define::IMG_MAP1] = "Map/MapImage/MapImage1.jpg";
	_resMap[res::define::IMG_MAP2] = "Map/MapImage/MapImage2.jpg";
	_resMap[res::define::IMG_MAP3] = "Map/MapImage/MapImage3.jpg";
    
    //Map link
    _resMap[res::define::MAP1] = "Map/Map1.tmx";
    _resMap[res::define::MAP2] = "Map/Map2.tmx";
    _resMap[res::define::MAP3] = "Map/Map3.tmx";
    
    //UI
    _resMap[res::define::BTN] = "Button.png";
    _resMap[res::define::FONT_KENVECTOR_FUTURE_THIN] = "fonts/kenvector_future_thin.ttf";
    _resMap[res::define::FONT_ARIAL] = "fonts/arial.ttf";
    _resMap[res::define::LOADINGBAR_HP] = "CharacterAssets/HPLoadingUpdate.png";
    _resMap[res::define::LOADINGBAR_BG_HP] = "CharacterAssets/HPLoadingBG.png";
    
    //musics
    _resMap[res::define::MUSIC_ACTION_FIGHT] = "SFX/action_fight.mp3";
    _resMap[res::define::MUSIC_READY_TO_FIGHT] = "SFX/ready_to_fight.mp3";
    _resMap[res::define::MUSIC_BOSS_FIGHT] = "SFX/boss_fight.mp3";
    _resMap[res::define::MUSIC_VICTORY] = "SFX/VictoryTheme.mp3";
    _resMap[res::define::MUSIC_DEFEAT] = "SFX/DefeatTheme.mp3";
    
    _resMap[res::define::SOUND_VICTORY] = "SFX/Victory.mp3";
    _resMap[res::define::SOUND_DEFEAT] = "SFX/Defeat.mp3";
    _resMap[res::define::SOUND_FIRSTBLOOD] = "SFX/FirstBlood.mp3";
    _resMap[res::define::SOUND_DOUBLEKILL] = "SFX/Doublekill.mp3";
    _resMap[res::define::SOUND_TRIPLEKILL] = "SFX/Triplekill.mp3";
    _resMap[res::define::SOUND_QUADRAKILL] = "SFX/Quadrakill.mp3";
    
    SimpleAudioEngine* audio = SimpleAudioEngine::getInstance();
    
    audio->preloadBackgroundMusic(at(res::define::MUSIC_ACTION_FIGHT).c_str());
    audio->preloadBackgroundMusic(at(res::define::MUSIC_READY_TO_FIGHT).c_str());
    audio->preloadBackgroundMusic(at(res::define::MUSIC_BOSS_FIGHT).c_str());
    audio->preloadEffect(at(res::define::SOUND_VICTORY).c_str());
    audio->preloadEffect(at(res::define::SOUND_DEFEAT).c_str());
    audio->preloadEffect(at(res::define::SOUND_FIRSTBLOOD).c_str());
    audio->preloadEffect(at(res::define::SOUND_DOUBLEKILL).c_str());
    audio->preloadEffect(at(res::define::SOUND_TRIPLEKILL).c_str());
    audio->preloadEffect(at(res::define::SOUND_QUADRAKILL).c_str());
    
    audio->stopBackgroundMusic();
    audio->stopAllEffects();
    
    //sounds   
    _listBulletCreations.clear();
    _listBulletCreations[res::define::BULLET_TRIANGLE] = BulletCreation("CharacterAssets/TriangleBullet.png",
                                                                        "CharacterAssets/TriBasicBullet.png",
                                                                        10, 750.f, "VFX/polygonGreen.plist",
                                                                        "VFX/expGreen.plist",
                                                                        "SFX/laser6.mp3",
                                                                        "SFX/laser_exp.mp3");
    
    _listBulletCreations[res::define::BULLET_RECT] = BulletCreation("CharacterAssets/RectBullet.png",
                                                                    "CharacterAssets/RectBasicBullet.png",
                                                                    40, 500.f,
                                                                    "VFX/rectFire.plist",
                                                                    "VFX/expFire.plist",
                                                                    "SFX/Fire_shoot.mp3",
                                                                    "SFX/Fire_exp.mp3");
    
    _listBulletCreations[res::define::BULLET_CIRCLE] = BulletCreation("CharacterAssets/CircleBullet.png",
                                                                      "CharacterAssets/CircleBasicBullet.png",
                                                                      20, 625.f,
                                                                      "VFX/circleBlue.plist",
                                                                      "VFX/expBlue.plist",
                                                                      "SFX/laser2.mp3",
                                                                      "SFX/lowDown.mp3");
    
    _listCharacterCreations.clear();
    _listCharacterCreations.push_back(CharacterCreation("CharacterAssets/CircleCharacter.png",
                                                                                "CharacterAssets/CircleImage.jpg",
                                                                                "CharacterAssets/CircleLoad.png",
                                                                                "VFX/dieBlue.plist",
                                                                                "SFX/Lose.mp3",
                                                                                100, 250, 30, 0.35,
                                                                                _listBulletCreations[res::define::BULLET_CIRCLE]));
    _listCharacterCreations.push_back(CharacterCreation("CharacterAssets/RectCharacter.png",
                                                                            "CharacterAssets/RectImage.jpg",
                                                                            "CharacterAssets/RectLoad.png",
                                                                            "VFX/dieFire.plist",
                                                                            "SFX/Lose.mp3",
                                                                            200, 200, 15, 0.6,
                                                                            _listBulletCreations[res::define::BULLET_RECT]));
    _listCharacterCreations.push_back(CharacterCreation("CharacterAssets/TriangleCharacter.png",
                                                                                "CharacterAssets/TriangleImage.jpg",
                                                                                "CharacterAssets/TriangleLoad.png",
                                                                                "VFX/dieGreen.plist",
                                                                                "SFX/Lose.mp3",
                                                                                70, 350, 45, 0.15,
                                                                                _listBulletCreations[res::define::BULLET_TRIANGLE]));
    
    
    //preload sounds
    for(auto& creation : _listCharacterCreations)
    {
        audio->preloadEffect(creation.getDieSound().c_str());
        audio->preloadEffect(creation._bullet.getShootSound().c_str());
        audio->preloadEffect(creation._bullet.getExplosionSound().c_str());
    }
    audio->setEffectsVolume(75);

}

ResourceManager::~ResourceManager()
{
    clear();
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

void ResourceManager::clear()
{
    _resMap.clear();
    _listBulletCreations.clear();
    _listCharacterCreations.clear();
}
