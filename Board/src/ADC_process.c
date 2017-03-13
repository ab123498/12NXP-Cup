void Read_ADC()//读取AD值，滑动滤波
{  
    for(char i=9;i>0;i--)//采10次AD值
    {
        ad_1.AD[i] = ad_1.AD[i-1];
        ad_2.AD[i] = ad_2.AD[i-1];      
        ad_3.AD[i] = ad_3.AD[i-1];
        ad_4.AD[i] = ad_4.AD[i-1];
    }
    
    AD_valu[0]=adc_ave(AMP3,ADC_12bit,3);//采样N次的ADC值平均值 
    AD_valu[1]=adc_ave(AMP4,ADC_12bit,3); 
    AD_valu[2]=adc_ave(AMP5,ADC_12bit,3); 
    AD_valu[3]=adc_ave(AMP6,ADC_12bit,3); 

    ad_1.AD[0] = (unsigned int)(((unsigned long)AD_valu[0])*3301>>12);  //right0
    ad_2.AD[0] = (unsigned int)(((unsigned long)AD_valu[1])*3301>>12);  //right1
    ad_3.AD[0] = (unsigned int)(((unsigned long)AD_valu[2])*3301>>12);  //left1
    ad_4.AD[0]= (unsigned int)(((unsigned long)AD_valu[3])*3301>>12);  //left0                                          
}