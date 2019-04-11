//
//  Vision.cpp
//  ShooterTopDown2D
//
//  Created by Khanh on 4/6/19.
//

#include "Vision.h"
#include "../Game.h"
#include "../Characters/Character.h"
#include "../Physics/RigidBody.h"
#include "../Physics/RigidWorld.h"
#include "../Physics/RigidBodyPolygon.h"
#include "../Physics/RigidBodyCircle.h"

void Vision::update(cocos2d::DrawNode *draw, ClippingNode* clipper)
{
	//getPointIntersect();
    
	Vec2 objPos = _obj->_sprite->getPosition();
    if(_isDraw)
    {
        //chieu sang
		vector<Vec2> temp = _points2;
		int sizePointToDraw = (int)temp.size();
        Color4F light(0, 0, 0, 0);
        for(int i = 0; i < sizePointToDraw; i++)
        {
            if(i == sizePointToDraw - 1)
            {
                Vec2 pointToDraw[3] { objPos, temp[i], temp[0] };
                draw->drawPolygon(pointToDraw, 3, light, 0, light);
            }
            else
            {
                Vec2 pointToDraw[3] { objPos, temp[i], temp[i+1] };
                draw->drawPolygon(pointToDraw, 3, light, 0, light);
            }
            
        }
        
        if(clipper)
            clipper->setStencil(draw);
    }
    
}

void Vision::threadGetPoint()
{
	while(_isStop)
	{ 
		getPointIntersect();
		_points2.swap(_points);
	}
}

Vision::Vision():
	_obj(nullptr),
	_isDraw(false),
	_isStop(true)
{}

Vision::Vision(shared_ptr<Character> obj):
_obj(obj),
_isDraw(false),
_isStop(true)
{
	thread t(&Vision::threadGetPoint, this);
	t.detach();
}

Vision::~Vision()
{
	_isStop = false;
}

void Vision::setDraw(bool draw)
{
    _isDraw = draw;
}

void Vision::getPointIntersect()
{
	Vec2 objPos = _obj->_sprite->getPosition();
	float dimention = 500.f;
	auto& rigidWord = Game::getInstance()->getRigidWord();

	_points.clear();

	float view = 360;
	for (int i = 0; i < view; i += 1)
	{
		float x = sin(CC_DEGREES_TO_RADIANS(i)) * dimention + objPos.x;
		float y = cos(CC_DEGREES_TO_RADIANS(i)) * dimention + objPos.y;

		Vec2 point = Vec2(x, y);
		bool isIntersect = false;
		vector<Line> listLine = rigidWord->getListLines();
		for (auto& checkline : listLine)
		{
			isIntersect = Vec2::isSegmentIntersect(objPos, point, checkline.start, checkline.end);

			if (isIntersect)
			{
				Vec2 intersect = Vec2::getIntersectPoint(objPos, point, checkline.start, checkline.end);
				if ((point - objPos).length() > (intersect - objPos).length())
					point = intersect;
			}
		}

#if DEBUG_SIGHT
		draw->drawLine(objPos, point, Color4F(1, 1, 0, 0.25));
#endif
		_points.push_back(point);
	}
}
