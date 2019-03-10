//
//  Game.cpp
//  Shooter
//
//  Created by Khanh on 3/9/19.
//

#include "Game.h"
#include "Characters/Player.h"
#include "Commands/CommandMoveBy.h"

Game::Game():
_currentState(nullptr),
_player(nullptr),
_isHoldKey(false)
{
    
}

Game::~Game()
{
    
}

void Game::init()
{
    
}

void Game::update(float dt)
{
    handleKeyboardHold();
    
    _player->update(dt);
}

void Game::setCurrentState(Layer* layer)
{
    _currentState = layer;
}

void Game::handleKeyboardPress(EventKeyboard::KeyCode keycode, Event*)  //used in gameplay
{
    _isHoldKey = true;
    _keyIsHolds.push_back(keycode);
}

void Game::handleKeyboardHold()
{
    if(_isHoldKey)
    {
        for(auto& key : _keyIsHolds)
            handleMovePlayerKeyCode(key);
    }
}

void Game::handleKeyboardRelease(EventKeyboard::KeyCode keycode, Event*)    //used in gameplay
{
    for(int i = 0; i < _keyIsHolds.size(); i++)
    {
        if(_keyIsHolds[i] == keycode)
        {
            _keyIsHolds.erase(_keyIsHolds.begin() + i);
            break;
        }
    }
    
    if(_keyIsHolds.size() == 0)_isHoldKey = false;
}

void Game::createCharacters()
{
    _player = make_shared<Player>();
    _player->init();
    
    
    _currentState->addChild(_player->_sprite);
}

void Game::handleMovePlayerKeyCode(EventKeyboard::KeyCode keycode)
{
    Vec2 movespeed;
    shared_ptr<Command> cmd;
    
    switch (keycode) {
        case cocos2d::EventKeyboard::KeyCode::KEY_W:
            movespeed.y = _player->getSpeedMove();
            cmd = CommandMoveBy::createCommandMoveBy(movespeed, 0.001);
            _player->pushCommand(cmd);
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_S:
            movespeed.y = -_player->getSpeedMove();
            cmd = CommandMoveBy::createCommandMoveBy(movespeed, 0.001);
            _player->pushCommand(cmd);
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_A:
            movespeed.x = -_player->getSpeedMove();
            cmd = CommandMoveBy::createCommandMoveBy(movespeed, 0.001);
            _player->pushCommand(cmd);
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_D:
            movespeed.x = _player->getSpeedMove();
            cmd = CommandMoveBy::createCommandMoveBy(movespeed, 0.001);
            _player->pushCommand(cmd);
            break;
        default:
            break;
    }
}
