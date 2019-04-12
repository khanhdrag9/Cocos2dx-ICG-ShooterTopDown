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
	virtual void threadGetPoint();
	bool _isStop;
	bool _avaiableToDelete;
	atomic_bool _threadRun;
	thread _threadVision;
	void getPointIntersect();
public:
	Vision();
    Vision(shared_ptr<Character> obj);
	virtual ~Vision();
    virtual void update(DrawNode* draw, ClippingNode* clipper = nullptr);
    void setDraw(bool draw);
	bool avaibleToDelete() const;
	void stop();
	shared_ptr<Character> getObject();
};
