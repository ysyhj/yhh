
#include "ADC.h"



    //TRISA0 = 1;		//ģ��ͨ��0����Ϊ����
    ADCON1 = 0x8E;	//10001110  Vref+ = VDD Vref- = VSS
    ADCON0 = 0x01;	//ʹ��AD
    //ADIF = 0;		//�жϱ�־����
    //ADIE = 1;		//ADC�ж�ʹ��
    //GIE = 1;		//���ж�ʹ��
    //PEIE = 1;		//�����ж�����
    //ADON = 1;		//����ADCת��



uint16 adc10_conv()
{
    uint16 ad_value;
//    ADCON1 = 0x30;
//    ADCON0 = 0x80;
    //ADON = 1;     // ��ADģ��
//    NOP();
//    NOP();
//    NOP();
//    NOP();
//    NOP();
    GO = 1; //����ת��
    while(GO); //�ȴ�ת�����
    //ADON = 0; //�ر�ADģ��

   ad_value = (uint16)(ADRESL | (ADRESH << 8));
   return (ad_value);

//uchar8 i;
//uint16 result,temph;
//result = 0;
////ADON = 1;     // ��ADģ��
//for(i = 8;i > 0;i--)
//{
//GO=1;
//while(GO);
//temph = ADRESH;
//result+=((temph << 8) + ADRESL);
//
//}
// //ADON = 0; //�ر�ADģ��
//return (result>>3);
}
