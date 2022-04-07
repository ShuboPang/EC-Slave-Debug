#ifndef AXISMOTION_H
#define AXISMOTION_H

#include <stdint.h>

class AxisMotion
{
public:
    AxisMotion();

    enum{
        Axis_Motion_Type_Null,      //< 无运动
        Axis_Motion_Type_Jog,       //< 点动
        Axis_Motion_Type_Pos,       //< 绝对位置运动
        Axis_Motion_Type_RPos,      //< 相对位置运动
    };

    void CycleRun();

    int GetAxisMotionState();

    int GetAxisMotionSpeed();

    int GetAxisMotionPos();

    void SetAxisMotionJog(int32_t speed);

    void SetAxisStop();

public:
    uint32_t state;     //< 运动状态
    int32_t speed;
    int32_t target_speed;
    int32_t timer;
};

#endif // AXISMOTION_H
