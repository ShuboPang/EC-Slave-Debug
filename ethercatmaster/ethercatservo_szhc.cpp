#include "ethercatservobase.h"

typedef union
{
    struct
    {
        uint32_t ready:1,
            act:1,
            alarm:1,
            alarm_num:7,
            buf_count:7,
            res:15;
    } bit;
    uint32_t all;
} SERVO_STATUS_UNION;

typedef union
{
    struct
    {
        uint32_t servo_on:1,
            clr_alarm:1,
            origin_mode:6,     //原点模式
            controlMode:3,
            res1:5,
            pos_num:7,
            dnID:8,
            clearExp:1;
    } bit;
    uint32_t all;
} SERVO_CTRL_UNION;


static ethercat_dev_id_info_t szhc_servo_support_list[] =
{
        {0x766,0x501},
};

static ec_pdo_entry_info_t servo_pdo_entries_output[] = {
    {0x7000, 0x01, 32}, /* Control Status */
    {0x7000, 0x02, 16}, /* Max Torque Limit */
    {0x7000, 0x03, 16}, /* Velocity Limit */
    {0x7000, 0x04, 16}, /* Target Torque */
    {0x7000, 0x05, 16}, /* Target Velocity */
    {0x7000, 0x06, 16}, /* Pos offset */
    {0x7000, 0x07, 16}, /* Sync Pos ERR */
    {0x7000, 0x08, 32}, /* Target Pos 2 */
    {0x7000, 0x09, 32}, /* Target Pos 3 */
    {0x7000, 0x0a, 32}, /* Target Pos 4 */
    {0x7000, 0x0b, 32}, /* Sync Pos ERR */
    {0x7001, 0x01, 32}, /* Control Status */
    {0x7001, 0x02, 16}, /* Max Torque Limit */
    {0x7001, 0x03, 16}, /* Velocity Limit */
    {0x7001, 0x04, 16}, /* Target Torque */
    {0x7001, 0x05, 16}, /* Target Velocity */
    {0x7001, 0x06, 16}, /* Pos offset */
    {0x7001, 0x07, 16}, /* Sync Pos ERR */
    {0x7001, 0x08, 32}, /* Target Pos 2 */
    {0x7001, 0x09, 32}, /* Target Pos 3 */
    {0x7001, 0x0a, 32}, /* Target Pos 4 */
    {0x7001, 0x0b, 32}, /* Sync Pos ERR */
};

static ec_pdo_entry_info_t servo_pdo_entries_input[] = {
    {0x6000, 0x01, 32}, /* Current Status */
    {0x6000, 0x02, 16}, /* Current Torque */
    {0x6000, 0x03, 16}, /* Current Velocity */
    {0x6000, 0x04, 32}, /* Current Pos */
    {0x6000, 0x05, 32}, /* Pos CMD ERR */
    {0x6001, 0x01, 32}, /* Current Status */
    {0x6001, 0x02, 16}, /* Current Torque */
    {0x6001, 0x03, 16}, /* Current Velocity */
    {0x6001, 0x04, 32}, /* Current Pos */
    {0x6001, 0x05, 32}, /* Pos CMD ERR */
};

typedef struct{
    SERVO_CTRL_UNION axis_1_ControlStatus;
    int32_t axis_1_MaxTorqueLimit:16;
    int32_t axis_1_VelocityLimit:16;
    int32_t axis_1_TargetTorque :16;
    int32_t axis_1_TargetVelocity :16;
    int32_t axis_1_PosOffset :16;
    int32_t axis_1_SyncPosERR :16;
    int32_t axis_1_TargetPos2 ;
    int32_t axis_1_TargetPos3 ;
    int32_t axis_1_TargetPos4 ;
    int32_t axis_1_SyncPosERR2 ;
    SERVO_CTRL_UNION axis_2_ControlStatus ;
    int32_t axis_2_MaxTorqueLimit :16;
    int32_t axis_2_VelocityLimit :16;
    int32_t axis_2_TargetTorque :16;
    int32_t axis_2_TargetVelocity :16;
    int32_t axis_2_PosOffset :16;
    int32_t axis_2_SyncPosERR :16;
    int32_t axis_2_TargetPos2 ;
    int32_t axis_2_TargetPos3 ;
    int32_t axis_2_TargetPos4 ;
    int32_t axis_2_SyncPosERR2 ;
}SERVO_SZHC_OUTPUT;


typedef struct{
    SERVO_STATUS_UNION axis_1_Current_Status ;
    int32_t axis_1_Current_Torque:16;
    int32_t axis_1_Current_Velocity :16;
    int32_t axis_1_Current_Pos ;
    int32_t axis_1_Pos_CMD_ERR ;
    SERVO_STATUS_UNION axis_2_Current_Status ;
    int32_t axis_2_Current_Torque:16 ;
    int32_t axis_2_Current_Velocity :16;
    int32_t axis_2_Current_Pos ;
    int32_t axis_2_Pos_CMD_ERR ;
}SERVO_SZHC_INPUT;

static int szhc_is_surport(ec_slavet* slave){
    for(int i = 0;i<sizeof (szhc_servo_support_list)/sizeof(ethercat_dev_id_info_t);i++){
        if(slave->eep_man == szhc_servo_support_list[i].vendor_id && slave->eep_id == szhc_servo_support_list[i].product_id){
            return 1;
        }
    }
    return 0;
}

static int szhc_axis_num(){
    return 2;
}

static void szhc_setup_config(ec_slavet* slave){
    SERVO_SZHC_OUTPUT* output = (SERVO_SZHC_OUTPUT*)slave->outputs;
    SERVO_SZHC_INPUT* input = (SERVO_SZHC_INPUT*)slave->inputs;
    output->axis_1_MaxTorqueLimit = 300;
    output->axis_2_MaxTorqueLimit = 300;
}

static void szhc_cycle_run(ec_slavet* slave,AxisMotion* axis,int sub_id){
    SERVO_SZHC_OUTPUT* output = (SERVO_SZHC_OUTPUT*)slave->outputs;
    SERVO_SZHC_INPUT* input = (SERVO_SZHC_INPUT*)slave->inputs;
    output->axis_1_MaxTorqueLimit = 300;
    output->axis_2_MaxTorqueLimit = 300;
    output->axis_1_ControlStatus.bit.pos_num = 1;
    output->axis_2_ControlStatus.bit.pos_num = 1;

    if(sub_id == 0){
        output->axis_1_ControlStatus.bit.servo_on = axis->GetAxisServoOn();
    }
    else if(sub_id == 1){
        output->axis_2_ControlStatus.bit.servo_on = axis->GetAxisServoOn();
    }

    if(input->axis_1_Current_Status.bit.act){
        if(sub_id == 0){
            if(axis->GetAxisMotionState() == AxisMotion::Axis_Motion_Type_Jog){
                output->axis_1_TargetPos2 += axis->GetAxisMotionSpeed();
            }
        }
    }
    else{
        output->axis_1_TargetPos2 = input->axis_1_Current_Pos;
        axis->SetAxisStop();
    }
    if(input->axis_2_Current_Status.bit.act){
        if(sub_id == 1){
            if(axis->GetAxisMotionState() == AxisMotion::Axis_Motion_Type_Jog){
                output->axis_2_TargetPos2 += axis->GetAxisMotionSpeed();
            }
        }
    }
    else{
        output->axis_2_TargetPos2 = input->axis_2_Current_Pos;
        axis->SetAxisStop();
    }
}

static int szhc_pdo_config_setup(uint16 slave)
{
    int retval;

    // map velocity
    uint16 map_1c12[] = {0x0001, 0x1600};
    uint16 map_1c13[] = {0x0001, 0x1a00};

    retval = 0;
    ec_SDOwrite(slave, 0x1600, 0x00, FALSE, sizeof(retval), &retval, EC_TIMEOUTSAFE);

    retval = 0;
    ec_SDOwrite(slave, 0x1a00, 0x00, FALSE, sizeof(retval), &retval, EC_TIMEOUTSAFE);

    for(int i = 0;i<sizeof(servo_pdo_entries_output)/sizeof(ec_pdo_entry_info_t);i++){
        retval = (servo_pdo_entries_output[i].index<<16)|(servo_pdo_entries_output[i].subindex<<8)|(servo_pdo_entries_output[i].bit_length);
        ec_SDOwrite(slave, 0x1600, 0x01+i, FALSE, sizeof(retval), &retval, EC_TIMEOUTSAFE);
    }

    retval = sizeof(servo_pdo_entries_output)/sizeof(ec_pdo_entry_info_t);
    ec_SDOwrite(slave, 0x1600, 0x00, FALSE, sizeof(retval), &retval, EC_TIMEOUTSAFE);

    for(int i = 0;i<sizeof(servo_pdo_entries_input)/sizeof(ec_pdo_entry_info_t);i++){
        retval = (servo_pdo_entries_input[i].index<<16)|(servo_pdo_entries_input[i].subindex<<8)|(servo_pdo_entries_input[i].bit_length);
        ec_SDOwrite(slave, 0x1a00, 0x01+i, FALSE, sizeof(retval), &retval, EC_TIMEOUTSAFE);
    }

    retval = sizeof(servo_pdo_entries_input)/sizeof(ec_pdo_entry_info_t);
    ec_SDOwrite(slave, 0x1a00, 0x00, FALSE, sizeof(retval), &retval, EC_TIMEOUTSAFE);

    // Set PDO mapping using Complete Access
    // Strange, writing CA works, reading CA doesn't
    // This is a protocol error of the slave.
    retval += ec_SDOwrite(slave, 0x1c12, 0x00, TRUE, sizeof(map_1c12), &map_1c12, EC_TIMEOUTSAFE);
    retval += ec_SDOwrite(slave, 0x1c13, 0x00, TRUE, sizeof(map_1c13), &map_1c13, EC_TIMEOUTSAFE);



    while(EcatError) printf("%s", ec_elist2string());

    printf("HC2000setup slave %d set, retval = %d\n", slave, retval);

    return 1;
}

void szhc_clear_alarm(ec_slavet* slave,int axis_id){
    SERVO_SZHC_OUTPUT* output = (SERVO_SZHC_OUTPUT*)slave->outputs;
    if(axis_id == 0){
        output->axis_1_ControlStatus.bit.clr_alarm = 1;
    }
    else if(axis_id == 1){
        output->axis_2_ControlStatus.bit.clr_alarm = 1;
    }
}

void szhc_servo_on(ec_slavet* slave,int axis_id,bool state){
    SERVO_SZHC_OUTPUT* output = (SERVO_SZHC_OUTPUT*)slave->outputs;
    if(axis_id == 0){
        output->axis_1_ControlStatus.bit.servo_on = state?1:0;
    }
    else if(axis_id == 1){
        output->axis_2_ControlStatus.bit.servo_on = state?1:0;
    }
}

int szhc_get_servo_alarm(ec_slavet* slave,int axis_id){
    SERVO_SZHC_INPUT* input = (SERVO_SZHC_INPUT*)slave->inputs;
    if(axis_id == 0){
        if(input->axis_1_Current_Status.bit.alarm){
            return input->axis_1_Current_Status.bit.alarm_num;
        }
        return 0;
    }

    else if(axis_id == 1){
        if(input->axis_2_Current_Status.bit.alarm){
            return input->axis_2_Current_Status.bit.alarm_num;
        }
        return 0;
    }
    return 0;
}

int szhc_get_servo_pos (ec_slavet* slave,int axis_id){
    SERVO_SZHC_INPUT* input = (SERVO_SZHC_INPUT*)slave->inputs;
    if(axis_id == 0){
        return input->axis_1_Current_Pos;
    }

    else if(axis_id == 1){
        return input->axis_2_Current_Pos;
    }
    return 0;
}

int szhc_get_servo_on(ec_slavet* slave,int axis_id){
    SERVO_SZHC_INPUT* input = (SERVO_SZHC_INPUT*)slave->inputs;
    if(axis_id == 0){
        return input->axis_1_Current_Status.bit.act;
    }
    else if(axis_id == 1){
        return input->axis_2_Current_Status.bit.act;
    }
    return 0;
}

int szhc_get_servo_cmd_pos(ec_slavet* slave,int axis_id){
    SERVO_SZHC_OUTPUT* output = (SERVO_SZHC_OUTPUT*)slave->outputs;
    if(axis_id == 0){
        return output->axis_1_TargetPos2;
    }
    else if(axis_id == 1){
        return output->axis_2_TargetPos2;
    }
    return 0;
}


float szhc_gget_servo_torque(ec_slavet* slave,int axis_id){
    float torque = 0.0;
    SERVO_SZHC_INPUT* input = (SERVO_SZHC_INPUT*)slave->inputs;
    if(axis_id == 0){
        torque =  input->axis_1_Current_Torque/25.08;
    }
    else if(axis_id == 1){
        torque =  input->axis_2_Current_Torque/25.08;
    }
    return torque;
}

float szhc_gget_servo_velocity(ec_slavet* slave,int axis_id){
    float velocity = 0.0;
    SERVO_SZHC_INPUT* input = (SERVO_SZHC_INPUT*)slave->inputs;
    if(axis_id == 0){
        velocity =  input->axis_1_Current_Velocity;
    }
    else if(axis_id == 1){
        velocity =  input->axis_2_Current_Velocity;
    }
    return velocity;
}


const _EthercatSlaveConfig ethercatservo_szhc={
    szhc_is_surport,
    szhc_axis_num,
    szhc_setup_config,
    szhc_cycle_run,
    szhc_pdo_config_setup,
    szhc_clear_alarm,
    szhc_servo_on,
    szhc_get_servo_alarm,
    szhc_get_servo_pos,
    szhc_get_servo_on,
    szhc_get_servo_cmd_pos,
    szhc_gget_servo_torque,
    szhc_gget_servo_velocity
};
