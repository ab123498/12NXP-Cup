/*  Include-------------------------------------------------------------------*/
    #include "common.h"
    #include "include.h"
    #include "MK60_it.h"
    #include "stdio.h"
    #include <string.h>
    #include "DebugOutput.h"
/*  Define--------------------------------------------------------------------*/
/*  Variable------------------------------------------------------------------*/
    uint32 ser_mid;
    int16 OutData[4];
/*  Function declaration------------------------------------------------------*/
/*  Declare-------------------------------------------------------------------*/
    extern Dtype user_flag;                               //定义在MK60_it源文件
    extern int8 ch_buffer[];                              //串口接收缓冲区
    extern uint16 temp_serial;
    extern uint32 temp_speed;
  
             
                
    

unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)
{
    unsigned short CRC_Temp;
    unsigned char i,j;
    CRC_Temp = 0xffff;

    for (i=0;i<CRC_CNT; i++){      
        CRC_Temp ^= Buf[i];
        for (j=0;j<8;j++) {
            if (CRC_Temp & 0x01)
                CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
            else
                CRC_Temp = CRC_Temp >> 1;
        }
    }
    return(CRC_Temp);
}
void OutPut_Data()
{
  int temp[4] = {0};
  unsigned int temp1[4] = {0};
  unsigned char databuf[10] = {0};
  unsigned char i;
  unsigned short CRC16 = 0;
  for(i=0;i<4;i++)
   {
    
    temp[i]  = (int16)OutData[i];
    temp1[i] = (uint16)temp[i];
    
   }
   
  for(i=0;i<4;i++) 
  {
    databuf[i*2]   = (int8)(temp1[i]%256);
    databuf[i*2+1] = (int8)(temp1[i]/256);
  }
  
  CRC16 = CRC_CHECK(databuf,8);
  databuf[8] = CRC16%256;
  databuf[9] = CRC16/256;
  
  for(i=0;i<10;i++)
  uart_putchar(VCAN_PORT,databuf[i]); 
}
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
        sscanf(strchr(ch_buffer, ' ')+1,"%ld",&temp_speed);//将空格后的数值存入变量
		printf("%ld\n",temp_speed);
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
