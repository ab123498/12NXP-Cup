/*  Include-------------------------------------------------------------------*/
    #include "common.h"
    #include "include.h"
    #include "steer.h"
    #include <math.h>

/*  Define--------------------------------------------------------------------*/
/*  Variable------------------------------------------------------------------*/
    float ADflag1,ADflag2,AD_error1,AD_error2;
    uint16 sum_3;
    uint16 kdc;
    uint8 num;
    int steer_inc;
    float position1[4],piancha[4],positiony,ADx1[5],psum,xianzhiflag,positionerror,d,e,f,kpc=1,kdc_1;
    int steer_PWM;
    uint8 zhijiao_flag;
    uint16 temp_serial=100;
    
/*  Function declaration------------------------------------------------------*/
    uint16 abs_jdz(int X);
    
/*  Declare-------------------------------------------------------------------*/
    extern int middle0,left_zhi,right_zhi;
    extern int steer_PWM;
    extern int AD_dif1,AD_sum1,AD_dif2,AD_sum2,AD_flag1,AD_flag2;
    extern uint8 zhijiao_flag;
    extern uint16 right0,right1,left1,left0;
    
float QuZongZhi(float adc_i,float adc_j,float adc_k)
{
    float tmp;
    //取中值
    tmp = adc_i > adc_j ? adc_i : adc_j;          //tmp取两者最大值
    return adc_k > tmp ? tmp : ( adc_k > adc_i ? adc_k : adc_i );
}

uint16 abs_jdz(int X)//设定绝对值
{
    X=X>=0?X:-X;
    
    return X;
}

float jueduizhif(float x) 
{ 
    x=x>=0?x:-x;
    
    return x; 
} 

void ser_ctrl(void)
{    
    int16 str_inc;
    uint16 ser_pwm;
    //for(int i=4;i>0;i--)
    //    position1[i]=position1[i-1]; 
    AD_dif1=(left0+left1)-(right0+right1);//x+y-z-w;
    AD_sum1=left0+right0+left1+right1;  
    ADflag1 = (float)AD_dif1/(float)AD_sum1;  
    LCD_Show_Number(0,3  ,(uint16)ADflag1);
    position1[0] = (int)(ADflag1*(float)temp_serial); 
    if(position1[0]<0)                      
        position1[0]=-position1[0]; 
    if(right0<left0)                   //方向偏移标标志处理
        position1[0]=-position1[0];
    
    str_inc = (int)(0.8*position1[0]);
    //str_inc= ((left0+left1)/2)-((right0+right1)/2);
    //if(left1>left0 && left1>right0 && left1>right1)
    //    str_inc = 520;
    //if(right1>right0 && right1>left0 && right1>left1)
    //    str_inc = 720;
    ser_pwm = 620+str_inc;
    if(ser_pwm>720) ser_pwm=720;
    if(ser_pwm<520) ser_pwm=520;
    ftm_pwm_duty(FTM0,FTM_CH1,ser_pwm);
    
    
    
//    for(int i=4;i>0;i--)
//        position1[i]=position1[i-1]; 
//    for(int i=4;i>0;i--) 
//        piancha[i]=piancha[i-1]; 
//    if((left1+right1)>130) {   
//        num =0;
//#if 1
//    AD_dif1=(left0+left1)-(right0+right1);//x+y-z-w;
//    AD_sum1=left0+left1+right0+right1;  
//    ADflag1  = (float)AD_dif1/(float)AD_sum1;  
//    position1[0] = (int)(ADflag1*(float)110); 
//    if(position1[0]<0)                      
//        position1[0]=-position1[0]; 
//    if(right1<left1)                   //方向偏移标标志处理
//        position1[0]=-position1[0];
//    else if(right1==left1)
//        position1[0]=0;
//#endif
//    }
//    else {  
//        ADx1[1]=abs_jdz(95-left0);
//        ADx1[2]=abs_jdz(95-right0);
//        if((ADx1[1]<7)||(ADx1[2]<7)) {//差都大于10且差的差小于30
//            num=1;
//            ADx1[3]=(ADx1[1]+ADx1[2]);
//            if((ADx1[2]>65)||((ADx1[2]-ADx1[1])>40))
//            ADx1[3]=-ADx1[3];
//        }
//        else if(left1>20&&right1>20&&(abs_jdz(left1-right1)<10)&&(abs_jdz(left0-right0)>20)) {
//            ADx1[3]=ADx1[1]+ADx1[2];
//            if(left0-right0>0)
//              ADx1[3]=-ADx1[3];
//            num = 5;
//        }
//        else if((left0<30)||(right0<30)) {//有一个>30   大范围 
//            // PTB20_T = 1;
//            num=2;
//            ADx1[3]=ADx1[1]+ADx1[2];
//            if((position1[1]<-20)||((ADx1[2]-ADx1[1])>10)||((position1[1]+position1[2]+position1[3])<-30))
//                ADx1[3]=-ADx1[3];
//            if((left0+right0)<40) {
//                num=3;
//                if((position1[1]<-20)||((ADx1[2]-ADx1[1])>5)||((position1[1]+position1[2]+position1[3])<-30))
//                    num=4;
//            }
//        }
//        else if(left0<10||right0<10) {     
//            num = 3;
//            ADx1[3]=ADx1[1]+ADx1[2];
//            if((ADx1[2]-ADx1[1])>25)
//              ADx1[3]=-ADx1[3];
//        }          
//        else { //两个都10~68
//            ADx1[3]=(ADx1[1]-ADx1[2])*2;num =6;
//        }  
//        position1[0]=ADx1[3]/2;
//        if(ADx1[3]<0)
//            ADx1[3]=-ADx1[3];
//    }    
//    LCD_Show_Number(36,6,num);
//    piancha[0] = QuZongZhi(position1[0],position1[1],position1[2]);
//    position1[0] = position1[0]/2+piancha[0]/2;
//    LCD_Show_Number(36,5,abs_jdz(position1[0]/1));
//    positionerror = position1[0]-position1[1]; 
//    kpc=d*position1[0]*position1[0]-e*jueduizhif(position1[0])+f;
//    if(jueduizhif(position1[0])<40)
//        kpc =1;
//    if(num==0) {
//
//        kdc_1=kdc - 25;
//        xianzhiflag = 0.9;
//        // kpc = 1.3;
//    }
//    if((num==1)) {
//
//        xianzhiflag=1.2;
//        kdc_1=kdc;
//    }
//    if((num==2)) {
//
//        xianzhiflag=1.2;
//        kdc_1=kdc;
//    }
//    if((num==3)) {
//
//        xianzhiflag=1; 
//        kdc_1=kdc;
//    }
//    if(num==4) {
//
//        xianzhiflag=1;
//        kdc_1=kdc;
//    }
//    if(num==5) {
//
//        xianzhiflag = 1.5;
//        kdc_1 =kdc+10;
//        zhijiao_flag = 1;
//    }
//    else 
//        zhijiao_flag = 0;
//    if(num==6) {
//        xianzhiflag = 1.5;
//        kdc_1 =kdc+10;
//    }
//    steer_inc  = (int)((kpc*position1[0]+kdc_1*positionerror)*xianzhiflag);    
//    steer_PWM  =620 - steer_inc; 
//    if(steer_PWM > 720)
//        steer_PWM = 720;  
//    if(steer_PWM < 520)    
//        steer_PWM = 520;
//    ftm_pwm_duty(FTM0,FTM_CH1,steer_PWM );
}