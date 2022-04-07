#include "axismotion.h"

AxisMotion::AxisMotion()
{
    state = Axis_Motion_Type_Null;
}

void AxisMotion::CycleRun(){
    if(target_speed > 0){
        if(speed < target_speed){
            speed += 10;
            if(speed >= target_speed){
                speed = target_speed;
            }

        }
    }
    else if(target_speed < 0){
        speed -= 10;
        if(speed <= target_speed){
            speed = target_speed;
        }
    }
    else{
        if(speed>0){
            speed -= 10;
            if(speed < 0){
                speed = target_speed;
            }
        }
        else if(speed < 0){

            speed += 10;
            if(speed > target_speed){
                speed = target_speed;
            }
        }
        else if(speed == 0){
            state = Axis_Motion_Type_Null;
        }
    }
}


int AxisMotion::GetAxisMotionState(){
    return state;
}

int AxisMotion::GetAxisMotionSpeed(){

    return speed;
}

int AxisMotion::GetAxisMotionPos(){
    return 0;
}

void AxisMotion::SetAxisMotionJog(int32_t speed){
    if(state != Axis_Motion_Type_Null){
        return;
    }
    state = Axis_Motion_Type_Jog;
    this->target_speed = speed;
    this->speed = 0;
}

void AxisMotion::SetAxisStop(){
    target_speed = 0;
//    state = Axis_Motion_Type_Null;
}
