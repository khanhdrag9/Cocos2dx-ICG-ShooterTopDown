#pragma once
#include "../Headers.h"

class RigidBody;
class RigidBodyPolygon;
class Character;
class BulletBasic;

struct Line 
{
	Vec2 start;
	Vec2 end;

	Line() {}
	Line(const Vec2& first, const Vec2& second):
	start(first),
	end(second)
	{}

	bool operator==(const Line& other)
	{
		return ((start == other.start && end == other.end) || (start == other.end && end == other.start));
	}
};

class RigidWorld
{
    vector<shared_ptr<RigidBody>> _listRigidBodies;
	vector<Line> _listLines;
    
    bool checkCollisionOther(shared_ptr<RigidBody> body);
	void pushLineByRect(const Rect& rect);
public:
    RigidWorld();
    virtual ~RigidWorld();
    
    void update(float dt);
    
    shared_ptr<RigidBodyPolygon> createRigidBodyPolygon(const Rect& rect);
    shared_ptr<RigidBody> createRigidBodyPolygon(shared_ptr<Character> character);
    shared_ptr<RigidBody> createRigidBodyCircle(shared_ptr<Character> character);
    
    
    void onCollision(shared_ptr<RigidBody> body1, shared_ptr<RigidBody> body2);
    const vector<shared_ptr<RigidBody>>& getListBodies() const;
	const vector<Line>& getListLines() const;

	void pushLine(const Line& line);
	void generateLineAgain();
};
