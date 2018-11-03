/*          动态扫描数码管，是指每隔一段时间单片机必须及时去扫描数码管，否则数码管显示就不正常，单片机很累，从来没有时间休息，感觉一直处于运动的状态，因此叫动态扫描。静态驱动数码管恰恰相反，是指单片机需要更新显示内容的时候才会去扫描一次数码管，平时数码管维持当前显示的内容。举个例子，一家公司雇一个全职工程师，不管一个月内有没有项目开发，每个月都要按时给他发工资，这个就叫动态的。而另外一家公司请鸿哥开发单片机项目，只有等鸿哥把项目开发完了，他才会给我开发费，这个就叫做静态的。很明显，动态驱动的软件开销比较大，静态驱动的软件开销比较小，如果条件允许，尽可能选静态驱动的。就像鸿哥为什么受那么多深圳的中小企业喜欢，就是因为鸿哥是静态的，不用每个月给我工资，少数VIP客户除外。用74HC595静态驱动数码管，第二个好处就是驱动电流比较大，尤其是用在驱动大型数码管的时候，用静态驱动的方式，不用担心驱动电流不够的问题。第三个好处就是单片机省IO口，用3根IO口就可以至少驱动9个数码管。这节用的是共阳数码管，还用到内部集成了7个三极管的ULN2003A芯片，用来增强驱动电流。
       记住，在吴坚鸿的程序风格中，凡是静态驱动显示的，就跟液晶屏显示一样，都有一个_update更新显示变量。本程序是led_update。
（2）        功能需求：
在9个数码管中，从左到右，依次显示“123456789”。
（3）硬件原理：
由于显示的内容不用小数点，因此只用到共阳数码管的7个SEG。用3根单片机的IO口驱动9片联级的74HC595，每个74hc595只用7根输出口(Q0悬空不用)，这7根输出口再连接ULN2003A的B口，ULN2003A另外一端的C口再串接470欧的电阻连接到共阳数码管的SEG，共阳数码管的COM直接接5V电源。本程序是驱动9位的共阳数码管。
（4）源码适合的单片机: PIC18f4520,晶振为11.0592MHz。
（5）源代码讲解如下：
*/

#include<pic18.h>         //包含芯片相关头文件

//74HC595的OE引脚直接硬件接地
# define hc595_sh_dr     LATA3     //74hc595的3根驱动IO之一
# define hc595_st_dr     LATA4   //74hc595的3根驱动IO之一
# define hc595_ds_dr     LATA5  //74hc595的3根驱动IO之一


void _nop_();         //驱动时序延时函数
void hc595_drive();    //驱动9片联级的74hc595的子程序
void display_drive(); //数码管驱动程序，放在main函数的while(1)循环里

unsigned char number_left1=0;   //左边第1位数码管显示的内容
unsigned char number_left2=0;   //左边第2位数码管显示的内容
unsigned char number_left3=0;   //左边第3位数码管显示的内容
unsigned char number_left4=0;   //左边第4位数码管显示的内容
unsigned char number_left5=0;   //左边第5位数码管显示的内容
unsigned char number_left6=0;   //左边第6位数码管显示的内容
unsigned char number_left7=0;   //左边第7位数码管显示的内容
unsigned char number_left8=0;   //左边第8位数码管显示的内容
unsigned char number_left9=0;   //左边第9位数码管显示的内容

unsigned char led_update=0;     //更新显示变量，等于1时将执行一次更新显示数码管的程序


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



    TRISA3=0;    //74hc595的4根驱动IO之一
    TRISA4=0;    //74hc595的4根驱动IO之一
    TRISA5=0;    //74hc595的4根驱动IO之一


    number_left1=1;   //左边第1位数码管显示"1"
    number_left2=2;   //左边第2位数码管显示"2"
    number_left3=3;   //左边第3位数码管显示"3"
    number_left4=4;   //左边第4位数码管显示"4"
    number_left5=5;   //左边第5位数码管显示"5"
    number_left6=6;   //左边第6位数码管显示"6"
    number_left7=7;   //左边第7位数码管显示"7"
    number_left8=8;   //左边第8位数码管显示"8"
    number_left9=8;   //左边第9位数码管显示"8"
    
    led_update=1;  //更新显示



   while(1)    
   {
       CLRWDT(); //喂看门狗，大家不用过度关注此行
       display_drive(); //数码管驱动程序，放在main函数的while(1)循环里

   }

}


void display_drive()    //数码管驱动程序，放在main函数的while(1)循环里
{
    if(led_update==1)  //有数据更新
    {
        led_update=0; //标志及时清零，避免一直扫描
        hc595_drive(); //驱动9片联级的74hc595的子程序
    }
}


void hc595_drive()     //驱动9片联级的74hc595的子程序
{
   unsigned char tempdata; //每个字节的每一位,共8位
   unsigned char com_select;  //中间变量
   unsigned char number_temp[9];  //即将更新显示内容的中间变量
   unsigned char tube_cnt; //数码管个数，共9个

   number_temp[0]=number_table[number_left1];  //载入即将显示的内容
   number_temp[1]=number_table[number_left2];  //载入即将显示的内容
   number_temp[2]=number_table[number_left3];  //载入即将显示的内容
   number_temp[3]=number_table[number_left4];  //载入即将显示的内容
   number_temp[4]=number_table[number_left5];  //载入即将显示的内容
   number_temp[5]=number_table[number_left6];  //载入即将显示的内容
   number_temp[6]=number_table[number_left7];  //载入即将显示的内容
   number_temp[7]=number_table[number_left8];  //载入即将显示的内容
   number_temp[8]=number_table[number_left9];  //载入即将显示的内容


   hc595_sh_dr=0;
   hc595_st_dr=0;
   for(tube_cnt=9;tube_cnt!=0;tube_cnt--)  //9个74hc595
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
