#pragma once
#include "Vision.h"

class Character;

class VisionEnemy : public Vision
{
	mutex _m;
public:
	VisionEnemy();
	VisionEnemy(shared_ptr<Character> obj);
	void update(DrawNode* draw, ClippingNode* clipper = nullptr) override;


	static const float ranger_detect_collision;
};
