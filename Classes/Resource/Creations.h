#pragma once
#include "../Headers.h"

class BulletCreation
{
    CC_SYNTHESIZE_READONLY(string, _bulletSprite, BulletSprite);
    CC_SYNTHESIZE_READONLY(int,_damgeBullet, DamgeBullet);
    CC_SYNTHESIZE_READONLY(string, _particle, Particle);
    CC_SYNTHESIZE_READONLY(string, _explosion, Explosion);
public:
    BulletCreation(){}
    BulletCreation(const BulletCreation& bullet);
    BulletCreation(const string& sprite, const int& damge, const string& particel = "", const string& explosion = "");
};

class CharacterCreation
{
    string _sprite;
    CC_SYNTHESIZE_READONLY(string, _image, Image);
    CC_SYNTHESIZE_READONLY(string, _loadImage, LoadImage);
    CC_SYNTHESIZE_READONLY(string, _die, DieEffect);
    int _maxHP;
    float _speed;
    int _maxBullet;
    float _timeReload;
    BulletCreation _bullet;
public:
    CharacterCreation(const string& sprite,
                      const string& image,
                      const string& loadImage,
                      const string& die,
                      const int& maxHP,
                      const float& speed,
                      const int& maxBullet,
                      const float& timeReload,
                      const BulletCreation& bullet);
    
    
    friend class Character;
    friend class Player;
    friend class Bot;
};
