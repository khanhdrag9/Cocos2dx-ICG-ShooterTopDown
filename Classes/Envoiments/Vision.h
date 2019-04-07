#pragma once
#include "../Headers.h"

class Character;

class Vision
{
    shared_ptr<Character> _obj;
    bool _isDraw;
public:
    Vision(){}
    Vision(shared_ptr<Character> obj);
    void update(DrawNode* draw, ClippingNode* clipper = nullptr);
    void setDraw(bool draw);
};
