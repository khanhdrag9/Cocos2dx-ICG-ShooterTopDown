#pragma once
#include "../Defines/Templates.h"
#include "../Headers.h"
#include "Informations.h"
#include "../Physics/RigidWorld.h"

class Character;
class Bot;
class Command;

class SolutionWay
{
    vector<list<Vec2>> _ways;
    
public:
    void push(Vec2 first, Vec2 second);
    void clear();
    list<Vec2> findWayMin(const Vec2& target) const;
    int size() const;
    const vector<list<Vec2>>& getWays() const;
};

class InformationCenter : public pattern::Singleton<InformationCenter>
{
public:
	enum class statusBot : int
	{
		NONE,
		WALK,
		COLLISION
	};
	struct BotFindWay
	{
		bool isFinish;
		bool isReady;
		bool isThreadAvaiable;
		statusBot status;
		shared_ptr<Bot> bot;
		future<queue<Vec2>> task;
		pair<float, float> countDetect;
		queue<shared_ptr<Command>> commands;
		BotFindWay() {}
		BotFindWay(shared_ptr<Bot> b):
			bot(b),
			isFinish(true),
			isReady(true)
			, isThreadAvaiable(false)
			, status(statusBot::NONE)
			, countDetect(0.f, 10.f)
		{}

		void clear()
		{
			if (isThreadAvaiable)
				task.get();
			while (commands.size() > 0)
				commands.pop();
			bot = nullptr;
		}

		bool countDetectOK()
		{
			countDetect.first = countDetect.first > countDetect.second ? 0.f : countDetect.first;
			return countDetect.first == 0.f;
		}
	};

private:
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

	list<Vec2> findPointAvaiableAroud(Vec2 position, const vector<Vec2>& arrayFind, float radius = 0);
    bool findWayToPoint(Vec2 start, Vec2 target, vector<Vec2>& grahp, SolutionWay& result, float radius = 0);
	float getRotateForwardAPoint(shared_ptr<Character> character, const Vec2& point) const;
	void pushBot(shared_ptr<Bot> bot);
	BotFindWay* findBotWayByBot(const shared_ptr<Character>& character);
	void clear();
};
