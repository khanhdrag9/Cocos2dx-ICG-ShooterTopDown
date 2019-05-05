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
#include "../Physics/RigidBody.h"
#include "../Characters/Player.h"
#include "../Bot/Bot.h"
#include "../Resource/Creations.h"
#include "../Resource/ResourceManager.h"
#include "../Game.h"

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
    auto object = _object.lock();
    if(!_isFinished && object)
    {
        auto bulletCreation = object->getBullet();
        auto bullet = BulletBasic::createBulletBasic(bulletCreation->getBulletSprite(),object->_sprite->getPosition(), object->_sprite->getRotation(), _speed, true);
        bullet->setDamege(bulletCreation->getDamgeBullet());
        bullet->setParticle(bulletCreation->getParticle());
        bullet->setDieEffect(bulletCreation->getExplosion());
        bullet->setDieSound(bulletCreation->getExplosionSound());

		if (object->getType() == Character::type::PLAYER)
		{
            bullet->_rigidBody->setTag(RigidBody::tag::BULLET_PLAYER);
            if(auto player = dynamic_pointer_cast<Player>(object))
            {
                player->getMag()->decreBullet();
            }
		}
		else if (object->getType() == Character::type::ENEMY)
		{
			bullet->_rigidBody->setTag(RigidBody::tag::BULLET_ENEMY);
            if(auto enemy = dynamic_pointer_cast<Bot>(object))
            {
                enemy->getMag()->decreBullet();
            }
		}

        ObjectsPool::getInstance()->pushBulletBasic(bullet);
        _isFinished = true;
        
        //load sound
        if(Game::getInstance()->isEnableVolumn())
        {
            auto audio = SimpleAudioEngine::getInstance();
            if(!audio->isBackgroundMusicPlaying())
                SimpleAudioEngine::getInstance()->playEffect(bulletCreation->getShootSound().c_str());
        }
    }
}
