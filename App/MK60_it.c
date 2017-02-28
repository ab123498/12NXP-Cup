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

/*  Variable------------------------------------------------------------------*/
	int16 encoder1;//编码器输出
    Dtype user_flag;//用户标志结构
    uint32 span_pit_cycle;//pit中断时间
    int8 ch_buffer[81];//串口接收buffer
    
/*  Declare-------------------------------------------------------------------*/
    extern uint32 span_main_cycle;

/*!
 *  @brief      UART4中断服务函数
 *  @since      v5.0
 */
void uart4_handler(void)
{
    static uint16 count=0;
    if(uart_query(UART4) == 1 && !user_flag.b1) {                    //接收数据寄存器满
        //用户需要处理接收数据
        uart_getchar(UART4, &ch_buffer[count]);                    //无限等待接受1个字节
        if(ch_buffer[count] == '\n' || count++ == 79) {
            count = 0;
            user_flag.b1 = 1;//接收完成标识
        }
    }
}

/*!
 *brief: PIT0_IRQHandler
 *note:  PIT0中断服务函数
 */
void PIT0_IRQHandler(void)
{
    int16 val;
    
    lptmr_timing_ms(65535);
    val = ftm_quad_get(FTM1);                           //获取FTM 正交解码 的脉冲数(负数表示反方向)
    ftm_quad_clean(FTM1);
    if(val>=0)
        encoder1=val;
    else
        encoder1=-val;
    PIT_Flag_Clear(PIT0);                               //清中断标志位
    user_flag.b0 = 1;                                   //b0 用于大循环printf
    span_pit_cycle = lptmr_time_get_ms();               //获得pit周期
}

