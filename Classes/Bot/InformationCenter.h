#pragma once
#include "../Defines/Templates.h"
#include "../Headers.h"
#include "Informations.h"
#include "../Physics/RigidWorld.h"

class Character;
class Bot;
class Command;

enum class status_remote : int
{
	NONE,
	WALKING
};

struct RemoteBot
{
	RemoteBot(shared_ptr<Bot> b):
		bot(b),
		status(status_remote::NONE)
	{}
	~RemoteBot() { 
		bot = nullptr;
		status = status_remote::NONE; 
	}
	shared_ptr<Bot> bot;
	status_remote status;
};

class InformationCenter : public pattern::Singleton<InformationCenter>
{
	vector<unique_ptr<RemoteBot>> _listBot;
	vector<Vec2> _graph;
#if DEBUG_GRAHP
	DrawNode* _draw;
#endif

	enum direction : int
	{
		LEFT = 0,
		RIGHT = 1,
		TOP = 2,
		BOT = 3
	};

	struct Direction_des
	{
		bool avaiable;
		list<Line> collisionPoints;
		Vec2 velocity;

		Direction_des(bool a, const Vec2& v):
			avaiable(a),
			velocity(v)
		{}

	};

	bool _isStop;
	thread _threadDetectAround;
	mutex _m;
	void detectAroundAvaiable();
	Vec2 detectNewRoadACharacter(unique_ptr<RemoteBot>& remote) const;
    
    Vec2 getRandomVec(const float& speed);
public:
    InformationCenter();
    ~InformationCenter();
    
	void initGraph(TMXTiledMap* tileMap);
	void startThreads();
	void update(float dt);
	void pushBot(shared_ptr<Bot> bot);

	void clear();
	
	static float dimension_detect_around;
	static float dimension_detect_collision;
};
