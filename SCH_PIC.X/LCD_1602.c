
#include "LCD_1602.h"

//LCD ��ʾ����
uchar8 LCD_data_G[LCD_LINES][LCD_CHARACTERS+1] 
     = {" PLEASE WAIT    ","          ...   "};


static void LCD_Send_Byte(const uchar8, const bit) ;
static void LCD_Create_Character(const uchar8, const uchar8* const);

static void LCD_SetDDRAM(uchar8);  
static void LCD_Delay(void);


// λͼ������ַ�

// Ӣ��Ӣ��(����)
// 765 43210
// ... ...11 - 3 (С��) = 4^2 + 3^2 + 2^2 + 1^2 + 0^2
// ... ..1.. - 4 
// ... .111. - 14 
// ... ..1.. - 4 
// ... ..1.. - 4 
// ... ..1.. - 4 
// ... 11111 - 31 
// ... ..... - 0 
const uchar8 LCD_UDC_Pounds[8] = {3,4,14,4,4,4,31,0};
// #define LCD_UDC_POUNDS 1

// �����϶ȡ�
// 765 43210
// ... .11.. = 12 (С��) = 4^2 + 3^2 + 2^2 + 1^2 + 0^2
// ... 1..1. = 18
// ... .11.. = 12
// ... ...11 = 3
// ... ..1.. = 4
// ... ..1.. = 4
// ... ...11 = 3  
// ... ..... - 0 
const uchar8 LCD_UDC_Degrees_C[8] = {12,18,12,3,4,4,3,0};
// #define LCD_UDC_DEGREES_C 2

#define LCD_DISPLAY_OFF_CURSOR_OFF_BLINKING_OFF 0x08
#define LCD_DISPLAY_ON_CURSOR_OFF_BLINKING_OFF 0x0C

#define LCD_INC_ADDR_NO_SCROLL 0x06
#define LCD_CURSOR_OFF 0x08
#define LCD_DISPLAY_ON 0x04
#define LCD_CLEAR_DISPLAY 0x01
#define LCD_8BIT_2LINE_5x8FONT 0x38  // 0011 1000 
#define LCD_4BIT_2LINE_5x8FONT 0x28  // 0010 1000

//50 us ��ʱ����ֵ
#define PRELOAD50micros  (65536 - (uint16)((OSC_FREQ / 22000)/(OSC_PER_INST)))    
#define PRELOAD50microsH (PRELOAD50micros / 256)
#define PRELOAD50microsL (PRELOAD50micros % 256)


void LCD_Init(const bit TURN_DISPLAY_ON)
   {
   uchar8 loop;
   uchar8 l,c;

   Hardware_Delay_T1(10);  

   //����Һ���˿�
   LCD_D4 = 1;
   LCD_D5 = 1;
   LCD_D6 = 1;
   LCD_D7 = 1;

   LCD_RS = 1;
   LCD_EN = 1;    

   Hardware_Delay_T1(10);  

   LCD_RS = 0;
   LCD_EN = 0;    

   // �ȴ���ʾ��ʼ��
   //���ݱ����� 40 ms ��ʱ
   Hardware_Delay_T1(100);

   // ���ݱ�˵��ָ���3��
   for (loop = 0; loop < 3; loop++)
      {
      // ʹ��4 ������,2����ʾ��5 x7������
      LCD_Send_Byte(LCD_4BIT_2LINE_5x8FONT,0);  
      Hardware_Delay_T1(1);
      }
   
   // ��LCD ��겻��˸
   LCD_Send_Byte(LCD_DISPLAY_OFF_CURSOR_OFF_BLINKING_OFF,0); 
   Hardware_Delay_T1(1);
   
   // ����ʾ
   LCD_Send_Byte(LCD_CLEAR_DISPLAY,0); 
   Hardware_Delay_T1(1);

   // �����
   LCD_Control_Cursor(0,0,0);
   Hardware_Delay_T1(1);

   // ����ʾ
   LCD_Send_Byte(LCD_CLEAR_DISPLAY,0); 
   Hardware_Delay_T1(1);

   if (TURN_DISPLAY_ON)
      {
      // �����ַ���ʾ��ַ����Ҫ����
      LCD_Send_Byte(LCD_INC_ADDR_NO_SCROLL,0); 
      Hardware_Delay_T1(1);

      //������ʾ�����ַ�
      for (l = 0; l < LCD_LINES; l++)
          {
          for (c = 0; c < LCD_CHARACTERS; c++)
              {
              LCD_data_G[l][c] = ' ';
              LCD_Update();
              Hardware_Delay_T1(1);
              } 
          }

      // �����û�������ַ�
      LCD_Create_Character(LCD_UDC_DEGREES_C, LCD_UDC_Degrees_C);
      Hardware_Delay_T1(1);

      LCD_Create_Character(LCD_UDC_POUNDS, LCD_UDC_Pounds);
      Hardware_Delay_T1(1); 

      // �򿪹��,��˸��ʾ
      LCD_Send_Byte(LCD_DISPLAY_ON_CURSOR_OFF_BLINKING_OFF,0); 
      Hardware_Delay_T1(1);
      }      
   }


void LCD_Update(void) 
   {
   static uchar8 Line;
   static uchar8 Character;

   uchar8 Tests, Data;
   bit Update_required;

   // ���Ҹ�����һ���ַ�
   Tests = LCD_CHARACTERS * LCD_LINES;
   do {
      if (++Character == LCD_CHARACTERS)
         {
         Character = 0;

         if (++Line == LCD_LINES)
            {
            Line = 0;
            }
         }

      // ����д��Һ����������������Ϊ '\0'
      Update_required = (LCD_data_G[Line][Character] != '\0');
      } 
      while ((Tests-- > 0) && (!Update_required));

   if (!Update_required)
      {
      return;  // û������Ҫ����
      }

  // ����DDRAM��ַд���ַ�
  // - ����2����ʾ(��16 x1)
  // - ���ı�����������ʾ��С
  if (Line == 0)
      {
      LCD_SetDDRAM(0x00 + Character);  // ��1��
      }
   else
      {
      LCD_SetDDRAM(0x40 + Character);  // ��2��
      }

   // ��ʾ����
   Data = LCD_data_G[Line][Character];

   // ��ʾ
   LCD_Send_Byte(Data,1);

   // ���ݱ�д��LCD��������������Ϊ '\0'
   LCD_data_G[Line][Character] = '\0';  
   }


void LCD_Send_Byte(const uchar8 DATA, const bit DATA_FLAG) 
   {
   // ��Ҫ�ӳ�
   LCD_D4 = 0;
   LCD_D5 = 0;
   LCD_D6 = 0;
   LCD_D7 = 0;
   LCD_RS = DATA_FLAG; // ָ��L/����H
   LCD_EN = 0;
   LCD_Delay();

   // д����(��λ)
   LCD_D4 = ((DATA & 0x10) == 0x10);
   LCD_D5 = ((DATA & 0x20) == 0x20);
   LCD_D6 = ((DATA & 0x40) == 0x40);
   LCD_D7 = ((DATA & 0x80) == 0x80); 

   LCD_Delay();
   LCD_EN = 1; // ����4λ
   LCD_Delay();
   LCD_EN = 0; //ִ��ָ��
   
   LCD_Delay();
   LCD_D4 = 0;
   LCD_D5 = 0;
   LCD_D6 = 0;
   LCD_D7 = 0;
   LCD_RS = DATA_FLAG;
   LCD_EN = 0; //ִ��ָ��
   LCD_Delay();

   // д����(��λ)
   LCD_D4 = ((DATA & 0x01) == 0x01);
   LCD_D5 = ((DATA & 0x02) == 0x02);
   LCD_D6 = ((DATA & 0x04) == 0x04);
   LCD_D7 = ((DATA & 0x08) == 0x08);

   LCD_Delay();
   LCD_EN = 1; // ����4λ
   LCD_Delay();
   LCD_EN = 0; //ִ��ָ��
   LCD_Delay();   
   } 

 
 // VISIBLE ����Ƿ���ʾ
 // BLINKING �ַ��Ƿ���˸
 // ADDRESS  �����ĵ�ַ
void LCD_Control_Cursor(const bit VISIBLE, const bit BLINKING, const uchar8 ADDRESS) 
   {
   // �����ʾ�� DDRAM ��ַ 
   // - ��SetDDRAM() �ı���λ��
   uchar8 Command = 0x0C;  

   if (VISIBLE) 
      {
      Command |= 0x02;
      }

   if (BLINKING) 
      {
      Command |= 0x01;
      }

   LCD_Send_Byte(Command,0);
   LCD_SetDDRAM(ADDRESS);
   }


void LCD_SetDDRAM(uchar8 ADDRESS)
   {
   ADDRESS &= 0x7f;
   ADDRESS |= 0x80;
   LCD_Send_Byte(ADDRESS,0);
   }


void LCD_SetCGRAM(uchar8 Address)
   {
   Address &= 0x3f;
   Address |= 0x40;
   LCD_Send_Byte(Address,0);
   }


void LCD_Create_Character(const uchar8 UDC_ID, const uchar8* const pUDC_PAT)
   {
   uchar8 Row;  

   LCD_SetCGRAM(UDC_ID << 3);
   
   // д����
   for (Row = 0; Row < 8; Row++)
      { 
      LCD_Send_Byte(pUDC_PAT[Row], 1);
      }
   
   //ȷ����һ��д DDRAM
   LCD_SetDDRAM(0x00);
   }                                       


void LCD_Delay(void)   
   {
   int x;
   x++;
   x++;
   x++;
   }        
