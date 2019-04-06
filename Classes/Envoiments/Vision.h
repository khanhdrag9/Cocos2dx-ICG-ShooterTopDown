#pragma once
#include "../Headers.h"

class Character;

class Vision
{
    shared_ptr<Character> _obj;
    ClippingNode* _clipper;
    
public:
    Vision(){}
    Vision(shared_ptr<Character> obj, DrawNode* draw, Node* soild);
    void update(DrawNode* draw);
};
