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

void ObjectsPool::update()
{
    for(auto i = _bulletBasicPool.begin(); i != _bulletBasicPool.end();)
    {
        if(i->get()->isDestroy())
        {
            i = _bulletBasicPool.erase(i);
        }
        else
        {
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
    _bulletBasicPool.clear();
}
