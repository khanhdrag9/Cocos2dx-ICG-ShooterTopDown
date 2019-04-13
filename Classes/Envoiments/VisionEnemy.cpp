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
	Vec2 objPos = _obj->_sprite->getPosition();

	vector<Vec2> temp = _points2;
	for (auto body : Game::getInstance()->getRigidWord()->getListBodies())
	{
		if (body->getTag() == RigidBody::tag::BULLET_PLAYER)
		{

		}
		
		if (body->getTag() == RigidBody::tag::PLAYER && body->getObject() && body->getObject()->getType() == Character::type::PLAYER)
		{
			if (&(*_obj) == &(*body->getObject()))	//so sanh xem co dang check trung` bot ko!
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
//                    Vec2 vectorAngle = target - bot->_sprite->getPosition();
//                    auto angle = atan2(vectorAngle.y, vectorAngle.x);
//                    _obj->_sprite->setRotation(CC_RADIANS_TO_DEGREES(-angle) + 90);
//
//                    bot->setShoot(true);

					//move to target
//                    shared_ptr<Command> moveCmd = CommandMoveTo::createCommandMoveTo(bot->getSpeedMove(), target);
//                    bot->pushCommand(moveCmd, true);
                    
                    float distance = (target - _obj->_sprite->getPosition()).length();
                    shared_ptr<Information> information = make_shared<InformationDetectEnemy>(body->getObject(), distance);
                    InformationCenter::getInstance()->pushInformation(_obj, information);
				}
                else
                {
//                    bot->setShoot(false);
                    shared_ptr<Information> information = make_shared<InformationEnemyOutVision>();
                    InformationCenter::getInstance()->pushInformation(_obj, information);
                }
			}
            else
            {
//                bot->setShoot(false);
                shared_ptr<Information> information = make_shared<InformationEnemyOutVision>();
                InformationCenter::getInstance()->pushInformation(_obj, information);
            }

		}
	}
}
