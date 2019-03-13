//
//  CommandShoot.cpp
//  Shooter
//
//  Created by Khanh on 3/11/19.
//

#include "CommandShoot.h"
#include "../Objects/BulletBasic.h"
#include "../Characters/Character.h"
#include "../Objects/ObjectsPool.h"
#include "../Defines/constants.h"

CommandShoot::CommandShoot():
    _speed(Vec2(0,0))
{
    
}

shared_ptr<CommandShoot> CommandShoot::createCommandShoot(const Vec2& speed)
{
    auto cmd = make_shared<CommandShoot>();
    cmd->_name = constants::commnad_shoot;
    cmd->_speed = speed;
    return cmd;
}

void CommandShoot::update(float dt)
{
    if(!_isFinished && _object != nullptr)
    {
        auto bullet = BulletBasic::createBulletBasic(_object->_sprite->getPosition(), _object->_sprite->getRotation(), _speed, true);

		if (_object->getType() == Character::type::PLAYER)
		{
			bullet->_rigidBody->setContactTestBitmask(physics_code::physics_bullet_player);
			bullet->_rigidBody->setCategoryBitmask(physics_code::physics_bullet_player);
			bullet->_rigidBody->setCollisionBitmask(physics_code::physics_bullet_player);
		}
		else if (_object->getType() == Character::type::ENEMY)
		{
			bullet->_rigidBody->setContactTestBitmask(physics_code::physics_bullet_enemy);
			bullet->_rigidBody->setCategoryBitmask(physics_code::physics_bullet_enemy);
			bullet->_rigidBody->setCollisionBitmask(physics_code::physics_bullet_enemy);
		}

        ObjectsPool::getInstance()->pushBulletBasic(bullet);
        _isFinished = true;
    }
}
