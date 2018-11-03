/*  我写的所有章节都是我亲身经历过的，但是这一节的灵感完全来源于电子发烧友Stone_up。他告诉我他曾经用两片联级74HC595动态扫描8位数码管，我仔细一想，用1片74HC595驱动数码管的8个COM,用另外1片74HC595驱动数码管的8个SEG，这种方法实在是巧妙，妙在特别省硬件资源，外围的电路也非常简单，也非常节省单片机的IO口，仅用3根单片机IO口就可以驱动8位数码管，这么巧妙的方法我之前 居然没有想到，实在是惭愧。在此特别点名表扬”Stone_up”的贡献。

（2）功能需求：
       在8个数码管中，从左到右，依次显示“12345678”。

（3）硬件原理：
       用单片机的3根IO口驱动2片联级的74HC595，其中1片74HC595的8个输出口分别串接470欧电阻之后连接到数码管的SEG引脚，另外1片74HC595的8个输出口分别经过NPN三极管（也可以用ULN2003A替代）之后连接到数码管的COM引脚。本程序是驱动8位的共阴数码管。

（4）源码适合的单片机: PIC18f4520,晶振为11.0592MHz。

（5）源代码讲解如下：
*/

#include<pic18.h>         //包含芯片相关头文件
//74HC595的OE引脚直接硬件接地
# define hc595_sh_dr     LATA3     //74hc595的3根驱动IO之一
# define hc595_st_dr     LATA4   //74hc595的3根驱动IO之一
# define hc595_ds_dr     LATA5  //74hc595的3根驱动IO之一

#define  const_left_com1        0x01   //左边第1个数码管的com显示
#define  const_left_com2        0x02   //左边第2个数码管的com显示
#define  const_left_com3        0x04   //左边第3个数码管的com显示
#define  const_left_com4        0x08   //左边第4个数码管的com显示
#define  const_left_com5        0x10   //左边第5个数码管的com显示
#define  const_left_com6        0x20   //左边第6个数码管的com显示
#define  const_left_com7        0x40   //左边第7个数码管的com显示
#define  const_left_com8        0x80   //左边第8个数码管的com显示
#define  const_left_com_off     0x00   //所有的数码管都不显示

void display_drive();                  //数码管驱动程序，放在定时中断里
void hc595_drive();                    //驱动2片联级的74hc595的子程序,放在display_drive()里
void _nop_();                          //驱动时序延时函数
void delay1(unsigned int de);

unsigned char number_left1=0;   //左边第1位数码管显示的内容
unsigned char number_left2=0;   //左边第2位数码管显示的内容
unsigned char number_left3=0;   //左边第3位数码管显示的内容
unsigned char number_left4=0;   //左边第4位数码管显示的内容
unsigned char number_left5=0;   //左边第5位数码管显示的内容
unsigned char number_left6=0;   //左边第6位数码管显示的内容
unsigned char number_left7=0;   //左边第7位数码管显示的内容
unsigned char number_left8=0;   //左边第8位数码管显示的内容

unsigned char number_temp[2];  //即将更新显示内容的中间变量

unsigned char dis_step=1; //扫描的步骤

const unsigned char number_table[]=  //数码管的字模转换表
{
0xbe,   //显示"0"
0x06,   //显示"1"
0x7c,   //显示"2"
0x5e,   //显示"3"
0xc6,   //显示"4"
0xda,   //显示"5"
0xfa,   //显示"6"
0x0e,   //显示"7"
0xfe,   //显示"8"
0xde,   //显示"9"
};

//主程序
main()
{
    ADCON0=0x00;  
    ADCON1=0x0f;                               //全部为数字信号
    ADCON2=0xa1;                               //右对齐
    RBPU=0;                                      //上拉电阻
    SSPEN=0;                                  

    TRISA3=0;    //74hc595的3根驱动IO之一
    TRISA4=0;    //74hc595的3根驱动IO之一
    TRISA5=0;    //74hc595的3根驱动IO之一

    T1CON=0x24;     //定时器中断配置
    TMR1H=0xFE;
    TMR1L=0xEF;
    TMR1IF=0;
    TMR1IE=1;
    TMR1ON=1;
    TMR1IE=1;


   while(1)    
   {
       CLRWDT(); //喂看门狗，大家不用过度关注此行

       number_left1=1;   //左边第1位数码管显示"1"
       number_left2=2;   //左边第2位数码管显示"2"
       number_left3=3;   //左边第3位数码管显示"3"
       number_left4=4;   //左边第4位数码管显示"4"
       number_left5=5;   //左边第5位数码管显示"5"
       number_left6=6;   //左边第6位数码管显示"6"
       number_left7=7;   //左边第7位数码管显示"7"
       number_left8=8;   //左边第8位数码管显示"8"

              
   }

}

void interrupt timer1rbint(void) //中断
{
    if(TMR1IE==1&&TMR1IF==1)    //定时中断
    {
       TMR1IF=0;     //定时中断标志位关闭
       TMR1ON=0;    //定时中断开关关闭

       display_drive(); //数码管驱动程序

       TMR1H=0xFe;   //重新设置定时时间间隔
       TMR1L=0x00;
       TMR1ON=1;        //定时中断开关打开
    }
}

void display_drive()                          //数码管驱动程序，放在定时中断里
{

  switch(dis_step)
   {

   case 1:
      number_temp[0]=const_left_com1;                //载入即将显示的那位数码管的COM
      number_temp[1]=number_table[number_left1];     //载入即将显示的内容
      break;
   case 2: 
      number_temp[0]=const_left_com2;                //载入即将显示的那位数码管的COM
      number_temp[1]=number_table[number_left2];     //载入即将显示的内容
      break;
   case 3:
      number_temp[0]=const_left_com3;                //载入即将显示的那位数码管的COM
      number_temp[1]=number_table[number_left3];     //载入即将显示的内容
      break;
   case 4: 
      number_temp[0]=const_left_com4;                //载入即将显示的那位数码管的COM
      number_temp[1]=number_table[number_left4];     //载入即将显示的内容
      break;
  case 5:
      number_temp[0]=const_left_com5;                //载入即将显示的那位数码管的COM
      number_temp[1]=number_table[number_left5];     //载入即将显示的内容
      break;
  case 6:
      number_temp[0]=const_left_com6;                //载入即将显示的那位数码管的COM
      number_temp[1]=number_table[number_left6];     //载入即将显示的内容
      break;
  case 7:
      number_temp[0]=const_left_com7;                //载入即将显示的那位数码管的COM
      number_temp[1]=number_table[number_left7];     //载入即将显示的内容
      break;
  case 8:   
      number_temp[0]=const_left_com8;                //载入即将显示的那位数码管的COM
      number_temp[1]=number_table[number_left8];     //载入即将显示的内容
      break;
   }
   hc595_drive();     //驱动2片联级的74hc595的子程序

   delay1(15); //每一位数码管显示的停留时间


   number_temp[0]=number_table[const_left_com_off];  //让所有的数码管都不显示，让显示效果更好
   number_temp[1]=0x00;                              //载入即将显示的空内容
   hc595_drive();     //驱动2片联级的74hc595的子程序

   ++dis_step;   //进入下一个数码管的扫描
   if(dis_step>8)
   {
      dis_step=1;
   } 

}

void hc595_drive()     //驱动2片联级的74hc595的子程序
{
   unsigned char tempdata; //每个字节的每一位,共8位
   unsigned char com_select;  //中间变量
   unsigned char tube_cnt; //数码管个数，共2个

   hc595_sh_dr=0;
   hc595_st_dr=0;
   for(tube_cnt=2;tube_cnt!=0;tube_cnt--)  //2个74hc595
   {
      com_select=number_temp[tube_cnt-1];
      for(tempdata=0;tempdata<8;tempdata++)  //每个8位
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
   hc595_sh_dr=0;    //拉低，抗干扰就增强
   hc595_st_dr=0;
   hc595_ds_dr=0;

}

void _nop_()         //驱动时序延时函数
{
  unsigned char n;
  for(n=0;n<0x0f;n++);
}

void delay1(unsigned int de)
{
        unsigned int t;
        for(t=0;t<de;t++);
}