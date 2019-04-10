#pragma once
#include "../Headers.h"

class Character;

class Vision
{
protected:
    shared_ptr<Character> _obj;
    bool _isDraw;
	vector<Vec2> _points;
public:
	Vision();
    Vision(shared_ptr<Character> obj);
    virtual void update(DrawNode* draw, ClippingNode* clipper = nullptr);
    void setDraw(bool draw);
	void getPointIntersect();
};
