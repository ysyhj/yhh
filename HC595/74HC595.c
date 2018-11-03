/*          ��̬ɨ������ܣ���ָÿ��һ��ʱ�䵥Ƭ�����뼰ʱȥɨ������ܣ������������ʾ�Ͳ���������Ƭ�����ۣ�����û��ʱ����Ϣ���о�һֱ�����˶���״̬����˽ж�̬ɨ�衣��̬���������ǡǡ�෴����ָ��Ƭ����Ҫ������ʾ���ݵ�ʱ��Ż�ȥɨ��һ������ܣ�ƽʱ�����ά�ֵ�ǰ��ʾ�����ݡ��ٸ����ӣ�һ�ҹ�˾��һ��ȫְ����ʦ������һ��������û����Ŀ������ÿ���¶�Ҫ��ʱ���������ʣ�����ͽж�̬�ġ�������һ�ҹ�˾���翪����Ƭ����Ŀ��ֻ�еȺ�����Ŀ�������ˣ����Ż���ҿ����ѣ�����ͽ�����̬�ġ������ԣ���̬��������������Ƚϴ󣬾�̬��������������Ƚ�С�������������������ѡ��̬�����ġ�������Ϊʲô����ô�����ڵ���С��ҵϲ����������Ϊ����Ǿ�̬�ģ�����ÿ���¸��ҹ��ʣ�����VIP�ͻ����⡣��74HC595��̬��������ܣ��ڶ����ô��������������Ƚϴ�����������������������ܵ�ʱ���þ�̬�����ķ�ʽ�����õ��������������������⡣�������ô����ǵ�Ƭ��ʡIO�ڣ���3��IO�ھͿ�����������9������ܡ�����õ��ǹ�������ܣ����õ��ڲ�������7�������ܵ�ULN2003AоƬ��������ǿ����������
       ��ס��������ĳ������У����Ǿ�̬������ʾ�ģ��͸�Һ������ʾһ��������һ��_update������ʾ��������������led_update��
��2��        ��������
��9��������У������ң�������ʾ��123456789����
��3��Ӳ��ԭ��
������ʾ�����ݲ���С���㣬���ֻ�õ���������ܵ�7��SEG����3����Ƭ����IO������9Ƭ������74HC595��ÿ��74hc595ֻ��7�������(Q0���ղ���)����7�������������ULN2003A��B�ڣ�ULN2003A����һ�˵�C���ٴ���470ŷ�ĵ������ӵ���������ܵ�SEG����������ܵ�COMֱ�ӽ�5V��Դ��������������9λ�Ĺ�������ܡ�
��4��Դ���ʺϵĵ�Ƭ��: PIC18f4520,����Ϊ11.0592MHz��
��5��Դ���뽲�����£�
*/

#include<pic18.h>         //����оƬ���ͷ�ļ�

//74HC595��OE����ֱ��Ӳ���ӵ�
# define hc595_sh_dr     LATA3     //74hc595��3������IO֮һ
# define hc595_st_dr     LATA4   //74hc595��3������IO֮һ
# define hc595_ds_dr     LATA5  //74hc595��3������IO֮һ


void _nop_();         //����ʱ����ʱ����
void hc595_drive();    //����9Ƭ������74hc595���ӳ���
void display_drive(); //������������򣬷���main������while(1)ѭ����

unsigned char number_left1=0;   //��ߵ�1λ�������ʾ������
unsigned char number_left2=0;   //��ߵ�2λ�������ʾ������
unsigned char number_left3=0;   //��ߵ�3λ�������ʾ������
unsigned char number_left4=0;   //��ߵ�4λ�������ʾ������
unsigned char number_left5=0;   //��ߵ�5λ�������ʾ������
unsigned char number_left6=0;   //��ߵ�6λ�������ʾ������
unsigned char number_left7=0;   //��ߵ�7λ�������ʾ������
unsigned char number_left8=0;   //��ߵ�8λ�������ʾ������
unsigned char number_left9=0;   //��ߵ�9λ�������ʾ������

unsigned char led_update=0;     //������ʾ����������1ʱ��ִ��һ�θ�����ʾ����ܵĳ���


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



    TRISA3=0;    //74hc595��4������IO֮һ
    TRISA4=0;    //74hc595��4������IO֮һ
    TRISA5=0;    //74hc595��4������IO֮һ


    number_left1=1;   //��ߵ�1λ�������ʾ"1"
    number_left2=2;   //��ߵ�2λ�������ʾ"2"
    number_left3=3;   //��ߵ�3λ�������ʾ"3"
    number_left4=4;   //��ߵ�4λ�������ʾ"4"
    number_left5=5;   //��ߵ�5λ�������ʾ"5"
    number_left6=6;   //��ߵ�6λ�������ʾ"6"
    number_left7=7;   //��ߵ�7λ�������ʾ"7"
    number_left8=8;   //��ߵ�8λ�������ʾ"8"
    number_left9=8;   //��ߵ�9λ�������ʾ"8"
    
    led_update=1;  //������ʾ



   while(1)    
   {
       CLRWDT(); //ι���Ź�����Ҳ��ù��ȹ�ע����
       display_drive(); //������������򣬷���main������while(1)ѭ����

   }

}


void display_drive()    //������������򣬷���main������while(1)ѭ����
{
    if(led_update==1)  //�����ݸ���
    {
        led_update=0; //��־��ʱ���㣬����һֱɨ��
        hc595_drive(); //����9Ƭ������74hc595���ӳ���
    }
}


void hc595_drive()     //����9Ƭ������74hc595���ӳ���
{
   unsigned char tempdata; //ÿ���ֽڵ�ÿһλ,��8λ
   unsigned char com_select;  //�м����
   unsigned char number_temp[9];  //����������ʾ���ݵ��м����
   unsigned char tube_cnt; //����ܸ�������9��

   number_temp[0]=number_table[number_left1];  //���뼴����ʾ������
   number_temp[1]=number_table[number_left2];  //���뼴����ʾ������
   number_temp[2]=number_table[number_left3];  //���뼴����ʾ������
   number_temp[3]=number_table[number_left4];  //���뼴����ʾ������
   number_temp[4]=number_table[number_left5];  //���뼴����ʾ������
   number_temp[5]=number_table[number_left6];  //���뼴����ʾ������
   number_temp[6]=number_table[number_left7];  //���뼴����ʾ������
   number_temp[7]=number_table[number_left8];  //���뼴����ʾ������
   number_temp[8]=number_table[number_left9];  //���뼴����ʾ������


   hc595_sh_dr=0;
   hc595_st_dr=0;
   for(tube_cnt=9;tube_cnt!=0;tube_cnt--)  //9��74hc595
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
