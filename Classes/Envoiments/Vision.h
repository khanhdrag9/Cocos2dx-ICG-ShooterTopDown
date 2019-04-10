#pragma once
#include "../Headers.h"

class Character;

class Vision
{
protected:
    shared_ptr<Character> _obj;
    bool _isDraw;
	vector<Vec2> _points;
	vector<Vec2> _points2;
	void threadGetPoint();
	bool _isStop;
	
public:
	Vision();
    Vision(shared_ptr<Character> obj);
	virtual ~Vision();
    virtual void update(DrawNode* draw, ClippingNode* clipper = nullptr);
    void setDraw(bool draw);
	void getPointIntersect();
};
