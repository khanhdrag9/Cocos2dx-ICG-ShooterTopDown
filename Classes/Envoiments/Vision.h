#pragma once
#include "../Headers.h"

class Character;

class Vision
{
protected:
	mutex _m;
    shared_ptr<Character> _obj;
    bool _isDraw;
	vector<Vec2> _points;
	void getPointIntersect();
	float _vision;
    float _angleVision;
public:
	Vision();
    Vision(shared_ptr<Character> obj);
	virtual ~Vision();
    virtual void update(DrawNode* draw, ClippingNode* clipper = nullptr);
    void setDraw(bool draw);
    bool isDraw();
	shared_ptr<Character> getObject();
    void setAngleVision(const float& value);
    void setDistanceVision(const float& value);

	static const float origin_vision;
};
