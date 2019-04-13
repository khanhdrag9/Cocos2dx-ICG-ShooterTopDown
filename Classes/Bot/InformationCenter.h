#pragma once
#include "../Defines/Templates.h"
#include "../Headers.h"

class Character;
class Information;


struct InformationCharacter
{
    weak_ptr<Character> object;
    shared_ptr<Information> information;
    
    InformationCharacter(){}
    InformationCharacter(const shared_ptr<Character>& character, shared_ptr<Information> info)
    {
        information = info;
        object = character;
    }
    ~InformationCharacter()
    {
        information = nullptr;
    }
};

class InformationCenter : public pattern::Singleton<InformationCenter>
{
    queue<InformationCharacter> _queueInformation;
    bool _isStop;
public:
    InformationCenter();
    ~InformationCenter();
    
    void update();
    void pushInformation(const shared_ptr<Character>& character, shared_ptr<Information> information);
    void stop();
    void clear();
};
