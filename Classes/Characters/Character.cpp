//
//  Character.cpp
//  Shooter
//
//  Created by Khanh on 3/9/19.
//

#include "Character.h"
#include "../Commands/Command.h"
#include "../Defines/constants.h"
#include "../Physics/RigidBody.h"
#include "../Resource/Creations.h"
#include "../Game.h"

Character::Character():
_sprite(nullptr),
_rigidBody(nullptr),
_destroy(false),
_maxHP(0),
_currentHP(_maxHP),
_dieEffect("")
{
}


Character::~Character()
{
    CCLOG("Delete Character");
    //_commandHandle = nullptr;
    if(_sprite)
    {
        //_sprite->removeFromParent();
        _sprite->removeFromParentAndCleanup(true);
    }
}


void Character::init(CharacterCreation* creation)
{
    _destroy = false;
    _type = Character::type::NONE;
    _name = constants::character_none;
    
    if(creation)
    {
        _sprite = Sprite::create(creation->_sprite);
        _maxHP = creation->_maxHP;
        _currentHP = _maxHP;
        _bullet = &creation->_bullet;
        _dieEffect = creation->getDieEffect();
    }
}


void Character::update(float dt)
{
    if(_currentHP <= 0)
        destroy();
    if(_destroy)return;
        
	for (auto& cmd : _commandQueue)
		cmd->update(dt);
}


void Character::releaseCommands()
{
	_commandQueue.clear();
}


bool Character::pushCommand(shared_ptr<Command>& command, bool replace)
{
	if (_destroy || _currentHP <= 0)return false;
    
   // command->registAnObject(shared_from_this(), _commandQueue);
	//release cmd has finished
	for (auto begin = _commandQueue.begin(); begin != _commandQueue.end();)
	{
		shared_ptr<Command> cmd = *begin;
		if (cmd->isFinished())
		{
			begin = _commandQueue.erase(begin);
		}
		else
			++begin;
	}

	list<shared_ptr<Command>> queueTemp;
	bool isUsed = false;

	while (_commandQueue.size() > 0)
	{
        if(_commandQueue.front())
        {
            if (_commandQueue.front()->getName() == command->getName())
            {
                isUsed = true;
            }

            if (!isUsed)
                queueTemp.push_back(_commandQueue.front());
            else if (!replace)
                queueTemp.push_back(_commandQueue.front());
            else
                isUsed = false;
        }

		_commandQueue.pop_front();
	}

	_commandQueue.swap(queueTemp);

	if (!isUsed)
	{
		command->registAnObject(shared_from_this(), _commandQueue);
		return true;
	}

	return false;
}


const string& Character::getName() const
{
    return _name;
}


const Character::type& Character::getType() const
{
    return _type;
}

int Character::getMaxHP() const
{
    return _maxHP;
}

int Character::getCurrentHP()
{
    return _currentHP;
}

void Character::decreHP(int decre)
{
    _currentHP -= decre;
    if(_currentHP < 0)
    {
        _currentHP = 0;
    }
}

void Character::heal(int heal)
{
    _currentHP += heal;
    if(_currentHP > _maxHP)_currentHP = _maxHP;
}

void Character::destroy()
{
    releaseCommands();
    if(_rigidBody)
        _rigidBody->_object = nullptr;
    _rigidBody = nullptr;
    _destroy = true;
    
    //particle
    if(_dieEffect != "")
    {
        ParticleSystemQuad* par = ParticleSystemQuad::create(_dieEffect);
        if(par)
        {
            Size size = _sprite->getContentSize();
            Vec2 position = _sprite->getPosition();
            par->setPosition(position.x + size.width / 2.f, position.y + size.height / 2.f);
            par->setRotation(_sprite->getRotation());
            Game::getInstance()->getCurrentState()->addChild(par, 10);
        }
    }
}

bool Character::isDestroyed()
{
    return _destroy;
}

BulletCreation* Character::getBullet() const
{
    return _bullet;
}

void Character::setDieEffect(const string& effect)
{
    _dieEffect = effect;
}
