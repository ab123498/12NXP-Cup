#ifndef	_OLED_H_
#define _OLED_H_

#define RESET_cfg  PTC17
#define DC_cfg     PTC16
#define D1_cfg     PTC18
#define D0_cfg     PTC19

#define RESET  PTC17_OUT
#define DC     PTC16_OUT
#define D1     PTC18_OUT
#define D0     PTC19_OUT
//OLED:GND VCC D0 D1 RES DC CS

//extern uint8 beyond96x64[512];
void LCD_DLY_ms(uint32 ms);
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