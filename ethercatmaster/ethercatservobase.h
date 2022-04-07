#ifndef ETHERCATSERVOBASE_H
#define ETHERCATSERVOBASE_H

#include "ethercatmain.h"
#include "ethercatcoe.h"
#include "ethercatprint.h"
#include "axismotion.h"


typedef struct{
    uint32_t vendor_id;
    uint32_t product_id;
}ethercat_dev_id_info_t;


typedef struct {
    uint16_t index; /**< PDO entry index. */
    uint8_t subindex; /**< PDO entry subindex. */
    uint8_t bit_length; /**< Size of the PDO entry in bit. */
} ec_pdo_entry_info_t;

typedef struct {

    ///
    /// 判断是否支持该从站
    ///
    int (*is_surport)(ec_slavet*);

    int (*axis_num)();

    ///
    /// 启动参数配置
    ///
    void (*setup_config)(ec_slavet*);

    ///
    ///  定时周期循环
    ///
    void (*cycle_run)(ec_slavet*,AxisMotion*,int);

    ///
    /// pdo 配置
    ///
    int (*setup_pdo_config)(uint16);

    ///
    /// 清除报警  从站 轴号
    ///
    void (*clear_alarm)(ec_slavet*,int);

    ///
    /// 控制伺服使能状态  从站 轴号 状态
    ///
    void (*servo_on)(ec_slavet*,int,bool);

    ///
    /// 获取伺服报警
    ///
    int (*get_servo_alarm)(ec_slavet*,int);

    ///
    /// 获取伺服位置
    ///
    int (*get_servo_pos)(ec_slavet*,int);

    ///
    /// 获取伺服使能状态
    ///
    int (*get_servo_on)(ec_slavet*,int);

    ///
    /// 获取伺服指令位置
    ///
    int (*get_servo_cmd_pos)(ec_slavet*,int);


    int (*get_axis)(ec_slavet*,int);
}_EthercatSlaveConfig;

extern const _EthercatSlaveConfig ethercatservo_szhc;

extern const _EthercatSlaveConfig ethercatservo_single;

#endif // ETHERCATSERVOBASE_H
