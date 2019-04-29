//
//  Mag.cpp
//  ShooterTopDown2D
//
//  Created by Khanh on 4/14/19.
//

#include "Mag.h"
#include "Game.h"

Mag::Mag(const float& timeReshoot, const int& maxbullet, const float& timeReload):
_timeReshoot(timeReshoot),
_countTime(0.0f),
_currentBullets(maxbullet),
_maxBullet(maxbullet),
_timeReload(timeReload),
_isReload(false)
{

}

void Mag::update(float dt)
{
    if(_isReload && _countTime > _timeReload)
    {
        _currentBullets = _maxBullet;
        _isReload = false;
    }
    
    _countTime += dt;
}

bool Mag::canShoot()
{
    bool can = _countTime >= _timeReshoot;
    if(can && _currentBullets > 0)_countTime = 0.f;
//    if(_currentBullets == 0.f)_isReload = true;
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
    
    if(!_isReload && _currentBullets == 0)
    {
        _isReload = true;
        _countTime = 0.f;
    }
}

float Mag::reload()
{
    if(_currentBullets < _maxBullet)
    {
        _isReload = true;
        _countTime = 0.f;
        _currentBullets = 0;
        return _timeReload;
    }
    
    return 0.f;
}

bool Mag::isReloading() const
{
    return _isReload;
}

const int& Mag::getMaxBullet() const
{
    return _maxBullet;
}

const int& Mag::getCurrentBullet() const
{
    return _currentBullets;
}
