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
	enum class statusBot : int
	{
		NONE,
		WALK
	};
	struct BotFindWay
	{
		bool isFinish;
		bool isReady;
		bool isThreadAvaiable;
		statusBot status;
		shared_ptr<Bot> bot;
		future<queue<Vec2>> task;
		queue<shared_ptr<Command>> commands;

		BotFindWay(shared_ptr<Bot> b):
			bot(b),
			isFinish(true),
			isReady(true)
			, isThreadAvaiable(false)
			, status(statusBot::NONE)
		{}


	};

	//vector<shared_ptr<Bot>> _listBot;
	vector<BotFindWay> _listBot;
	vector<Vec2> _graph;
	
#if DEBUG_GRAHP
	DrawNode* _draw;
	DrawNode* _canMovePointDrawer;
#endif

	bool _isStop;
	//thread _threadDetectAround;
	mutex _m;

    void threadAI();
public:
    InformationCenter();
    ~InformationCenter();
    
	void initGraph(TMXTiledMap* tileMap);
	void startThreads();
	void update(float dt);

	list<Vec2> findPointAvaiableAroud(Vec2 position, vector<Vec2>& arrayFind, float radius = 0);
    bool findWayToPoint(Vec2 start, Vec2 target, vector<Vec2>& grahp, queue<Vec2>& result, float radius = 0);
    
	void pushBot(shared_ptr<Bot> bot);
	void clear();
};
