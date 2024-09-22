#include "Planning.h"
#include <math.h>

/**
 * @brief  Planning类的构造函数
 * @param  Pstart_：开始位置
 * @param  Pend_：结束位置
 * @param  Vstart_：开始的速度，单位为RPM 绝对值
 * @param  Vmax_：最大的速度
 * @param  Vend_：末尾的速度
 * @param  Rac_：加速路程的比例
 * @param  Rde_：减速路程的比例
 */
Planning::Planning(float Pstart_, float Pend_, float Vstart_, float Vmax_, float Vend_, float Rac_, float Rde_)
{
    Pstart = Pstart_;
    Pend = Pend_;
    Vstart = Vstart_;
    Vmax = Vmax_;
    Vend = Vend_;
    Rac = Rac_;
    Rde = Rde_;
    done_flag = 0;

    Ssu = fabsf(Pstart - Pend);
    Sac = Ssu * Rac;
    Sde = Ssu * Rde;
    Sco = Ssu - Sac - Sde;
    Aac = (Vmax * Vmax - Vstart * Vstart) / (2.0f * Sac);
    Ade = (Vend * Vend - Vmax * Vmax) / (2.0f * Sde);
}


/**
 * @brief  计算目标转速
 * @param  real_angle：处理过的真实角度
 * @retval float：目标转速
 */
float Planning::updateTargetRPM(float real_angle) {
    
    // 异常数据检查
    if ((Rac > 1) || (Rac < 0) || 
        (Rde > 1) || (Rde < 0) || 
        (Vmax < Vstart))         
    {
        this->TARGET_RPM = 0;
        return this->TARGET_RPM;
    }
    
    // 匀速模式
    if (Pstart == Pend) 
    {
        this->TARGET_RPM = Vmax;
        return this->TARGET_RPM;
    }
    
    //T型模式
    if (((Pend > Pstart) && (real_angle < Pstart)) || 
        ((Pend < Pstart) && (real_angle > Pstart)))  
    {
        this->TARGET_RPM = Vstart; 
    }
    else if (((Pend > Pstart) && (real_angle > Pend)) ||
             ((Pend < Pstart) && (real_angle < Pend))) 
    {
        this->TARGET_RPM = Vend; 
    }
    else
    {
        S = fabsf(real_angle - Pstart);
        
        // 规划RPM
        if (S < Sac){
            this->TARGET_RPM = sqrt(2.0f * Aac * S + Vstart * Vstart); 
        }else if (S < (Sac + Sco)){
            this->TARGET_RPM = Vmax; 
        }else{
            this->TARGET_RPM = sqrt(Vend * Vend - 2.0f * Ade * fabsf(Ssu - S)); 
        }
    }

    if (Pend < Pstart)  { this->TARGET_RPM = -this->TARGET_RPM; }

    // 判断是否完成
    if (fabsf(real_angle - Pend) < 5)
    {
        this->done_flag = 1; 
        this->TARGET_RPM = 0;
    }

    if (fabsf(real_angle - Pend) > 5)
    {
        this->done_flag = 0;
    }

    return this->TARGET_RPM;
}
