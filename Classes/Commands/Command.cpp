//
//  Command.cpp
//  Shooter
//
//  Created by Khanh on 3/9/19.
//

#include "Command.h"
#include "../Characters/Character.h"

Command::Command():
_isFinished(true),
_object(nullptr)
{
}

const string& Command::getName() const
{
    return _name;
}

void Command::registAnObject(shared_ptr<Character> object, queue<shared_ptr<Command>>& queueOfObject)
{
    _object = object;
    shared_ptr<Command> cmd = shared_from_this();
    queueOfObject.push(cmd);
    _isFinished = false;
}

void Command::setFinished(bool finished)
{
    _isFinished = finished;
}

bool Command::isFinished()
{
    return _isFinished;
}
