/*  Include-------------------------------------------------------------------*/
    #include "common.h"
    #include "include.h"
    #include "ADC_process.h"
/*  Define--------------------------------------------------------------------*/
    
/*  Variable------------------------------------------------------------------*/
    AD_V ad_1,ad_2,ad_3,ad_4,ad_5,ad_6;    
    
/*  Function declaration------------------------------------------------------*/
    
/*  Declare-------------------------------------------------------------------*/

/*  Function -----------------------------------------------------------------*/
void proc_AD_conv(void)
{
    static uint16 chain_num = 0,temp_max=4095;
    
    ad_1.AD[chain_num % CHAIN_NUM] = adc_ave(AMPleft0,ADC_12bit,3);
    ad_2.AD[chain_num % CHAIN_NUM] = adc_ave(AMPleft1,ADC_12bit,3);
    ad_3.AD[chain_num % CHAIN_NUM] = adc_ave(AMPright0,ADC_12bit,3);
    ad_4.AD[chain_num % CHAIN_NUM] = adc_ave(AMPright1,ADC_12bit,3);
    ad_5.AD[chain_num % CHAIN_NUM] = adc_ave(AMPleft2,ADC_12bit,3);
    ad_6.AD[chain_num % CHAIN_NUM] = adc_ave(AMPright2,ADC_12bit,3);
    
    if(chain_num % CHAIN_NUM ==0) {
        ad_1.max = ad_1.AD[CHAIN_NUM-1];
        ad_2.max = ad_2.AD[CHAIN_NUM-1];
        ad_3.max = ad_3.AD[CHAIN_NUM-1];
        ad_4.max = ad_4.AD[CHAIN_NUM-1];
        ad_5.max = ad_5.AD[CHAIN_NUM-1];
        ad_6.max = ad_6.AD[CHAIN_NUM-1];
        
        if(ad_1.max>temp_max) ad_1.max=temp_max;//把大于1200的值削掉       
        if(ad_2.max>temp_max) ad_2.max=temp_max;        
        if(ad_3.max>temp_max) ad_3.max=temp_max;  
        if(ad_4.max>temp_max) ad_4.max=temp_max;
        if(ad_5.max>temp_max) ad_3.max=temp_max;  
        if(ad_6.max>temp_max) ad_4.max=temp_max;
        
        if(chain_num % 1000 ==0) {           
            LCD_Show_Number(24, 1 ,ad_1.max);       //OLED显示 AD数据
            LCD_Show_Number(72, 1,ad_3.max);
            LCD_Show_Number(0,2  ,ad_2.max);
            LCD_Show_Number(96,2 ,ad_4.max);
            LCD_Show_Number(0,4  ,ad_5.max);
            LCD_Show_Number(96,4 ,ad_6.max);
            //LCD_Show_Number(0,3  ,ad_5.max);
            //LCD_Show_Number(30,3 ,ad_6.max);
        }
    }
    if(chain_num++ > 0x13ff)
        chain_num = 0;
}