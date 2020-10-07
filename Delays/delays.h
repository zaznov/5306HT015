/**
  ******************************************************************************
  * @file    delays.h
  * @author  Zaznov NIIKP
  * @version V1.0.1
  * @date    28/10/2019
  * @brief   This file contains all the functions prototypes for work with
  delays-library of Cortex-M4 family
  ******************************************************************************
  * FILE delays.h
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DELAY_H_
#define __DELAY_H_
/* Includes ------------------------------------------------------------------*/  
#include "stm32f4xx.h"

/* defines -------------------------------------------------------------------*/  
#define SYSTICK_MAX_VALUE 0xFFFFFF

/* Variables -----------------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/
void delay_mls(uint32_t delay_in_mils);
void delay_mks(uint32_t delay_in_mils);

#endif /* __DELAY_H_ */

/************************* 2020 Zaznov NIIKP ***********************************
*
* END OF FILE delays.h */

