/*  Include-------------------------------------------------------------------*/
    #include "common.h"
    #include "include.h"
    #include "steer.h"
    #include <math.h>
/*  Define--------------------------------------------------------------------*/
    #define MAXSPEED 20
/*  Variable------------------------------------------------------------------*/
    float d = 0.00009,e = 0,f = 0.4,\
          position[ADEEP],positionerror,kpc,kdc_1=10 ;
    int steer_inc,steer_PWM,speed_ctl_output;
    uint16 real_position_num;
    uint16 kdc=1,steer_plus=86;//差和比系数
/*  Function declaration------------------------------------------------------*/
    void bell_set(PTXn_e bell,uint8 data);
/*  Declare-------------------------------------------------------------------*/
    extern uint16 right1,right0,middle,left0,left1,right2;
    extern uint16 position_num;//差和比循环队列的头部
    extern Dtype user_flag;
/*  Function -----------------------------------------------------------------*/
    void position_measure(void);

uint16 abs_jdz(int X)//设定绝对值
{
    X=X>=0?X:-X; 
    return X;
}
 
void ser_ctrl(void)
{    
    int16 str_inc;//最后舵机中值
    
    static uint16 ser_pwm;
        
    float Outdata[2],real_position;
    
    uint16 temp=0;
    
    real_position_num = (position_num+ADEEP-ADELAY) % ADEEP;

    position_measure();
    
    real_position = position[real_position_num];
    
    kpc=d*real_position*real_position\
        +f;//很明显加这个动态p会乱抖
    
    positionerror = real_position\
        -position[(real_position_num - 1) % ADEEP];
    
    str_inc = (int)(kpc*real_position+kdc_1*positionerror);
    
    if(position_num<ADEEP&&temp%6==0&&user_flag.b5) {
        Outdata[0] = position[position_num];
        Outdata[1] = position[real_position_num];
        
        vcan_sendware((uint8_t *)Outdata,sizeof(Outdata));
        user_flag.b4 =0;
        
    }
    
/*  速度控制部分----------------------------------------------------------BEG*/
    if((left1==0)&&(left0==0)&&(right1==0)&&(right0==0)) {
        speed_ctl_output=0;
        //str_inc=0;
    }
/*  速度控制部分----------------------------------------------------------END*/
    
    ser_pwm = 620-str_inc;
    
    if(ser_pwm>720) {   ser_pwm=720;    bell_set(BELLPORT,BELLON); } else bell_set(BELLPORT,BELLOFF);
    if(ser_pwm<520) {   ser_pwm=520;    bell_set(BELLPORT,BELLON); } else bell_set(BELLPORT,BELLOFF);
    
    ftm_pwm_duty(STEERFTM,STEERFTM_CH,ser_pwm);   
    
    if(temp++ ==100) temp=0;
}

void position_measure(void)
{
    int32 AD_dif,AD_sum;
    float ADflag;
    AD_dif=(left0+left1)-(right0+right1);//左减右
    AD_sum=left0+right0+left1+right1;
    ADflag = (float)AD_dif/(float)AD_sum;
    position[position_num] = (int)(ADflag*(float)steer_plus);
    if(position[position_num] > (steer_plus-5) || \
       position[position_num] < (-steer_plus+5) )  
       position[position_num]= position[(position_num + ADEEP - 1) % ADEEP];
}