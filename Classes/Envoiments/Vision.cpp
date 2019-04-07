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

void Vision::update(cocos2d::DrawNode *draw)
{
    Vec2 objPos = _obj->_sprite->getPosition();
    float dimention = 400.f;
    
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
            
#if DEBUG_SIGHT
        draw->drawLine(objPos, point, Color4F::YELLOW);
#endif
        points.push_back(point);
            
    }
    
    //chieu sang
    int sizePointToDraw = (int)points.size();
    Color4F light(1, 1, 0, 0.1);
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

    _clipper->setStencil(draw);
}


Vision::Vision(shared_ptr<Character> obj, DrawNode* draw, Node* soild):
_obj(obj)
{
    _clipper = ClippingNode::create();
    _clipper->setStencil(draw);
    _clipper->setInverted(true);
    
    _clipper->addChild(soild);
    Game::getInstance()->getCurrentState()->addChild(_clipper, 100);
}
