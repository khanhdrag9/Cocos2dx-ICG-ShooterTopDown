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
    _bulletImage = bullet._bulletImage;
    _damgeBullet = bullet._damgeBullet;
    _particle = bullet._particle;
    _explosion = bullet._explosion;
    _speed = bullet._speed;
    _shootSound = bullet._shootSound;
    _explosionSound = bullet._explosionSound;
}

BulletCreation::BulletCreation(const string& sprite, const string& image, const int& damge, const float& speed, const string& particle, const string& explosion, const string& shootSound, const string& expSound = ""):
_bulletSprite(sprite),
_bulletImage(image),
_damgeBullet(damge),
_speed(speed),
_particle(particle),
_explosion(explosion),
_shootSound(shootSound),
_explosionSound(expSound)
{
}

CharacterCreation::CharacterCreation(const string& sprite,
                                     const string& image,
                                     const string& loadImage,
                                     const string& die,
                                     const string& dieSound,
                                     const int& maxHP,
                                     const float& speed,
                                     const int& maxBullet,
                                     const float& timeReload,
                                     const BulletCreation& bullet)
:
_sprite(sprite)
,_image(image)
,_loadImage(loadImage)
,_die(die)
,_dieSound(dieSound)
,_maxHP(maxHP)
,_speed(speed)
,_maxBullet(maxBullet)
,_timeReload(timeReload)
{
    _bullet = BulletCreation(bullet);
}
