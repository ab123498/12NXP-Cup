/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       main.c
 * @brief      山外K60 平台主程序
 * @author     山外科技
 * @version    v5.0
 * @date       2013-08-28
 */
/*  Include-------------------------------------------------------------------*/
	#include "common.h"
	#include "include.h"
	#include "MK60_it.h"
	 
/*  Variable------------------------------------------------------------------*/
	

/*  Function------------------------------------------------------------------*/
	void bell_init(PTXn_e bell);
	void encoder_init(void);

/*  Declare-------------------------------------------------------------------*/
	extern uint8 sig1;
	extern uint16 encoder1;

/*!
 *  @brief      main函数
 *  @since      v5.0
 *  @note       测试 LED 功能是否正常
                看到的效果是LED0和LED1同时亮灭闪烁
 */
void main()
{
	led_all_init();
	bell_init(PTA25);
	encoder_init();
	DisableInterrupts;
	LCD_Init(); 
	EnableInterrupts;
	DELAY_MS(50);
	LCD_P6x8Str(0,0,"imTim12138 94NB 946");
	uart_putstr   (UART4 ,"\n\n\n接收中断测试：");           //发送字符串
    set_vector_handler(UART4_RX_TX_VECTORn,uart4_handler);   // 设置中断服务函数到中断向量表里
    uart_rx_irq_en(UART4);                                 //开串口接收中断
	
	ftm_pwm_init(FTM0,FTM_CH0,300,620);         	//初始化 FTM PWM ，使用 FTM0_CH3，频率为10k ，占空比为 100 / FTM0_PRECISON
                                                    // FTM0_PRECISON 配置 为 100 ，即占空比 为 100%
                                                    // port_cfg.h 里 配置 FTM0_CH3 对应为 PTA6
													// 舵机初始化，频率50~300,改动后中值需要另调，482为初始化中值  525
    gpio_init(PTD15,GPO,0);                         // 使能端 输入为 0
	while(1)
	{   
		int i;
		DELAY_MS(500);                                
		led_all_turn();
		if(sig1!=0) {
			LCD_CLS();
			LCD_write_char(0,0,sig1);
			sig1=0;
		}
		//gpio_turn(PTA25);
		for(i= 0;i<=100;i+=10)
        {
            ftm_pwm_duty(FTM0,FTM_CH0,i);       //改变 占空比 ，K60 输出 PWM 占空比 逐渐增大，电机逐渐 降速
            DELAY_MS(500);
        }
	}
}

/*!
 *brief: bell_init
 *note: 蜂鸣器初始化
 */
void bell_init(PTXn_e bell)
{
	gpio_init(bell,GPO,0);
}

void encoder_init(void)
{
    ftm_quad_init(FTM1);                                    //FTM1 正交解码初始化（所用的管脚可查 port_cfg.h ）
    pit_init_ms(PIT0, 500);                                 //初始化PIT0，定时时间为： 1000ms
    set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);      //设置PIT0的中断服务函数为 PIT0_IRQHandler
    enable_irq (PIT0_IRQn);                                 //使能PIT0中断
}