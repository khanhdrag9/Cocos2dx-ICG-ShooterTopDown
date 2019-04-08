#include "VisionEnemy.h"
#include "../Game.h"
#include "../Characters/Character.h"
#include "../Bot/Bot.h"
#include "../Physics/RigidBody.h"
#include "../Physics/RigidWorld.h"
#include "../Physics/RigidBodyPolygon.h"
#include "../Physics/RigidBodyCircle.h"

VisionEnemy::VisionEnemy() : Vision()
{
}

VisionEnemy::VisionEnemy(shared_ptr<Character> obj) : Vision(obj)
{
}

void VisionEnemy::update(DrawNode * draw, ClippingNode * clipper)
{
	Vision::update(draw, clipper);

	Vec2 objPos = _obj->_sprite->getPosition();

	//visible Enemy
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
					for (auto& point : _points)
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
					bot->setShoot(true);
				}
				else
				{
					bot->setShoot(false);
				}
			}
			else
			{
				bot->setShoot(false);
			}

		}
	}
}
