#ifndef __ADC_process_H__
#define __ADC_process_H__
/*  Define--------------------------------------------------------------------*/
    #define AMP1     ADC1_SE8     //PTB0 //运放板输出接口从左至右
    #define AMP2     ADC1_SE9     //PTB1
    #define AMP3     ADC1_SE10    //PTB4
    #define AMP4     ADC1_SE11    //PTB5
    #define AMP5     ADC1_SE12    //PTB6
    #define AMP6     ADC1_SE13    //PTB7

    #define AMPleft0 AMP4
    #define AMPleft1 AMP6
    #define AMPleft2 AMP5
    #define AMPright0 AMP3
    #define AMPright1 AMP1
    #define AMPright2 AMP2
    #define CONV_TIMES 10
    #define CHAIN_NUM 50
    typedef struct 
    {
        uint16 max;
        uint16 min;
        uint16 AD[CHAIN_NUM];
        uint16 data;
        uint16 hubuzhi;
    }AD_V;

/*  Function declaration------------------------------------------------------*/
    void Read_ADC(void);//读取AD值，滑动滤波
    void AD_data(void);//互补和归一化
    void proc_AD_conv(void);//伪环形队列

#endif /*end of ADC_process*/
