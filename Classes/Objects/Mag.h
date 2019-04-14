#pragma once

class Mag
{
    float _timeReshoot;
    float _countTime;
    
    int _currentBullets;
    int _maxBullet;
    
    float _timeReload;
    
public:
    Mag(const float& timeReshoot, const int& maxbullet, const float& timeReload);
    virtual ~Mag(){}
    void update(float dt);
    bool canShoot();
    void resetCountTime();
    void decreBullet(int decre = 1);
    float reload();
};
