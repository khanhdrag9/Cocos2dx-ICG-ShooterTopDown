#pragma once
#include "Defines/Templates.h"
#include "Headers.h"

class Character;
class Player;
class Bot;
class RigidWorld;
class Vision;

class Game : public pattern::Singleton<Game>
{
    enum class direction : int
    {
        NONE,
        UP,
        DOWN,
        LEFT,
        RIGHT
    };
    
    
private:
    Layer* _currentState;
    
    shared_ptr<Player> _player;
    float _counttimePlayerShoot;
    float _intervelPlayerShoot;
    
    bool _isHoldKey;
    vector<EventKeyboard::KeyCode> _keyIsHolds;
    
	TMXTiledMap* _tileMap;
	TMXLayer* _backgroundLayer;
	TMXLayer* _collisionLayer;

	shared_ptr<Character> _objIsFollow;

	unique_ptr<RigidWorld> _rigidWorld;
    
    DrawNode* _sightNode;
    
#if DEBUG_SIGHT
    DrawNode* _debugWall;
#endif
    Sprite* _fogSprite;
    ClippingNode* _fogClip;
    list<shared_ptr<Vision>> _listVision;
    
public:
    Game();
    ~Game();
    
    void init();
    void initGamePlay();
    void update(float dt);
    void setCurrentState(Layer* layer);
    Layer* getCurrentState();
	void setObjectFollowByCam(shared_ptr<Character> object);
    
    void handleKeyboardPress(EventKeyboard::KeyCode, Event*);   //used in gameplay
    void handleKeyboardHold();
    void handleKeyboardRelease(EventKeyboard::KeyCode, Event*); //used in gameplay
    
    bool handleTouchBegan(Touch* touch, Event* event);
    void handleTouchMoved(Touch* touch, Event* event);
    void handleTouchRelease(Touch* touch, Event* event);
	void handleShootCharacter(shared_ptr<Character> object, const float& speed);

    const unique_ptr<RigidWorld>& getRigidWord() const;
    void releaseGamePlay();

	TMXTiledMap* getTileMap() const;

private:
    shared_ptr<Player> createAPlayer();
    
    void handleMovePlayerKeyCode(EventKeyboard::KeyCode keycode);
    void handleMovePlayer(shared_ptr<Player> player, const direction& direct);
    void updateAngle(shared_ptr<Character> object, const Vec2& point);

	void createPhysicsWorld();
	void createMap();
	void createMainPlayer();
	void createStartCameraView();
	void updateCameraView();
    void createSight();
    
    void updatePhysics(float dt);
    void updateSight(float dt);
    
};
