#pragma once
#include "Headers.h"
#include "Player.h"

class Bot : public Character
{
public:
	Bot();
	virtual ~Bot();

	void init() override;
	void upgrade() override;
public:
	Sprite* sprite;
};