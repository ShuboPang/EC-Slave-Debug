.pragma library

function func() {

}

var __EthercatSlaveNameList=[]

var __EthercatSlaveStateEnum={
    0:qsTr("State Error"),
    1:qsTr("INIT"),
    2:qsTr("PRE_OP"),
    3:qsTr("BOOT"),
    4:qsTr("SAFE_OP"),
    8:qsTr("OPERATIONAL"),
    16:qsTr("ERROR"),
}

var huacheng_sdo=[
            {"main_index":0x2e00,"sub_index":1,"type":4,"describe":qsTr("软件版本/恢复出厂"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":2,"type":4,"describe":qsTr("驱动器机型"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":3,"type":4,"describe":qsTr("电机代码"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":4,"type":4,"describe":qsTr("参数管理"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":5,"type":4,"describe":qsTr("强制电机代码"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":6,"type":4,"describe":qsTr("最大转速阈值"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":7,"type":4,"describe":qsTr("正向最大转矩限制"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":8,"type":4,"describe":qsTr("反向最大转矩限制"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":9,"type":4,"describe":qsTr("电机方向逻辑"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":10,"type":4,"describe":qsTr("编码器类型选择"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":11,"type":4,"describe":qsTr("伺服模式选择"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":12,"type":4,"describe":qsTr("抱闸ON延迟时间"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":13,"type":4,"describe":qsTr("静止状态，抱闸OFF-伺服OFF延时"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":14,"type":4,"describe":qsTr("编码器电池故障清除"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":15,"type":4,"describe":qsTr("电机过载等级"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":16,"type":4,"describe":qsTr("电机过载保护系数"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":17,"type":4,"describe":qsTr("电机过速水平"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":18,"type":4,"describe":qsTr("使能"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":19,"type":4,"describe":qsTr("外部急停屏蔽"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":20,"type":4,"describe":qsTr("单双轴选择"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":21,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":22,"type":4,"describe":qsTr("速度环增益"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":23,"type":4,"describe":qsTr("速度环积分时间常数"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":24,"type":4,"describe":qsTr("位置环增益"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":25,"type":4,"describe":qsTr("第2速度环增益"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":26,"type":4,"describe":qsTr("第2速度环积分时间常数"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":27,"type":4,"describe":qsTr("第2位置环增益"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":28,"type":4,"describe":qsTr("速度前馈增益"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":29,"type":4,"describe":qsTr("阻尼系数"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":30,"type":4,"describe":qsTr("速度反馈低通滤波系数"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":31,"type":4,"describe":qsTr("电流反馈低通滤波系数"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":32,"type":4,"describe":qsTr("转矩前馈系数"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":33,"type":4,"describe":qsTr("速度环微分增益（低频）"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":34,"type":4,"describe":qsTr("S曲线平滑等级"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":35,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":36,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":37,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":38,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":39,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":40,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":41,"type":4,"describe":qsTr("电机旋转1周对应的位置指令数"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":42,"type":4,"describe":qsTr("移动平均滤波常数"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":43,"type":4,"describe":qsTr("一阶低通滤波常数"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":44,"type":4,"describe":qsTr("电子齿轮1（分子）低位"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":45,"type":4,"describe":qsTr("电子齿轮1（分子）高位"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":46,"type":4,"describe":qsTr("电子齿轮1（分母）低位"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":47,"type":4,"describe":qsTr("电子齿轮1（分母）高位"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":48,"type":4,"describe":qsTr("伺服监控状态地址"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":49,"type":4,"describe":qsTr("伺服监控状态值"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":50,"type":4,"describe":qsTr("位置偏差限制"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":51,"type":4,"describe":qsTr("EtherCAT同步周期"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":52,"type":4,"describe":qsTr("伺服状态机"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":53,"type":4,"describe":qsTr("伺服报警码"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":54,"type":4,"describe":qsTr("母线电压"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":55,"type":4,"describe":qsTr("功率模块温度"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":56,"type":4,"describe":qsTr("弱磁水平"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":57,"type":4,"describe":qsTr("弱磁水平"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":58,"type":4,"describe":qsTr("重力补偿值"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":59,"type":4,"describe":qsTr("正方向动摩擦补偿"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":60,"type":4,"describe":qsTr("负方向动摩擦补偿"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":61,"type":4,"describe":qsTr("电机电角度偏移"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":62,"type":4,"describe":qsTr("指令速度"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":63,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":64,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":65,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":66,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":67,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":68,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":69,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":70,"type":4,"describe":qsTr("转矩指令低通滤波系数"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":71,"type":4,"describe":qsTr("速度指令极性"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":72,"type":4,"describe":qsTr("动作命令"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":73,"type":4,"describe":qsTr("陷波滤波频率"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":74,"type":4,"describe":qsTr("陷波滤波频宽"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":75,"type":4,"describe":qsTr("特殊功能地址1"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":76,"type":4,"describe":qsTr("特殊功能地址2"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":77,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":78,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":79,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":80,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":81,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":82,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":83,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":84,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":85,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":86,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":87,"type":4,"describe":qsTr("编码器单圈位置高16位"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":88,"type":4,"describe":qsTr("编码器单圈位置低16位"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":89,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":90,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":91,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":92,"type":4,"describe":qsTr("示波器通道配置"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":93,"type":4,"describe":qsTr("速度曲线显示"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":94,"type":4,"describe":qsTr("力矩曲线显示"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":95,"type":4,"describe":qsTr("特殊功能地址3"),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":96,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":97,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":98,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":99,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2e00,"sub_index":100,"type":4,"describe":qsTr("软件版本"),"unit":qsTr("")},

            {"main_index":0x2f00,"sub_index":0,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":1,"type":4,"describe":qsTr("软件版本/恢复出厂"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":2,"type":4,"describe":qsTr("驱动器机型"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":3,"type":4,"describe":qsTr("电机代码"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":4,"type":4,"describe":qsTr("参数管理"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":5,"type":4,"describe":qsTr("强制电机代码"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":6,"type":4,"describe":qsTr("最大转速阈值"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":7,"type":4,"describe":qsTr("正向最大转矩限制"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":8,"type":4,"describe":qsTr("反向最大转矩限制"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":9,"type":4,"describe":qsTr("电机方向逻辑"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":10,"type":4,"describe":qsTr("编码器类型选择"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":11,"type":4,"describe":qsTr("伺服模式选择"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":12,"type":4,"describe":qsTr("抱闸ON延迟时间"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":13,"type":4,"describe":qsTr("静止状态，抱闸OFF-伺服OFF延时"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":14,"type":4,"describe":qsTr("编码器电池故障清除"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":15,"type":4,"describe":qsTr("电机过载等级"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":16,"type":4,"describe":qsTr("电机过载保护系数"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":17,"type":4,"describe":qsTr("电机过速水平"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":18,"type":4,"describe":qsTr("使能"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":19,"type":4,"describe":qsTr("外部急停屏蔽"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":20,"type":4,"describe":qsTr("单双轴选择"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":21,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":22,"type":4,"describe":qsTr("速度环增益"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":23,"type":4,"describe":qsTr("速度环积分时间常数"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":24,"type":4,"describe":qsTr("位置环增益"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":25,"type":4,"describe":qsTr("第2速度环增益"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":26,"type":4,"describe":qsTr("第2速度环积分时间常数"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":27,"type":4,"describe":qsTr("第2位置环增益"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":28,"type":4,"describe":qsTr("速度前馈增益"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":29,"type":4,"describe":qsTr("阻尼系数"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":30,"type":4,"describe":qsTr("速度反馈低通滤波系数"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":31,"type":4,"describe":qsTr("电流反馈低通滤波系数"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":32,"type":4,"describe":qsTr("转矩前馈系数"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":33,"type":4,"describe":qsTr("速度环微分增益（低频）"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":34,"type":4,"describe":qsTr("S曲线平滑等级"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":35,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":36,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":37,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":38,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":39,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":40,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":41,"type":4,"describe":qsTr("电机旋转1周对应的位置指令数"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":42,"type":4,"describe":qsTr("移动平均滤波常数"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":43,"type":4,"describe":qsTr("一阶低通滤波常数"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":44,"type":4,"describe":qsTr("电子齿轮1（分子）低位"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":45,"type":4,"describe":qsTr("电子齿轮1（分子）高位"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":46,"type":4,"describe":qsTr("电子齿轮1（分母）低位"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":47,"type":4,"describe":qsTr("电子齿轮1（分母）高位"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":48,"type":4,"describe":qsTr("伺服监控状态地址"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":49,"type":4,"describe":qsTr("伺服监控状态值"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":50,"type":4,"describe":qsTr("位置偏差限制"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":51,"type":4,"describe":qsTr("EtherCAT同步周期"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":52,"type":4,"describe":qsTr("伺服状态机"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":53,"type":4,"describe":qsTr("伺服报警码"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":54,"type":4,"describe":qsTr("母线电压"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":55,"type":4,"describe":qsTr("功率模块温度"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":56,"type":4,"describe":qsTr("弱磁水平"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":57,"type":4,"describe":qsTr("弱磁水平"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":58,"type":4,"describe":qsTr("重力补偿值"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":59,"type":4,"describe":qsTr("正方向动摩擦补偿"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":60,"type":4,"describe":qsTr("负方向动摩擦补偿"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":61,"type":4,"describe":qsTr("电机电角度偏移"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":62,"type":4,"describe":qsTr("指令速度"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":63,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":64,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":65,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":66,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":67,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":68,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":69,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":70,"type":4,"describe":qsTr("转矩指令低通滤波系数"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":71,"type":4,"describe":qsTr("速度指令极性"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":72,"type":4,"describe":qsTr("动作命令"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":73,"type":4,"describe":qsTr("陷波滤波频率"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":74,"type":4,"describe":qsTr("陷波滤波频宽"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":75,"type":4,"describe":qsTr("特殊功能地址1"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":76,"type":4,"describe":qsTr("特殊功能地址2"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":77,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":78,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":79,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":80,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":81,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":82,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":83,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":84,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":85,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":86,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":87,"type":4,"describe":qsTr("编码器单圈位置高16位"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":88,"type":4,"describe":qsTr("编码器单圈位置低16位"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":89,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":90,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":91,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":92,"type":4,"describe":qsTr("示波器通道配置"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":93,"type":4,"describe":qsTr("速度曲线显示"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":94,"type":4,"describe":qsTr("力矩曲线显示"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":95,"type":4,"describe":qsTr("特殊功能地址3"),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":96,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":97,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":98,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":99,"type":4,"describe":qsTr(""),"unit":qsTr("")},
            {"main_index":0x2f00,"sub_index":100,"type":4,"describe":qsTr("软件版本"),"unit":qsTr("")},

        ]

var yako_sdo=[{"main_index":0x2000,"sub_index":0,"type":2,"describe":qsTr("驱动器版本号"),"unit":qsTr("")},
              {"main_index":0x2001,"sub_index":0,"type":2,"describe":qsTr("软件版本号"),"unit":qsTr("")},
              {"main_index":0x2201,"sub_index":0,"type":2,"describe":qsTr("电子齿轮比与细分切换"),"unit":qsTr("")},
              {"main_index":0x2205,"sub_index":0,"type":2,"describe":qsTr("电机默认方向"),"unit":qsTr("")},
              {"main_index":0x2400,"sub_index":0,"type":2,"describe":qsTr("细分设置"),"unit":qsTr("")},
              {"main_index":0x2401,"sub_index":0,"type":2,"describe":qsTr("最大有效电流"),"unit":qsTr("mA")},
              {"main_index":0x2408,"sub_index":0,"type":2,"describe":qsTr("电子齿轮比分母"),"unit":qsTr("")},
              {"main_index":0x2409,"sub_index":0,"type":2,"describe":qsTr("电子齿轮比分子"),"unit":qsTr("")},
              {"main_index":0x603F,"sub_index":0,"type":2,"describe":qsTr("驱动器故障码"),"unit":qsTr("")},
            ]


var __EthercatSlave_huacheng={
    "name":"",
    "eep_man":0x766,
    "eep_id":0x501,
    "sdo":huacheng_sdo,
//    "alarm_addr":{
//        "main_index":0,
//        "sub_index":0,
//    },
//    "eeprom_addr":{
//        "main_index":0,
//        "sub_index":0,
//    }
}

var __EthercatSlave_huacheng402={
    "name":"",
    "eep_man":0x92b,
    "eep_id":0x402,
    "sdo":huacheng_sdo,
//    "alarm_addr":{
//        "main_index":0,
//        "sub_index":0,
//    },
//    "eeprom_addr":{
//        "main_index":0,
//        "sub_index":0,
//    }
}
var __EthercatSlave_huacheng3000_402={
    "name":"",
    "eep_man":0x92b,
    "eep_id":0x502,
    "sdo":huacheng_sdo,
//    "alarm_addr":{
//        "main_index":0,
//        "sub_index":0,
//    },
//    "eeprom_addr":{
//        "main_index":0,
//        "sub_index":0,
//    }
}

var __EthercatSlave_yako2405pe={
    "name":"",
    "eep_man":0x994,
    "eep_id":0x2000,
    "sdo":yako_sdo,
    "alarm_addr":{
        "main_index":0x603F,
        "sub_index":0,
        "type":2,
    },
    "eeprom_addr":{
        "main_index":0x2102,
        "sub_index":0,
        "value":2,
        "type":2,
    }
}

var __EthercatSlave_yako2205pe={
    "name":"",
    "eep_man":0x994,
    "eep_id":0x3000,
    "sdo":yako_sdo,
    "alarm_addr":{
        "main_index":0x603F,
        "sub_index":0,
        "type":2,
    },
    "eeprom_addr":{
        "main_index":0x2102,
        "sub_index":0,
        "value":2,
        "type":2,
    }
}



var __EthercatSlaveDeviceMap=new Map();

var ethercatSlaveDeviceList =[__EthercatSlave_huacheng,
                              __EthercatSlave_yako2405pe,
                              __EthercatSlave_yako2205pe,
                              __EthercatSlave_huacheng402,
                              __EthercatSlave_huacheng3000_402,
                            ];


function init() {
    for(let i = 0;i<ethercatSlaveDeviceList.length;i++){
        if(ethercatSlaveDeviceList[i].hasOwnProperty("eep_man") && ethercatSlaveDeviceList[i].hasOwnProperty("eep_id")){
            var name = ethercatSlaveDeviceList[i]["eep_man"]+"|"+ethercatSlaveDeviceList[i]["eep_id"]
            __EthercatSlaveDeviceMap.set(name,ethercatSlaveDeviceList[i]);
//            console.log("init name",ethercatSlaveDeviceList[i].name,name);
        }
    }
}

function ethercatSlaveDeviceFind(eep_man,eep_id){
    var name = eep_man + "|"+eep_id;
//    console.log("ethercatSlaveDeviceFind name",name,eep_man,eep_id);
    return __EthercatSlaveDeviceMap.get(name);
}
