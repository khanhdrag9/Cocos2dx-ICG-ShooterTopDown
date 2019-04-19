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

const float Vision::origin_vision = 500.f;

void Vision::update(cocos2d::DrawNode *draw, ClippingNode* clipper)
{
	//getPointIntersect();
	_threadRun = true;
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
	_threadRun = false;
}

void Vision::threadGetPoint()
{
	while(!_isStop)
	{ 
		if (!_threadRun)continue;
		getPointIntersect();
		_points2.swap(_points);
	}

	_avaiableToDelete = true;
}

Vision::Vision():
	_obj(nullptr),
	_isDraw(false),
	_isStop(false),
	_threadRun(false),
	_avaiableToDelete(false),
	_vision(origin_vision)
{}

Vision::Vision(shared_ptr<Character> obj):
_obj(obj),
_isDraw(false),
_isStop(false),
_threadRun(false),
_avaiableToDelete(false),
_vision(origin_vision)
{
	_threadVision = thread(&Vision::threadGetPoint, this);
	//_threadVision.detach();
}

Vision::~Vision()
{
	_threadRun = false;
	_obj = nullptr;
}

void Vision::setDraw(bool draw)
{
    _isDraw = draw;
}

bool Vision::isDraw()
{
    return _isDraw;
}

bool Vision::avaibleToDelete() const
{
	return _avaiableToDelete;
}

void Vision::stop()
{
	if (_avaiableToDelete)return;

	_isStop = true;
	while (_threadVision.joinable() == false || _avaiableToDelete == false);
	_threadVision.join();
	
}

shared_ptr<Character> Vision::getObject()
{
	return _obj;
}

void Vision::getPointIntersect()
{
	Vec2 objPos = _obj->_sprite->getPosition();
	float dimention = _vision;
	auto listLine = Game::getInstance()->getRigidWord()->getListLines();

	_points.clear();

	float view = 360;
	for (int i = 0; i < view; i += 1)
	{
		float x = sin(CC_DEGREES_TO_RADIANS(i)) * dimention + objPos.x;
		float y = cos(CC_DEGREES_TO_RADIANS(i)) * dimention + objPos.y;

		Vec2 point = Vec2(x, y);
		bool isIntersect = false;
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
