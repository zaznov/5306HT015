/**
  ******************************************************************************
  * @file    5306ht015.h
  * @author  Zaznov NIIKP
  * @version V1.3.0
  * @date    13/07/2020
  * @brief   This file contains all the functions prototypes for work with 
  temperature sensor 5306HT015 of Disain Centre Soyuz. (Functional analog of 
  DS18D20) For connection 1-Wire protocol is used. Sensor provides 12-bit Celsius 
  temperature. Operation temperature range: -55°C to +125°C.
  To view Datasheet visit:
  https://dcsoyuz.ru/products/temperature-sensors/art/1588
  ******************************************************************************
  * FILE 5306ht015.h
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __5306HT015_H_
#define __5306HT015_H_

/* Includes ------------------------------------------------------------------*/  
#include "stm32f4xx.h" 
#include "delays.h"

/* Defines -------------------------------------------------------------------*/  

/* Functions -----------------------------------------------------------------*/
void ds18b20_init(void);
uint16_t ds18b20_get_temperature(void);
uint64_t ds18b20_get_serial_number(void);

/* Variables -----------------------------------------------------------------*/
//extern char sign;
//static char sign = '+';

#endif /* __5306HT015_H_ */

/************************* 2020 Zaznov NIIKP ***********************************
*
* END OF FILE 5306ht015.h */
