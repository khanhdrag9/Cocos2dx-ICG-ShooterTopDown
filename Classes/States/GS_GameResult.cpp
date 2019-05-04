//
//  GS_GameResult.cpp
//  ShooterTopDown2D
//
//  Created by Khanh on 4/30/19.
//

#include "GS_GameResult.h"
#include "../Game.h"
#include "../Bot/BotManager.h"
#include "../Resource/ResourceManager.h"

bool GS_GameResult::init()
{
    if(!Layer::init())
        return false;
    
    Size sz = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 center = Vec2(origin.x + sz.width / 2.f, origin.y + sz.height / 2.f);
    
    auto playerCreation = Game::getInstance()->getPlayerCreation();
    auto botCreations = BotManager::getInstance()->getBotCreations();
    
    ResourceManager* resMgr = ResourceManager::getInstance();
    
    //Background
    Sprite* backgroud = Sprite::create(resMgr->at(res::define::IMG_BLACK));
    backgroud->setPosition(center);
    Size bgSize = Size(sz.width, sz.height * 0.98);
    backgroud->setContentSize(bgSize);
    backgroud->setOpacity(180);
    this->addChild(backgroud, 0);
    
//    float column0 = origin.x + sz.width * 0.12f
    float column1 = origin.x + sz.width * 0.36f;
    float column2 = origin.x + sz.width * 0.64f;
    float column3 = origin.x + sz.width * 0.88f;
    float yaxis = origin.y + bgSize.height * 0.48f;
    
    Game::game_result result = Game::getInstance()->getGameResult();
    //player
    Sprite* player = Sprite::create(playerCreation->getLoadImage());
    backgroud->addChild(player, 2);
    Size playerSize = player->getContentSize();
    player->setPosition(column1, yaxis);
    if(result == Game::game_result::LOSE)
    {
        Sprite* x = Sprite::create("CharacterAssets/XLose.png");
        x->setPosition(playerSize.width / 2.f, playerSize.height / 2.f);
        player->addChild(x, 3);
    }
    
    //Bot
    const int sizeBot = (int)botCreations.size();
    for(int i = 0; i < sizeBot; i++)
    {
        Sprite* sprite = Sprite::create(botCreations[i].getLoadImage());
        sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        backgroud->addChild(sprite, 2);
        float height = sprite->getContentSize().height;
        float column = i < 2 ? column2 : column3;
        int order = i%2==0 ? 1 : 0;
        Vec2 position = Vec2(column ,yaxis - (height * 2) / 2.f + order * height);
        sprite->setPosition(position);
        
        if(result == Game::game_result::WIN)
        {
            Sprite* x = Sprite::create("CharacterAssets/XLose.png");
            Size enemySize = sprite->getContentSize();
            x->setPosition(enemySize.width / 2.f, enemySize.height / 2.f);
            sprite->addChild(x, 3);
        }
    }
    
//    Sprite* titleResult;
//    if(result == Game::game_result::WIN)
//        titleResult = Sprite::create("WinTitle.png");
//    else titleResult = Sprite::create("LoseTitle.png");
    ui::Button* titleResult;
    string soundEffect = "";
    string musicsEffect = "";
    if(result == Game::game_result::WIN)
    {
        titleResult = ui::Button::create("WinTitle.png");
        soundEffect = resMgr->at(res::define::SOUND_VICTORY);
        musicsEffect = resMgr->at(res::define::MUSIC_VICTORY);
    }
    else
    {
        titleResult = ui::Button::create("LoseTitle.png");
        soundEffect = resMgr->at(res::define::SOUND_DEFEAT);
        musicsEffect = resMgr->at(res::define::MUSIC_DEFEAT);
    }
    
    if(Game::getInstance()->isEnableVolumn())
    {
        SimpleAudioEngine* audio = SimpleAudioEngine::getInstance();
        if(musicsEffect != "")audio->playBackgroundMusic(musicsEffect.c_str());
        if(soundEffect != "")audio->playEffect(soundEffect.c_str());
    }
    
    titleResult->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    titleResult->setPosition(Vec2(center.x, origin.y + sz.height * 0.99));
    titleResult->addTouchEventListener([](Ref*, ui::Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            Game::getInstance()->backToHomeMenu();
        }
    });
    this->addChild(titleResult, 1);
    
    
    return true;
}
