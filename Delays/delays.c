/**
  ******************************************************************************
  * @file    delays.c
  * @author  Zaznov NIIKP
  * @version V1.0.1
  * @date    28/10/2019
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the delays-library of Cortex-M4 family:           
  *           + Delay in milliseconds
  *           + Delay in microseconds
  *           + ...
  * @attention SysTick Timer is used for delays!
  ******************************************************************************
  * FILE delays.c
  */
/* Includes ------------------------------------------------------------------*/
#include "delays.h"

/* Variables -----------------------------------------------------------------*/
volatile uint32_t delay_Wert = 0;                                               //delay_Value
volatile uint32_t ticks_Wert = 0;                                               //ticks_Value

/* Functions -----------------------------------------------------------------*/

/**
  * @brief  Function creats a delay in milliseconds
  * @param  delay_in_mils: delay value in milliseconds.
  * @retval None
  */
void delay_mls(uint32_t delay_in_mils)
{  
    SysTick_Config(SystemCoreClock/1000);                                       //SystemCoreClock - релаьна€ часота контроллера 
    uint32_t start = ticks_Wert;
    while((ticks_Wert - start) < delay_in_mils);
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;                                  // ќтключить таймер SysTick
}   

/**
  * @brief  Function creats a delay in microseconds
  * @param  delay_in_mks: delay value in microseconds.
  * @retval None
  */
void delay_mks(const uint32_t delay_in_mks)
{  
    if (delay_in_mks > SYSTICK_MAX_VALUE || delay_in_mks == 0)                  // ќграничение на размер задержки в св€зи с возможностью использовать только 24 разр€да
        return;
    
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;                                 // Ѕит TICKINT = 0 Ч запретить прерывание по достижении 0
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;                                // Ѕит CLKSOURCE = 1 Ч ”казать источник тактировани€ 0 Ч внешний, 1 Ч процессор
    SysTick->LOAD = (SystemCoreClock / 1000000 * delay_in_mks - 1);             // ƒанный регистр отвечает за то, какие значени€ будут выставлены при перегрузке таймера
    SysTick->VAL = 0;                                                           // —брасываем текущее значение
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;                                   // Ѕит ENABLE = 1 Ч ¬ключение или выключение таймера 0 Ч отключить таймер, 1 Ч запустить таймер
    
    while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));                      // ќжидать по€влени€ 1 в бите, отвечающем за COUNTFLAG 
    SysTick->CTRL &= ~SysTick_CTRL_COUNTFLAG_Msk;                               // —бросить COUNTFLAG ;
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;                                  // ќтключить таймер SysTick
} 

/************************* 2020 Zaznov NIIKP ***********************************
*
* END OF FILE delays.c */

