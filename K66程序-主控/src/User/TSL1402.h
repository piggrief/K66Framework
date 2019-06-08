# ifndef _TSL1402_H_
# define _TSL1402_H_

# define TSL1401_DebugSerials UART_3

void TSL1401_GetLine(unsigned char *pixel);
void TSL1401_Init();
void TSL1401_SendLine(uint8 *LineImage);
int CheckRoadBlock(uint8 * image);
void LCD_ShowOneLineImage(uint8 * image);

void lookline(uint8 * image);
float CalPiancha();
#endif