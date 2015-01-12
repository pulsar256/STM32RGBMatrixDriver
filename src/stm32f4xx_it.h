#ifndef __STMF4XX_H
#define __STMF4XX_H

#include "main.h"
#include "stm32f4xx_hal.h"

/**
 * IRQ Handlers, TODO: move into a conventional stmf4xx_it.c file.
 */
void USARTx_DMA_RX_IRQHandler(void);

void USARTx_DMA_TX_IRQHandler(void);

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle);

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle);

void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle);


#endif
