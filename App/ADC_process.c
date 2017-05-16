/*  Include-------------------------------------------------------------------*/
    #include "common.h"
    #include "include.h"
    #include "ADC_process.h"
/*  Define--------------------------------------------------------------------*/
    
/*  Variable------------------------------------------------------------------*/
    AD_V ad_1,ad_2,ad_3,ad_4,ad_5,ad_6;
    int16  AD_valu[6],AD1_valu[5];
    uint16  AD_conv[CONV_TIMES];
    uint16 left0,left1,middle0,right1,right0;
    int AD_dif1,AD_sum1;
    
/*  Function declaration------------------------------------------------------*/
    void quicksort(uint16 a[], uint16 Length);
    uint16 split(uint16 a[], uint16 low, uint16 high);
/*  Declare-------------------------------------------------------------------*/

/*  Function -----------------------------------------------------------------*/
//void Read_ADC()
//{
//    for(char i=CONV_TIMES-1;i>0;i--) {
//        ad_1.AD[i] = ad_1.AD[i-1];
//        ad_2.AD[i] = ad_2.AD[i-1];
//        ad_3.AD[i] = ad_3.AD[i-1]; 
//        ad_4.AD[i] = ad_4.AD[i-1];
//        ad_5.AD[i] = ad_5.AD[i-1]; 
//        ad_6.AD[i] = ad_6.AD[i-1];
//    }
//    AD_valu[0]= adc_ave(AMPleft0,ADC_12bit,3);   
//    AD_valu[1]= adc_ave(AMPright0,ADC_12bit,3); 
//    AD_valu[2]= adc_ave(AMPleft1,ADC_12bit,3);
//    AD_valu[3]= adc_ave(AMPright1,ADC_12bit,3); 
//    AD_valu[4]= adc_ave(AMPleft2,ADC_12bit,3);   
//    AD_valu[5]= adc_ave(AMPright2,ADC_12bit,3); 
//
//    ad_1.AD[0] = (unsigned int)(((unsigned long)AD_valu[0])*3301>>12);
//    ad_2.AD[0] = (unsigned int)(((unsigned long)AD_valu[1])*3301>>12);
//    ad_3.AD[0] = (unsigned int)(((unsigned long)AD_valu[2])*3301>>12);
//    ad_4.AD[0] = (unsigned int)(((unsigned long)AD_valu[3])*3301>>12);
//    ad_5.AD[0] = (unsigned int)(((unsigned long)AD_valu[4])*3301>>12);
//    ad_6.AD[0] = (unsigned int)(((unsigned long)AD_valu[5])*3301>>12);
//}  
    
void DirectionVoltageSigma(void)
{
    /*******把上一次采集电感的AD值给到数组的下一位，给后来的加和除以10用********/
     for(char i=9;i>0;i--)                          
    {
        ad_1.AD[i] = ad_1.AD[i-1]; 
        ad_2.AD[i] = ad_2.AD[i-1];
        ad_3.AD[i] = ad_3.AD[i-1];
        ad_4.AD[i] = ad_4.AD[i-1];
    }
       AD1_valu[0]= adc_ave(ADC1_SE4a,ADC_12bit,3); 
       AD1_valu[1]= adc_ave(ADC1_SE5a,ADC_12bit,3); 
       AD1_valu[2]= adc_ave(ADC1_SE6a,ADC_12bit,3); 
       AD1_valu[3]= adc_ave(ADC1_SE9,ADC_12bit,3); 
       
     /********第一次AD采集的数值范围在0~3301************/     
     ad_1.AD[0] = (unsigned int)(((unsigned long)AD1_valu[0])*3301>>12);  //right0 23
     ad_2.AD[0] = (unsigned int)(((unsigned long)AD1_valu[1])*3301>>12);  //right1 18
     ad_3.AD[0] = (unsigned int)(((unsigned long)AD1_valu[2])*3301>>12);  //left1  20
     ad_4.AD[0] = (unsigned int)(((unsigned long)AD1_valu[3])*3301>>12);  //left0  21
}
    
void AD_data(void)
{    ad_1.data=0;
     ad_2.data=0;
     ad_3.data=0;
     ad_4.data=0;
    
    for(int i=0;i<10;i++)                  //将采集的值进行10次加和
    { 
        ad_1.data=ad_1.data+ad_1.AD[i];  
        ad_2.data=ad_2.data+ad_2.AD[i];
        ad_3.data=ad_3.data+ad_3.AD[i];
        ad_4.data=ad_4.data+ad_4.AD[i];
        if(i==4)
        {
           ad_1.hubuzhi=ad_1.data;         //取前5组的值的和赋给互补值
           ad_2.hubuzhi=ad_2.data;
           ad_3.hubuzhi=ad_3.data;
           ad_4.hubuzhi=ad_4.data;
        }  
    }
    ad_1.data=ad_1.data/10;               //加10次再除以10 ，取平均值
    ad_2.data=ad_2.data/10;
    ad_3.data=ad_3.data/10;
    ad_4.data=ad_4.data/10;
    
#if 1    
    ad_1.hubuzhi=ad_2.hubuzhi/5;
    ad_4.hubuzhi=ad_3.hubuzhi/5;
    ad_1.data=(ad_1.data/2+ad_1.hubuzhi/2); //互补后right0电感值等于right0的一半加right1一半
    ad_4.data=(ad_4.data/2+ad_4.hubuzhi/2); //互补后left0电感值等于left0的一半加left1一半

   
    ad_1.max=3300;   //3000                
    ad_1.min=100;                          
    ad_2.max=3300;   //3000
    ad_2.min=100;
    ad_3.max=3300;   //3000
    ad_3.min=100;
    ad_4.max=3300;   //3000
    ad_4.min=100;
    if(ad_1.data>ad_1.max)       ad_1.data=ad_1.max;          //把大于3300的值削掉
    else if(ad_1.data<ad_1.min)  ad_1.data=ad_1.min;          //把小于100的值削掉
    
    if(ad_2.data>ad_2.max)        ad_2.data=ad_2.max;
    else if(ad_2.data<ad_2.min)   ad_2.data=ad_2.min;
    
    if(ad_3.data>ad_3.max)         ad_3.data=ad_3.max;
    else if(ad_3.data<ad_3.min)    ad_3.data=ad_3.min;
    
    if(ad_4.data>ad_4.max)          ad_4.data=ad_4.max;
    else if(ad_4.data<ad_4.min)     ad_4.data=ad_4.min;
   
    ad_1.data=(ad_1.data-ad_1.min)*100/(ad_1.max-ad_1.min);      //将数据归一化
    ad_2.data=(ad_2.data-ad_2.min)*100/(ad_2.max-ad_2.min);
    ad_3.data=(ad_3.data-ad_3.min)*100/(ad_3.max-ad_3.min);
    ad_4.data=(ad_4.data-ad_4.min)*100/(ad_4.max-ad_4.min);

#endif
    right0= ad_1.data;  //E23
    right1= ad_2.data;  //E18
    left1 = ad_3.data;  //E20
    left0 = ad_4.data;  //E21
    
    LCD_Show_Number(0, 2,right0);       //OLED显示 AD数据
    LCD_Show_Number(0, 1,right1);
    LCD_Show_Number(36,1,left1);        
    LCD_Show_Number(36,2,left0);
}  

void proc_AD_conv(void)
{
    static uint16 chain_num = 0,temp_max=10000;
    
    ad_1.AD[chain_num % CHAIN_NUM] = adc_ave(AMPleft0,ADC_12bit,3);
    ad_2.AD[chain_num % CHAIN_NUM] = adc_ave(AMPleft1,ADC_12bit,3);
    ad_3.AD[chain_num % CHAIN_NUM] = adc_ave(AMPright0,ADC_12bit,3);
    ad_4.AD[chain_num % CHAIN_NUM] = adc_ave(AMPright1,ADC_12bit,3);
    
    if(chain_num % CHAIN_NUM ==0) {
        //quicksort(ad_1.AD,CHAIN_NUM);
        //quicksort(ad_2.AD,CHAIN_NUM);
        //quicksort(ad_3.AD,CHAIN_NUM);
        //quicksort(ad_4.AD,CHAIN_NUM);
        //quicksort(ad_5.AD,CHAIN_NUM);
        //quicksort(ad_6.AD,CHAIN_NUM);
        ad_1.max = ad_1.AD[CHAIN_NUM-1];
        ad_2.max = ad_2.AD[CHAIN_NUM-1];
        ad_3.max = ad_3.AD[CHAIN_NUM-1];
        ad_4.max = ad_4.AD[CHAIN_NUM-1];
        ad_5.max = ad_5.AD[CHAIN_NUM-1];
        ad_6.max = ad_6.AD[CHAIN_NUM-1];
        
        if(ad_1.max>temp_max)       ad_1.max=temp_max;//把大于1200的值削掉
        
        if(ad_2.max>temp_max)       ad_2.max=temp_max;
        
        if(ad_3.max>temp_max)       ad_3.max=temp_max;
  
        if(ad_4.max>temp_max)       ad_4.max=temp_max;
 
        right1= ad_3.max/33;  //E23
        right0= ad_4.max/33;  //E18
        left0 = ad_2.max/33;  //E20
        left1 = ad_1.max/33;  //E21
        if(chain_num % 1000 ==0) {
            LCD_Show_Number(0, 1 ,ad_1.max);       //OLED显示 AD数据
            LCD_Show_Number(96, 1,ad_3.max);
            LCD_Show_Number(0,2  ,ad_2.max);
            LCD_Show_Number(96,2 ,ad_4.max);
            //LCD_Show_Number(0,3  ,ad_5.max);
            //LCD_Show_Number(30,3 ,ad_6.max);
        }
    }
    if(chain_num++ > 0x13ff)
        chain_num = 0;
}

void quicksort(uint16 arr[], uint16 Length)
{
    uint16 temp = 0;
    for (int j = 0; j < Length - 1 ; j++)
    {
        if (arr[j] > arr[j + 1])
        {
            temp = arr[j + 1];
            arr[j + 1] = arr[j];
            arr[j] = temp;
        }
    }
}

//void AD_data(void)//互补和归一化
//{
//    ad_1.data=0;
//    ad_2.data=0;
//    ad_3.data=0;
//    ad_4.data=0;
//    ad_5.data=0;
//    ad_6.data=0;
//    for(int i=0;i<10;i++) 
//    { 
//        ad_1.data=ad_1.data+ad_1.AD[i];  
//        ad_2.data=ad_2.data+ad_2.AD[i];
//        ad_3.data=ad_3.data+ad_3.AD[i];
//        ad_4.data=ad_4.data+ad_4.AD[i];
//        ad_5.data=ad_5.data+ad_5.AD[i];
//        ad_6.data=ad_6.data+ad_6.AD[i];
//        if(i==2)
//        {
//           ad_1.hubuzhi=ad_1.data;
//           ad_2.hubuzhi=ad_2.data;
//           ad_3.hubuzhi=ad_3.data;
//           ad_4.hubuzhi=ad_4.data;
//           ad_5.hubuzhi=ad_5.data;
//           ad_6.hubuzhi=ad_6.data;
//        }  
//    }
//    
//    ad_1.data=ad_1.data/10;
//    ad_2.data=ad_2.data/10;
//    ad_3.data=ad_3.data/10;
//    ad_4.data=ad_4.data/10;
//    ad_5.data=ad_5.data/10;
//    ad_6.data=ad_6.data/10;
//    
//    ad_1.hubuzhi=ad_2.hubuzhi/5;
//    ad_4.hubuzhi=ad_3.hubuzhi/5;
//    
//    ad_1.data=(ad_1.data/2+ad_1.hubuzhi);
//    ad_4.data=(ad_4.data/2+ad_4.hubuzhi);
//    
//    //归一化 
//    ad_1.max=3000;//3000
//    ad_1.min=100;
//    ad_2.max=3000;//3000
//    ad_2.min=100;
//    ad_3.max=3000;
//    ad_3.min=100;
//    ad_4.max=3000;//3000
//    ad_4.min=100;
//    ad_5.max=3000;//3000
//    ad_5.min=100;
//    ad_6.max=3000;//3000
//    ad_6.min=100;
//
//    if(ad_1.data>ad_1.max)ad_1.data=ad_1.max;
//    else if(ad_1.data<ad_1.min)ad_1.data=ad_1.min;
//    if(ad_2.data>ad_2.max)ad_2.data=ad_2.max;
//    else if(ad_2.data<ad_2.min)ad_2.data=ad_2.min;
//    if(ad_3.data>ad_3.max)ad_3.data=ad_3.max;
//    else if(ad_3.data<ad_3.min)ad_3.data=ad_3.min;
//    if(ad_4.data>ad_4.max)ad_4.data=ad_4.max;
//    else if(ad_4.data<ad_4.min)ad_4.data=ad_4.min;
//    if(ad_5.data>ad_5.max)ad_5.data=ad_5.max;
//    else if(ad_5.data<ad_5.min)ad_5.data=ad_5.min;
//    if(ad_6.data>ad_6.max)ad_6.data=ad_6.max;
//    else if(ad_6.data<ad_6.min)ad_6.data=ad_6.min;
//
//    ad_1.data=(ad_1.data-ad_1.min)*100/(ad_1.max-ad_1.min);//归一化
//    ad_2.data=(ad_2.data-ad_2.min)*100/(ad_2.max-ad_2.min);
//    ad_3.data=(ad_3.data-ad_3.min)*100/(ad_3.max-ad_3.min);
//    ad_4.data=(ad_4.data-ad_4.min)*100/(ad_4.max-ad_4.min);
//    ad_5.data=(ad_5.data-ad_5.min)*100/(ad_5.max-ad_5.min);
//    ad_6.data=(ad_6.data-ad_6.min)*100/(ad_6.max-ad_6.min);
//    
//    right0 = ad_2.data; //四个电感值
//    right1 = ad_4.data; 
//    left1  = ad_3.data;
//    left0  = ad_1.data; 
//
//    LCD_Show_Number(0, 1,ad_1.data);       //OLED显示 AD数据
//    LCD_Show_Number(30, 1,ad_2.data);    
//    LCD_Show_Number(0,2,ad_3.data);        
//    LCD_Show_Number(30,2,ad_4.data);   
//    LCD_Show_Number(0,3,ad_5.data);        
//    LCD_Show_Number(30,3,ad_6.data);
//}