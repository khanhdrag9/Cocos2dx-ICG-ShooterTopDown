#pragma once
#include "../Headers.h"
#include "../Characters/Character.h"

class BulletBasic : public Character
{
protected:
    Vec2 _speed;
    bool _isDestroy;
    int _damge;
public:
    BulletBasic();
    virtual ~BulletBasic();
    static shared_ptr<BulletBasic> createBulletBasic(const Vec2& position, const float& angle, const Vec2& speed = Vec2(0,0), bool forceShoot = false);
    void Shoot(const Vec2& velocity);
    void Shoot();
    
    void init() override;
    void update(float dt) override;
    
    Vec2 getSpeed() const;
    int getDamge() const;
    
    bool isDestroy();
    void destroy();
};
