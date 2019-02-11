#include "Command.h"
#include "Player.h"
#include "GamePlay.h"
#include "Support.h"

Command::Command():
	_countshoottime(0.f),
	_shootinterval(BULLET1_INTERVAL)
{

}

void Command::setGamePlay(GamePlay* gameplay)
{
	_gameplay = gameplay;
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
void Command::remote(shared_ptr<Character> character, const EventKeyboard::KeyCode& code, bool value)
{
	switch (code)
	{
	case EventKeyboard::KeyCode::KEY_W:
		character->actions[command::MOVE_UP] = value;
		break;
	case EventKeyboard::KeyCode::KEY_S:
		character->actions[command::MOVE_DOWN] = value;
		break;
	case EventKeyboard::KeyCode::KEY_A:
		character->actions[command::MOVE_LEFT] = value;
		break;
	case EventKeyboard::KeyCode::KEY_D:
		character->actions[command::MOVE_RIGHT] = value;
		break;
	default:
		break;
	}
}
void Command::remote(shared_ptr<Character> character, const Vec2& touchpos, const EventMouse::MouseEventType& code, bool value)
{
	switch (code)
	{
	case EventMouse::MouseEventType::MOUSE_DOWN:
		character->actions[command::SHOOT] = value;
		_touchPos = touchpos;
		break;
	case EventMouse::MouseEventType::MOUSE_MOVE:
		_touchPos = touchpos;
		break;
	case EventMouse::MouseEventType::MOUSE_UP:
		character->actions[command::SHOOT] = value;
		_touchPos = Vec2(0, 0);
		break;
	default:
		break;
	}
}
#else

#endif

void Command::handleActionsCharacter(shared_ptr<Character>& character, float dt)
{
	character->setArrowWorldSpace(character->sprite->getParent()->convertToWorldSpace(character->sprite->getPosition()));	//update to rotate this
	float incre = 4.f;
	if (character->actions[command::MOVE_UP])
	{
		move(character, Vec2(0, incre));
	}
	if (character->actions[command::MOVE_DOWN])
	{
		move(character, Vec2(0, -incre));
	}
	if (character->actions[command::MOVE_LEFT])
	{
		move(character, Vec2(-incre, 0));
	}
	if (character->actions[command::MOVE_RIGHT])
	{
		move(character, Vec2(incre, 0));
	}
	if (character->actions[command::SHOOT])
	{
		if (_countshoottime >= _shootinterval)
		{
			shot(character);
			_countshoottime = 0.f;
		}
	}

	_countshoottime += dt;
}

void Command::move(shared_ptr<Character>& character, const Vec2& speed)
{
	Vec2 newpos = character->sprite->getPosition() + speed;

    if(checkcollisionatpos(character, newpos))
        return;

	character->sprite->setPosition(newpos);
	//camera follow player
	if (character->getType() == Character::typecharacter::PLAYER)
	{
		_gameplay->setViewPointCenter(character->sprite);
	}

}

void Command::rotate(shared_ptr<Character>& character, const Vec2& loc)
{
    Vec2 arrowWorldSpace = character->getArrowWorldSpace();
    float oldAngle = character->sprite->getRotation();
    
    auto angle = atan2(loc.y - arrowWorldSpace.y, loc.x - arrowWorldSpace.x);
    character->sprite->setRotation(CC_RADIANS_TO_DEGREES(-angle) + 90);
    
    if(checkcollisionatpos(character, character->sprite->getPosition()))
        character->sprite->setRotation(oldAngle);   //rollback old position
}

void Command::shot(shared_ptr<Character>& character)
{
	Vec2 poscreate = character->sprite->getPosition();

	auto bullet = Sprite::create(BULLET1);
	//bullet->setRotation(character->sprite->getRotation());
	bullet->setPosition(poscreate);
	float ratio = 0.25;
	bullet->setScale(ratio);
	bullet->setTag(objecttag::BULLET);
	character->sprite->getParent()->addChild(bullet);

	//physic for bullet
	Size bulletsize = bullet->getContentSize();
	PhysicsBody* body = PhysicsBody::createBox(bulletsize);
	body->setRotationEnable(true);
	//body->setGravityEnable(false);
	if (character->getType() == Character::typecharacter::PLAYER)
	{
		body->setContactTestBitmask(PHYSICS_BULLET_PLAYER);
		body->setCategoryBitmask(PHYSICS_BULLET_PLAYER);
		body->setCollisionBitmask(PHYSICS_BULLET_PLAYER);
	}
	bullet->setPhysicsBody(body);

	bullet->setRotation(character->sprite->getRotation());	//set rotate after set physicsbody to physicsbody right with rotate of bullet sprite
	//calculate target of bullet


	//shoot the bullet

	/*if shoot forward mouse without care about angle of character*/
	//Vec2 characterPos = character->sprite->getParent()->convertToWorldSpace(character->sprite->getPosition());	//realpos is shown on the screen
	//Vec2 offset = _touchPos - characterPos;

	float rotate = character->sprite->getRotation();
	float vx = sin(CC_DEGREES_TO_RADIANS(rotate));
	float vy = cos(CC_DEGREES_TO_RADIANS(rotate));
	Vec2 offset = Vec2(vx, vy);
	offset.normalize();
	Vec2 shootAmount = offset * BULLET1_SPEED;
	//Vec2 realPosTo = shootAmount + poscreate;

//    auto move = MoveTo::create(BULLET1_SPEED, realPosTo);
//    auto release = RemoveSelf::create();
//    auto finalaction = Sequence::createWithTwoActions(move, release);
//    finalaction->setTag(actiontag::SHOOT_MOVE);
//    bullet->runAction(finalaction);

    body->setVelocity(shootAmount);    //if want to colli
}

void Command::handleCollisionWithScreen(shared_ptr<Character> &character) { 
   
}

bool Command::checkcollisionatpos(const shared_ptr<Character> &character, const cocos2d::Vec2 &pos) { 
    //check rotation collision
    auto tiledMap = _gameplay->getTiledMap();
    auto collision = _gameplay->getCollisionLayer();
    vector<Vec2> listCheck = support::getListVec2(character, pos);
    
    for(auto& p : listCheck)
    {
        Vec2 coord = support::getCoordInTileMap(tiledMap, p);
        //CCLOG("Coord %f - %f", coord.x, coord.y);
        if(coord.x < 0 || coord.y < 0)return true;
        auto tileGid = collision->getTileGIDAt(coord);
        if (tileGid)
        {
            auto properties = tiledMap->getPropertiesForGID(tileGid).asValueMap();
            if (properties.size() > 0)
            {
                if (properties.at("Collision").asBool() == true)
                {
                    return true;
                }
            }
        }
    }
    
    return false;
}


