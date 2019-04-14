//
//  Mag.cpp
//  ShooterTopDown2D
//
//  Created by Khanh on 4/14/19.
//

#include "Mag.h"

Mag::Mag(const float& timeReshoot, const int& maxbullet, const float& timeReload):
_timeReshoot(timeReshoot),
_countTime(0.0f),
_currentBullets(maxbullet),
_maxBullet(maxbullet),
_timeReload(timeReload)
{}

void Mag::update(float dt)
{
    _countTime += dt;
}

bool Mag::canShoot()
{
    bool can = _countTime >= _timeReshoot;
    if(can && _currentBullets > 0)_countTime = 0.f;
    return can && _currentBullets > 0;
}
void Mag::resetCountTime()
{
    _countTime = 0.f;
}

void Mag::decreBullet(int decre)
{
    if(_currentBullets > 0)
        _currentBullets -= decre;
}

float Mag::reload()
{
    _currentBullets = _maxBullet;
    return _timeReload;
}

const int& Mag::getMaxBullet() const
{
    return _maxBullet;
}

const int& Mag::getCurrentBullet() const
{
    return _currentBullets;
}
