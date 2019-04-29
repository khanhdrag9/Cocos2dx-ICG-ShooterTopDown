//
//  Creations.cpp
//  ShooterTopDown2D
//
//  Created by Khanh on 4/29/19.
//

#include "Creations.h"

BulletCreation::BulletCreation(const BulletCreation& bullet)
{
    _bulletSprite = bullet._bulletSprite;
    _damgeBullet = bullet._damgeBullet;
    _particle = bullet._particle;
}

BulletCreation::BulletCreation(const string& sprite, const int& damge, const string& particle):
_bulletSprite(sprite),
_damgeBullet(damge),
_particle(particle)
{
}

CharacterCreation::CharacterCreation(const string& sprite,
                                     const string& image,
                                     const string& loadImage,
                                     const int& maxHP,
                                     const float& speed,
                                     const int& maxBullet,
                                     const float& timeReload,
                                     const BulletCreation& bullet)
:
_sprite(sprite)
,_image(image)
,_loadImage(loadImage)
,_maxHP(maxHP)
,_speed(speed)
,_maxBullet(maxBullet)
,_timeReload(timeReload)
{
    _bullet = BulletCreation(bullet);
}
