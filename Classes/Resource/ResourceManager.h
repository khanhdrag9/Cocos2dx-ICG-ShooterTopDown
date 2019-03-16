#pragma once

#include <iostream>
#include "Resources.h"
#include "../Defines/Templates.h"
#include <map>


class ResourceManager : public pattern::Singleton<ResourceManager>
{
    std::map<res::define, std::string> _resMap;
    
public:
    ResourceManager();
    ~ResourceManager();
    
    const std::string& at(const res::define& name) const;
};
