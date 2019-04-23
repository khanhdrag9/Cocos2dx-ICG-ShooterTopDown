#pragma once
#include "../Defines/Templates.h"
#include "../Headers.h"
#include "Informations.h"

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
public:
    InformationCenter();
    ~InformationCenter();
    
	void initGraph(TMXTiledMap* tileMap);
	void update(float dt);
	void pushBot(shared_ptr<Bot> bot);
	list<Vec2> findAroundPoint(const Vec2& point, shared_ptr<Bot> bot);
	list<Vec2> findNearestToPoint(const Vec2& point);

	void clear();
	
};
