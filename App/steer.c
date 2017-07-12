/*  Include-------------------------------------------------------------------*/
    #include "common.h"
    #include "include.h"
    #include "steer.h"
    #include <math.h>
/*  Define--------------------------------------------------------------------*/
    #define MAXSPEED 20
/*  Variable------------------------------------------------------------------*/
    float d = DX,e = EX,f = FX,\
          position[ADEEP],positionerror,kpc,kdc_1=DS ;
    int offset;
    int steer_inc,steer_PWM,speed_ctl_output;
    uint16 real_position_num,s_position[ADEEP],dlyt=180,set_cirt=CIRT;
    uint16 steer_plus=SP,AD_delay=0;//差和比系数
    int speed_ctl_output_close;
    static int chain_speed;
/*  Function declaration------------------------------------------------------*/
    void bell_set(PTXn_e bell,uint8 data);
/*  Declare-------------------------------------------------------------------*/
    extern uint16 right1,right0,right2,left2,left0,left1,middle;
    extern uint16 position_num;//差和比循环队列的头部
    extern Dtype user_flag;
/*  Function -----------------------------------------------------------------*/
    void position_measure(void);

uint16 abs_jdz(int X)//设定绝对值
{
    X=X>=0?X:-X; 
    return X;
}

void quicksort(uint16 arr[], uint16 Length)
{
    uint16 temp = 0;
    for (int j = 0; j < Length - 1 ; j++) {
        if (arr[j] > arr[j + 1]) {
            temp = arr[j + 1];
            arr[j + 1] = arr[j];
            arr[j] = temp;
        }
    }
}

void ser_ctrl(void)
{    
    int16 str_inc;//最后舵机中值
    
    static uint16 ser_pwm;
    static uint16 circle_time;
        
    float real_position;
    
    static uint16 temp;
    static uint16 loop_num;
    
    real_position_num = (position_num+ADEEP-AD_delay) % ADEEP;

    position_measure();
    
    user_flag.b10 = \
        ((float)left0/(float)right0>0.73 && (float)right0/(float)left0<1.36  &&\
         (float)left0/(float)right0<=1    && (float)right0/(float)left0>1   ) ||\
        ((float)left0/(float)right0<1.36 && (float)right0/(float)left0>0.73  &&\
         (float)left0/(float)right0>1    && (float)right0/(float)left0<=1   );
    
    user_flag.b11 = \
        left0<(L0R0LOW)&&right0<(L0R0LOW);
    
    user_flag.b12 = \
        left2<L2R2TOP && right2<L2R2TOP && left2>L2R2BOT && right2>L2R2BOT;
    
    if(user_flag.W[0] >= 0x0400 && !user_flag.b6 ) {
        //char *temp;
        //int i; 
        //for(i=2;i<=5;i++) {
        //    char out; 
        //    out = user_flag.B[1]>>i;
        //    sprintf(temp++,"%d",out & 0x01);
        //}
        //printf("%s",temp-4);
        //printf("0x%X,",user_flag.B[1]);
    }
        
    if( user_flag.b10 && user_flag.b11 && user_flag.b12 && !user_flag.b6 ) {
        AD_delay = ADELAY;
        circle_time =set_cirt;
        printf("666\n");
        user_flag.b6 = 1;
        user_flag.b19 = 1;
        chain_speed = speed_ctl_output;
        speed_ctl_output = 13;
        switch(loop_num) {
            case 0:
                user_flag.b17 = user_flag.b13;
                break;
            case 1:
                user_flag.b17 = user_flag.b14;
                break;
            case 2:
                user_flag.b17 = user_flag.b15;
                break;
            case 3:
                user_flag.b17 = user_flag.b16;
                break;
        }
        if(user_flag.b17) offset = 480; else offset = 720;
    }

    if(circle_time>0) {
        if( --circle_time == 5 ) {
            AD_delay=0;
            user_flag.b6 = 0;
            speed_ctl_output = speed_ctl_output_close;
            if(loop_num++ > 4) loop_num=0;
            printf("lp_nm:%d\n",loop_num);
        }
        if((set_cirt-circle_time) == DLYT) user_flag.b19 = 0;
        if(right0>OUTC||left0>OUTC) { 
            AD_delay = 0; 
            speed_ctl_output = speed_ctl_output_close;
        }
    }
    
    real_position = position[real_position_num];
    //if(user_flag.b6) { d=0.0009;e=0.08;f=0.4; kdc_1 = 10;}
    kpc=d*real_position*real_position+e*fabsf(real_position)\
        +f;//很明显加这个动态p会乱抖
    
    positionerror = real_position\
        -position[(real_position_num - 1) % ADEEP];
    
    
    str_inc = (int)(kpc*real_position+kdc_1*positionerror);
#if 0
    if(position_num<ADEEP&&temp%6==0) {
        static float Outdata[8];
        Outdata[0] = position[position_num];
        Outdata[1] = position[real_position_num];
        Outdata[2] = left1;
        Outdata[3] = left0;
        Outdata[4] = right0;
        Outdata[5] = right1;
        Outdata[6] = left2;
        Outdata[7] = middle;   
        vcan_sendware((uint8_t *)Outdata,sizeof(Outdata));
        user_flag.b4 =0;
    }
#endif
/*  速度策略--------------------------------------------------------------BEG*/
    //speed_ctl_output = (s_position[ADEEP-1]/15)+12;
    //if(speed_ctl_output>18) speed_ctl_output=18;
    //else if(speed_ctl_output<0) speed_ctl_output=0;
    
/*  丢线停车--------------------------------------------------------------BEG*/
    if((left1==0)&&(left0==0)&&(right1==0)&&(right0==0)) {
        if(!user_flag.b6) speed_ctl_output=13;
        user_flag.b7 = 1;
        //str_inc=0;
    }
    else {
        if(!user_flag.b8 && !user_flag.b6) speed_ctl_output = speed_ctl_output_close;
        user_flag.b7 = 0;
    }
    
/*  舵机输出--------------------------------------------------------------END*/    
    ser_pwm = 620-str_inc;
    
    if(ser_pwm>STEERH) {   ser_pwm=STEERH;    bell_set(BELLPORT,BELLON); } else bell_set(BELLPORT,BELLOFF);
    if(ser_pwm<STEERL) {   ser_pwm=STEERL;    bell_set(BELLPORT,BELLON); } else bell_set(BELLPORT,BELLOFF);
    
    if(user_flag.b19) ser_pwm = offset;
    ftm_pwm_duty(STEERFTM,STEERFTM_CH,ser_pwm);   
    
    if(temp++ ==6*ADEEP) {
        //quicksort(s_position,ADEEP);
        temp=0;
        //printf("L/R%d\n",(int)(1000*(float)left0/(float)right0));
        //printf("R/L%d\n",(int)(1000*(float)right0/(float)left0));
        //printf("l/r%d\n",(int)(1000*(float)left1/(float)right1));
        //printf("r/l%d\n\n",(int)(1000*(float)right1/(float)left1));
        //printf("l2%d\n",left2);
    }
}

void position_measure(void)
{
    int32 AD_dif,AD_sum;
    float ADflag;
    AD_dif=(left0+left1)-(right0+right1);//左减右
    AD_sum=left0+right0+left1+right1;
    //if(user_flag.b6) {
    //    if(user_flag.b17) {
    //        AD_dif=middle-right1;//右减中
    //        AD_sum=middle+right1;
    //    }
    //    else {
    //        AD_dif=left1-middle;//左减中
    //        AD_sum=left1+middle;
    //    }
    //}
    ADflag = (float)AD_dif/(float)AD_sum;
    position[position_num] = (int)(ADflag*(float)steer_plus);
    //s_position[position_num] = (uint16)position[position_num];
    if(position[position_num] > (steer_plus-2) || \
       position[position_num] < (-steer_plus+2) )  
       position[position_num]= position[(position_num + ADEEP - 1) % ADEEP];//防止丢线阿
}
