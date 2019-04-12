#pragma once
#include "../Headers.h"

class Command;
class RigidBody;

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
    
    virtual void init();
    virtual void update(float dt);
    void releaseCommands();
    
    virtual void pushCommand(shared_ptr<Command>& command, bool replace = false);
    const string& getName() const;
    const Character::type& getType() const;
    void destroy();
    bool isDestroyed();
protected:
    queue<shared_ptr<Command>> _commandQueue;
    bool _destroy;
    
public:
    Sprite* _sprite;
    shared_ptr<RigidBody> _rigidBody;
    
    string _name;
    Character::type _type;
};
