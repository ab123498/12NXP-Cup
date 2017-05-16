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
    uint8 num;
    int steer_inc;
    float position1[4],position2,piancha[4],positiony,ADx1[5],psum,\
        xianzhiflag,positionerror,d,e,f,kpc,kdc_1;
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
    extern int AD_dif1,AD_sum1,AD_dif2,AD_sum2,AD_flag1,AD_flag2;
    extern uint8 zhijiao_flag;
    extern uint16 right0,right1,left1,left0;
    extern uint32 temp_speed;
    
float QuZongZhi(float adc_i,float adc_j,float adc_k)
{
    float tmp;
    //取中值
    tmp = adc_i > adc_j ? adc_i : adc_j;          //tmp取两者最大值
    return adc_k > tmp ? tmp : ( adc_k > adc_i ? adc_k : adc_i );
}

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
    int16 str_inc;
    uint16 ser_pwm;
    for(int i=4;i>0;i--)
        position1[i]=position1[i-1];
    AD_dif1=(left0+left1)-(right0+right1);
    AD_sum1=left0+right0+left1+right1;
    ADflag1 = (float)AD_dif1/(float)AD_sum1;
    position1[0] = (int)(ADflag1*(float)temp_serial); 
    //LCD_Show_Number(0,3  ,abs_jdz((uint16)position1[0]));
    if(position1[0]<0)  position1[0]=-position1[0];     
    if(right0<left0)    position1[0]=-position1[0];
    if(fabs(position1[0])>100)  position1[0]=100;  
    
    if((abs(left1-60)<5)&&(abs(right1-60)<5)) {          //识别圆环
        ser_pwm=560;
        ftm_pwm_duty(STEERFTM,STEERFTM_CH,ser_pwm);
        bell_init(BELLPORT,1);
        dwt_delay_ms(150);
        bell_init(BELLPORT,0);
    }
    
    if(left0<60&&right0<60) 
        position1[0]=position1[0]/1.5;
//    if (abs(left0-right0)<10&&abs(left1-right1)<10) {                                //简易动态P
//        kpc=0.6;        
//    }
//    else {
//        kpc=0.9;
//        kdc_1=kdc;
//    }
    kpc=d*position1[0]*position1[0]-e*jueduizhif(position1[0])+f;
    positionerror = position1[0]-position1[1]; 
    str_inc = (int)(kpc*position1[0]+kdc_1*positionerror);             
    temp_speed=(int)( 1.7*(100-abs(str_inc)) );
    if(abs(str_inc)>=70)
        temp_speed=100; 
    if((left1<2)&&(left0<2)&&(right1<2)&&(right0<2)) {
        temp_speed=30;
     
    }
    ser_pwm = 620+str_inc;
    if(ser_pwm>720) ser_pwm=720;
    if(ser_pwm<520) ser_pwm=520;
    ftm_pwm_duty(STEERFTM,STEERFTM_CH,ser_pwm);   
}