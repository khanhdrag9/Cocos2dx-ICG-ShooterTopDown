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


void Vision::update(cocos2d::DrawNode *draw)
{
    Vec2 objPos = _obj->_sprite->getPosition();
    float dimention = 400.f;
    
    auto& rigidWord = Game::getInstance()->getRigidWord();
    //for lines (new)
    vector<Vec2> points;
    
    for (auto& line : rigidWord->getListLines())
    {
        Vec2 vertices[]{ line.start, line.end };

        for (auto& point : vertices)
        {
            Vec2 des = point - objPos;
            des.normalize();
            des *= dimention;
            des += objPos;
            
            if((point - objPos).length() <= (des - objPos).length())
            {
                
                bool isIntersect = false;
                for(auto& checkline : rigidWord->getListLines())
                {
                    isIntersect = Vec2::isSegmentIntersect(objPos, point, checkline.start, checkline.end);
                    if(isIntersect)
                    {
                        if(checkline.start == point || checkline.end == point)
                        {
                            des = point;
                            isIntersect = false;
                        }
                        else
                            break;
                    }
                }
                
                if(!isIntersect)
                {
                    draw->drawLine(objPos, des, Color4F::WHITE);
                    points.push_back(des);
                }
            }
        }
    }
    
    //chieu sang
    int sizePointToDraw = (int)points.size();
    Vec2* pointToDraw = new Vec2[sizePointToDraw]();
    for(int i = 0; i < sizePointToDraw; i++)
    {
        pointToDraw[i] = points[i];
    }
    
    draw->drawPolygon(pointToDraw, sizePointToDraw, Color4F(0,0,0,0), 0, Color4F(0,0,0,0));
    //draw->drawSolidPoly(pointToDraw, sizePointToDraw, Color4F::WHITE);
    _clipper->setStencil(draw);
    
    delete[]pointToDraw;
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
