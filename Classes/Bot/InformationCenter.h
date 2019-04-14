#pragma once
#include "../Defines/Templates.h"
#include "../Headers.h"
#include "Informations.h"

class Character;

class InformationCenter : public pattern::Singleton<InformationCenter>
{
    bool _isStop;
    
    //detect enemy
    queue<pair<shared_ptr<Character>,shared_ptr<InformationDetectEnemy>>> _enemyIsDetected;
    void triggerDetectEnemy();
    
    //EnemyOutVision
    pair<shared_ptr<Character>, shared_ptr<InformationEnemyOutVision>> _enemyOutVision;
    void triggerEnemyOutVision();
    
public:
    InformationCenter();
    ~InformationCenter();
    
    void pushInformation(const shared_ptr<Character>& character, shared_ptr<InformationDetectEnemy> information);
    void pushInformation(const shared_ptr<Character>& character, shared_ptr<InformationEnemyOutVision> information);
    
    void stop();
    void clear();
};
