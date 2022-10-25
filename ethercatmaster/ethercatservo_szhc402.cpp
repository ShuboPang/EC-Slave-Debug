#include "ethercatservobase.h"

enum{
    S_ADDR_Error_Code = 0x603F, //错误码（重复，即一个错误码对应多个报警名称，不建议使用） UNSIGNED16  RO  Y
    S_ADDR_Control_Word = 0x6040, //控制字 UNSIGNED16 RW Y
    S_ADDR_Status_Word = 0x6041, //状态字 UNSIGNED16 RO  Y

    S_ADDR_Mode_Of_Operation = 0x6060, //运行模式 INTEGER8 RW  Y   8:周期性同步位置模式； 9：周期性同步速度模式；
    S_ADDR_Mode_Of_Operation_Display = 0x6061, //运行模式读取 INTEGER8 RO  Y

    S_ADDR_Position_Actual_Value_Increment = 0x6063, //位置反馈增量值 INTEGER32 RO  Y
    S_ADDR_Position_Actual_Value_Abs = 0x6064, //绝对位置反馈 INTEGER32 RO  Y
    S_ADDR_Velocity_Actual_Value = 0x606C, //实际速度 INTEGER32 RO  Y 单位：r/min 精度：0
    S_ADDR_Target_Torque = 0x6071, //目标转矩  INTEGER16 RW Y 单位：0.1%额定转矩；P4.14
    S_ADDR_Max_Torque_Limit = 0x6072, //最大转矩限制  UNSIGNED16 RW Y
    S_ADDR_Torque_Actual_Value = 0x6077, //实际转矩 INTEGER16 RO  Y 精度：0.1%； 单位：额定转矩
    S_ADDR_Current_Actual_Value = 0x6078, //实际输出电流 INTEGER16 RO Y 精度：0.1； 单位：mA；
    S_ADDR_DC_Link_Circuit_Voltage = 0x6079, //母线电压 UNSIGNED32 RO N
    S_ADDR_Target_Position = 0x607A, //目标位置 INTEGER32 RW  Y
    S_ADDR_Max_Profile_Velocity = 0x607F, //最大转速限制  UNSIGNED32 RW  Y
    S_ADDR_HOMING_MODE  = 0X6098, //回原点模式 USINT8 RW Y
    S_ADDR_Positive_Torque_Limit = 0x60E0, //正向力矩限幅 UNSIGNED16 RW  Y
    S_ADDR_Negative_Torque_Limit = 0x60E1, //反向力矩限幅 UNSIGNED16 RW  Y
    S_ADDR_Following_Error_Actual_Value = 0x60F4, //位置指令偏差 INTEGER32 RO  Y
    S_ADDR_Target_Velocity = 0x60FF, //目标速度 INTEGER32 RW Y （P4.13总线速度指令）


    //轴2的PDO 0x68××
    S_ADDR_Error_Code2 = 0x683F, //错误码（重复，即一个错误码对应多个报警名称，不建议使用） UNSIGNED16  RO  Y
    S_ADDR_Control_Word2 = 0x6840, //控制字 UNSIGNED16 RW Y
    S_ADDR_Status_Word2 = 0x6841, //状态字 UNSIGNED16 RO  Y

    S_ADDR_Mode_Of_Operation2 = 0x6860, //运行模式 INTEGER8 RW  Y   8:周期性同步位置模式； 9：周期性同步速度模式；
    S_ADDR_Mode_Of_Operation_Display2 = 0x6861, //运行模式读取 INTEGER8 RO  Y

    S_ADDR_Position_Actual_Value_Increment2 = 0x6863, //位置反馈增量值 INTEGER32 RO  Y
    S_ADDR_Position_Actual_Value_Abs2 = 0x6864, //绝对位置反馈 INTEGER32 RO  Y
    S_ADDR_Velocity_Actual_Value2 = 0x686C, //实际速度 INTEGER32 RO  Y 单位：r/min 精度：0
    S_ADDR_Target_Torque2 = 0x6871, //目标转矩  INTEGER16 RW Y 单位：0.1%额定转矩；P4.14
    S_ADDR_Max_Torque_Limit2 = 0x6872, //最大转矩限制  UNSIGNED16 RW Y
    S_ADDR_Torque_Actual_Value2 = 0x6877, //实际转矩 INTEGER16 RO  Y 精度：0.1%； 单位：额定转矩
    S_ADDR_Current_Actual_Value2 = 0x6878, //实际输出电流 INTEGER16 RO Y 精度：0.1； 单位：mA；
    S_ADDR_DC_Link_Circuit_Voltage2 = 0x6879, //母线电压 UNSIGNED32 RO N
    S_ADDR_Target_Position2 = 0x687A, //目标位置 INTEGER32 RW  Y
    S_ADDR_Max_Profile_Velocity2 = 0x687F, //最大转速限制  UNSIGNED32 RW  Y
    S_ADDR_HOMING_MODE2  = 0X6898, //回原点模式 USINT8 RW Y
    S_ADDR_Positive_Torque_Limit2 = 0x68E0, //正向力矩限幅 UNSIGNED16 RW  Y
    S_ADDR_Negative_Torque_Limit2 = 0x68E1, //反向力矩限幅 UNSIGNED16 RW  Y
    S_ADDR_Following_Error_Actual_Value2 = 0x68F4, //位置指令偏差 INTEGER32 RO  Y

    S_ADDR_Target_Velocity2 = 0x68FF, //目标速度 INTEGER32 RW Y （P4.13总线速度指令）

};



static ethercat_dev_id_info_t szhc_servo_support_list[] =
{
    {0x92b,0x402},
};

static ec_pdo_entry_info_t servo_pdo_entries_output[] = {
    /************* Out ***********/
    {S_ADDR_Control_Word, 0x00, 16}, //control
    {S_ADDR_Target_Position, 0x00, 32}, //目标位置
    {S_ADDR_Positive_Torque_Limit, 0x00, 16}, //正向转矩限幅
    {S_ADDR_Negative_Torque_Limit, 0x00, 16}, //反向转矩限幅
    {S_ADDR_Mode_Of_Operation, 0x00, 8}, // 操作模式
    {S_ADDR_HOMING_MODE, 0x00, 8}, // HomingMode
};
static ec_pdo_entry_info_t servo_pdo_entries_output2[] = {

    {S_ADDR_Control_Word2, 0x00, 16}, //control
    {S_ADDR_Target_Position2, 0x00, 32}, //目标位置
    {S_ADDR_Positive_Torque_Limit2, 0x00, 16}, //正向转矩限幅
    {S_ADDR_Negative_Torque_Limit2, 0x00, 16}, //反向转矩限幅
    {S_ADDR_Mode_Of_Operation2, 0x00, 8}, // 操作模式
    {S_ADDR_HOMING_MODE2, 0x00, 8}, // HomingMode
};

static ec_pdo_entry_info_t servo_pdo_entries_input[] = {
    {S_ADDR_Status_Word, 0x00, 16}, //状态字
    {S_ADDR_Error_Code, 0x00, 16}, //错误码
    {S_ADDR_Position_Actual_Value_Abs, 0x00, 32}, // 实际位置
    {S_ADDR_Following_Error_Actual_Value, 0x00, 32}, // 跟随误差
    {S_ADDR_Velocity_Actual_Value, 0x00, 32}, // 实际速度
    {S_ADDR_Torque_Actual_Value, 0x00, 16}, // 实际转矩
    {S_ADDR_Mode_Of_Operation_Display, 0x00, 8}, // 实际转矩
};
static ec_pdo_entry_info_t servo_pdo_entries_input2[] = {
    {S_ADDR_Status_Word2, 0x00, 16}, //状态字
    {S_ADDR_Error_Code2, 0x00, 16}, //错误码
    {S_ADDR_Position_Actual_Value_Abs2, 0x00, 32}, // 实际位置
    {S_ADDR_Following_Error_Actual_Value2, 0x00, 32}, // 跟随误差
    {S_ADDR_Velocity_Actual_Value2, 0x00, 32}, // 实际速度
    {S_ADDR_Torque_Actual_Value2, 0x00, 16}, // 实际转矩
    {S_ADDR_Mode_Of_Operation_Display2, 0x00, 8}, // 实际转矩
};

typedef union
{
    struct
    {
        uint16_t so:1,
            ev:1,
            qs:1,
            eo:1,
            oms:3,
            fr:1,
            h:1,
            oms2:1,
            res:6;
    } bit;
    uint16_t all;
} SERVO_CTRL_UNION;

typedef union
{
    struct
    {
        uint16_t rtso:1,        //< ready to switch on
            so:1,       //< switched on
            oe:1,       //< operation enabled
            f:1,        //< fault
            ve:1,       //< voltage enabled
            qs:1,       //<  quick stop
            sod:1,      //< switch on disabled
            w:1,        //< warning
            r1:1,       //<
            rm:1,       //<  remote
            oms:1,      //<  operation mode specific
            ila:1,      //< internal limit active
            oms2:2,     //<  operation mode specific
            r2:2;       //< r

    } bit;
    uint16_t all;
} SERVO_STATUS_UNION;

PACKED_BEGIN
typedef struct{
    SERVO_CTRL_UNION ControlStatus_1;
    int32 target_pos_1;
    int32 positive_torque_limit_1:16;
    int32 negative_torque_limit_1:16;
    uint16 mode_option_1:8;
    uint16 homing_mode_1:8;

    SERVO_CTRL_UNION ControlStatus_2;
    int32 target_pos_2;
    int32 positive_torque_limit_2:16;
    int32 negative_torque_limit_2:16;
    uint16 mode_option_2:8;
    uint16 homing_mode_2:8;
}SERVO_SZHC_OUTPUT;


typedef struct{
    SERVO_STATUS_UNION CurrentStatus_1 ;
    uint16 error_code_1;
    int32 postion_actual_abs_1;
    int32 following_error_value_1;
    int32 velocity_actual_value_1;
    uint16 torque_actual_value_1;
    uint8 mode_of_operation_display_1;

    SERVO_STATUS_UNION CurrentStatus_2 ;
    uint16 error_code_2;
    int32 postion_actual_abs_2;
    int32 following_error_value_2;
    int32 velocity_actual_value_2;
    uint16 torque_actual_value_2;
    uint8 mode_of_operation_display_2;
}SERVO_SZHC_INPUT;

PACKED_END

static int szhc402_is_surport(ec_slavet* slave){
    printf("SERVO_SZHC_INPUT = %d %d\n",sizeof (SERVO_SZHC_INPUT),sizeof (SERVO_SZHC_INPUT));
    for(int i = 0;i<sizeof (szhc_servo_support_list)/sizeof(ethercat_dev_id_info_t);i++){
        if(slave->eep_man == szhc_servo_support_list[i].vendor_id && slave->eep_id == szhc_servo_support_list[i].product_id){
            return 1;
        }
    }
    return 0;
}

static int szhc402_axis_num(){
    return 2;
}

static void szhc402_setup_config(ec_slavet* slave){
    SERVO_SZHC_OUTPUT* output = (SERVO_SZHC_OUTPUT*)slave->outputs;
    SERVO_SZHC_INPUT* input = (SERVO_SZHC_INPUT*)slave->inputs;
    output->positive_torque_limit_1 = 300;
    output->positive_torque_limit_2 = 300;
    output->negative_torque_limit_1 = 300;
    output->negative_torque_limit_2 = 300;
}

static void szhc402_cycle_run(ec_slavet* slave,AxisMotion* axis,int sub_id){
    SERVO_SZHC_OUTPUT* output = (SERVO_SZHC_OUTPUT*)slave->outputs;
    SERVO_SZHC_INPUT* input = (SERVO_SZHC_INPUT*)slave->inputs;
    output->positive_torque_limit_1 = 300;
    output->positive_torque_limit_2 = 300;
    output->negative_torque_limit_1 = 300;
    output->negative_torque_limit_2 = 300;

    if(sub_id == 0){
        uint32 readStatus = input->CurrentStatus_1.all;
        int controlW = 6;
        if(axis->GetAxisServoOn()){
            int readStatus  = input->CurrentStatus_1.all &0x03;

            switch (readStatus) {
            case 0x00:
                controlW = 0x6;
            case 0x1:
                controlW = 0x7;
                break;
            case 0x3:
                controlW = 0xF;
                break;
            default:
                break;
            }
        }
        else{
            controlW = 0;
        }
        if(axis->clear_alarm){
            printf("axis->clear_alarm = %d\n",axis->clear_alarm);
            if(readStatus & 8){
                controlW = 0x80;
            }
            axis->clear_alarm = false;

        }
        output->ControlStatus_1.all = controlW;
    }
    else if(sub_id == 1){
        uint32 readStatus = input->CurrentStatus_2.all;
        int controlW = 6;
        if(axis->GetAxisServoOn()){
            int readStatus  = input->CurrentStatus_2.all &0x03;

            switch (readStatus) {
            case 0x00:
                controlW = 0x6;
            case 0x1:
                controlW = 0x7;
                break;
            case 0x3:
                controlW = 0xF;
                break;
            default:
                break;
            }
        }
        else{
            controlW = 0;
        }
        if(axis->clear_alarm){
            if(readStatus & 8){
                axis->clear_alarm = false;
                controlW = 0x80;
            }

        }
        output->ControlStatus_2.all = controlW;
    }

    if(input->CurrentStatus_1.bit.rtso && input->CurrentStatus_1.bit.so &&input->CurrentStatus_1.bit.oe){
        if(sub_id == 0){
            if(axis->GetAxisMotionState() == AxisMotion::Axis_Motion_Type_Jog){
                output->target_pos_1 += axis->GetAxisMotionSpeed();
            }
        }
    }
    else{
        output->target_pos_1 = input->postion_actual_abs_1;
        axis->SetAxisStop();
    }
    if(input->CurrentStatus_1.bit.rtso && input->CurrentStatus_1.bit.so &&input->CurrentStatus_1.bit.oe){
        if(sub_id == 1){
            if(axis->GetAxisMotionState() == AxisMotion::Axis_Motion_Type_Jog){
                output->target_pos_2 += axis->GetAxisMotionSpeed();
            }
        }
    }
    else{
        output->target_pos_2 = input->postion_actual_abs_2;
        axis->SetAxisStop();
    }
}

static int szhc402_pdo_config_setup(uint16 slave)
{
    int retval;

    // map velocity
    uint16 map_1c12[] = {0x0002, 0x1600,0x1610};
    uint16 map_1c13[] = {0x0002, 0x1a00,0x1a10};

    retval = 0;
    ec_SDOwrite(slave, 0x1600, 0x00, FALSE, sizeof(retval), &retval, EC_TIMEOUTSAFE);
    ec_SDOwrite(slave, 0x1610, 0x00, FALSE, sizeof(retval), &retval, EC_TIMEOUTSAFE);


    retval = 0;
    ec_SDOwrite(slave, 0x1a00, 0x00, FALSE, sizeof(retval), &retval, EC_TIMEOUTSAFE);
    ec_SDOwrite(slave, 0x1a00, 0x00, FALSE, sizeof(retval), &retval, EC_TIMEOUTSAFE);

    for(int i = 0;i<sizeof(servo_pdo_entries_output)/sizeof(ec_pdo_entry_info_t);i++){
        retval = (servo_pdo_entries_output[i].index<<16)|(servo_pdo_entries_output[i].subindex<<8)|(servo_pdo_entries_output[i].bit_length);
        ec_SDOwrite(slave, 0x1600, 0x01+i, FALSE, sizeof(retval), &retval, EC_TIMEOUTSAFE);
        printf("pdo config write[%d]:1600_%d -> %x\n",i,0x01+i,retval);
    }

    retval = sizeof(servo_pdo_entries_output)/sizeof(ec_pdo_entry_info_t);
    ec_SDOwrite(slave, 0x1600, 0x00, FALSE, sizeof(retval), &retval, EC_TIMEOUTSAFE);



    for(int i = 0;i<sizeof(servo_pdo_entries_output2)/sizeof(ec_pdo_entry_info_t);i++){
        retval = (servo_pdo_entries_output2[i].index<<16)|(servo_pdo_entries_output2[i].subindex<<8)|(servo_pdo_entries_output2[i].bit_length);
        ec_SDOwrite(slave, 0x1610, 0x01+i, FALSE, sizeof(retval), &retval, EC_TIMEOUTSAFE);
        printf("pdo config write[%d]:1600_%d -> %x\n",i,0x01+i,retval);
    }

    retval = sizeof(servo_pdo_entries_output2)/sizeof(ec_pdo_entry_info_t);
    ec_SDOwrite(slave, 0x1610, 0x00, FALSE, sizeof(retval), &retval, EC_TIMEOUTSAFE);

    printf("=====================================\n");

    for(int i = 0;i<sizeof(servo_pdo_entries_input)/sizeof(ec_pdo_entry_info_t);i++){
        retval = (servo_pdo_entries_input[i].index<<16)|(servo_pdo_entries_input[i].subindex<<8)|(servo_pdo_entries_input[i].bit_length);
        ec_SDOwrite(slave, 0x1a00, 0x01+i, FALSE, sizeof(retval), &retval, EC_TIMEOUTSAFE);
        printf("pdo config read[%d]:1a00_%d -> %x\n",i,0x01+i,retval);
    }

    retval = sizeof(servo_pdo_entries_input)/sizeof(ec_pdo_entry_info_t);
    ec_SDOwrite(slave, 0x1a00, 0x00, FALSE, sizeof(retval), &retval, EC_TIMEOUTSAFE);

    for(int i = 0;i<sizeof(servo_pdo_entries_input2)/sizeof(ec_pdo_entry_info_t);i++){
        retval = (servo_pdo_entries_input2[i].index<<16)|(servo_pdo_entries_input2[i].subindex<<8)|(servo_pdo_entries_input2[i].bit_length);
        ec_SDOwrite(slave, 0x1a10, 0x01+i, FALSE, sizeof(retval), &retval, EC_TIMEOUTSAFE);
        printf("pdo config read[%d]:1a00_%d -> %x\n",i,0x01+i,retval);
    }

    retval = sizeof(servo_pdo_entries_input2)/sizeof(ec_pdo_entry_info_t);
    ec_SDOwrite(slave, 0x1a10, 0x00, FALSE, sizeof(retval), &retval, EC_TIMEOUTSAFE);

    // Set PDO mapping using Complete Access
    // Strange, writing CA works, reading CA doesn't
    // This is a protocol error of the slave.
    retval += ec_SDOwrite(slave, 0x1c12, 0x00, TRUE, sizeof(map_1c12), &map_1c12, EC_TIMEOUTSAFE);
    retval += ec_SDOwrite(slave, 0x1c13, 0x00, TRUE, sizeof(map_1c13), &map_1c13, EC_TIMEOUTSAFE);



    while(EcatError) printf("EcatError : %s", ec_elist2string());
//    printf("sizeof ec_pdo_entry_info_t = %d servo_pdo_entries_input= %d\n",sizeof (ec_pdo_entry_info_t),sizeof (servo_pdo_entries_input));
    printf("HC2000setup slave %d set, retval = %d\n", slave, retval);

    return 1;
}

void szhc402_clear_alarm(ec_slavet* slave,int axis_id){
    SERVO_SZHC_OUTPUT* output = (SERVO_SZHC_OUTPUT*)slave->outputs;
    SERVO_SZHC_INPUT* input = (SERVO_SZHC_INPUT*)slave->inputs;
    if(axis_id == 0){
//        output->ctrl_1.bit.clr_alarm = 1;
    }
    else if(axis_id == 1){
//        output->ctrl_1.bit.clr_alarm = 1;
    }
}

void szhc402_servo_on(ec_slavet* slave,int axis_id,bool state){
    SERVO_SZHC_INPUT* output = (SERVO_SZHC_INPUT*)slave->outputs;
    if(axis_id == 0){
//        output->ctrl_1.bit.servo_on = state?1:0;
    }
    else if(axis_id == 1){
//        output->ctrl_2.bit.servo_on = state?1:0;
    }
}

int szhc402_get_servo_alarm(ec_slavet* slave,int axis_id){
    SERVO_SZHC_INPUT* input = (SERVO_SZHC_INPUT*)slave->inputs;
    if(axis_id == 0){
        return input->error_code_1;
    }
    else if(axis_id == 1){
        return input->error_code_2;
    }
    return 0;
}

int szhc402_get_servo_pos (ec_slavet* slave,int axis_id){
    SERVO_SZHC_INPUT* input = (SERVO_SZHC_INPUT*)slave->inputs;
    if(axis_id == 0){
        return input->postion_actual_abs_1;
    }

    else if(axis_id == 1){
        return input->postion_actual_abs_2;
    }
    return 0;
}

int szhc402_get_servo_on(ec_slavet* slave,int axis_id){
    SERVO_SZHC_INPUT* input = (SERVO_SZHC_INPUT*)slave->inputs;
    if(axis_id == 0){
         return input->CurrentStatus_1.bit.rtso && input->CurrentStatus_1.bit.so &&input->CurrentStatus_1.bit.oe;
    }
    else if(axis_id == 1){
        return input->CurrentStatus_2.bit.rtso && input->CurrentStatus_2.bit.so &&input->CurrentStatus_2.bit.oe;
    }
    return 0;
}

int szhc402_get_servo_cmd_pos(ec_slavet* slave,int axis_id){
    SERVO_SZHC_OUTPUT* output = (SERVO_SZHC_OUTPUT*)slave->outputs;
    if(axis_id == 0){
        return output->target_pos_1;
    }
    else if(axis_id == 1){
        return output->target_pos_2;
    }
    return 0;
}


float szhc402_gget_servo_torque(ec_slavet* slave,int axis_id){
    float torque = 0.0;
    SERVO_SZHC_INPUT* input = (SERVO_SZHC_INPUT*)slave->inputs;
    if(axis_id == 0){
        torque =  input->torque_actual_value_1/25.08;
    }
    else if(axis_id == 1){
        torque =  input->torque_actual_value_2/25.08;
    }
    return torque;
}

float szhc402_gget_servo_velocity(ec_slavet* slave,int axis_id){
    float velocity = 0.0;
    SERVO_SZHC_INPUT* input = (SERVO_SZHC_INPUT*)slave->inputs;
    if(axis_id == 0){
        velocity =  input->velocity_actual_value_1;
    }
    else if(axis_id == 1){
        velocity =  input->velocity_actual_value_2;
    }
    return velocity;
}


const _EthercatSlaveConfig ethercatservo_szhc402={
    szhc402_is_surport,
    szhc402_axis_num,
    szhc402_setup_config,
    szhc402_cycle_run,
    szhc402_pdo_config_setup,
    szhc402_clear_alarm,
    szhc402_servo_on,
    szhc402_get_servo_alarm,
    szhc402_get_servo_pos,
    szhc402_get_servo_on,
    szhc402_get_servo_cmd_pos,
    szhc402_gget_servo_torque,
    szhc402_gget_servo_velocity
};
