#pragma once
#include "../Headers.h"

class Command;
class RigidBody;

class CharacterCreation
{
    string _sprite;
    CC_SYNTHESIZE_READONLY(string, _image, Image);
    CC_SYNTHESIZE_READONLY(string, _loadImage, LoadImage);
    string _bullet;
    int _maxHP;
    float _speed;
    int _maxBullet;
    float _timeReload;
    int _damgeBullet;
public:
    CharacterCreation(const string& sprite,
                      const string& image,
                      const string& loadImage,
                      const string& bullet,
                      const int& maxHP,
                      const float& speed,
                      const int& maxBullet,
                      const float& timeReload,
                      const int& damge);
    
    
    friend class Character;
    friend class Player;
    friend class Bot;
};

class Character : public enable_shared_from_this<Character>
{
public:
    enum class type : int
    {
        NONE,
        PLAYER,
		ENEMY
    };
    
public:
    
    Character();
    virtual ~Character();
    
    virtual void init(CharacterCreation* creation);
    virtual void update(float dt);
    void releaseCommands();
    
    virtual bool pushCommand(shared_ptr<Command>& command, bool replace = false);
    const string& getName() const;
    const Character::type& getType() const;
    int getMaxHP() const;
    int getCurrentHP();
    void decreHP(int decre);
    void heal(int heal);
    void destroy();
    bool isDestroyed();
    const string& getBulletSprite() const;
    CC_SYNTHESIZE_READONLY(int, _bulletDamge, BulletDamge);
protected:
    list<shared_ptr<Command>> _commandQueue;
    bool _destroy;
    
    int _maxHP;
    int _currentHP;
    string _bulletSprite;
public:
    Sprite* _sprite;
    shared_ptr<RigidBody> _rigidBody;
    
    string _name;
    Character::type _type;
};
