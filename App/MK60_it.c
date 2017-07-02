/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_it.c
 * @brief      山外K60 平台中断服务函数
 * @author     山外科技
 * @version    v5.0
 * @date       2013-06-26
 */

#include "MK60_it.h"
#include "common.h"
#include "include.h"
#include "stdio.h"

/*  Variable------------------------------------------------------------------*/
	Dtype  user_flag;        //用户标志结构
    uint32 span_pit_cycle;   //pit中断时间
    uint32 time_sum=0;       //秒级计时
    uint32 PIT0_Time_count;
    int16  encoder1;         //编码器输出
    uint16 right1,right0,middle,left0,left1,right2,left2;
    uint16 position_num=0;
    int8   ch_buffer[81];    //串口接收buffer
    
/*  Declare-------------------------------------------------------------------*/
    extern AD_V ad_1,ad_2,ad_3,ad_4,ad_5,ad_6;
    extern float position[];
    extern int pwm;
    extern int speed_ctl_output;
    extern uint8 speed_array[5];
    
/*  Callback Function -------------------------------------------------------------*/    
/*!
 *  @brief      UART4中断服务函数
 *  @since      v5.0
 */
void uart5_handler(void)
{
    static uint16 count=0;
    if(uart_query(UART5) == 1 && !user_flag.b1) {                  //接收数据寄存器满
        //用户需要处理接收数据
        uart_getchar(UART5, &ch_buffer[count]);                    //无限等待接受1个字节
        putchar(ch_buffer[count]);
        if(ch_buffer[count] == '\n' || count++ == 79) {
            count = 0;
            user_flag.b1 = 1;//接收完成标识
        }
    }
    if( user_flag.b1 ) {
        printf("%s",ch_buffer);
    }
}

/*!
 *brief: PIT0_IRQHandler
 *note:  PIT0中断服务函数
 */
void PIT0_IRQHandler(void)//！！！命名：count是记中断次数的，num是记数组角标的
{
    int16 val;
    static uint32 position_count;
    static uint16 speed_array_count_num;
    uint8 ch[4];
    
    lptmr_timing_ms(65535);
/*  speed input---------------------------------------------------------------*/    
    val = ftm_quad_get(FTM1);                           //获取FTM 正交解码 的脉冲数(负数表示反方向)
    ftm_quad_clean(FTM1);
    speed_array[speed_array_count_num] = -val;
    pwm = Getspeed(speed_ctl_output,-val);//
    set_speed(pwm);
    
/*  senser input array--------------------------------------------------------*/    
    if( position_count % ADWIDE ) {
        right0= ad_3.max/40;
        left1 = ad_2.max/40;
        left0 = ad_1.max/40;
        right1= ad_4.max/40;
        left2= ad_5.max/40;
        middle= ad_6.max/40;
        position_num%=ADEEP;
        ser_ctrl();
        position_num++;
    }
    if(PIT0_Time_count%20==0) {
        key_IRQHandler();
    }
    
    if(PIT0_Time_count%20==0) {
        user_flag.b4 = 1;
    }
    
    if(PIT0_Time_count%300==0) {
        user_flag.b0 = 1;                               //b0 用于大循环printf
    }
    if(PIT0_Time_count==1000) {
        PIT0_Time_count=0;
        if(time_sum == 999) time_sum=0;
        sprintf(ch,"%ds",time_sum++);
        LCD_P6x8Str(52,7,ch);
        //printf("pwm%d\n",pwm);
        //printf("!!!sj %d\n",-val);
        //printf("mb %d\n",speed_ctl_output);
    }
    
    if(position_count==10000) position_count=0;
    if(speed_array_count_num==4) speed_array_count_num=0;
    PIT0_Time_count++;
    position_count++;
    speed_array_count_num++;
    
    if( !(gpio_get(PTD4) && gpio_get(PTD6)) ) user_flag.b8=1;
    
    span_pit_cycle = lptmr_time_get_ms();               //获得pit周期
    PIT_Flag_Clear(PIT0);                               //清中断标志位
}
