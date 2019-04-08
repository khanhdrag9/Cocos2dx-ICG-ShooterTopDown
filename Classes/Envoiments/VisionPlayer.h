#pragma once
#include "Vision.h"

class Character;

class VisionPlayer : public Vision
{
public:
	VisionPlayer();
	VisionPlayer(shared_ptr<Character> obj);
	void update(DrawNode* draw, ClippingNode* clipper = nullptr) override;
};
