#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include  "common.h"

    #define BELLPORT PTA9
    #define BELLON  0
    #define BELLOFF   1

    #define ADEEP   100//队列的存储深度
    #define ADNUM   (ADEEP-1)
    #define ADWIDE  3  //几毫秒出一次ad及差和比  

    #define DX              0.000091
    #define EX              0
    #define FX              0.31
    #define DS              10

    #define L0R0LOW         (3300)          //入环衰减打脚值
    #define L0R0HIG         (720/40)        //入环衰减打脚值
    #define L2R2TOP         (1200)          //最边电感识别环范围
    #define L2R2BOT         25

    #define SP              95              //打脚因数
    #define ADELAY          0               //环内历史值
    #define DLYT            (180/3)         //打脚延时
    #define CIRT            (1500/3)         //重入环时间
    #define OUTC            1800            //出环值

    #define STOPT           200             //停车延时
    #define SPEEDEDGE       700             

    #define RHSD            13//入环系数

/*
 * Include 用户自定义的头文件
 */
   
#include  "MK60_wdog.h"
#include  "MK60_gpio.h"     //IO口操作
#include  "MK60_uart.h"     //串口
#include  "MK60_SysTick.h"
#include  "MK60_lptmr.h"    //低功耗定时器(延时)
#include  "MK60_pit.h"      //PIT
#include  "MK60_FLASH.h"    //FLASH
#include  "MK60_FTM.h"
#include  "MK60_spi.h"
#include  "MK60_i2c.h"
#include  "MK60_adc.h"
#include  "MK60_DWT.h"
#include  "VCAN_LED.H"          //LED
#include  "VCAN_KEY.H"          //KEY
#include  "oled.h"			//OLED
#include "ADC_process.h"
#include  "VCAN_computer.h"     //多功能调试助手
#include "steer.h"
#include "DebugOutput.h"
#include "speed.h"
   
#endif  //__INCLUDE_H__
