/*
 * setup_hw.c
 *
 *  Created on: 07.12.2014
 *      Author: pulsar
 */

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_uart.h"

void initUART(UART_HandleTypeDef* uartDef){
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC->APB2ENR |= RCC_APB2ENR_USART6EN;

	GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	uartDef->Instance = USART6;
	uartDef->Init.BaudRate = 9600;
	uartDef->Init.WordLength = UART_WORDLENGTH_8B;
	uartDef->Init.StopBits = UART_STOPBITS_1;
	uartDef->Init.Parity = UART_PARITY_NONE;
	uartDef->Init.Mode = UART_MODE_TX_RX;
	uartDef->Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uartDef->Init.OverSampling = UART_OVERSAMPLING_16;

	HAL_UART_Init(uartDef);
}

void initGPIO(){
	// setup hardware / gpio ports

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN ;       // enable ports A, B, C
	GPIOA->MODER |= (1 << 10) | (1 << 12) | (1 << 14) | (1 << 18) | (1 << 16) | (1 << 20);  // enable PORT A pins 5,6,7,9,8,10
	GPIOB->MODER |= (1 << 12) | (1 << 20) | (1 << 18) | (1 << 10) | (1 << 16);              // enable PORT B pins 6,10,4,5,3
	GPIOC->MODER |= (1 << 14) | (1 << 0);                                                   // enable PORT C pin 0,7
	GPIOA->OSPEEDR = GPIO_SPEED_HIGH;
	GPIOB->OSPEEDR = GPIO_SPEED_HIGH;
	GPIOC->OSPEEDR = GPIO_SPEED_HIGH;

	// set initial control lines values.
	GPIOC->ODR &= ~(1 << 0);  // latch low
	GPIOC->ODR &= ~(1 << 7);  // clock low
}
