#pragma once
#include "../Headers.h"
#include "../Characters/Character.h"

class BulletBasic : public Character
{
protected:
    Vec2 _speed;
    int _damge;
    CC_SYNTHESIZE(float, _liveDuration, LiveDuration);
public:
    BulletBasic();
    virtual ~BulletBasic();
    static shared_ptr<BulletBasic> createBulletBasic(const string& sprite, const Vec2& position, const float& angle, const Vec2& speed = Vec2(0,0), bool forceShoot = false);
    void Shoot(const Vec2& velocity);
    void Shoot();
    
    void init(CharacterCreation*) override;
    void update(float dt) override;
    
    Vec2 getSpeed() const;
    void setDamege(int damge);
    int getDamge() const;
    void setParticle(const string& particle);
};
