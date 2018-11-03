#ifndef __HC595_IO_H__
#define __HC595_IO_H__

#define LED_COM    (4)    //Max 8

unsigned char 	LED[LED_COM]; //显示内存

void DisplayScan(void); //显示扫描函数

#endif