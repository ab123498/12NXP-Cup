/*  Include-------------------------------------------------------------------*/
	#include "common.h"
	#include "include.h"
	#include "MK60_it.h"
    #include <string.h>
    #include "DebugOutput.h"
/*  Define--------------------------------------------------------------------*/
/*  Variable------------------------------------------------------------------*/
    uint32 ser_mid;
/*  Function declaration------------------------------------------------------*/
/*  Declare-------------------------------------------------------------------*/
    extern Dtype user_flag;                               //定义在MK60_it源文件
    extern int8 ch_buffer[];                              //串口接收缓冲区
    extern uint16 temp_serial;

void poll_printf(void)
{   
#ifdef DEBUG
    if(user_flag.b0) {//大循环内有关输出
        //printf("encoder1:%04d \n",encoder1);
        //printf("span_main_cycle:%04d ms\n",span_main_cycle);
        //printf("span_pit_cycle:%04d ms\n\n",span_pit_cycle);
        user_flag.b0=0;
    }
#endif
}

void uart_input_format(void)
{
    if(user_flag.b1) {//接收到数据需要处理
        if(strcmp(ch_buffer,"flash_test\n") == 0)
            user_flag.b2=1;
        sscanf(strchr(ch_buffer, ' ')+1,"%hd",&temp_serial);//将空格后的数值存入变量
		printf("%hd\n",temp_serial);
        memset(ch_buffer,0,80);
        user_flag.b1=0;
    }
}

void set_ftm_ser(void) 
{
    if(user_flag.b3) {
        ftm_pwm_duty(FTM0,FTM_CH1,ser_mid);
        user_flag.b3=0;
        printf("ser_seted:%ld",ser_mid);
    }
}
