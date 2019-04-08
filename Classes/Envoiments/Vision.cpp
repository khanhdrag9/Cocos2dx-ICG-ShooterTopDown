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


float getAngleByAxis(Vec2 point, Vec2 axis)
{
    Vec2 vector = point - axis;
    float angle = atan(vector.y / vector.x);
    if(vector.x >= 0 && vector.y < 0)
        angle += 360;
    else if(vector.x < 0 || vector.y < 0)
        angle += 180;
    
    return angle;
}

void Vision::update(cocos2d::DrawNode *draw, ClippingNode* clipper)
{
    Vec2 objPos = _obj->_sprite->getPosition();
    float dimention = 500.f;
    
    auto& rigidWord = Game::getInstance()->getRigidWord();
    //for lines (new)
    vector<Vec2> points;
    
    float view = 360;
    for(int i = 0; i < view; i+=1)
    {
        float x = sin(CC_DEGREES_TO_RADIANS(i)) * dimention + objPos.x;
        float y = cos(CC_DEGREES_TO_RADIANS(i)) * dimention + objPos.y;
            
        Vec2 point = Vec2(x,y);
        bool isIntersect = false;
        for(auto& checkline : rigidWord->getListLines())
        {
            isIntersect = Vec2::isSegmentIntersect(objPos, point, checkline.start, checkline.end);

            if(isIntersect)
            {
                Vec2 intersect = Vec2::getIntersectPoint(objPos, point, checkline.start, checkline.end);
                if((point - objPos).length() > (intersect - objPos).length())
                    point = intersect;
            }
        }
        
//        //check with other object
//        for(auto body : Game::getInstance()->getRigidWord()->getListBodies())
//        {
//            if(body->getTag() == RigidBody::tag::ENEMY)
//            {
//                auto bodyCasted = dynamic_pointer_cast<RigidBodyPolygon>(body);
//                if(bodyCasted)
//                {
//                    Rect rect = bodyCasted->getRect();
//                    Line lines[]
//                    {
//                        Line( Vec2(rect.getMinX(), rect.getMaxY()) , Vec2(rect.getMaxX(), rect.getMaxY()) ),
//                        Line( Vec2(rect.getMaxX(), rect.getMaxY()) , Vec2(rect.getMaxX(), rect.getMinY()) ),
//                        Line( Vec2(rect.getMaxX(), rect.getMinY()) , Vec2(rect.getMinX(), rect.getMinY()) ),
//                        Line( Vec2(rect.getMinX(), rect.getMinY()) , Vec2(rect.getMinX(), rect.getMaxY()) )
//                    };
//
//                    for(auto& checkline : lines)
//                    {
//                        bool isIntersect = Vec2::isSegmentIntersect(objPos, point, checkline.start, checkline.end);
//                        if(isIntersect)
//                        {
//                            Vec2 intersect = Vec2::getIntersectPoint(objPos, point, checkline.start, checkline.end);
//                            if((point - objPos).length() > (intersect - objPos).length())
//                                point = intersect;
//                        }
//                    }
//                }
//            }
//        }
        
        
#if DEBUG_SIGHT
        draw->drawLine(objPos, point, Color4F::YELLOW);
#endif
        points.push_back(point);
            
    }
    
    if(_isDraw)
    {
        //chieu sang
        int sizePointToDraw = (int)points.size();
        Color4F light(0, 0, 0, 0);
        for(int i = 0; i < sizePointToDraw; i++)
        {
            if(i == sizePointToDraw - 1)
            {
                Vec2 pointToDraw[3] { objPos, points[i], points[0] };
                draw->drawPolygon(pointToDraw, 3, light, 0, light);
            }
            else
            {
                Vec2 pointToDraw[3] { objPos, points[i], points[i+1] };
                draw->drawPolygon(pointToDraw, 3, light, 0, light);
            }
            
        }
        
        if(clipper)
            clipper->setStencil(draw);
    }
    
    //visible Enemy
    for(auto body : Game::getInstance()->getRigidWord()->getListBodies())
    {
        if(body->getTag() != RigidBody::tag::ENEMY)continue;
        
		if (&(*_obj) == &(*body->getObject()))	//so sanh xem co dang check trung` bot ko!
			continue;

        bool isInterest = false;
        if(auto circleBody = dynamic_pointer_cast<RigidBodyCircle>(body))
        {
            auto obj = circleBody->getObject();
            Vec2 positionBody = obj->_sprite->getPosition();

            Rect rect = body->_object->_sprite->getBoundingBox();
            Line lines[]
            {
                Line( Vec2(rect.getMinX(), rect.getMaxY()) , Vec2(rect.getMaxX(), rect.getMinY()) ),
                Line( Vec2(rect.getMaxX(), rect.getMaxY()) , Vec2(rect.getMinX(), rect.getMinY()) ),
            };
            
            for(auto& checkline : lines)
            {
                for(auto& point : points)
                {
					if (Vec2::isSegmentIntersect(objPos, point, checkline.start, checkline.end))
                    {
						Vec2 interest = Vec2::getIntersectPoint(objPos, point, checkline.start, checkline.end);
						if ((interest - positionBody).length() <= circleBody->getRadius())
						{
							isInterest = true;
							break;
						}
                    }
                }
                
                if(isInterest)
                    break;
            }
        }
        
        if(isInterest)
            body->getObject()->_sprite->setVisible(true);
        else
            body->getObject()->_sprite->setVisible(false);
    }
    
}


Vision::Vision(shared_ptr<Character> obj):
_obj(obj),
_isDraw(false)
{

}

void Vision::setDraw(bool draw)
{
    _isDraw = draw;
}
