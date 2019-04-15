#pragma once

#include "../Headers.h"

class Character;

class Command : public enable_shared_from_this<Command>
{
public:
    Command();
    ~Command();
    
    void setFinished(bool finished);
    bool isFinished();
    
    void registAnObject(shared_ptr<Character> object, queue<shared_ptr<Command>>& queueOfObject);
    
    const string& getName() const;
    
    virtual void update(float dt) = 0;
    
protected:
    bool _isFinished;
    string _name;
    
    weak_ptr<Character> _object;
};
