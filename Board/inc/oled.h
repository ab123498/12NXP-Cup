#ifndef	_OLED_H_
#define _OLED_H_

#define RESET_cfg  PTC12
#define DC_cfg     PTC10
#define D1_cfg     PTC14
#define D0_cfg     PTC16

#define RESET  PTC12_OUT
#define DC     PTC10_OUT
#define D1     PTC14_OUT
#define D0     PTC16_OUT


 //extern uint8 beyond96x64[512];
 void LCD_WrDat(uint8 data);
 void LCD_Set_Pos(uint8 x, uint8 y);
 void LCD_Init(void);
 void LCD_CLS(void);
 void LCD_P6x8Str(uint8 x,uint8 y,uint8 ch[]);//tested
 void LCD_P8x16Str(uint8 x,uint8 y,uint8 ch[]);
 void LCD_P14x16Str(uint8 x,uint8 y,uint8 ch[]);
 void LCD_Print(uint8 x, uint8 y, uint8 ch[]);
 void LCD_PutPixel(uint8 x,uint8 y);
 void LCD_Rectangle(uint8 x1,uint8 y1,uint8 x2,uint8 y2,uint8 gif);
 void Draw_BMP(uint8 x0,uint8 y0,uint8 x1,uint8 y1,uint8 bmp[]); 
 void LCD_Fill(uint8 dat);
 void Display(int16 i,uint8 x,uint8 y);
 void DisplayFloat(int16 i,uint8 x,uint8 y);
 void LCD_write_char(uint8 X, uint8 Y,uint8 c);
 void LCD_Show_Number (uint8 X,uint8 Y,uint32 number);
#endif