
#include "DELAY_T1.h"


//#define PRELOAD01  (65536 - (uint16)(OSC_FREQ / (OSC_PER_INST * 8048)))    //2us
#define PRELOAD01  (65536 - (uint16)(OSC_FREQ / (OSC_PER_INST * 1006)))    //0.25us
#define PRELOAD01H (PRELOAD01 / 256)
#define PRELOAD01L (PRELOAD01 % 256)


void Hardware_Delay_1ms_T1(const uint16 N)   
   {
   uint16 ms;
   
   /*
   T1CON: 
bit 7:6 未定义: 始终读为 '0'
bit 5:4 T1CKPS1:T1CKPS0: Timer1 输入时钟预分频选择位
   11 = 1:8 预分频值
   10 = 1:4 预分频值
   01 = 1:2 预分频值
   00 = 1:1 预分频值 
bit 3 T1OSCEN: Timer1 振荡器使能位
   1 = 振荡器被使能
   0 = 振荡器被关闭。振荡器的反相器和反馈电阻被关断，以降低功耗
bit 2 T1SYNC: Timer1 外部时钟输入同步控制位
   当TMR1CS = 1 时:
   1 = 不同步外部时钟
   0 = 同步外部时钟
   当 TMR1CS = 0 时:
   此位被忽略。 TMR1CS = 0 时Timer1 使用内部时钟。
bit 1 TMR1CS: Timer1 时钟源选择位
   1 ：选择T1OSO/T1CKI 引脚的外部时钟 ( 上升沿计数)
   0 ：选择内部时钟(FOSC/4) 
bit 0 TMR1ON: Timer1 使能位
   1 ：使能Timer1
   0 ：关闭 Timer1
   
    * //16Mhz  Fosc/4 0.25us Max: 16ms  预分频值1:2 0.5us Max: 32ms   1:4 1us Max: 65ms    1:8 2us Max: 131ms
 * //           T1CON = 0x00;                   T1CON = 0x10;        T1CON = 0x20;          T1CON = 0x30;
   
   
   */
   
   // Configure Timer 1 as a 16-bit timer  1:1 0.25us Max: 16ms@16Mhz
   T1CON = 0x00;

   TMR1IE = 0;  // No interupts

   // Delay value is *approximately* 1 ms per loop   
   for (ms = N; ms > 0; ms--)
      {                 
      TMR1H = PRELOAD01H;  
      TMR1L = PRELOAD01L;   

      TMR1IF = 0;          // clear overflow flag
      TMR1ON = 1;          // start timer 1

      while (TMR1IF == 0); // Loop until Timer 1 overflows (TF1 == 1)

      TMR1ON = 0;          // Stop Timer 1
      }
   }        
