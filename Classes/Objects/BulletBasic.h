#pragma once
#include "../Headers.h"

class BulletBasic
{
protected:
    Vec2 _speed;
    string _name;
    
public:
    BulletBasic();
    virtual ~BulletBasic();
    static shared_ptr<BulletBasic> createBulletBasic(const Vec2& position, const float& angle, const Vec2& speed = Vec2(0,0), bool forceShoot = false);
    void Shoot(const Vec2& velocity);
    void Shoot();
    
    Vec2 getSpeed() const;
    const string& getName() const;
    
    Sprite* _sprite;
    PhysicsBody* _rigidBody;
};
