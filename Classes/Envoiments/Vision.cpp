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
#include "../Bot/InformationCenter.h"

const float Vision::origin_vision = 500.f;

void Vision::update(cocos2d::DrawNode *draw, ClippingNode* clipper)
{
	//threadGetPoint();
	getPointIntersect();

	Vec2 objPos = _obj->_sprite->getPosition();
    if(_isDraw)
    {
        //chieu sang
		//std::lock_guard<mutex> guard(_m);
		//vector<Vec2> temp = _points2;
		//int sizePointToDraw = (int)temp.size();
		int sizePointToDraw = (int)_points.size();
        Color4F light(0, 0, 0, 0);
        for(int i = 0; i < sizePointToDraw; i++)
        {
            if(i == sizePointToDraw - 1)
            {
                //Vec2 pointToDraw[3] { objPos, temp[i], temp[0] };
                Vec2 pointToDraw[3] { objPos, _points[i], _points[0] };
                draw->drawPolygon(pointToDraw, 3, light, 0, light);
            }
            else
            {
                //Vec2 pointToDraw[3] { objPos, temp[i], temp[i+1] };
                Vec2 pointToDraw[3] { objPos, _points[i], _points[i+1] };
                draw->drawPolygon(pointToDraw, 3, light, 0, light);
            }
            
        }
        
        if(clipper)
            clipper->setStencil(draw);
    }
}

//void Vision::threadGetPoint()
//{
//    getPointIntersect();
//    _points2.swap(_points);
//}

Vision::Vision():
	_obj(nullptr),
	_isDraw(false),
	_vision(origin_vision)
{}

Vision::Vision(shared_ptr<Character> obj):
_obj(obj),
_isDraw(false),
_vision(origin_vision)
{
}

Vision::~Vision()
{
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
		Vec2 pointTemp = point;
		bool isIntersect = false;
		for (auto& checkline : listLine)
		{
			isIntersect = Vec2::isSegmentIntersect(objPos, pointTemp, checkline.start, checkline.end);

			if (isIntersect)
			{
				Vec2 intersect = Vec2::getIntersectPoint(objPos, pointTemp, checkline.start, checkline.end);
				if ((pointTemp - objPos).length() > (intersect - objPos).length())
					pointTemp = intersect;
			}
		}

		point = pointTemp;
		_points.push_back(point);
	}
}
