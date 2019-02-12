#pragma once
#include "Headers.h"
#include "Player.h"

enum class statetype : int
{
	STATE_STOP,
	STATE_MOVE,
	STATE_ROTATE,
	STATE_SHOT
};

template<typename S>
class StateMap
{
	pair<statetype, S> pair;
public:
	StateMap(const statetype& type, const S& value)
	{
		pair.first = type;
		pair.second = value;
	}
	const statetype& getStateType()const { return pair.first; }
	const S& getS()const { return pair.second; }
};

class Bot : public Character
{
	friend class BotManager;

	vector<StateMap<Value>> _states;
public:
	Bot();
	virtual ~Bot();

	void init() override;
	void upgrade() override;
public:
};