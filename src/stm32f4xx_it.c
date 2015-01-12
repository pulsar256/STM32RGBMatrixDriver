#include "stm32f4xx_it.h"

extern volatile ITStatus uartReady;
extern UART_HandleTypeDef uartHandle;

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle) {
  uartReady = SET;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle) {
  uartReady = SET;
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle) {
	while(1);
}

void USARTx_DMA_RX_IRQHandler(void) {
  HAL_DMA_IRQHandler(uartHandle.hdmarx);
}

void USARTx_DMA_TX_IRQHandler(void) {
  HAL_DMA_IRQHandler(uartHandle.hdmatx);
}
