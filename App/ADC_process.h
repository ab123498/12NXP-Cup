#ifndef __ADC_process_H__
#define __ADC_process_H__
/*  Define--------------------------------------------------------------------*/
    #define AMP1     ADC1_SE8     //PTB0
    #define AMP2     ADC1_SE9     //PTB1
    #define AMP3     ADC1_SE10     //PTB4 //right0
    #define AMP4     ADC1_SE11     //PTB5 //right1
    #define AMP5     ADC1_SE12     //PTB6 //left1
    #define AMP6     ADC1_SE13     //PTB7 //left0
/*  Function declaration------------------------------------------------------*/
    void Read_ADC(void);//��ȡADֵ�������˲�
    void AD_data(void);//�����͹�һ��
#endif /*end of ADC_process*/
