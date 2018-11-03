#include "Yhh.h"

enum {STOP, START_UP, MOTOR_START, MOTOR_STOP, MANUAL_AUTO, UP, DOWN, URGENT_STOP, OK} WORK_S;

#define COUNT 60

static __bit i;

void YHH_Init(void)
{
    UP_DOWN = 0;
    MOTOR_ST_SP = 0;
    HEAD = 0;
}

void YHH_Update(void)
{
    static uchar8 j;
    static int16 k;
    switch(Key_cont)
    {
        case STOP_VAL:
        {
            
            break;
        }
        
        case START_UP_VAL:
        {
            UP_DOWN = 1;
            MOTOR_ST_SP = 1;
            if(++j > COUNT)
            {
              j = 0;
              i = 1;
            }

            break;
        }
        
        case MOTOR_START_VAL:
        {
            
            break;
        }
        
        case MOTOR_STOP_VAL:
        {
          k = -1;
          i = 1;
            break;
        }
        
        case MANUAL_AUTO_VAL:
        {
            
            break;
        }
        
        case UP_VAL:
        {
            
            break;
        }
        
        case DOWN_VAL:
        {
            
            break;
        }
        
        case URGENT_STOP_VAL:
        {
            
            break;
        }
        
        default:
        {
            UP_DOWN = 0;
            MOTOR_ST_SP = 0;
            
          if(i)
            {
              i = 0;
              if(++k > 9999) k = 0;
              LED[0] = k/1000;
              LED[1] = k/100%10;
              LED[2] = k/10%10;
              LED[3] = k%10;
            }
          
            break;
        }
    }

}