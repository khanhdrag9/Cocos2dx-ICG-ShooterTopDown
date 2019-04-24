#pragma once
#include "../Defines/Templates.h"
#include "../Headers.h"
#include "Informations.h"
#include "../Physics/RigidWorld.h"

class Character;
class Bot;
class Command;

class InformationCenter : public pattern::Singleton<InformationCenter>
{
	vector<shared_ptr<Bot>> _listBot;
	vector<Vec2> _graph;
#if DEBUG_GRAHP
	DrawNode* _draw;
#endif

	bool _isStop;
	thread _threadDetectAround;
	mutex _m;

    void threadAI();
public:
    InformationCenter();
    ~InformationCenter();
    
	void initGraph(TMXTiledMap* tileMap);
	void startThreads();
	void update(float dt);
    queue<Vec2> findWayToPoint(const Vec2& target);
    
	void pushBot(shared_ptr<Bot> bot);
	void clear();
};
