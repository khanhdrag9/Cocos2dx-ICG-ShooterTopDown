#pragma once
#include "../Headers.h"
#if USE_JOYSTICK

class Joystick : public Layer
{
    CC_SYNTHESIZE_READONLY(Vec2, _velocityLeft, VelocityLeft);
    CC_SYNTHESIZE_READONLY(float, _angleLeft, AngleLeft);
    CC_SYNTHESIZE_READONLY(bool, _isPressLeft, PressLeft);
    Sprite* _thumbLeft;
    Sprite* _bgLeft;
    Vec2 _centerLeft;
    
    CC_SYNTHESIZE_READONLY(Vec2, _velocityRight, VelocityRight);
    CC_SYNTHESIZE_READONLY(float, _angleRight, AngleRight);
    CC_SYNTHESIZE_READONLY(bool, _isPressRight, PressRight);
    Sprite* _thumbRight;
    Sprite* _bgRight;
    Vec2 _centerRight;
    
    enum side
    {
        LEFT,
        RIGHT
    };
private:
    bool init() override;
    void updateVelocity(Vec2 point, side s);
    void resetJoystick(side s);
    bool handleLastTouch(side s);
    
//    bool touchBegan(Touch*, Event*);
//    void touchMoved(Touch*, Event*);
//    void touchEnded(Touch*, Event*);
//    void touchCancelled(Touch*, Event*);

    void touchBegan(const vector<Touch*>&, Event*);
    void touchMoved(const vector<Touch*>&, Event*);
    void touchEnded(const vector<Touch*>&, Event*);
    void touchCancelled(const vector<Touch*>&, Event*);

public:
    static float offset_x;
    static float offset_y;
    static float radius;
    static float thumb_radius;
    
    CREATE_FUNC(Joystick);
};

#endif
