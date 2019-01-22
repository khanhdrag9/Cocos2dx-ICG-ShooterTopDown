#pragma once
#include "Headers.h"
#include "Command.h"

class Character : enable_shared_from_this<Character>
{
protected:

public:
	Character();
	virtual ~Character();

	virtual void init() = 0;
	virtual void update() = 0;
	void addParrent(Node* parrent);

public:
	Sprite* sprite;

	map<Command::command, bool> actions;
};

class Player : public Character
{
public:
	Player();
	virtual ~Player();

	void init() override;
	void update() override;
};