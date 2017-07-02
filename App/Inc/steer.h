#ifndef __STEER_H__
#define __STEER_H__
/*  Define--------------------------------------------------------------------*/
#define STEERFTM        FTM0
#define STEERFTM_CH     FTM_CH1
#define STEERMID        620
#define STEERH          720
#define STEERL          520
/*  Enum----------------------------------------------------------------------*/

/*  Function declaration------------------------------------------------------*/
extern void ser_ctrl(void);
extern uint16 abs_jdz(int X);//设定绝对值

#endif /*end of STEER*/
