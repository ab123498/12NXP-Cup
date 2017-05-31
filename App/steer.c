/*  Include-------------------------------------------------------------------*/
    #include "common.h"
    #include "include.h"
    #include "steer.h"
    #include <math.h>

/*  Define--------------------------------------------------------------------*/
/*  Variable------------------------------------------------------------------*/
    float ADflag1,ADflag2,AD_error1,AD_error2;
    uint16 sum_3;
    uint16 kdc=1;
    int AD_dif1,AD_sum1;
    uint8 num;
    int steer_inc;
    float position1[4],position2,piancha[4],positiony,ADx1[5],psum,\
        xianzhiflag,positionerror,d,e,f,kpc,kdc_1=50;
    int steer_PWM;
    uint8 zhijiao_flag;
    uint16 temp_serial=120;
    float d = 0.00013,e = 0.005,f = 0.8;
    
/*  Function declaration------------------------------------------------------*/
    uint16 abs_jdz(int X);
    void bell_init(PTXn_e bell,uint8 state);
    
/*  Declare-------------------------------------------------------------------*/
    extern int middle0,left_zhi,right_zhi;
    extern int steer_PWM;
    extern int AD_dif2,AD_sum2,AD_flag1,AD_flag2;
    extern uint8 zhijiao_flag;
    extern uint16 right0[],right1[],left1[],left0[];
    extern uint32 temp_speed;
    static uint16 AD_Array_num;//AD循环队列的头部
    
/*  Function -----------------------------------------------------------------*/
uint16 abs_jdz(int X)//设定绝对值
{
    X=X>=0?X:-X;
    
    return X;
}

void ser_ctrl(void)
{    
    int16 str_inc;
    uint16 ser_pwm;
    static uint16 real_ad_num;
    static uint16 real_right0,real_right1,real_left1,real_left0;
    
    real_ad_num = (AD_Array_num + 20) % ADEEP;
    real_right0=right0[real_ad_num];
    real_right1=right1[real_ad_num];
    real_left1 =left1[real_ad_num];
    real_left0 =left0[real_ad_num];
      
    for(int i=4;i>0;i--)
        position1[i]=position1[i-1];
    AD_dif1=(real_left0+real_left1)-(real_right0+real_right1);
    AD_sum1=real_left0+real_right0+real_left1+real_right1;
    ADflag1 = (float)AD_dif1/(float)AD_sum1;
    position1[0] = (int)(ADflag1*(float)temp_serial); 
    //LCD_Show_Number(0,3  ,abs_jdz((uint16)position1[0]));
    if(position1[0]<0)  position1[0]=-position1[0];     
    if(real_right0<real_left0)    position1[0]=-position1[0];
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
    
    //kpc=d*position1[0]*position1[0]-e*jueduizhif(position1[0])+f;//很明显加这个动态p会乱抖
    
    kpc=0.6; 
    
    positionerror = position1[0]-position1[1]; 
    
    str_inc = (int)(kpc*position1[0]+kdc_1*positionerror);   
    
    //temp_speed=200;
    temp_speed=(int)( 1.2*(100-abs(str_inc)) );
    
    if(abs(str_inc)>=70)
        temp_speed=120; 
    if((real_left1<2)&&(real_left0<2)&&(real_right1<2)&&(real_right0<2)) {
        temp_speed=0;
        str_inc=0;
    }
    
    ser_pwm = 620+str_inc;
    
    if(ser_pwm>720) ser_pwm=720;
    if(ser_pwm<520) ser_pwm=520;
    
    ftm_pwm_duty(STEERFTM,STEERFTM_CH,ser_pwm);   
}