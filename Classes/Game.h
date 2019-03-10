#pragma once
#include "Defines/Templates.h"
#include "Headers.h"

class Character;
class Player;

class Game : public pattern::Singleton<Game>
{
    Layer* _currentState;
    
    shared_ptr<Player> _player;
    
    bool _isHoldKey;
    vector<EventKeyboard::KeyCode> _keyIsHolds;
    
private:
    
    
public:
    Game();
    ~Game();
    
    void init();
    void update(float dt);
    void setCurrentState(Layer* layer);
    void handleKeyboardPress(EventKeyboard::KeyCode, Event*);   //used in gameplay
    void handleKeyboardHold();
    void handleKeyboardRelease(EventKeyboard::KeyCode, Event*); //used in gameplay
    
    void createCharacters();
    void handleMovePlayerKeyCode(EventKeyboard::KeyCode keycode);
};
