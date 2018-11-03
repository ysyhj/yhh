/*  ��д�������½ڶ��������������ģ�������һ�ڵ������ȫ��Դ�ڵ��ӷ�����Stone_up��������������������Ƭ����74HC595��̬ɨ��8λ����ܣ�����ϸһ�룬��1Ƭ74HC595��������ܵ�8��COM,������1Ƭ74HC595��������ܵ�8��SEG�����ַ���ʵ������������ر�ʡӲ����Դ����Χ�ĵ�·Ҳ�ǳ��򵥣�Ҳ�ǳ���ʡ��Ƭ����IO�ڣ�����3����Ƭ��IO�ھͿ�������8λ����ܣ���ô����ķ�����֮ǰ ��Ȼû���뵽��ʵ���ǲ������ڴ��ر�������Stone_up���Ĺ��ס�

��2����������
       ��8��������У������ң�������ʾ��12345678����

��3��Ӳ��ԭ��
       �õ�Ƭ����3��IO������2Ƭ������74HC595������1Ƭ74HC595��8������ڷֱ𴮽�470ŷ����֮�����ӵ�����ܵ�SEG���ţ�����1Ƭ74HC595��8������ڷֱ𾭹�NPN�����ܣ�Ҳ������ULN2003A�����֮�����ӵ�����ܵ�COM���š�������������8λ�Ĺ�������ܡ�

��4��Դ���ʺϵĵ�Ƭ��: PIC18f4520,����Ϊ11.0592MHz��

��5��Դ���뽲�����£�
*/

#include<pic18.h>         //����оƬ���ͷ�ļ�
//74HC595��OE����ֱ��Ӳ���ӵ�
# define hc595_sh_dr     LATA3     //74hc595��3������IO֮һ
# define hc595_st_dr     LATA4   //74hc595��3������IO֮һ
# define hc595_ds_dr     LATA5  //74hc595��3������IO֮һ

#define  const_left_com1        0x01   //��ߵ�1������ܵ�com��ʾ
#define  const_left_com2        0x02   //��ߵ�2������ܵ�com��ʾ
#define  const_left_com3        0x04   //��ߵ�3������ܵ�com��ʾ
#define  const_left_com4        0x08   //��ߵ�4������ܵ�com��ʾ
#define  const_left_com5        0x10   //��ߵ�5������ܵ�com��ʾ
#define  const_left_com6        0x20   //��ߵ�6������ܵ�com��ʾ
#define  const_left_com7        0x40   //��ߵ�7������ܵ�com��ʾ
#define  const_left_com8        0x80   //��ߵ�8������ܵ�com��ʾ
#define  const_left_com_off     0x00   //���е�����ܶ�����ʾ

void display_drive();                  //������������򣬷��ڶ�ʱ�ж���
void hc595_drive();                    //����2Ƭ������74hc595���ӳ���,����display_drive()��
void _nop_();                          //����ʱ����ʱ����
void delay1(unsigned int de);

unsigned char number_left1=0;   //��ߵ�1λ�������ʾ������
unsigned char number_left2=0;   //��ߵ�2λ�������ʾ������
unsigned char number_left3=0;   //��ߵ�3λ�������ʾ������
unsigned char number_left4=0;   //��ߵ�4λ�������ʾ������
unsigned char number_left5=0;   //��ߵ�5λ�������ʾ������
unsigned char number_left6=0;   //��ߵ�6λ�������ʾ������
unsigned char number_left7=0;   //��ߵ�7λ�������ʾ������
unsigned char number_left8=0;   //��ߵ�8λ�������ʾ������

unsigned char number_temp[2];  //����������ʾ���ݵ��м����

unsigned char dis_step=1; //ɨ��Ĳ���

const unsigned char number_table[]=  //����ܵ���ģת����
{
0xbe,   //��ʾ"0"
0x06,   //��ʾ"1"
0x7c,   //��ʾ"2"
0x5e,   //��ʾ"3"
0xc6,   //��ʾ"4"
0xda,   //��ʾ"5"
0xfa,   //��ʾ"6"
0x0e,   //��ʾ"7"
0xfe,   //��ʾ"8"
0xde,   //��ʾ"9"
};

//������
main()
{
    ADCON0=0x00;  
    ADCON1=0x0f;                               //ȫ��Ϊ�����ź�
    ADCON2=0xa1;                               //�Ҷ���
    RBPU=0;                                      //��������
    SSPEN=0;                                  

    TRISA3=0;    //74hc595��3������IO֮һ
    TRISA4=0;    //74hc595��3������IO֮һ
    TRISA5=0;    //74hc595��3������IO֮һ

    T1CON=0x24;     //��ʱ���ж�����
    TMR1H=0xFE;
    TMR1L=0xEF;
    TMR1IF=0;
    TMR1IE=1;
    TMR1ON=1;
    TMR1IE=1;


   while(1)    
   {
       CLRWDT(); //ι���Ź�����Ҳ��ù��ȹ�ע����

       number_left1=1;   //��ߵ�1λ�������ʾ"1"
       number_left2=2;   //��ߵ�2λ�������ʾ"2"
       number_left3=3;   //��ߵ�3λ�������ʾ"3"
       number_left4=4;   //��ߵ�4λ�������ʾ"4"
       number_left5=5;   //��ߵ�5λ�������ʾ"5"
       number_left6=6;   //��ߵ�6λ�������ʾ"6"
       number_left7=7;   //��ߵ�7λ�������ʾ"7"
       number_left8=8;   //��ߵ�8λ�������ʾ"8"

              
   }

}

void interrupt timer1rbint(void) //�ж�
{
    if(TMR1IE==1&&TMR1IF==1)    //��ʱ�ж�
    {
       TMR1IF=0;     //��ʱ�жϱ�־λ�ر�
       TMR1ON=0;    //��ʱ�жϿ��عر�

       display_drive(); //�������������

       TMR1H=0xFe;   //�������ö�ʱʱ����
       TMR1L=0x00;
       TMR1ON=1;        //��ʱ�жϿ��ش�
    }
}

void display_drive()                          //������������򣬷��ڶ�ʱ�ж���
{

  switch(dis_step)
   {

   case 1:
      number_temp[0]=const_left_com1;                //���뼴����ʾ����λ����ܵ�COM
      number_temp[1]=number_table[number_left1];     //���뼴����ʾ������
      break;
   case 2: 
      number_temp[0]=const_left_com2;                //���뼴����ʾ����λ����ܵ�COM
      number_temp[1]=number_table[number_left2];     //���뼴����ʾ������
      break;
   case 3:
      number_temp[0]=const_left_com3;                //���뼴����ʾ����λ����ܵ�COM
      number_temp[1]=number_table[number_left3];     //���뼴����ʾ������
      break;
   case 4: 
      number_temp[0]=const_left_com4;                //���뼴����ʾ����λ����ܵ�COM
      number_temp[1]=number_table[number_left4];     //���뼴����ʾ������
      break;
  case 5:
      number_temp[0]=const_left_com5;                //���뼴����ʾ����λ����ܵ�COM
      number_temp[1]=number_table[number_left5];     //���뼴����ʾ������
      break;
  case 6:
      number_temp[0]=const_left_com6;                //���뼴����ʾ����λ����ܵ�COM
      number_temp[1]=number_table[number_left6];     //���뼴����ʾ������
      break;
  case 7:
      number_temp[0]=const_left_com7;                //���뼴����ʾ����λ����ܵ�COM
      number_temp[1]=number_table[number_left7];     //���뼴����ʾ������
      break;
  case 8:   
      number_temp[0]=const_left_com8;                //���뼴����ʾ����λ����ܵ�COM
      number_temp[1]=number_table[number_left8];     //���뼴����ʾ������
      break;
   }
   hc595_drive();     //����2Ƭ������74hc595���ӳ���

   delay1(15); //ÿһλ�������ʾ��ͣ��ʱ��


   number_temp[0]=number_table[const_left_com_off];  //�����е�����ܶ�����ʾ������ʾЧ������
   number_temp[1]=0x00;                              //���뼴����ʾ�Ŀ�����
   hc595_drive();     //����2Ƭ������74hc595���ӳ���

   ++dis_step;   //������һ������ܵ�ɨ��
   if(dis_step>8)
   {
      dis_step=1;
   } 

}

void hc595_drive()     //����2Ƭ������74hc595���ӳ���
{
   unsigned char tempdata; //ÿ���ֽڵ�ÿһλ,��8λ
   unsigned char com_select;  //�м����
   unsigned char tube_cnt; //����ܸ�������2��

   hc595_sh_dr=0;
   hc595_st_dr=0;
   for(tube_cnt=2;tube_cnt!=0;tube_cnt--)  //2��74hc595
   {
      com_select=number_temp[tube_cnt-1];
      for(tempdata=0;tempdata<8;tempdata++)  //ÿ��8λ
      { 
         CLRWDT();
         if(com_select>=0x80)hc595_ds_dr=1;
         else hc595_ds_dr=0;
         hc595_sh_dr=0;
         _nop_();
         _nop_();
         hc595_sh_dr=1;
         _nop_();
         _nop_();
         com_select<<=1;
      }
   }

   hc595_st_dr=0;
   _nop_();
   _nop_();
   hc595_st_dr=1;
   _nop_();
   _nop_();
   hc595_sh_dr=0;    //���ͣ������ž���ǿ
   hc595_st_dr=0;
   hc595_ds_dr=0;

}

void _nop_()         //����ʱ����ʱ����
{
  unsigned char n;
  for(n=0;n<0x0f;n++);
}

void delay1(unsigned int de)
{
        unsigned int t;
        for(t=0;t<de;t++);
}