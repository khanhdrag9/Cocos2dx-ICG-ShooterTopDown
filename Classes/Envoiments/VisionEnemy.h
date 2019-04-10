#pragma once
#include "Vision.h"

class Character;

class VisionEnemy : public Vision
{
public:
	VisionEnemy();
	VisionEnemy(shared_ptr<Character> obj);
	void update(DrawNode* draw, ClippingNode* clipper = nullptr) override;
};