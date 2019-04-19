#pragma once
#include "../Defines/Templates.h"
#include "../Headers.h"
#include "Informations.h"

class Character;
class Command;

class InformationCenter : public pattern::Singleton<InformationCenter>
{
    bool _isStop;

    //detect enemy
    thread detectEnemy;
    queue<pair<shared_ptr<Character>,shared_ptr<InformationDetectEnemy>>> _enemyIsDetected;
    void triggerDetectEnemy();
    float getRotateForwardAPoint(shared_ptr<Character> character ,const Vec2& point);
    
    //EnemyOutVision
    thread enemyOutVision;
    pair<shared_ptr<Character>, shared_ptr<InformationEnemyOutVision>> _enemyOutVision;
    void triggerEnemyOutVision();
    
    //MoveAround
    thread enemyMoveAround;
    using pairCharacterMove = pair<shared_ptr<Character>, shared_ptr<InformationMoveAround>>;
    list<pairCharacterMove> _enemyMoveAround;
    void triggerEnemyMoveAround();
   
    Vec2 getRandomMove(float speed) const;
    void moveWithVelocity(pairCharacterMove& pair, const Vec2& velocity);

public:
	inline bool speedAvaiable(const Vec2& speedCheck, const float& min, const float& max) const;
    
public:
    InformationCenter();
    ~InformationCenter();
    
    void pushInformation(const shared_ptr<Character>& character, shared_ptr<InformationDetectEnemy> information);
    void pushInformation(const shared_ptr<Character>& character, shared_ptr<InformationEnemyOutVision> information);
    void pushInformation(const shared_ptr<Character>& character, shared_ptr<InformationMoveAround> information);
    
    void start();
    void update();
    void stop();
    void clear(bool full);


};
