//
//  BulletPool.cpp
//  Shooter
//
//  Created by Khanh on 3/11/19.
//

#include "ObjectsPool.h"
#include "BulletBasic.h"

ObjectsPool::~ObjectsPool()
{
	_bulletBasicPool.clear();
}

void ObjectsPool::update(float dt)
{
    for(auto i = _bulletBasicPool.begin(); i != _bulletBasicPool.end();)
    {
        if(i->get()->isDestroyed())
        {
            i = _bulletBasicPool.erase(i);
        }
        else
        {
            i->get()->update(dt);
            i++;
        }
    }
}

void ObjectsPool::pushBulletBasic(shared_ptr<BulletBasic> bulletbasic)
{
    _bulletBasicPool.push_back(bulletbasic);
}

void ObjectsPool::clear()
{
    for(auto& bullet : _bulletBasicPool)
        bullet->destroy();
    _bulletBasicPool.clear();
}
