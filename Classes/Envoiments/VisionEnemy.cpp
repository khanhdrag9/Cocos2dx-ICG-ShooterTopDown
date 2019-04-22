#include "VisionEnemy.h"
#include "../Game.h"
#include "../Characters/Character.h"
#include "../Bot/Bot.h"
#include "../Physics/RigidBody.h"
#include "../Physics/RigidWorld.h"
#include "../Physics/RigidBodyPolygon.h"
#include "../Physics/RigidBodyCircle.h"
#include "../Commands/CommandMoveBy.h"
#include "../Commands/CommandMoveTo.h"
#include "../Bot/Informations.h"
#include "../Bot/InformationCenter.h"

const float VisionEnemy::ranger_detect_collision = 96.f;

VisionEnemy::VisionEnemy() : Vision()
{
}

VisionEnemy::VisionEnemy(shared_ptr<Character> obj) : Vision(obj)
{
}

void VisionEnemy::update(DrawNode * draw, ClippingNode * clipper)
{
	Vision::update(draw, clipper);
	//return;	//disble vision enemy

	if (_obj->isDestroyed())return;

	Vec2 objPos = _obj->_sprite->getPosition();

	//for detect collision with wall
	if (auto bot = dynamic_pointer_cast<Bot>(_obj))
	{
		Vec2 speedCheck = bot->_rigidBody->_velocity;
		//float speed = bot->getSpeedMove() * Director::getInstance()->getDeltaTime();
		Rect rect = bot->_sprite->getBoundingBox();
		Vec2 startPoint[1];
		startPoint[0] = objPos;

		for(auto& point : startPoint)
		//if (InformationCenter::getInstance()->speedAvaiable(speedCheck, speed / 4.f, speed * 4.f))
		{
			Vec2 offset = speedCheck.getNormalized() * ranger_detect_collision;
			Vec2 target = offset + point;
#if DEBUG_ENEMY
			draw->drawLine(point, target, Color4F::BLUE);
#endif
			bool isIntersect = false;
			auto listLine = Game::getInstance()->getRigidWord()->getListLines();
			for (auto& checkline : listLine)
			{
				if (Vec2::isSegmentIntersect(point, target, checkline.start, checkline.end))
				{
					isIntersect = true;
					auto des = make_shared<des_detect_collision_wall>(speedCheck);
					shared_ptr<InformationMoveAround> information = make_shared<InformationMoveAround>(des);
					InformationCenter::getInstance()->pushInformation(bot, information);
					break;
				}
			}
		}

	}
	
    
    vector<Vec2> temp = _points2;
    
    for (auto body : Game::getInstance()->getRigidWord()->getListBodies())
    {
        if (body->getTag() == RigidBody::tag::BULLET_PLAYER)
        {

        }

        if (body->getTag() == RigidBody::tag::WALL)
        {

        }
        
        if (body->getTag() == RigidBody::tag::PLAYER && body->getObject() && body->getObject()->getType() == Character::type::PLAYER)
        {
            if (&(*_obj) == &(*body->getObject()))    //so sanh xem co dang check trung` bot ko!
                continue;

            bool isInterest = false;
            if (auto circleBody = dynamic_pointer_cast<RigidBodyCircle>(body))
            {
                auto obj = circleBody->getObject();
                Vec2 positionBody = obj->_sprite->getPosition();

                Rect rect = body->_object->_sprite->getBoundingBox();
                Line lines[]
                {
                    Line(Vec2(rect.getMinX(), rect.getMaxY()) , Vec2(rect.getMaxX(), rect.getMinY())),
                    Line(Vec2(rect.getMaxX(), rect.getMaxY()) , Vec2(rect.getMinX(), rect.getMinY())),
                };

                for (auto& checkline : lines)
                {
                    for (auto& point : temp)
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

                    if (isInterest)
                        break;
                }
             
            }

            if (auto bot = dynamic_pointer_cast<Bot>(_obj))
            {
                if (isInterest)
                {
                    //Shoot when in vision!
                    shared_ptr<Character> enemy = body->getObject();
                    Vec2 target = enemy->_sprite->getPosition();
                    
                    float distance = (target - _obj->_sprite->getPosition()).length();
                    auto information = make_shared<InformationDetectEnemy>(body->getObject(), distance);
                    InformationCenter::getInstance()->pushInformation(_obj, information);
                }
                else
                {
                    auto information = make_shared<InformationEnemyOutVision>();
                    InformationCenter::getInstance()->pushInformation(_obj, information);
                }
            }
            else
            {
                auto information = make_shared<InformationEnemyOutVision>();
                InformationCenter::getInstance()->pushInformation(_obj, information);
            }

        }
    }
}
