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
}
