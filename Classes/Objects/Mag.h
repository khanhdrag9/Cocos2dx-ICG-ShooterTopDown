#pragma once

class Mag
{
    float _timeReload;
    float _countTime;
    
    int _currentBullets;
    int _maxBullet;
    
public:
    Mag(const float& timereload, const int& maxbullet);
    virtual ~Mag(){}
    void update(float dt);
    bool canShoot();
    void resetCountTime();
};
