#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_dma.h"

#include <stdlib.h>
#include <math.h>

#include "setup_hw.h"

#define USARTx                           USART6
#define USARTx_CLK_ENABLE()              __USART6_CLK_ENABLE();
#define DMAx_CLK_ENABLE()                __DMA2_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __USART6_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __USART6_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_11
#define USARTx_RX_PIN                    GPIO_PIN_12
#define USARTx_TX_GPIO_PORT              GPIOA
#define USARTx_RX_GPIO_PORT              GPIOA
#define USARTx_TX_AF                     GPIO_AF8_USART6
#define USARTx_RX_AF                     GPIO_AF8_USART6

#define USARTx_TX_DMA_CHANNEL             DMA_CHANNEL_5
#define USARTx_RX_DMA_CHANNEL             DMA_CHANNEL_5
#define USARTx_RX_DMA_STREAM              DMA2_Stream1
#define USARTx_TX_DMA_STREAM              DMA2_Stream6
#define USARTx_DMA_RX_IRQn                DMA2_Stream1_IRQn
#define USARTx_DMA_TX_IRQn                DMA2_Stream6_IRQn
#define USARTx_DMA_RX_IRQHandler          DMA2_Stream1_IRQHandler
#define USARTx_DMA_TX_IRQHandler          DMA2_Stream6_IRQHandler

#define TXBUFFERSIZE                      (COUNTOF(aTxBuffer) - 1)
#define RXBUFFERSIZE                      TXBUFFERSIZE
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
