//
//  Mag.cpp
//  ShooterTopDown2D
//
//  Created by Khanh on 4/14/19.
//

#include "Mag.h"

Mag::Mag(const float& timereload, const int& maxbullet):
_timeReload(timereload),
_countTime(0.0f),
_currentBullets(maxbullet),
_maxBullet(maxbullet)
{}

void Mag::update(float dt)
{
    _countTime += dt;
}

bool Mag::canShoot()
{
    bool can = _countTime >= _timeReload;
    if(can)_countTime = 0.f;
    return can;
}
void Mag::resetCountTime()
{
    _countTime = 0.f;
}
