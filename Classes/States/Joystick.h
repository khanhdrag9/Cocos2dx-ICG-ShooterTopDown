#pragma once
#include "../Headers.h"
#if USE_JOYSTICK

class Joystick : public Layer
{
    CC_SYNTHESIZE_READONLY(Vec2, _velocity, Velocity);
    CC_SYNTHESIZE_READONLY(float, _angle, Angle);
    CC_SYNTHESIZE_READONLY(bool, _isPress, Press);
    Sprite* _thumb;
    Vec2 _center;
    
private:
    bool init() override;
    void updateVelocity(Vec2 point);
    void resetJoystick();
    bool handleLastTouch();
    
    bool touchBegan(Touch*, Event*);
    void touchMoved(Touch*, Event*);
    void touchEnded(Touch*, Event*);
    void touchCancelled(Touch*, Event*);
    
public:
    static float offset_x;
    static float offset_y;
    static float radius;
    static float thumb_radius;
    
    CREATE_FUNC(Joystick);
};

#endif
