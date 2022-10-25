#include "axismotion.h"

AxisMotion::AxisMotion()
{
    state = Axis_Motion_Type_Null;
    clear_alarm= false;
    servo_on_state = false;
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

void AxisMotion::SetAxisStop(bool quick){
    target_speed = 0;
    if(quick){
        speed = 0;
    }
//    state = Axis_Motion_Type_Null;
}

void AxisMotion::SetAxisServoOn(bool state){
    servo_on_state = state;
}

bool AxisMotion::GetAxisServoOn(){
    return servo_on_state;
}
