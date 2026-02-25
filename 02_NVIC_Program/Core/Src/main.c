/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define USART1_IRQNO    36
#define TIM2_IRQn       28
#define I2C1_ER_IRQn    31

volatile int x=0;
/* NVIC register addresses. Refer to the processor generic guide */
volatile uint32_t *pNVIC_IPRBase =  (volatile uint32_t*)0xE000E400;
volatile uint32_t *pNVIC_ISERBase = (volatile uint32_t*)0xE000E100;
volatile uint32_t *pNVIC_ISPRBase = (volatile uint32_t*)0xE000E200;

void configure_priority_for_irqs(uint8_t irq_no, uint8_t priority_value)
{
	//1. find out iprx
	uint8_t iprx = irq_no / 4;
	volatile uint32_t *ipr =  pNVIC_IPRBase+iprx;

	//2. position in iprx
	uint8_t pos = (irq_no % 4) * 8;

	//3. configure the priority
	*ipr &= ~(0xFF << pos);//clear
	*ipr |=  (priority_value << pos);

}

/*
    Set a simple USART1 IRQ handler
*/
void Activity1(void)
{
  //1. Manually pend the pending bit for the USART1 IRQ number in NVIC
	uint32_t *pISPR1 = (uint32_t*)0XE000E204; //0XE000E200+4

  /*Why %32? Because each ISPR register is 32 bits wide,
    so we need to find the correct bit position within the register for the given IRQ number.*/
  *pISPR1 |= ( 1 << (USART1_IRQNO % 32));

	//2. Enable the USART1 IRQ number in NVIC
	uint32_t *pISER1 = (uint32_t*)0xE000E104; //0xE000E100+4
	*pISER1 |= ( 1 << (USART1_IRQNO % 32));
}

/*When two interrupts with the same priority are pending,
    the one with the lower IRQ number will be served first. In this case,
    TIM2_IRQn has a lower IRQ number than I2C1_ER_IRQn,
    so TIM2_IRQHandler will be executed before I2C1_ER_IRQHandler when both interrupts are pending.
 */
void Activity2(void)
{
  //1. Lets configure the priority for the peripherals
	 configure_priority_for_irqs(TIM2_IRQn,0x80);
	 configure_priority_for_irqs(I2C1_ER_IRQn,0x80); 

	//2. Set the interrupt pending bit in the NVIC PR
	 *pNVIC_ISPRBase |= ( 1 << TIM2_IRQn);

	//3. Enable the IRQs in NVIC ISER
	 *pNVIC_ISERBase |= ( 1 << I2C1_ER_IRQn);
	 *pNVIC_ISERBase |= ( 1 << TIM2_IRQn);
}

/*When two interrupts with different priorities are pending,
    the one with the higher priority (lower numerical value) will be served first. In this case,
    I2C1_ER_IRQn has a higher priority than TIM2_IRQn,
    so I2C1_ER_IRQHandler will be executed before TIM2_IRQHandler when both interrupts are pending.
*/
void Activity3(void)
{
  //1. Lets configure the priority for the peripherals
	 configure_priority_for_irqs(TIM2_IRQn,0x80);
	 configure_priority_for_irqs(I2C1_ER_IRQn,0x70); 

	//2. Set the interrupt pending bit in the NVIC PR
	 *pNVIC_ISPRBase |= ( 1 << TIM2_IRQn);

	//3. Enable the IRQs in NVIC ISER
	 *pNVIC_ISERBase |= ( 1 << I2C1_ER_IRQn);
	 *pNVIC_ISERBase |= ( 1 << TIM2_IRQn);
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  //Activity1();


  //Activity2();


  Activity3();

  while (1)
  {

  }
}

void USART1_IRQHandler(void)
{
	;//printf("in USART1 isr\n");
}

void TIM2_IRQHandler(void)
{
  //printf("[TIM2_IRQHandler]\n");
  /*Here, we are pending I2C interrupt request manually */
  *pNVIC_ISPRBase |= ( 1 << I2C1_ER_IRQn);
  while(1);

}


void I2C1_ER_IRQHandler(void)
{
  x++;//printf("[I2C1_EV_IRQHandler]\n");
}
