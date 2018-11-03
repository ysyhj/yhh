/*------------------------------------------------------------------*-

   ���̿�,4��x 4��.

                     ��λ:------------
                          1  2  3  A
                          4  5  6  B
                          7  8  9  C
                          *  0  #  D
                         -------------

   ֧�����ֹ��ܼ�(��*���͡�#��).

-*------------------------------------------------------------------*/

#include "Keypad.h"

#define KEYPAD_RECV_BUFFER_LENGTH (6)

bit KEYPAD_Scan(char* const, char* const);

#define KEYPAD_NO_NEW_DATA (char) '-'


static char KEYPAD_recv_buffer[KEYPAD_RECV_BUFFER_LENGTH+1][2];

static uchar8 KEYPAD_in_read_index;     // ���ݻ������Ѿ�����ȡ
static uchar8 KEYPAD_in_waiting_index;  // ���ݻ�����û�ж�ȡ


static char Last_valid_key_G = KEYPAD_NO_NEW_DATA;


void KEYPAD_Init(void)
   {
   KEYPAD_in_read_index = 0;
   KEYPAD_in_waiting_index = 0;
   }


void KEYPAD_Update(void)
   {
   char Key, FnKey;
   
   //ɨ�����
   if (KEYPAD_Scan(&Key, &FnKey) == 0)
      {
      return;
      }

   if (KEYPAD_in_waiting_index == KEYPAD_in_read_index)
      { 
      KEYPAD_in_waiting_index = 0;
      KEYPAD_in_read_index = 0;
      } 
   
   //�������ݼ��ص�������
   KEYPAD_recv_buffer[KEYPAD_in_waiting_index][0] = Key;
   KEYPAD_recv_buffer[KEYPAD_in_waiting_index][1] = FnKey;

   if (KEYPAD_in_waiting_index < KEYPAD_RECV_BUFFER_LENGTH)
      {
      KEYPAD_in_waiting_index++;
      }
   }

bit KEYPAD_Get_Data_From_Buffer(char* const pKey, char* const pFuncKey)
   {
   //�����������
   if (KEYPAD_in_read_index < KEYPAD_in_waiting_index)
      {
      *pKey = KEYPAD_recv_buffer[KEYPAD_in_read_index][0];
      *pFuncKey = KEYPAD_recv_buffer[KEYPAD_in_read_index][1];

      KEYPAD_in_read_index++;

      return 1;
      }

   return 0;
   }

void KEYPAD_Clear_Buffer(void)
   {
   KEYPAD_in_waiting_index = 0;
   KEYPAD_in_read_index = 0;
   }

bit KEYPAD_Scan(char* const pKey, char* const pFuncKey)
   {
   static char Old_Key;

   char Key = KEYPAD_NO_NEW_DATA;
   char Fn_key = (char) 0x00;

   Column1 = 0; // ɨ���� 1
      if (Row1 == 0) Key = 1;
      if (Row2 == 0) Key = 4;
      if (Row3 == 0) Key = 7;
      //if (Row4 == 0) Fn_key = '*';
   Column1 = 1;

   Column2 = 0; // ɨ���� 2
      if (Row1 == 0) Key = 2;
      if (Row2 == 0) Key = 5;
      if (Row3 == 0) Key = 8;
      //if (Row4 == 0) Key = '0';
   Column2 = 1;

   Column3 = 0; // ɨ���� 3
      if (Row1 == 0) Key = 3;
      if (Row2 == 0) Key = 6;
      if (Row3 == 0) Key = 9;
      //if (Row4 == 0) Fn_key = '#';
   Column3 = 1;
   
   //Column4 = 0; // ɨ���� 4
      //if (Row1 == 0) Key = 'A';
      //if (Row2 == 0) Key = 'B';
      //if (Row3 == 0) Key = 'C';
      //if (Row4 == 0) key = 'D';
   //Column4= 1;

   if (Key == KEYPAD_NO_NEW_DATA)
      {
      //û�м�����(���߽�����һ�����ܼ�)
      Old_Key = KEYPAD_NO_NEW_DATA;
      Last_valid_key_G = KEYPAD_NO_NEW_DATA;

      return 0;  // û���µ�����
      }
   
   if (Key == Old_Key)
      {

      if (Key != Last_valid_key_G)
         { 
         // �¼�
         *pKey = Key;
         Last_valid_key_G = Key;

         if (Fn_key)
            {
            *pFuncKey = Fn_key;
            }
         else
            {
            *pFuncKey = (char) 0x00;
            }

         return 1;
         }
      }

   // û���¼�
   Old_Key = Key;
   return 0;
   }