//SSD1306 library for STM8S
//   ewgeny7@speccy.su

void LCD_Init(void);
void LCD_Clear(void);
void LCD_Update(void);
void LCD_Chr(uint8_t ch);
void LCD_2xChr(uint8_t ch);
void LCD_FStr(char *dataPtr);
void LCD_2xFStr(char *dataPtr);
void LCD_User(char *udg);
void LCD_GotoXY(uint8_t x,uint8_t y);
void LCD_Picture(unsigned char const *data);



