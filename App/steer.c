/*  Include-------------------------------------------------------------------*/
    #include "common.h"
    #include "include.h"
    #include "steer.h"
    #include <math.h>
/*  Define--------------------------------------------------------------------*/
    #define MAXSPEED 20
/*  Variable------------------------------------------------------------------*/
    float ADflag1,ADflag2,AD_error1,AD_error2;
    uint16 sum_3;
    uint16 kdc=1;
    int AD_dif1,AD_sum1,AD_dif2,AD_sum2;
    uint8 num;
    int steer_inc;
    float position1[4],position2[4],positionerror,d,e,f,kpc,kdc_1=4 ;
    int steer_PWM;
    uint16 temp_serial=110;//差和比系数
    float d = 0.00013,e = 0,f = 0;
    int speed_ctl_output;
/*  Function declaration------------------------------------------------------*/
    void bell_set(PTXn_e bell,uint8 data);
/*  Declare-------------------------------------------------------------------*/
    extern int middle0,left_zhi,right_zhi;
    extern uint16 right1[],right0[],middle[],left0[],left1[],right2[];
    static uint16 AD_Array_num;//AD循环队列的头部
/*  Function -----------------------------------------------------------------*/
uint16 abs_jdz(int X)//设定绝对值
{
    X=X>=0?X:-X;
    
    return X;
}


float jueduizhif(float x) 
{ 
    x=x>=0?x:-x;
    
    return x; 
} 

void ser_ctrl(void)
{    
    int16 str_inc;//最后舵机中值
    uint16 ser_pwm;
    static uint16 real_ad_num;
    static uint16 real_right1,real_right0,real_middle,real_left0,real_left1,
                  real_right2;
    
/*----------------------------------------------------------------------------*/
    AD_dif2=(left0[AD_Array_num]+left1[AD_Array_num])-(right0[AD_Array_num]+right1[AD_Array_num]);//左减右
    AD_sum2=left0[AD_Array_num]+right0[AD_Array_num]+left1[AD_Array_num]+right1[AD_Array_num];
    ADflag2 = (float)AD_dif2/(float)AD_sum2;
    position2[0] = (int)(ADflag2*(float)temp_serial);
    
    //if(right1>left1)                      
    //    position2[0]=-position2[0];
    //else if(right1<left1)                   //方向偏移标标志处理,右面小是正数
    //    position2[0]=position2[0];
    //else 
    //    position2[0]=position2[3];
    
    real_ad_num = (AD_Array_num + 95) % ADEEP;
    real_right0=right0[real_ad_num];
    real_right1=right1[real_ad_num];
    real_left1 =left1[real_ad_num];
    real_left0 =left0[real_ad_num];
    real_right2=right2[real_ad_num];
    real_middle=middle[real_ad_num];
    
    //if(1000<real_middle<2000 && 1000<real_right2<1700) {
    //    real_right1 = real_right2;//用处置电感临时替代45度电感，以后改
    //    bell_set(BELLPORT,BELLON);
    //}
    //else bell_set(BELLPORT,BELLOFF);
    
    AD_dif1=(real_left0 +real_left1/2 +left0[(AD_Array_num + 98) % ADEEP])-\
            (real_right0+real_right1/2+right0[(AD_Array_num + 98) % ADEEP]);//左减右
    AD_sum1=real_left0+real_right0+\
        (real_left1+real_right1+\
         left0[(AD_Array_num + 98) % ADEEP]+\
         right0[(AD_Array_num + 98) % ADEEP])/2;
    ADflag1 = (float)AD_dif1/(float)AD_sum1;
    position1[0] = (int)(ADflag1*(float)temp_serial); 
    //if(right1>left1)                      
    //    position1[0]=-position1[0]; 
    //else if(right1<left1)                   //方向偏移标标志处理,右面小是正数
    //    position1[0]=position1[0];
    //else 
    //    position1[0]=position1[3];
    //LCD_Show_Number(0,3  ,abs_jdz((uint16)position1[0]));
    //if(position1[0]<0)  position1[0]=-position1[0];     
    //if(real_right0<real_left0)    position1[0]=-position1[0];
    //if(fabs(position1[0])>100)  position1[0]=100;  
    
    /*    if((abs(real_left1-60)<5)&&(abs(real_right1-60)<5)) {          //识别圆环
     *        ser_pwm=560;
     *        ftm_pwm_duty(STEERFTM,STEERFTM_CH,ser_pwm);
     *        bell_init(BELLPORT,1);
     *        dwt_delay_ms(150);
     *        bell_init(BELLPORT,0);
     *    }
     */
    
    /*    if(real_left0<60&&real_right0<60) 
     *        position1[0]=position1[0]/1.5;
     *    if (abs(real_left0-real_right0)<10&&abs(real_left1-real_right1)<10) {                                //简易动态P
     *        kpc=0.6;        
     *    }
     *    else {
     *        kpc=0.9;
     *        kdc_1=kdc;
     *    }
     */
    
    kpc=d*position1[0]*position1[0]-e*jueduizhif(position1[0])+f;//很明显加这个动态p会乱抖
    
    positionerror = position2[0]-position2[1]; 
    
    str_inc = (int)(kpc*position2[0]+kdc_1*positionerror);   
    
/*  速度控制部分----------------------------------------------------------BEG*/
    //speed_ctl_output=(int)( 0.1*(100-abs(str_inc)) );
    //if(abs(str_inc)>=70)
    //    speed_ctl_output=MAXSPEED; 
    if((real_left1==0)&&(real_left0==0)&&(real_right1==0)&&(real_right0==0)) {
        speed_ctl_output=0;
        str_inc=0;
    }
/*  速度控制部分----------------------------------------------------------END*/
    
    ser_pwm = 620-str_inc;
    
    if(ser_pwm>720) {   ser_pwm=720;    bell_set(BELLPORT,BELLON); } else bell_set(BELLPORT,BELLOFF);
    if(ser_pwm<520) {   ser_pwm=520;    bell_set(BELLPORT,BELLON); } else bell_set(BELLPORT,BELLOFF);
    
    ftm_pwm_duty(STEERFTM,STEERFTM_CH,ser_pwm);   
}