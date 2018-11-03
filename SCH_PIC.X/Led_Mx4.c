
#include "Led_Mx4.h"


uchar8 Led_Mx4_Data_G[LED_NUM_DIGITS] = {0x40,0x40,0x40,0x40}; //0.0.0.0.

static uchar8 Digit_G = 0; //扫描的步骤

void Led_MX4_Display_Update(void)	// 数码管驱动程序
{
    
    LED_DATA_PORT = 0xFF;//消影

   switch (Digit_G)
      {
      case 0:
         {      
         LED_DIGIT_1 = ON;
         LED_DIGIT_2 = OFF;
         LED_DIGIT_3 = OFF;
         LED_DIGIT_4 = OFF;
         break;
         }

      case 1:
         {      
         LED_DIGIT_1 = OFF;
         LED_DIGIT_2 = ON;
         LED_DIGIT_3 = OFF;
         LED_DIGIT_4 = OFF;
         break;
         }

      case 2:
         {      
         LED_DIGIT_1 = OFF;
         LED_DIGIT_2 = OFF;
         LED_DIGIT_3 = ON;
         LED_DIGIT_4 = OFF;
         break;
         }

      case 3:
         {      
         LED_DIGIT_1 = OFF;
         LED_DIGIT_2 = OFF;
         LED_DIGIT_3 = OFF;
         LED_DIGIT_4 = ON;
         break;
         } 
      default:
         {
         LED_DIGIT_1 = OFF;
         LED_DIGIT_2 = OFF;
         LED_DIGIT_3 = OFF;
         LED_DIGIT_4 = OFF;
         break;
         }
 
      }

   LED_DATA_PORT = Led_Mx4_Data_G[Digit_G];

      if (++Digit_G == LED_NUM_DIGITS)
      {
      Digit_G = 0;
      }
            
    }
