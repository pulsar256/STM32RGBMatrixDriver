#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_dma.h"
#include "setup_hw.h"

#include <stdlib.h>
#include <math.h>

void setRow(int row);
void showLine(int amount);
void setRGB(uint32_t rgb1, uint32_t rgb2, uint8_t plane );
void displayBuffer(uint32_t buffer[]);
void randomizeFramebuffer(uint32_t buffer[]);

#define DISP_ON         GPIOB->BSRRH = GPIO_PIN_10;
#define DISP_OFF        GPIOB->BSRRL = GPIO_PIN_10;
#define CLK_TOGGLE      GPIOC->ODR ^= (1 << 7); GPIOC->ODR ^= (1 << 7);
#define STROBE          GPIOC->BSRRL = GPIO_PIN_0; GPIOC->BSRRH = GPIO_PIN_0;

#define MATRIX_WIDTH    32
#define MATRIX_HEIGHT   32
#define MATRIX_SIZE     MATRIX_WIDTH*MATRIX_HEIGHT

#define USARTx                           USART6
#define USARTx_TX_PIN                    GPIO_PIN_11
#define USARTx_RX_PIN                    GPIO_PIN_12
#define USARTx_TX_GPIO_PORT              GPIOA
#define USARTx_RX_GPIO_PORT              GPIOA
#define USARTx_TX_AF                     GPIO_AF8_USART6
#define USARTx_RX_AF                     GPIO_AF8_USART6
#define USARTx_TX_DMA_CHANNEL            DMA_CHANNEL_5
#define USARTx_RX_DMA_CHANNEL            DMA_CHANNEL_5
#define USARTx_RX_DMA_STREAM             DMA2_Stream1
#define USARTx_TX_DMA_STREAM             DMA2_Stream6
#define USARTx_DMA_RX_IRQn               DMA2_Stream1_IRQn
#define USARTx_DMA_TX_IRQn               DMA2_Stream6_IRQn
#define USARTx_DMA_RX_IRQHandler         DMA2_Stream1_IRQHandler
#define USARTx_DMA_TX_IRQHandler         DMA2_Stream6_IRQHandler

#define USARTx_CLK_ENABLE()              __USART6_CLK_ENABLE();
#define DMAx_CLK_ENABLE()                __DMA2_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()
#define USARTx_FORCE_RESET()             __USART6_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __USART6_RELEASE_RESET()

#endif /* __MAIN_H */
