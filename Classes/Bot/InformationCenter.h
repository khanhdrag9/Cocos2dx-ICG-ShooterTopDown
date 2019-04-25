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
	struct BotFindWay
	{
		bool isFinish;
		bool isReady;
		bool isThreadAvaiable;
		shared_ptr<Bot> bot;
		future<queue<Vec2>> task;
		//std::packaged_task<queue<Vec2>(Vec2, list<Vec2>)> task;

		BotFindWay(shared_ptr<Bot> b):
			bot(b),
			isFinish(true),
			isReady(true)
			, isThreadAvaiable(false)
			//,task(nullptr)
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

	list<Vec2> findPointAvaiableAroud(Vec2 position, const list<Vec2>& unless);
    queue<Vec2> findWayToPoint(Vec2 start, Vec2 target);
    
	void pushBot(shared_ptr<Bot> bot);
	void clear();
};
