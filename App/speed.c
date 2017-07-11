/*  Include-------------------------------------------------------------------*/
	#include "common.h"
	#include "include.h"
	#include "MK60_it.h"
    #include "speed.h"
    #include <math.h>

/*  �������------------------------------------------------------------------*/
    int pid_speed,ideal,speed_max,speed_min,zhidao_speed,wandao_speed,brake_speed;
    int pwm,speed,djsd;
    uint8 zhidao_flag,wandao_flag;
    uint8 speed_array[5];
    int et,et1,et2,pianyi_duty,angel_duty;
    float  Kp=12,Ki=0.9,Kd=3;

void set_speed(int speed)//����ٶ�
{
    if(speed>=0) {
        ftm_pwm_duty(MOTORFTM,MOTORFTM_A,0);        //��ת
        ftm_pwm_duty(MOTORFTM,MOTORFTM_B,speed);    //��ת ռ�ձ� 1000����ٷְ�
    }
    else {
        ftm_pwm_duty(MOTORFTM,MOTORFTM_B,0);
        ftm_pwm_duty(MOTORFTM,MOTORFTM_A,-speed);
    }
#if 0
    if(speed<0) {
        ftm_pwm_duty(FTM0,FTM_CH3,0);
        ftm_pwm_duty(FTM0,FTM_CH2,abs_jdz(speed));
    }
#endif
}

void get_pulse(void)//�����ٶ�,����������ٶ�
{
    //Servo_count_1[0]=(int)(Servo_count_1[0]*0.4+Servo_count_1[1]*0.3+\
    //                       Servo_count_1[2]*0.2+Servo_count_1[3]*0.1);
    djsd = speed_array[0];
    LCD_Show_Number(36,4,djsd);
}

int Getspeed(int ideal,int now)//�����ٶ�PID,����nowΪ���Ŀǰ�ٶ�djsd��idealΪ����Ŀ���ٶ�
{ 
    static int16 et,et1=0,et2=0;
    static float  Kp=12,Ki=0.9,Kd=1;
    
    et=ideal-now;
    if(et<-50) et=-50;    
    else if(et>50) et=50;
       
    pid_speed=pid_speed+(int)(Ki*et+Kp*(et-et1)+Kd*(et-2*et1+et2));
    et2=et1;
    et1=et;

    if(pid_speed>SPEEDEDGE) pid_speed=SPEEDEDGE;
    if(pid_speed<-SPEEDEDGE) pid_speed=-SPEEDEDGE;
    //if(pid_speed<50 && pid_speed>-50) pid_speed = 0;
    return pid_speed;
}

void stop(void)
{
    ftm_pwm_duty(MOTORFTM,MOTORFTM_A,0);
    ftm_pwm_duty(MOTORFTM,MOTORFTM_B,0);
    DELAY_MS(10);
    ftm_pwm_duty(MOTORFTM,MOTORFTM_B,0);
    ftm_pwm_duty(MOTORFTM,MOTORFTM_A,400);
    DELAY_MS(200);
    ftm_pwm_duty(MOTORFTM,MOTORFTM_A,0);
}