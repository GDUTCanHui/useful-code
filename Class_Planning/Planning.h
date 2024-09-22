#pragma once
#ifdef __cplusplus


class Planning {
private:
    float Ssu;          // 总路程
    float Sac;          // 加速路程
    float Sde;          // 减速路程
    float Sco;          // 匀速路程
    float Aac;          // 加速加速度
    float Ade;          // 减速加速度
    float S;            // 当前路程
    float TARGET_RPM;   // 目标转速

public:
    float Distance; // 总路程
    float Pstart;   // 开始位置
    float Pend;     // 结束位置
    float Vstart;   // 开始的速度
    float Vmax;     // 最大的速度
    float Vend;     // 末尾的速度
    float Rac;      // 加速路程的比例
    float Rde;      // 减速路程的比例
    int done_flag;  // 完成标志位
    
    Planning(float Pstart_, float Pend_, float Vstart_, float Vmax_, float Vend_, float Rac_, float Rde_);
    float updateTargetRPM(float real_angle);
};

#endif
