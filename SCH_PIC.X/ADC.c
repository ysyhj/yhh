
#include "ADC.h"



    //TRISA0 = 1;		//模拟通道0设置为输入
    ADCON1 = 0x8E;	//10001110  Vref+ = VDD Vref- = VSS
    ADCON0 = 0x01;	//使能AD
    //ADIF = 0;		//中断标志清零
    //ADIE = 1;		//ADC中断使能
    //GIE = 1;		//总中断使能
    //PEIE = 1;		//外设中断设能
    //ADON = 1;		//启动ADC转换



uint16 adc10_conv()
{
    uint16 ad_value;
//    ADCON1 = 0x30;
//    ADCON0 = 0x80;
    //ADON = 1;     // 打开AD模块
//    NOP();
//    NOP();
//    NOP();
//    NOP();
//    NOP();
    GO = 1; //启动转换
    while(GO); //等待转换完成
    //ADON = 0; //关闭AD模块

   ad_value = (uint16)(ADRESL | (ADRESH << 8));
   return (ad_value);

//uchar8 i;
//uint16 result,temph;
//result = 0;
////ADON = 1;     // 打开AD模块
//for(i = 8;i > 0;i--)
//{
//GO=1;
//while(GO);
//temph = ADRESH;
//result+=((temph << 8) + ADRESL);
//
//}
// //ADON = 0; //关闭AD模块
//return (result>>3);
}
