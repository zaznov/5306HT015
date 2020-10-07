/**
  ******************************************************************************
  * @file    5306ht015.c
  * @author  Zaznov NIIKP
  * @version V1.3.0
  * @date    13/07/2020
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the 5306ht015-driver:           
  *           + Initialization and Configuration
  *           + Getting temperature results
  ******************************************************************************
  * FILE 5306ht015.c
  */
/* Includes ------------------------------------------------------------------*/
#include "5306ht015.h"

/* Defines -------------------------------------------------------------------*/ 
#define DELAY_RESET             500
#define DELAY_WRITE_0           60
#define DELAY_WRITE_0_PAUSE     10
#define DELAY_WRITE_1           10
#define DELAY_WRITE_1_PAUSE     60
#define DELAY_READ_SLOT         5
#define DELAY_BUS_RELAX         2
#define DELAY_READ_PAUSE        50
#define DELAY_T_CONVERT_mls     1000                                            // in milliseconds(!)
#define DELAY_RELAXATION        5
/* Variables -----------------------------------------------------------------*/
volatile char sign = '+';
/** 
  * @brief  COMMANDS enumeration 
  */  
typedef enum {
    SKIP_ROM = 0xCC,
    SEARCH_ROM = 0x0F,
    READ_ROM = 0x33,
    CONVERT_T = 0x44,
    READ_SCRATCHPAD = 0xBE,
} COMMANDS_t;


static void write_bit(const uint8_t bit);
static void write_byte(const uint8_t byte);
static uint8_t read_bit(void);
static uint16_t read_temperature(void);
static uint64_t read_number(void);
static void reset(void);


/**
  * @brief  Initialization of I/O Wire.
  * @note   This function define Pin, Output type, speed, turn on Clocking.
  * @param  None. Params can be changed directly inside function
  */
void ds18b20_init(void) 
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;                                        // Clock enable
    GPIOA->MODER |= GPIO_MODER_MODER1_0;                                        // Pin A1 - OUTPUT
    GPIOA->OTYPER |= GPIO_OTYPER_OT_1;                                          // Open-drain 
    GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR0;                                  // Hight speed   
}



/**
  * @brief  Initial reset function.
  * @param  None. 
  */
void reset(void) 
{
    GPIOA->ODR &= ~GPIO_ODR_ODR_1;
    delay_mks(DELAY_RESET);
    GPIOA->ODR |= GPIO_ODR_ODR_1;                                               // ѕросто пропускаем ответ, не читаем его
    delay_mks(DELAY_RESET);
}

/**
  * @brief  Temperature measurment function
  * @note   This function send a command to start a converting, wait for its 
  ending and return results in uint16_t form (view a read_temperature() for 
  details)
  * @param  None.
  */
uint16_t ds18b20_get_temperature(void) 
{
    reset();
    write_byte(SKIP_ROM);
    write_byte(CONVERT_T);
    
    while(read_bit()){
        delay_mls(DELAY_T_CONVERT_mls);
    }
    
    reset();
    write_byte(SKIP_ROM);
    write_byte(READ_SCRATCHPAD);
    return read_temperature();
    
    
}

/**
  * @brief  Function to get serial number of 5306HT015
  * @note   This function send a command to get a 64-bit serial number 
    of 5306HT015 and return the answer.
  * @param  None.
  */
uint64_t ds18b20_get_serial_number(void) 
{
    reset();
    write_byte(READ_ROM);
    
    return read_number();
}


/**
  * @brief  Function for writing 1 bit .
  * @param  bit: can be 0 or 1. 
  */
void write_bit(uint8_t bit) 
{
    GPIOA->ODR &= ~GPIO_ODR_ODR_1;
    delay_mks(bit ? DELAY_WRITE_1 : DELAY_WRITE_0);
    GPIOA->ODR |= GPIO_ODR_ODR_1;
    delay_mks(bit ? DELAY_WRITE_1_PAUSE : DELAY_WRITE_0_PAUSE);
} 

/**
  * @brief  Function for writing 1 bit .
  * @param  data: can be any uint8_t form value. So far only COMMANDS_t are used. 
  */
void write_byte(uint8_t data) 
{
    for (uint8_t i = 0; i < 8; i++) 
    {
        write_bit(data >> i & 1);
        delay_mks(DELAY_RELAXATION);
    }
}

/**
  * @brief  Reading 1-"1-Wire" bit .
  * @param  None. 
  */
uint8_t read_bit(void) 
{
    uint8_t bit = 0;
    GPIOA->ODR &= ~GPIO_ODR_ODR_1;
    delay_mks(DELAY_READ_SLOT);
    GPIOA->ODR |= GPIO_ODR_ODR_1;                                               
    GPIOA->MODER &= ~GPIO_MODER_MODER1;                                         // Pin A0 - INPUT// ... switch to INPUT
    delay_mks(DELAY_BUS_RELAX);
    bit = (GPIOA->IDR & GPIO_IDR_IDR_1 ? 1 : 0);
    delay_mks(DELAY_READ_PAUSE);
    GPIOA->MODER |= GPIO_MODER_MODER1_0;                                        // Pin A0 - OUTPUT// ... switch to OUTPUT
    return bit;
} 

/**
  * @brief  Temperature readind function
  * @note   This function get a temperature value from the 5306ht015 sensor and 
  return results in uint16_t form. the result is returned multiplied by 10. 
  For example: for temperature "24.7" -> "247" will be returned.
  Also function change sign value, when it is necessary
  * @param  None.
  */
uint16_t read_temperature(void) 
{
    uint16_t data = 0;
    for (uint8_t i = 0; i < 16; i++)
    data += (uint16_t) read_bit() << i;
    if(data&0xF000) 
    {
        sign = '-';
        data = ~data;
    }
    else sign = '+';
    return (uint16_t)(((float) data / 16.0f) * 10.0f);
}

/**
  * @brief  Number readind function
  * @note   This function return serial number of 5306HT015
  * @param  None.
  */
uint64_t read_number(void) 
{
    uint64_t data = 0;
    for (uint8_t i = 0; i < 64; i++)
    data += (uint64_t) read_bit() << i;
    
    return data;
}
/************************* 2020 Zaznov NIIKP ***********************************
*
* END OF FILE 5306ht015.c */
