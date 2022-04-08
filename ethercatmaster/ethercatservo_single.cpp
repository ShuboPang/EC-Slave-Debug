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
        uint32_t rtso:1,        //< ready to switch on
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
    uint32_t all;
} SERVO_CTRL_UNION;


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

    Se_ADDR_Motor_Code = 0x2000, //电机型号; int32 RW N P0.00
    Se_ADDR_Encoder_Type_Selection = 0x2001, //编码器类型选择；int32 RW N P0.01
    Se_ADDR_Motor_Running_Direction = 0x2002, //电机运行方向；int16 RW N P0.02

    Se_ADDR_Motor_Pulses_Per_Revolution = 0x2016, //电机每转脉冲数 int32 RW  N
    Se_ADDR_Actual_Iq_Current = 0x300A, //Iq输出电流（0.01A） int32 RO N  R0.10
    Se_ADDR_Servo_Semperature = 0x300B, //驱动器温度（0.1℃） int32 RO N  R0.11
    Se_ADDR_Error_Code_Abs = 0x4000, //故障码(唯一) UNSIGNED16  RO Y 与伺服显示的格式相同；
};


static ethercat_dev_id_info_t servo_support_list[] =
{
        {0x0000066f, 0x60380004}, //MADLN05BE 松下
        {0x0000066f, 0x60380005}, //MADLN15BE 松下
        {0x0000066f, 0x60380006}, //MADLN25BE 松下
        {0x0000066f, 0x60380007}, //MADLN35BE 松下
        {0x00000994, 0x00002000},   //< 研控步进2405
        {0x00000994, 0x00003000},   //< 研控步进2205
};

static ec_pdo_entry_info_t servo_pdo_entries_output[] = {
    {S_ADDR_Control_Word, 0x00, 16}, //control
    {S_ADDR_Target_Position, 0x00, 32}, //目标位置
    {S_ADDR_Mode_Of_Operation, 0x00, 8},  //控制模式
    {S_ADDR_HOMING_MODE, 0x00, 8}, // HomingMode
};

static ec_pdo_entry_info_t servo_pdo_entries_input[] = {
    {S_ADDR_Status_Word, 0x00, 16}, // 状态字
    {S_ADDR_Position_Actual_Value_Abs, 0x00, 32}, // 实际位置
    {S_ADDR_Velocity_Actual_Value, 0x00, 32}, // 实际速度
    {S_ADDR_Torque_Actual_Value, 0x00, 16}, // 实际转矩
    {S_ADDR_Error_Code, 0x00, 16},
    {S_ADDR_Mode_Of_Operation_Display, 0x00, 8}, //
};

PACKED_BEGIN
typedef struct PACKED{
    uint16 ControlStatus:16;
    int32_t TargetPos;
    uint16 ControlMode:8;
    uint16 HomingMode:8;
}SERVO_SINGLE_OUTPUT;
PACKED_END

PACKED_BEGIN
typedef struct PACKED{
    SERVO_CTRL_UNION CurrentStatus ;
    int32_t CurrentPos ;
    int32_t CurrentVelocity;
    int16_t CurrentTorque:16;
    uint16 ErrorCode ;
    uint16 CurrentMode:8;
}SERVO_SINGLE_INPUT;
PACKED_END

int single_is_surport(ec_slavet* slave){
    for(int i = 0;i<sizeof (servo_support_list)/sizeof(ethercat_dev_id_info_t);i++){
        if(slave->eep_man == servo_support_list[i].vendor_id && slave->eep_id == servo_support_list[i].product_id){
            return 1;
        }
    }
    return 0;
}

int single_axis_num(){
    return 1;
}

static void single_setup_config(ec_slavet* slave){
    SERVO_SINGLE_OUTPUT* output = (SERVO_SINGLE_OUTPUT*)slave->outputs;
    SERVO_SINGLE_INPUT* input = (SERVO_SINGLE_INPUT*)slave->inputs;

}

static void single_cycle_run(ec_slavet* slave,AxisMotion* axis,int sub_id){
    SERVO_SINGLE_OUTPUT* output = (SERVO_SINGLE_OUTPUT*)slave->outputs;
    SERVO_SINGLE_INPUT* input = (SERVO_SINGLE_INPUT*)slave->inputs;

}

static int single_pdo_config_setup(uint16 slave)
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

void single_clear_alarm(ec_slavet* slave,int axis_id){
    SERVO_SINGLE_OUTPUT* output = (SERVO_SINGLE_OUTPUT*)slave->outputs;

}

void single_servo_on(ec_slavet* slave,int axis_id,bool state){
    SERVO_SINGLE_OUTPUT* output = (SERVO_SINGLE_OUTPUT*)slave->outputs;

}

int single_get_servo_alarm(ec_slavet* slave,int axis_id){
    SERVO_SINGLE_INPUT* input = (SERVO_SINGLE_INPUT*)slave->inputs;

    return input->ErrorCode;
}

int single_get_servo_pos (ec_slavet* slave,int axis_id){
    SERVO_SINGLE_INPUT* input = (SERVO_SINGLE_INPUT*)slave->inputs;

    return input->CurrentPos;
}

int single_get_servo_on(ec_slavet* slave,int axis_id){
    SERVO_SINGLE_INPUT* input = (SERVO_SINGLE_INPUT*)slave->inputs;

    return input->CurrentStatus.bit.rtso && input->CurrentStatus.bit.so &&input->CurrentStatus.bit.oe;
}

int single_get_servo_cmd_pos(ec_slavet* slave,int axis_id){
    SERVO_SINGLE_OUTPUT* output = (SERVO_SINGLE_OUTPUT*)slave->outputs;
    return output->TargetPos;
}

float single_ggget_servo_torque(ec_slavet* slave,int axis_id){
    float torque = 0.0;

    return torque;
}

float single_ggget_servo_velocity(ec_slavet* slave,int axis_id){
    float velocity = 0.0;

    return velocity;
}


const _EthercatSlaveConfig ethercatservo_single={
    single_is_surport,
    single_axis_num,
    single_setup_config,
    single_cycle_run,
    single_pdo_config_setup,
    single_clear_alarm,
    single_servo_on,
    single_get_servo_alarm,
    single_get_servo_pos,
    single_get_servo_on,
    single_get_servo_cmd_pos,
    single_ggget_servo_torque,
    single_ggget_servo_velocity
};
