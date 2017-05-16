#ifndef __STEER_H__
#define __STEER_H__
/*  Define--------------------------------------------------------------------*/
#define STEERFTM        FTM0
#define STEERFTM_CH     FTM_CH1
/*  Enum----------------------------------------------------------------------*/

/*  Function declaration------------------------------------------------------*/
extern void ser_ctrl(void);
uint16 abs_jdz(int X);//设定绝对值

#endif /*end of STEER*/
