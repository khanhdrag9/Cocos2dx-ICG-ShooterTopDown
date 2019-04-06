#pragma once
#include "../Headers.h"

class RigidBody;
class RigidBodyPolygon;
class Character;
class BulletBasic;

//struct Line
//{
//    Vec2 start;
//    Vec2 end;
//
//    Line() {}
//    Line(const Vec2& first, const Vec2& second):
//    start(first),
//    end(second)
//    {}
//
//    bool operator==(const Line& other)
//    {
//        return ((start == other.start && end == other.end) || (start == other.end && end == other.start));
//    }
//
//    void operator=(const Line& other)
//    {
//        start = other.start;
//        end = other.end;
//    }
//
//    float lenght() const
//    {
//        return sqrt( (start.x - end.x)*(start.x - end.x) + (start.y - end.y)*(start.y - end.y) );
//    }
//};

#define LINE_ZERO Line(Vec2(0,0), Vec2(0,0))

class RigidWorld
{
    vector<shared_ptr<RigidBody>> _listRigidBodies;
	//vector<Line> _listLines;
    
    bool checkCollisionOther(shared_ptr<RigidBody> body);
	//void pushLineByRect(const Rect& rect);
public:
    RigidWorld();
    virtual ~RigidWorld();
    
    void update(float dt);
    
    shared_ptr<RigidBodyPolygon> createRigidBodyPolygon(const Rect& rect);
    shared_ptr<RigidBody> createRigidBodyPolygon(shared_ptr<Character> character);
    shared_ptr<RigidBody> createRigidBodyCircle(shared_ptr<Character> character);
    
    
    void onCollision(shared_ptr<RigidBody> body1, shared_ptr<RigidBody> body2);
    const vector<shared_ptr<RigidBody>>& getListBodies() const;
	//const vector<Line>& getListLines() const;

	//void pushLine(const Line& line);
	//void generateLineAgain();
    
private:
    //Line is2LinesStraight(const Line& line1, const Line& line2);
};
