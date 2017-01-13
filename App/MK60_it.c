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
	uint8 sig1=0;
	int16 encoder1;

/*!
 *  @brief      UART4中断服务函数
 *  @since      v5.0
 */
void uart4_handler(void)
{
    int8 ch;
	UARTn_e uratn = UART4;

    if(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK)   //接收数据寄存器满
    {
        //用户需要处理接收数据
        uart_getchar(UART4,&ch);                    //无限等待接受1个字节
		uart_putchar(UART4,ch); //发送字符串
		sig1 = (uint8)ch;
    }
}

/*!
 *brief: PIT0_IRQHandler
 *note:  PIT0中断服务函数
 */
void PIT0_IRQHandler(void)
{
    int16 val;
    val = ftm_quad_get(FTM1);          //获取FTM 正交解码 的脉冲数(负数表示反方向)
    ftm_quad_clean(FTM1);

    if(val>=0)
    {
        encoder1=val;
    }
    else
    {
        encoder1=-val;
    }
    PIT_Flag_Clear(PIT0);       //清中断标志位
}

