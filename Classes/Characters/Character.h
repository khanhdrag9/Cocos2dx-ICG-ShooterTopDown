#pragma once
#include "../Headers.h"

class Command;
//class CommandHandle;

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
    
    virtual void pushCommand(shared_ptr<Command>& command);
    const string& getName() const;
    const Character::type& getType() const;
    
protected:
    queue<shared_ptr<Command>> _commandQueue;
    //unique_ptr<CommandHandle> _commandHandle;
    
    
public:
    Sprite* _sprite;
    PhysicsBody* _rigidBody;
    
    string _name;
    Character::type _type;
};
