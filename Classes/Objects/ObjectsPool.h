#pragma once
#include "../Defines/Templates.h"
#include "../Headers.h"

class BulletBasic;
class Character;

class ObjectsPool : public pattern::Singleton<ObjectsPool>
{
    list<shared_ptr<BulletBasic>> _bulletBasicPool;
public:
    ~ObjectsPool();
    
    void update(float);
    void pushBulletBasic(shared_ptr<BulletBasic> bulletbasic);
    void clear();
};
