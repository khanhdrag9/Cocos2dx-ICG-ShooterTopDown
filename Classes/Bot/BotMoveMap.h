#pragma once
#include "Headers.h"

class CommandMoveTo;
class Bot;

class LinkPosition
{
	string name;
	Vec2 first;
	vector<string> others;
public:
    LinkPosition(){}
    LinkPosition(const Vec2& first, const string& name);
    ~LinkPosition();
    
	const Vec2& get() const;
	const string& getName() const;
	void push(string other);
	const string& otherAt(const int& index);
};

class BotMoveMap
{
	std::queue<shared_ptr<CommandMoveTo>> _queueMoveTo;
	shared_ptr<Bot> _bot;

public:
	BotMoveMap();
	BotMoveMap(shared_ptr<Bot> bot, queue<Vec2>& queuePointTo);
	virtual ~BotMoveMap();

	void init(shared_ptr<Bot> bot, queue<Vec2>& queuePointTo);
	
};
