#pragma once

#include "Resources.h"
#include "../Defines/Templates.h"
#include <map>
#include "Headers.h"
#include "../Resource/Creations.h"


class ResourceManager : public pattern::Singleton<ResourceManager>
{
    std::map<res::define, std::string> _resMap;
    vector<CharacterCreation> _listCharacterCreations;
    map<res::define, BulletCreation> _listBulletCreations;
public:
    ResourceManager();
    ~ResourceManager();
    
    const std::string& at(const res::define& name) const;
    vector<CharacterCreation>& getListCharacterCreation();
    map<res::define, BulletCreation>& getListBulletCreation();
};
