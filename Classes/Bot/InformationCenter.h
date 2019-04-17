#pragma once
#include "../Defines/Templates.h"
#include "../Headers.h"
#include "Informations.h"

class Character;

class InformationCenter : public pattern::Singleton<InformationCenter>
{
    bool _isStop;

    //detect enemy
    thread detectEnemy;
    queue<pair<shared_ptr<Character>,shared_ptr<InformationDetectEnemy>>> _enemyIsDetected;
    void triggerDetectEnemy();
    
    //EnemyOutVision
    thread enemyOutVision;
    pair<shared_ptr<Character>, shared_ptr<InformationEnemyOutVision>> _enemyOutVision;
    void triggerEnemyOutVision();
    
    //MoveAround
    thread enemyMoveAround;
    list<pair<shared_ptr<Character>, shared_ptr<InformationMoveAround>>> _enemyMoveAround;
    void triggerEnemyMoveAround();
    
public:
    InformationCenter();
    ~InformationCenter();
    
    void pushInformation(const shared_ptr<Character>& character, shared_ptr<InformationDetectEnemy> information);
    void pushInformation(const shared_ptr<Character>& character, shared_ptr<InformationEnemyOutVision> information);
    void pushInformation(const shared_ptr<Character>& character, shared_ptr<InformationMoveAround> information);
    
    void start();
    void stop();
    void clear();
};
