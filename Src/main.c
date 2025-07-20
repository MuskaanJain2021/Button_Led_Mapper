/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Muskaan Jain
 * @brief          : Button Led Mapper
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include <stdint.h>


/* DELAY USING SYSTICK HARDWARE TIMER
 *
 *
 *
 * */
volatile uint32_t msTicks = 0;//Milliseconds tick counter
uint32_t System_CoreClock = 16000000;
volatile uint8_t delay_mode = 0;//0-none 1 - milliseconds
void SysTick_Enable(uint32_t ticks, uint8_t mode) {
	delay_mode = mode;
	SysTick->LOAD = ticks - 1; // Set reload value ,LOAD=(System_CoreClock×Desired Delay (s))−1
	SysTick->VAL = 0; // Reset counter or Clear current value
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
	SysTick_CTRL_TICKINT_Msk |
	SysTick_CTRL_ENABLE_Msk; // Enable SysTick with interrupt
}

void SysTick_Disable(void) {
	delay_mode = 0;
	SysTick->CTRL = 0; // Disable SysTick
}

void delay_ms(uint32_t ms) {
	msTicks = ms;
	SysTick_Enable(((System_CoreClock / 1000)), 1); // 1ms tick // 16,000,000 / 1000 = 16,000 ticks
	while (msTicks != 0);
	SysTick_Disable();
}

void SysTick_Handler(void) {
	if (delay_mode == 1 && msTicks > 0) {
		msTicks--;
		if(msTicks == 0)SysTick_Disable();
	}
}
//-------------------------------------------------------------------------


/* -------------------------
   Function Pointer Type
------------------------- */
typedef void (*ButtonActionFn_t)(void);

/* -------------------------
   LED Functions
------------------------- */
void LED_Init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

    GPIOD->MODER &= ~((3U << (12 * 2)) | (3U << (13 * 2)) | (3U << (14 * 2)));
    GPIOD->MODER |=  ((1U << (12 * 2)) | (1U << (13 * 2)) | (1U << (14 * 2)));

    GPIOD->OTYPER &= ~((1U << 12) | (1U << 13) | (1U << 14));
    GPIOD->OSPEEDR |= ((2U << (12 * 2)) | (2U << (13 * 2)) | (2U << (14 * 2)));
    GPIOD->PUPDR &= ~((3U << (12 * 2)) | (3U << (13 * 2)) | (3U << (14 * 2)));
}

void LED_TOGGLE(uint16_t pin) {
    GPIOD->ODR ^= (1U << pin);
}

/* -------------------------
   Button Action Functions
------------------------- */
static void Act_toggle_led12(void) { LED_TOGGLE(12); }
static void Act_toggle_led13(void) { LED_TOGGLE(13); }
static void Act_toggle_led14(void) { LED_TOGGLE(14); }

/* -------------------------
   Button Mapping Table
------------------------- */
typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
    ButtonActionFn_t action;
} SW_ButtonMap_t;

static const SW_ButtonMap_t SW_ButtonTable[] = {
    { GPIOA, 0, Act_toggle_led12 },
    { GPIOA, 1, Act_toggle_led13 },
    { GPIOA, 2, Act_toggle_led14 }
};

#define NUM_SW_BUTTONS  (sizeof(SW_ButtonTable) / sizeof(SW_ButtonTable[0]))

/* -------------------------
   Button Released Flags
------------------------- */
static uint8_t SW_Button_Released[NUM_SW_BUTTONS];

/* -------------------------
   GPIOA Button Init
------------------------- */
void SWITCH_Button_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER &= ~((3U << (0 * 2)) | (3U << (1 * 2)) | (3U << (2 * 2)));
    GPIOA->PUPDR &= ~((3U << (0 * 2)) | (3U << (1 * 2)) | (3U << (2 * 2)));
    GPIOA->PUPDR |=  ((1U << (0 * 2)) | (1U << (1 * 2)) | (1U << (2 * 2)));

    for (uint32_t i = 0; i < NUM_SW_BUTTONS; i++) {
        SW_Button_Released[i] = 1;
    }
}

/* -------------------------
   Flag-Based Polling
------------------------- */
void SW_Button_Scanning(void)
{
    for (uint32_t i = 0; i < NUM_SW_BUTTONS; i++) {
        uint8_t state = (SW_ButtonTable[i].port->IDR & (1U << SW_ButtonTable[i].pin)) ? 1 : 0;

        if (state == 0 && SW_Button_Released[i]) {
            // Button just pressed
            SW_ButtonTable[i].action();
            SW_Button_Released[i] = 0;
            delay_ms(2000);
        }
        else if (state == 1) {
            // Button released
            SW_Button_Released[i] = 1;
        }
    }
}


int main(void)
{
    LED_Init();
    SWITCH_Button_init();

    while (1) {
        SW_Button_Scanning();  // Non-blocking flag-based polling
    }
}
