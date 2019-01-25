#pragma once
#include "Headers.h"
#include "Command.h"

class Character : enable_shared_from_this<Character>
{
protected:
	enum typecharacter
	{
		UNKNOWN,
		PLAYER
	};

	typecharacter _type;
public:
	Character();
	virtual ~Character();

	virtual void init() = 0;
	virtual void upgrade() = 0;
	void addParrent(Node* parrent);

	CC_SYNTHESIZE(Vec2, _arrowWorldSpace, ArrowWorldSpace);
public:
	Sprite* sprite;

	map<Command::command, bool> actions;
	const typecharacter& getType() const { return _type; }

	friend class Command;
};

class Player : public Character
{
public:
	Player();
	virtual ~Player();

	virtual void init() override;
	virtual void upgrade() override;

	enum typeplayer
	{
		NONE,
		SQUARE,
		CIRCLE,
		RECT
	};
};

class UpgradePlayer : public Player
{
protected:
	typeplayer _typeplayer;
public:
	virtual void upgrade() = 0;
	virtual void init() = 0;

	const typeplayer& getTypeplayer() const { return _typeplayer; };
};

class PlayerSquare : public UpgradePlayer
{
	shared_ptr<Character> _player;
public:
	PlayerSquare(){}
	PlayerSquare(const shared_ptr<Character>& player);
	virtual ~PlayerSquare();

	virtual void init() override;
	void upgrade() override;
};