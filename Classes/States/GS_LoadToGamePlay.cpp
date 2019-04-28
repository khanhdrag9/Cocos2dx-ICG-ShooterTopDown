//
//  GS_LoadToGamePlay.cpp
//  ShooterTopDown2D
//
//  Created by Khanh on 4/28/19.
//

#include "GS_LoadToGamePlay.h"
#include "../Game.h"
#include "../Bot/BotManager.h"
#include "../Resource/ResourceManager.h"
#include "GS_GamePlay.h"

GS_LoadToGamePlay::GS_LoadToGamePlay():
_loadfinish(false),
_countTime(0.f),
_gameplay(nullptr)
{
}

Scene* GS_LoadToGamePlay::createScene()
{
    Scene* scene = Scene::create();
    GS_LoadToGamePlay* layer = GS_LoadToGamePlay::create();
    scene->addChild(layer);
    return scene;
}

bool GS_LoadToGamePlay::init()
{
    if(!Layer::init())
        return false;
    
    Size sz = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 center = Vec2(sz.width / 2.f + origin.x, sz.height / 2.f + origin.y);
    
    Game* game = Game::getInstance();
    ResourceManager* res = ResourceManager::getInstance();
    
    //BG game
    auto gamemap = game->getCurrentMap();
    auto BG = Sprite::create(gamemap->linkImage);
    BG->setContentSize(this->getContentSize());
    BG->setPosition(center);
    this->addChild(BG, 0);
    
    auto black = Sprite::create("Black.jpg");
    black->setPosition(center);
    black->setOpacity(150);
    this->addChild(black, 1);
    
    
    float row1 = center.y + center.y / 2.f;
    float row2 = center.y - center.y / 2.f;
    //player
    auto playerCreation = game->getPlayerCreation();
    Sprite* player = Sprite::create(playerCreation->getLoadImage());
    this->addChild(player, 2);
    player->setPosition(center.x, row1);
    
    //Bot
    auto listCreations = res->getListCharacterCreation();
    vector<CharacterCreation> creationsBot{
        listCreations[random(0, (int)listCreations.size() - 1)],
        listCreations[random(0, (int)listCreations.size() - 1)]
    };
    BotManager::getInstance()->setBotCreations(listCreations);
    
    const int sizeBot = (int)listCreations.size();
    for(int i = 0; i <sizeBot; i++)
    {
        Sprite* sprite = Sprite::create(listCreations[i].getLoadImage());
        sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        this->addChild(sprite, 2);
        float width = sprite->getContentSize().width;
        Vec2 position = Vec2(center.x - (width * sizeBot) / 2.f + i * width, row2);
        sprite->setPosition(position);
    }
    
    //_loadGame = thread(&GS_LoadToGamePlay::loadGame, this);
    this->scheduleUpdate();
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    
    return true;
}

void GS_LoadToGamePlay::update(float dt)
{
//    if(_countTime == 0.f)
//        loadGame();
    
    if(_countTime > 3.f && !_gameplay)
    {
        loadGame();
    }
    
    _countTime += dt;
}

void GS_LoadToGamePlay::loadGame()
{
    _gameplay = GS_GamePlay::createScene();
//    Game::getInstance()->initGamePlay();
    Director::getInstance()->replaceScene(TransitionFade::create(1.f, _gameplay));
}
