#pragma once
#include "../Headers.h"

class Command;
class RigidBody;

class CharacterCreation;
class BulletCreation;

class Character : public enable_shared_from_this<Character>
{
public:
    enum class type : int
    {
        NONE,
        PLAYER,
		ENEMY
    };
    
public:
    
    Character();
    virtual ~Character();
    
    virtual void init(CharacterCreation* creation);
    virtual void update(float dt);
    void releaseCommands();
    
    virtual bool pushCommand(shared_ptr<Command>& command, bool replace = false);
    const string& getName() const;
    const Character::type& getType() const;
    int getMaxHP() const;
    int getCurrentHP();
    void decreHP(int decre);
    void heal(int heal);
    void destroy();
    bool isDestroyed();
    BulletCreation* getBullet() const;
    void setDieEffect(const string& effect);
protected:
    list<shared_ptr<Command>> _commandQueue;
    bool _destroy;
    
    int _maxHP;
    int _currentHP;
    BulletCreation* _bullet;
    string _dieEffect;
    
public:
    Sprite* _sprite;
//    ParticleSystemQuad* _particle;
    shared_ptr<RigidBody> _rigidBody;
    
    string _name;
    Character::type _type;
    
#if CHEAT
    bool _isBatTu = false;
#endif
};
