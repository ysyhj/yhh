
#include "LCD_1602.h"

//LCD 显示数据
uchar8 LCD_data_G[LCD_LINES][LCD_CHARACTERS+1] 
     = {" PLEASE WAIT    ","          ...   "};


static void LCD_Send_Byte(const uchar8, const bit) ;
static void LCD_Create_Character(const uchar8, const uchar8* const);

static void LCD_SetDDRAM(uchar8);  
static void LCD_Delay(void);


// 位图定义的字符

// 英国英镑(货币)
// 765 43210
// ... ...11 - 3 (小数) = 4^2 + 3^2 + 2^2 + 1^2 + 0^2
// ... ..1.. - 4 
// ... .111. - 14 
// ... ..1.. - 4 
// ... ..1.. - 4 
// ... ..1.. - 4 
// ... 11111 - 31 
// ... ..... - 0 
const uchar8 LCD_UDC_Pounds[8] = {3,4,14,4,4,4,31,0};
// #define LCD_UDC_POUNDS 1

// “摄氏度”
// 765 43210
// ... .11.. = 12 (小数) = 4^2 + 3^2 + 2^2 + 1^2 + 0^2
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

//50 us 延时加载值
#define PRELOAD50micros  (65536 - (uint16)((OSC_FREQ / 22000)/(OSC_PER_INST)))    
#define PRELOAD50microsH (PRELOAD50micros / 256)
#define PRELOAD50microsL (PRELOAD50micros % 256)


void LCD_Init(const bit TURN_DISPLAY_ON)
   {
   uchar8 loop;
   uchar8 l,c;

   Hardware_Delay_T1(10);  

   //设置液晶端口
   LCD_D4 = 1;
   LCD_D5 = 1;
   LCD_D6 = 1;
   LCD_D7 = 1;

   LCD_RS = 1;
   LCD_EN = 1;    

   Hardware_Delay_T1(10);  

   LCD_RS = 0;
   LCD_EN = 0;    

   // 等待显示初始化
   //数据表至少 40 ms 延时
   Hardware_Delay_T1(100);

   // 数据表说该指令发送3次
   for (loop = 0; loop < 3; loop++)
      {
      // 使用4 线驱动,2行显示和5 x7点字体
      LCD_Send_Byte(LCD_4BIT_2LINE_5x8FONT,0);  
      Hardware_Delay_T1(1);
      }
   
   // 关LCD 光标不闪烁
   LCD_Send_Byte(LCD_DISPLAY_OFF_CURSOR_OFF_BLINKING_OFF,0); 
   Hardware_Delay_T1(1);
   
   // 清显示
   LCD_Send_Byte(LCD_CLEAR_DISPLAY,0); 
   Hardware_Delay_T1(1);

   // 隐光标
   LCD_Control_Cursor(0,0,0);
   Hardware_Delay_T1(1);

   // 清显示
   LCD_Send_Byte(LCD_CLEAR_DISPLAY,0); 
   Hardware_Delay_T1(1);

   if (TURN_DISPLAY_ON)
      {
      // 增加字符显示地址但不要滚动
      LCD_Send_Byte(LCD_INC_ADDR_NO_SCROLL,0); 
      Hardware_Delay_T1(1);

      //更新显示所有字符
      for (l = 0; l < LCD_LINES; l++)
          {
          for (c = 0; c < LCD_CHARACTERS; c++)
              {
              LCD_data_G[l][c] = ' ';
              LCD_Update();
              Hardware_Delay_T1(1);
              } 
          }

      // 设置用户定义的字符
      LCD_Create_Character(LCD_UDC_DEGREES_C, LCD_UDC_Degrees_C);
      Hardware_Delay_T1(1);

      LCD_Create_Character(LCD_UDC_POUNDS, LCD_UDC_Pounds);
      Hardware_Delay_T1(1); 

      // 打开光标,闪烁显示
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

   // 查找更新下一个字符
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

      // 数据写入液晶后数组内容设置为 '\0'
      Update_required = (LCD_data_G[Line][Character] != '\0');
      } 
      while ((Tests-- > 0) && (!Update_required));

   if (!Update_required)
      {
      return;  // 没有数据要更新
      }

  // 设置DDRAM地址写入字符
  // - 假设2行显示(或16 x1)
  // - 依文本调整所需显示大小
  if (Line == 0)
      {
      LCD_SetDDRAM(0x00 + Character);  // 第1行
      }
   else
      {
      LCD_SetDDRAM(0x40 + Character);  // 第2行
      }

   // 显示数据
   Data = LCD_data_G[Line][Character];

   // 显示
   LCD_Send_Byte(Data,1);

   // 数据被写入LCD后数组内容设置为 '\0'
   LCD_data_G[Line][Character] = '\0';  
   }


void LCD_Send_Byte(const uchar8 DATA, const bit DATA_FLAG) 
   {
   // 需要延迟
   LCD_D4 = 0;
   LCD_D5 = 0;
   LCD_D6 = 0;
   LCD_D7 = 0;
   LCD_RS = DATA_FLAG; // 指令L/数据H
   LCD_EN = 0;
   LCD_Delay();

   // 写数据(高位)
   LCD_D4 = ((DATA & 0x10) == 0x10);
   LCD_D5 = ((DATA & 0x20) == 0x20);
   LCD_D6 = ((DATA & 0x40) == 0x40);
   LCD_D7 = ((DATA & 0x80) == 0x80); 

   LCD_Delay();
   LCD_EN = 1; // 锁高4位
   LCD_Delay();
   LCD_EN = 0; //执行指令
   
   LCD_Delay();
   LCD_D4 = 0;
   LCD_D5 = 0;
   LCD_D6 = 0;
   LCD_D7 = 0;
   LCD_RS = DATA_FLAG;
   LCD_EN = 0; //执行指令
   LCD_Delay();

   // 写数据(低位)
   LCD_D4 = ((DATA & 0x01) == 0x01);
   LCD_D5 = ((DATA & 0x02) == 0x02);
   LCD_D6 = ((DATA & 0x04) == 0x04);
   LCD_D7 = ((DATA & 0x08) == 0x08);

   LCD_Delay();
   LCD_EN = 1; // 锁低4位
   LCD_Delay();
   LCD_EN = 0; //执行指令
   LCD_Delay();   
   } 

 
 // VISIBLE 光标是否显示
 // BLINKING 字符是否闪烁
 // ADDRESS  调整的地址
void LCD_Control_Cursor(const bit VISIBLE, const bit BLINKING, const uchar8 ADDRESS) 
   {
   // 光标显示的 DDRAM 地址 
   // - 用SetDDRAM() 改变光标位置
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
   
   // 写数据
   for (Row = 0; Row < 8; Row++)
      { 
      LCD_Send_Byte(pUDC_PAT[Row], 1);
      }
   
   //确保下一个写 DDRAM
   LCD_SetDDRAM(0x00);
   }                                       


void LCD_Delay(void)   
   {
   int x;
   x++;
   x++;
   x++;
   }        
