#ifndef ETHERCATSERVOBASE_H
#define ETHERCATSERVOBASE_H

#include "ethercatmain.h"
#include "ethercatcoe.h"
#include "ethercatprint.h"


typedef struct{
    uint32_t vendor_id;
    uint32_t product_id;
}ethercat_dev_id_info_t;

typedef struct {

    int (*is_surport)(ec_slavet*);


    void (*setup_config)(ec_slavet*);

    void (*cycle_run)(ec_slavet*);
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


    int (*get_servo_alarm)(ec_slavet*,int);


    int (*get_servo_pos)(ec_slavet*,int);

    int (*get_servo_on)(ec_slavet*,int);

    int (*get_servo_cmd_pos)(ec_slavet*,int);

}_EthercatSlaveConfig;

extern const _EthercatSlaveConfig ethercatservo_szhc;

#endif // ETHERCATSERVOBASE_H
