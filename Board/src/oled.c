#include  "common.h"
#include "oled.h"
#include "oled_font.h"
#include "MK60_gpio.h"

#define XLevelL		0x00
#define XLevelH		0x10
#define XLevel		((XLevelH&0x0F)*16+XLevelL)
#define Max_Column	128
#define Max_Row		  64
#define	Brightness	0xCF 

#define X_WIDTH 128
#define Y_WIDTH 64
//======================================

const uint8 F6x8[][6] = {
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // sp
	{ 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
	{ 0x00, 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
	{ 0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
	{ 0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
	{ 0x00, 0x62, 0x64, 0x08, 0x13, 0x23 },   // %
	{ 0x00, 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
	{ 0x00, 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
	{ 0x00, 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
	{ 0x00, 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
	{ 0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *
	{ 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
	{ 0x00, 0x00, 0x00, 0xA0, 0x60, 0x00 },   // ,
	{ 0x00, 0x08, 0x08, 0x08, 0x08, 0x08 },   // -
	{ 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
	{ 0x00, 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
	{ 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
	{ 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
	{ 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
	{ 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
	{ 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
	{ 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
	{ 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
	{ 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
	{ 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
	{ 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
	{ 0x00, 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
	{ 0x00, 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
	{ 0x00, 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
	{ 0x00, 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
	{ 0x00, 0x00, 0x41, 0x22, 0x14, 0x08 },   // >
	{ 0x00, 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
	{ 0x00, 0x32, 0x49, 0x59, 0x51, 0x3E },   // @
	{ 0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C },   // A
	{ 0x00, 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
	{ 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
	{ 0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
	{ 0x00, 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
	{ 0x00, 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
	{ 0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
	{ 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H
	{ 0x00, 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
	{ 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
	{ 0x00, 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
	{ 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
	{ 0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
	{ 0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
	{ 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
	{ 0x00, 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
	{ 0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q
	{ 0x00, 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R
	{ 0x00, 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
	{ 0x00, 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
	{ 0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
	{ 0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
	{ 0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
	{ 0x00, 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
	{ 0x00, 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
	{ 0x00, 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
	{ 0x00, 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [
	{ 0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55 },   // 55
	{ 0x00, 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
	{ 0x00, 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
	{ 0x00, 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
	{ 0x00, 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
	{ 0x00, 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
	{ 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
	{ 0x00, 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
	{ 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
	{ 0x00, 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
	{ 0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
	{ 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C },   // g
	{ 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
	{ 0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
	{ 0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 },   // j
	{ 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
	{ 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
	{ 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
	{ 0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
	{ 0x00, 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
	{ 0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 },   // p
	{ 0x00, 0x18, 0x24, 0x24, 0x18, 0xFC },   // q
	{ 0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
	{ 0x00, 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
	{ 0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
	{ 0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
	{ 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
	{ 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
	{ 0x00, 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
	{ 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C },   // y
	{ 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z
	{ 0x14, 0x14, 0x14, 0x14, 0x14, 0x14 }    // horiz lines
};

__RAMFUNC void LCD_WrDat(uint8 data)
{
    uint8 i=8;	
    DC=1;
    asm("nop");
    asm("nop"); 
    asm("nop"); 
    asm("nop"); 
    D0=0;
    asm("nop");
    asm("nop"); 
    asm("nop"); 
    asm("nop");
    while(i--) {
        if(data&0x80) {
            D1=1;
        }
        else {
            D1=0;
        }
        D0=1;
        asm("nop");
        asm("nop"); 
        asm("nop"); 
        asm("nop"); 
        D0=0;  
        data<<=1;    
    }
}
__RAMFUNC void LCD_WrCmd(uint8 cmd)
{
    uint8 i=8;
    DC=0;
    asm("nop");
    asm("nop"); 
    asm("nop"); 
    asm("nop");
    D0=0;
    asm("nop");
    asm("nop"); 
    asm("nop"); 
    asm("nop");
    while(i--) {
        if(cmd&0x80) {
            D1=1;
        }
        else {
            D1=0; 
        }
        D0=1;
        asm("nop");
        asm("nop"); 
        asm("nop"); 
        asm("nop"); 
        D0=0;  
        cmd<<=1;   
    } 	
}
void LCD_Set_Pos(uint8 x, uint8 y)
{ 
    LCD_WrCmd(0xb0+y);
    LCD_WrCmd(((x&0xf0)>>4)|0x10);
    LCD_WrCmd(x&0x0f);//|0x01);
} 
void LCD_Fill(uint8 bmp_data)
{
    uint8 y,x;

    for(y=0;y<8;y++) {
        LCD_WrCmd(0xb0+y);
        LCD_WrCmd(0x01);
        LCD_WrCmd(0x10);
        for(x=0;x<X_WIDTH;x++)
            LCD_WrDat(bmp_data);
    }
}
void LCD_CLS(void)
{
    uint8 y,x;	
    for(y=0;y<8;y++) {
        LCD_WrCmd(0xb0+y);
        LCD_WrCmd(0x01);
        LCD_WrCmd(0x10); 
        for(x=0;x<X_WIDTH;x++)
            LCD_WrDat(0);
    }
}
void LCD_DLY_ms(uint32 ms)
{                         
    uint32 a;
    while(ms) {
        a=133500;
        while(a--);
        ms--;
    }
    return;
}

void LCD_Init(void)        
{
    gpio_init (RESET_cfg,GPO,0);
    gpio_init (DC_cfg   ,GPO,0);
    gpio_init (D1_cfg   ,GPO,1);
    gpio_init (D0_cfg   ,GPO,0);
    RESET=0;
    LCD_DLY_ms(50);
    RESET=1;

    LCD_WrCmd(0xae);//--turn off oled panel
    LCD_WrCmd(0x00);//---set low column address
    LCD_WrCmd(0x10);//---set high column address
    LCD_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    LCD_WrCmd(0x81);//--set contrast control register
    LCD_WrCmd(0xcf); // Set SEG Output Current Brightness
    LCD_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    LCD_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    LCD_WrCmd(0xa6);//--set normal display
    LCD_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
    LCD_WrCmd(0x3f);//--1/64 duty
    LCD_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    LCD_WrCmd(0x00);//-not offset
    LCD_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
    LCD_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
    LCD_WrCmd(0xd9);//--set pre-charge period
    LCD_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    LCD_WrCmd(0xda);//--set com pins hardware configuration
    LCD_WrCmd(0x12);
    LCD_WrCmd(0xdb);//--set vcomh
    LCD_WrCmd(0x40);//Set VCOM Deselect Level
    LCD_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
    LCD_WrCmd(0x02);//
    LCD_WrCmd(0x8d);//--set Charge Pump enable/disable
    LCD_WrCmd(0x14);//--set(0x10) disable
    LCD_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
    LCD_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
    LCD_WrCmd(0xaf);//--turn on oled panel
    LCD_Fill(0x00);  //初始清屏
    LCD_Set_Pos(0,0);  
} 
//==============================================================
//函数名： void LCD_PutPixel(uint8 x,uint8 y)
//功能描述：绘制一个点（x,y）
//参数：真实坐标值(x,y),x的范围0～127，y的范围0～64
//返回：无
//==============================================================
void LCD_PutPixel(uint8 x,uint8 y)
{
    uint8 data1;  //data1当前点的数据 

    LCD_Set_Pos(x,y); 
    data1 = 0x01<<(y%8); 	
    LCD_WrCmd(0xb0+(y>>3));
    LCD_WrCmd(((x&0xf0)>>4)|0x10);
    LCD_WrCmd((x&0x0f)|0x00);
    LCD_WrDat(data1); 	 	
}
//==============================================================
//函数名： void LCD_Rectangle(uint8 x1,uint8 y1,
//                   uint8 x2,uint8 y2,uint8 color,uint8 gif)
//功能描述：绘制一个实心矩形
//参数：左上角坐标（x1,y1）,右下角坐标（x2，y2）
//      其中x1、x2的范围0～127，y1，y2的范围0～63，即真实坐标值
//返回：无
//==============================================================
void LCD_Rectangle(uint8 x1,uint8 y1,uint8 x2,uint8 y2,uint8 gif)
{
    uint8 n; 

    LCD_Set_Pos(x1,y1>>3);
    for(n=x1;n<=x2;n++) {
        LCD_WrDat(0x01<<(y1%8)); 			
        if(gif == 1) 	
            LCD_DLY_ms(50);
    }  
    LCD_Set_Pos(x1,y2>>3);
    for(n=x1;n<=x2;n++) {
        LCD_WrDat(0x01<<(y2%8)); 			
        if(gif == 1) 	
            LCD_DLY_ms(5);
    }
}  
//==============================================================
//函数名：LCD_P6x8Str(uint8 x,uint8 y,uint8 *p)
//功能描述：写入一组标准ASCII字符串
//参数：显示的位置（x,y），y为页范围0～7，要显示的字符串
//返回：无
//==============================================================  
void LCD_P6x8Str(uint8 x,uint8 y,uint8 ch[])
{
    uint8 c=0,i=0,j=0;      
    while (ch[j]!='\0') {    
        c =ch[j]-32;
        if(x>126)
            {x=0;y++;}
        LCD_Set_Pos(x,y);    
        for(i=0;i<6;i++)     
            LCD_WrDat(F6x8[c][i]);  
            x+=6;
        j++;
    }
}
//==============================================================
//函数名：LCD_P8x16Str(uint8 x,uint8 y,uint8 *p)
//功能描述：写入一组标准ASCII字符串
//参数：显示的位置（x,y），y为页范围0～7，要显示的字符串
//返回：无
//==============================================================  
void LCD_P8x16Str(uint8 x,uint8 y,uint8 ch[])
{
    uint8 c=0,i=0,j=0;

    while (ch[j]!='\0') {    
        c =ch[j]-32;
        if(x>120){x=0;y++;}
            LCD_Set_Pos(x,y);    
        for(i=0;i<8;i++)     
            LCD_WrDat(F8X16[c*16+i]);
        LCD_Set_Pos(x,y+1);    
        for(i=0;i<8;i++)     
            LCD_WrDat(F8X16[c*16+i+8]);  
        x+=8;
        j++;
    }
}
//输出汉字字符串
void LCD_P14x16Str(uint8 x,uint8 y,uint8 ch[])
{
	uint8 wm=0,ii = 0;
	uint32 adder=1; 
	
	while(ch[ii] != '\0') {
        wm = 0;
        adder = 1;
        while(F14x16_Idx[wm] > 127) {
            if(F14x16_Idx[wm] == ch[ii]) {
                if(F14x16_Idx[wm + 1] == ch[ii + 1]) {
                    adder = wm * 14;
                    break;
                }
            }
            wm += 2;			
        }
        if(x>118) {
            x=0;
            y++;
        }
            LCD_Set_Pos(x , y); 
        if(adder != 1) {// 显示汉字	
            LCD_Set_Pos(x , y);				
            for(wm = 0;wm < 14;wm++) {
                LCD_WrDat(F14x16[adder]);	
                adder += 1;
            }      
            LCD_Set_Pos(x,y + 1); 
            for(wm = 0;wm < 14;wm++) {
                LCD_WrDat(F14x16[adder]);
                adder += 1;
            }   		
        }
        else {//显示空白字符
            ii += 1;
            LCD_Set_Pos(x,y);
            for(wm = 0;wm < 16;wm++)
                LCD_WrDat(0);
            LCD_Set_Pos(x,y + 1);
            for(wm = 0;wm < 16;wm++)  		
                LCD_WrDat(0);	
        }
        x += 14;
        ii += 2;
	}
}
//输出汉字和字符混合字符串
void LCD_Print(uint8 x, uint8 y, uint8 ch[])
{
    uint8 ch2[3];
    uint8 ii=0;        
    while(ch[ii] != '\0') {
        if(ch[ii] > 127) {
            ch2[0] = ch[ii];
            ch2[1] = ch[ii + 1];
            ch2[2] = '\0';			//汉字为两个字节
            LCD_P14x16Str(x , y, ch2);	//显示汉字
            x += 14;
            ii += 2;
        }
        else {
            ch2[0] = ch[ii];	
            ch2[1] = '\0';			//字母占一个字节
            LCD_P8x16Str(x , y , ch2);	//显示字母
            x += 8;
            ii+= 1;
        }
    }
} 

//==============================================================
//函数名： void Draw_BMP(uint8 x,uint8 y)
//功能描述：显示BMP图片128×64
//参数：起始点坐标(x,y),x的范围0～127，y为页的范围0～7
//返回：无
//==============================================================
void Draw_BMP(uint8 x0,uint8 y0,uint8 x1,uint8 y1,uint8 bmp[])
{ 	
    uint32 ii=0;
    uint8 x,y;

    if(y1%8==0) 
        y=y1/8;      
    else 
        y=y1/8+1;
    for(y=y0;y<=y1;y++) {
        LCD_Set_Pos(x0,y);				
        for(x=x0;x<=x1;x++) {      
            LCD_WrDat(bmp[ii++]);	    	
        }
    }
}
//==============================================================
//函数名：void Display(int16 i,uint8 x,uint8 y)
//功能描述：显示动态数字
//参数：起始点坐标(x,y),x的范围0～127，y为页的范围0～7
//返回：无
//==============================================================
void Display(int16 i,uint8 x,uint8 y)
{
    int8 a[6]={0}; 
    if(i<0) {
        a[0]='-';
        i=-i;
    }
    else
        a[0]='+';
    a[1]=i/10000;
    a[2]=i%10000/1000;
    a[3]=i%1000/100;
    a[4]=i%100/10;
    a[5]=i%10;
    for(uint8 j=0;j<6;j++) {
        switch(a[j]) {
            case 0:{LCD_Print(x,y,"0");}break;
            case 1:{LCD_Print(x,y,"1");}break;
            case 2:{LCD_Print(x,y,"2");}break;
            case 3:{LCD_Print(x,y,"3");}break;
            case 4:{LCD_Print(x,y,"4");}break;
            case 5:{LCD_Print(x,y,"5");}break;
            case 6:{LCD_Print(x,y,"6");}break;
            case 7:{LCD_Print(x,y,"7");}break;
            case 8:{LCD_Print(x,y,"8");}break;
            case 9:{LCD_Print(x,y,"9");}break; 
            case '-':{LCD_Print(x,y,"-");}break;
            case '+':{LCD_Print(x,y,"+");}break;
            default:break;
        }
        x+=10;
        if(x==80)
            x=20;
    }
}
//==============================================================
//函数名：void Display(int16 i,uint8 x,uint8 y)
//功能描述：显示动态数字6*8
//参数：起始点坐标(x,y),x的范围0～127，y为页的范围0～7
//返回：无
//==============================================================
void LCD_P6x8Str_Num(int16 i,uint8 x,uint8 y)
{  
    int8 a[5]={0}; 
    if(i<0) {
        a[0]='-';
        i=-i;
    }
    else
        a[0]='+';
    a[1]=i/1000;
    a[2]=i%1000/100;
    a[3]=i%100/10;
    a[4]=i%10;
    for(uint8 j=0;j<5;j++) {
        switch(a[j]) {
            case 0:LCD_P6x8Str(x,y,"0");break;
            case 1:LCD_P6x8Str(x,y,"1");break;
            case 2:LCD_P6x8Str(x,y,"2");break;
            case 3:LCD_P6x8Str(x,y,"3");break; 
            case 4:LCD_P6x8Str(x,y,"4");break;
            case 5:LCD_P6x8Str(x,y,"5");break;
            case 6:LCD_P6x8Str(x,y,"6");break;
            case 7:LCD_P6x8Str(x,y,"7");break;           
            case 8:LCD_P6x8Str(x,y,"8");break;
            case 9:LCD_P6x8Str(x,y,"9");break;  
            case '-':LCD_P6x8Str(x,y,"-");break;
            case '+':LCD_P6x8Str(x,y,"+");break;
            default:break;
        }
        x+=6;
        if(x==80)
            x=20;
    }
} 

//==============================================================
//函数名：void DisplayFloat(int16 i,uint8 x,uint8 y)
//功能描述：显示动态数字
//参数：起始点坐标(x,y),x的范围0～127，y为页的范围0～7
//返回：无
//==============================================================
void DisplayFloat(int16 i,uint8 x,uint8 y)
{
      int8 a[6]={0}; 
      if(i<0)
      {
        a[0]='-';
      i=-i;
      }
      else
        a[0]='+';
      a[1]=i/10000;
      a[2]=i%10000/1000;
      a[3]='.';
      a[4]=i%1000/100;
      a[5]=i%100/10;
      for(uint8 j=0;j<6;j++)
      {
         switch(a[j])
         {
           case 0:{LCD_Print(x,y,"0");}break;
           case 1:{LCD_Print(x,y,"1");}break;
           case 2:{LCD_Print(x,y,"2");}break;
           case 3:{LCD_Print(x,y,"3");}break;
           case 4:{LCD_Print(x,y,"4");}break;
           case 5:{LCD_Print(x,y,"5");}break;
           case 6:{LCD_Print(x,y,"6");}break;
           case 7:{LCD_Print(x,y,"7");}break;
           case 8:{LCD_Print(x,y,"8");}break;
           case 9:{LCD_Print(x,y,"9");}break; 
         case '-':{LCD_Print(x,y,"-");}break;
         case '+':{LCD_Print(x,y,"+");}break;
         case '.':{LCD_Print(x,y,".");}break;
           default:break;
          }
         x+=10;
         if(x==90)
           x=20;
         }
}
 
void LCD_write_char(uint8 X, uint8 Y,uint8 c)
{
 	 
    uint8 line;
	LCD_Set_Pos(X,Y);
    c -= 32;
    for (line=0; line<6; line++)
	{
        LCD_WrDat(F6x8[c][line]);
	}
}


void LCD_Show_Number (uint8 X,uint8 Y,uint32 number)  
{
  //LCD_write_char(X,Y,number/1000+48);
  //LCD_write_char(X-6,Y,number%100000/10000+48);
  LCD_write_char(X,Y,number%10000/1000+48);
  LCD_write_char(X+6,Y,number%1000/100+48);
  LCD_write_char(X+12,Y,number%100/10+48);
  LCD_write_char(X+18,Y,number%10+48);
 // LCD_write_char(X+24,Y,number%1+48);
}


