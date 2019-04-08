#include "VisionPlayer.h"
#include "../Game.h"
#include "../Characters/Character.h"
#include "../Physics/RigidBody.h"
#include "../Physics/RigidWorld.h"
#include "../Physics/RigidBodyPolygon.h"
#include "../Physics/RigidBodyCircle.h"

VisionPlayer::VisionPlayer() : Vision()
{
}

VisionPlayer::VisionPlayer(shared_ptr<Character> obj) : Vision(obj)
{
}

void VisionPlayer::update(DrawNode * draw, ClippingNode * clipper)
{
	Vision::update(draw, clipper);

	Vec2 objPos = _obj->_sprite->getPosition();

	//visible Enemy
	for (auto body : Game::getInstance()->getRigidWord()->getListBodies())
	{
		if (body->getTag() != RigidBody::tag::ENEMY)continue;

		if (&(*_obj) == &(*body->getObject()))	//so sanh xem co dang check trung` bot ko!
			continue;

		bool isInterest = false;
		if (auto circleBody = dynamic_pointer_cast<RigidBodyCircle>(body))
		{
			auto obj = circleBody->getObject();
			Vec2 positionBody = obj->_sprite->getPosition();

#if DEBUG_ENEMY
			draw->drawLine(objPos, positionBody, Color4F::RED);
			draw->drawCircle(positionBody, circleBody->getRadius(), 0, 360, false, Color4F::RED);
#endif

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

		if (isInterest)
			body->getObject()->_sprite->setVisible(true);
		else
			body->getObject()->_sprite->setVisible(false);
	}
}
