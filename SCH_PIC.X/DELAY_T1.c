
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
bit 7:6 δ����: ʼ�ն�Ϊ '0'
bit 5:4 T1CKPS1:T1CKPS0: Timer1 ����ʱ��Ԥ��Ƶѡ��λ
   11 = 1:8 Ԥ��Ƶֵ
   10 = 1:4 Ԥ��Ƶֵ
   01 = 1:2 Ԥ��Ƶֵ
   00 = 1:1 Ԥ��Ƶֵ 
bit 3 T1OSCEN: Timer1 ����ʹ��λ
   1 = ������ʹ��
   0 = �������رա������ķ������ͷ������豻�ضϣ��Խ��͹���
bit 2 T1SYNC: Timer1 �ⲿʱ������ͬ������λ
   ��TMR1CS = 1 ʱ:
   1 = ��ͬ���ⲿʱ��
   0 = ͬ���ⲿʱ��
   �� TMR1CS = 0 ʱ:
   ��λ�����ԡ� TMR1CS = 0 ʱTimer1 ʹ���ڲ�ʱ�ӡ�
bit 1 TMR1CS: Timer1 ʱ��Դѡ��λ
   1 ��ѡ��T1OSO/T1CKI ���ŵ��ⲿʱ�� ( �����ؼ���)
   0 ��ѡ���ڲ�ʱ��(FOSC/4) 
bit 0 TMR1ON: Timer1 ʹ��λ
   1 ��ʹ��Timer1
   0 ���ر� Timer1
   
    * //16Mhz  Fosc/4 0.25us Max: 16ms  Ԥ��Ƶֵ1:2 0.5us Max: 32ms   1:4 1us Max: 65ms    1:8 2us Max: 131ms
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
