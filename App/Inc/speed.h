#ifndef __header_H__
#define __header_H__
/*  Define--------------------------------------------------------------------*/
#define MOTORFTM FTM2
#define MOTORFTM_A FTM_CH0
#define MOTORFTM_B FTM_CH1
/*  Enum----------------------------------------------------------------------*/

/*  Function declaration------------------------------------------------------*/
float jueduizhif(float x);
void set_speed(int speed) ;
int Getspeed(int ideal,int now);
void get_pulse(void);
void speed_control(void);
#endif /*end of header*/
