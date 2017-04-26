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
    #define BELLPORT PTA9
    #define BELLOFF  0
    #define BELLON   1
   
/*  Variable------------------------------------------------------------------*/
	uint32 span_main_cycle;//大循环时间
    uint32 temp_speed;
    
/*  Function declaration------------------------------------------------------*/
	void bell_init(PTXn_e bell,uint8);
	void encoder_init(void);
    void write_flash_data(Dtype flash_area_write , uint16 offset);
    Dtype read_flash_data(uint16 offset);
    void push_data2flash(void);
    void adc_conv_init(void);
    void set_ftm_ser(void) ;
    void cut_AD_pause_init(void);
    void vscope_test( void );
    void NVIC_Config( void );

/*  Declare-------------------------------------------------------------------*/
	extern uint16 encoder1;                               //定义在MK60_it源文件
    extern Dtype user_flag;                               //定义在MK60_it源文件
    extern uint32 span_pit_cycle;                         //定义在MK60_it源文件
    extern AD_V ad_1,ad_2,ad_3,ad_4;
    extern int16 OutData[4];
    
void main()
{
	led_all_init();
	DisableInterrupts;
	LCD_Init();
	LCD_DLY_ms(50);
    LCD_P6x8Str(6,0,"imTim12138 94NB 946");
    flash_init();                                       //初始化flash
    flash_erase_sector(SECTOR_NUM);                     //擦除扇区
                                                        //写入flash数据前，需要先擦除对应的扇区(不然数据会乱)
    adc_conv_init();
    EnableInterrupts;
    NVIC_Config();
    encoder_init();
    //cut_AD_pause_init();
    uart_init(UART5,115200);
	uart_putstr   (UART5 ,"\n\n\necho:\n\n");             //发送字符串
    set_vector_handler(UART5_RX_TX_VECTORn,uart5_handler);//设置中断服务函数到中断向量表里
    NVIC_SetPriority(UART5_RX_TX_IRQn, 0x40);             //优先级（ 01 ）B
    uart_rx_irq_en(UART5);                                //开串口接收中断
	ftm_pwm_init(STEERFTM,STEERFTM_CH,300,620);           //初始化 FTM PWM ，使用 FTM0_CH3，频率为10k ，占空比为 100 / FTM0_PRECISON
                                                          //FTM0_PRECISON 配置 为 100 ，即占空比 为 100%
                                                          //port_cfg.h 里 配置 FTM0_CH3 对应为 PTA6
                                                          //舵机初始化，频率50~300,改动后中值需要另调，482为初始化中值  525
    ftm_pwm_init(MOTORFTM,MOTORFTM_A,10000, 0);
    ftm_pwm_init(MOTORFTM,MOTORFTM_B,10000, temp_speed);
    bell_init(BELLPORT,BELLOFF);                          //输入为 0 不响
	while(1) {  
        lptmr_timing_ms(60000);                           //以lptmr测量大循环周期   
        span_main_cycle = lptmr_time_get_ms();            //获得大循环周期
        if(user_flag.DW != 0) {
            poll_printf();
            uart_input_format();
            push_data2flash();
            set_ftm_ser();
            ftm_pwm_duty(MOTORFTM,MOTORFTM_A,temp_speed);
			//printf("%f",test);
        }
        span_main_cycle = lptmr_time_get_ms();
        //vscope_test();
	}
}

void NVIC_Config( void )
{
    NVIC_SetPriorityGrouping(0xC0);                        //2位优先级
}

void vscope_test( void ) 
{
  //Read_ADC(10);
  OutData[0] = ad_1.max;
  OutData[1] = ad_2.max;
  OutData[2] = ad_3.max;
  OutData[3] = ad_4.max;
  OutPut_Data(OutData);
}


void adc_conv_init( void ) 
{
    adc_init(AMP1);
    adc_init(AMP2);
    adc_init(AMP3);
    adc_init(AMP4);
    adc_init(AMP5);
    adc_init(AMP6);
}

void bell_init(PTXn_e bell,uint8 state)
{
	gpio_init(bell,GPO,state);
}

void encoder_init(void)
{
    ftm_quad_init(FTM1);                                  //FTM1 正交解码初始化（所用的管脚可查 port_cfg.h ）
    pit_init_us(PIT0, 10);                                //初始化PIT0，定时时间为： 50ms
    set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);    //设置PIT0的中断服务函数为 PIT0_IRQHandler
    NVIC_SetPriority(PIT0_IRQn, 0x80);                    //优先级（ 10 ）B
    enable_irq (PIT0_IRQn);                               //使能PIT0中断
}

void cut_AD_pause_init(void)
{
    pit_init_us(PIT1, 5);                                 //初始化PIT0，定时时间为： 50ms
    set_vector_handler(PIT1_VECTORn ,PIT1_IRQHandler);    //设置PIT0的中断服务函数为 PIT0_IRQHandler
    NVIC_SetPriority(PIT1_IRQn, 0xC0);                    //优先级（ 11 ）B
    enable_irq (PIT1_IRQn);                               //使能PIT0中断
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
    
    if( 1 == flash_write(SECTOR_NUM, offset, flash_area_write.DW) ) {  //写入数据到扇区，偏移地址为0，必须一次写入4字节
        printf("write echo:\n");                                        //if是用来检测是否写入成功，写入成功了就读取
        
        
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
