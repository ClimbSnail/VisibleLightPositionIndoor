#ifndef __LQ12864_H__
#define __LQ12864_H__



#define LCD_DC	PAout(3)  //F0
#define LCD_RST	PAout(2)  //F2
#define LCD_SDA	PAout(1) //F4
#define LCD_SCL	PAout(0)  //F6

#define X_WIDTH 128
#define Y_WIDTH 64

void LCD_Init(void);
void LCD_CLS(void);
void LCD_mDLY_ms(unsigned int ms);
void LCD_WrCmd(unsigned char cmd);
void LCD_Fill(unsigned char bmp_data);
void LCD_Set_Pos(unsigned char x, unsigned char y);
void LCD_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[]);
void LCD_P8x16Str(unsigned char x,unsigned char y,unsigned char ch[]);
void LCD_P14x16Str(unsigned char x,unsigned char y,unsigned char ch[]);
void LCD_Print(unsigned char x, unsigned char y, unsigned char ch[]);
void LCD_PutPixel(unsigned char x,unsigned char y);
void LCD_Rectangle(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char gif);
void Draw_LQLogo(void);
void Draw_LibLogo(void);
void Draw_BMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char * bmp); 
void Draw_Road(void);
void LCD_PrintU16(unsigned char x,unsigned char y,unsigned int num);

#endif

