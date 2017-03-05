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
    #include <string.h>

/*  Define--------------------------------------------------------------------*/
    #define SECTOR_NUM  (FLASH_SECTOR_NUM-1)              //尽量用最后面的扇区，确保安全

/*  Variable------------------------------------------------------------------*/
	uint32 span_main_cycle;//大循环时间
	float test=0.123;

/*  Function declaration------------------------------------------------------*/
	void bell_init(PTXn_e bell,uint8);
	void encoder_init(void);
    void poll_printf(void);
    void write_flash_data(Dtype flash_area_write , uint16 offset);
    Dtype read_flash_data(uint16 offset);
    void push_data2flash(void);
    void uart_input_format(void);

/*  Declare-------------------------------------------------------------------*/
	extern uint16 encoder1;                               //定义在MK60_it源文件
    extern Dtype user_flag;                               //定义在MK60_it源文件
    extern uint32 span_pit_cycle;                         //定义在MK60_it源文件
    extern int8 ch_buffer[];                              //串口接收缓冲区

void main()
{
	led_all_init();
	encoder_init();
	DisableInterrupts;
	LCD_Init(); 
	LCD_DLY_ms(50);
    LCD_P6x8Str(0,0,"imTim12138 94NB 946");
    flash_init();                                       //初始化flash
    flash_erase_sector(SECTOR_NUM);                     //擦除扇区
                                                        //写入flash数据前，需要先擦除对应的扇区(不然数据会乱)
    EnableInterrupts;
    uart_init(UART4,115200);
	uart_putstr   (UART4 ,"\n\n\necho:\n\n");                 //发送字符串
    set_vector_handler(UART4_RX_TX_VECTORn,uart4_handler);//设置中断服务函数到中断向量表里
    uart_rx_irq_en(UART4);                                //开串口接收中断
	ftm_pwm_init(FTM0,FTM_CH1,300,620);                   //初始化 FTM PWM ，使用 FTM0_CH3，频率为10k ，占空比为 100 / FTM0_PRECISON
                                                          // FTM0_PRECISON 配置 为 100 ，即占空比 为 100%
                                                          // port_cfg.h 里 配置 FTM0_CH3 对应为 PTA6
                                                          // 舵机初始化，频率50~300,改动后中值需要另调，482为初始化中值  525
    ftm_pwm_init(FTM2,FTM_CH0,300,620);
    ftm_pwm_init(FTM2,FTM_CH1,300,620);
    bell_init(PTA9,0);                                   // 使能端 输入为 0
	while(1) {  
        lptmr_timing_ms(60000);                           //以lptmr测量大循环周期
		led_all_turn();                                   
        LCD_DLY_ms(500);
        //ftm_pwm_duty(FTM0,FTM_CH3,i);                   //改变 占空比 ，K60 输出 PWM 占空比 逐渐增大，电机逐渐 降速
        span_main_cycle = lptmr_time_get_ms();            //获得大循环周期
        if(user_flag.DW != 0) {
            poll_printf();
            uart_input_format();
            push_data2flash();
			//printf("%f",test);
        }
	}
}

void bell_init(PTXn_e bell,uint8 state)
{
	gpio_init(bell,GPO,state);
}

void encoder_init(void)
{
    ftm_quad_init(FTM1);                                  //FTM1 正交解码初始化（所用的管脚可查 port_cfg.h ）
    pit_init_ms(PIT0, 500);                               //初始化PIT0，定时时间为： 500ms
    set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);    //设置PIT0的中断服务函数为 PIT0_IRQHandler
    enable_irq (PIT0_IRQn);                               //使能PIT0中断
}

void poll_printf(void)
{   
#ifdef DEBUG
    if(user_flag.b0) {//大循环内有关输出
        printf("encoder1:%04d \n",encoder1);
        printf("span_main_cycle:%04d ms\n",span_main_cycle);
        printf("span_pit_cycle:%04d ms\n\n",span_pit_cycle);
        user_flag.b0=0;
    }
#endif
}

void uart_input_format(void)
{
    int16 temp;
	
    if(user_flag.b1) {//接收到数据需要处理
        if(strcmp(ch_buffer,"flash_test\n") == 0)
            user_flag.b2=1;
        sscanf(strchr(ch_buffer, ' ')+1,"%hd",&temp);//将空格后的数值存入变量
		printf("%hd\n",temp);
        memset(ch_buffer,0,80);
        user_flag.b1=0;
    }
}

void push_data2flash(void)
{
    if(user_flag.b2) {          
		Dtype temp;
		temp.DW=0x12345678;
		write_flash_data(temp , 0);
		read_flash_data(0);
		user_flag.b2 = 0;
	}
}

void write_flash_data(Dtype flash_area_write , uint16 offset)
{
    uint32 data32;
    
    if( 1 == flash_write(SECTOR_NUM, offset, flash_area_write.DW) ) {  
        //写入数据到扇区，偏移地址为0，必须一次写入4字节
        //if是用来检测是否写入成功，写入成功了就读取
        printf("write echo:\n");
        data32 = flash_read(SECTOR_NUM, 0, uint32);  //读取4字节
        printf("32bit:0x%08x\n\n", data32);
    }
}

Dtype read_flash_data(uint16 offset)
{
    Dtype  flash_area_read;

    flash_area_read.DW = flash_read(SECTOR_NUM - offset, 0, uint32);

    return flash_area_read;
}
