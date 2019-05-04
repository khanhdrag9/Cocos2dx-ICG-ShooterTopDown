#pragma once
#include "Defines/Templates.h"
#include "Headers.h"

class CharacterCreation;
class Character;
class Player;
class Bot;
class RigidWorld;
class Vision;

class Game : public pattern::Singleton<Game>
{
public:
    enum class direction : int
    {
        NONE,
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

	enum type_vision
	{
		VISION_PLAYER,
		VISION_ENEMY
	};
    
    struct GameMap   //first is name, second is link tile map
    {
        string name;
        string linkMap;
        string linkImage;
	
        GameMap(const string& n, const string& lm, const string& li):
        name(n), linkMap(lm), linkImage(li)
        {}
    };
    
    enum layer
    {
        GAMELABEL = 2,
        PICKMAP = 3,
        GAMEPLAY,
        OPTION = 100
    };
    
    enum game_result
    {
        NONE,
        WIN,
        LOSE
    };
    
private:
    Layer* _currentState;
    
    shared_ptr<Player> _player;
    CC_SYNTHESIZE(int, _playerKills, PlayerKills);
	bool _playerShoot;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
    bool _isHoldKey;
    vector<EventKeyboard::KeyCode> _keyIsHolds;
#endif
    
    vector<GameMap> _listMaps;
    
	TMXTiledMap* _tileMap;
    GameMap* _linkMap;
    CharacterCreation* _playerCreation;

	shared_ptr<Character> _objIsFollow;
#if CHEAT
	int _currentIndexFollow;
#endif

	unique_ptr<RigidWorld> _rigidWorld;
    
    DrawNode* _sightNode;
    
#if DEBUG_SIGHT
    DrawNode* _debugWall;
#endif
    Sprite* _fogSprite;
    ClippingNode* _fogClip;
    list<shared_ptr<Vision>> _listVision;

	vector<Vec2> _revivalPosition;
    
    bool _isPopupInGameVisible;
    bool _isPopupKDAVisible;
    bool _enableVolumn;
    game_result _result;
    bool _isEndGame;
    bool _outGame;
    float _countTime = 0.f;
    condition_variable _threadSightAvaiable;
    bool _threadSightReady;
	thread _sight;
    mutex _m;
public:
    Game();
    ~Game();
    
    void init();
    void initGamePlay();
    void update(float dt);
    void setCurrentState(Layer* layer);
    Layer* getCurrentState();
	void setObjectFollowByCam(shared_ptr<Character> object);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
    void handleKeyboardPress(EventKeyboard::KeyCode, Event*);   //used in gameplay
    void handleKeyboardHold();
    void handleKeyboardRelease(EventKeyboard::KeyCode, Event*); //used in gameplay
#endif
    
#if USE_TOUCH
    bool handleTouchBegan(Touch* touch, Event* event);
    void handleTouchMoved(Touch* touch, Event* event);
    void handleTouchRelease(Touch* touch, Event* event);
#else
    bool handleMouseBegan(EventMouse* event);
    void handleMouseMoved(EventMouse* event);
    void handleMouseRelease(EventMouse* event);
    bool _isMouseDown;
#endif
	void handleShootCharacter(shared_ptr<Character> object, const float& speed);

    const unique_ptr<RigidWorld>& getRigidWord() const;
    void releaseGamePlay();

	TMXTiledMap* getTileMap() const;
    
    void pushView(shared_ptr<Vision> vision);
    shared_ptr<Vision> createView(shared_ptr<Character> object, type_vision type);
	shared_ptr<Player> getPlayer() const;
    shared_ptr<Character> getCurrentTargetView() const;
    
    void resetGame();
    void backToHomeMenu();
    
    void setEnableVolunm(bool enable);
    bool isEnableVolumn();

    const vector<GameMap>& getGameMaps() const;
    void setMap(const int& index);
    GameMap* getCurrentMap();
    void setPlayerCreation(const int& index);
    CharacterCreation* getPlayerCreation();
    game_result getGameResult() const;
    
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
    void updateAvaiableSight();
	void updateSight(float dt);
    
	void usePopupInGame(bool push);
    void useKDATab(bool show);
    void setResultGame();
public:
	Vec2 getRandomPosition() const;
   
    
};
