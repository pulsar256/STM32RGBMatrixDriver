#include <stdio.h>
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "math.h"
#include "string.h"
#include "stdlib.h"
#include "main.h"
#include "matrix_config.h"

const int waits[] = { 5, 10, 20, 40, 80, 160, 320, 640 };
const int scan = MATRIX_HEIGHT / 2;
uint8_t gammaTable[256];
uint32_t bufferA[MATRIX_SIZE];
uint32_t bufferB[MATRIX_SIZE];
uint32_t birthRate = 0;

// ----- Timing definitions -------------------------------------------------

int main() {
	setupRGBMatrixPorts();

	LED_PORT->BRR = LED_P;

	// precalculate the gamma lookup table
	for (int i = 0; i < 256; i++) gammaTable[i] = 255 * pow((i / 256.0), 1.6);

	// clear framebuffers
	memset(bufferA, 0, sizeof(bufferA));
	memset(bufferB, 0, sizeof(bufferB));


	// test pattern, light up a led in each corner
	bufferA[0]    = 0x00000050;
	bufferA[31]   = 0x00005000;
	bufferA[992]  = 0x00500000;
	bufferA[1023] = 0x00505000;

	// display test pattern for 500 frames
	for (int i = 0; i < 100; i++) {
		displayBuffer(bufferA);
	}

	LED_PORT->BSRR = LED_P;

	uint32_t* currentBuffer = bufferA;
	uint32_t* srcBuffer = bufferA;
	uint32_t* dstBuffer = bufferB;
	randomizeFramebuffer(currentBuffer);

	int frame = 0;
	while (1) {
		displayBuffer(currentBuffer);
		birthRate  = 0;
		int simRun = 0;

		if (++frame % 5 == 0)	{
			simRun = 1;
			processBuffer(srcBuffer,dstBuffer);

			currentBuffer = srcBuffer;
			srcBuffer     = dstBuffer;
			dstBuffer     = currentBuffer;
			currentBuffer = srcBuffer;
		}
		if (frame % 4000 == 0 || (birthRate < 10 && simRun == 1)) {
			randomizeFramebuffer(currentBuffer);
			LED_PORT->ODR ^= LED_P;
		}
	}
}

/**
 * Displays the buffer on the display using binary encoding (PWM equivalent).
 */
void displayBuffer(uint32_t buffer[]) {
	for (int s=0; s<scan; s++){
		setRow(s);
		int offset1 = MATRIX_WIDTH * s;
		int offset2 = MATRIX_WIDTH * (s+scan);
		for (int plane=0; plane < 8; plane ++) {
			for (int x=0; x<MATRIX_WIDTH; x++) {
				setRGB(buffer[offset1+x], buffer[offset2+x], plane);
				CLK_TOGGLE;
			}
			showLine(waits[plane]);
		}
	}
}

/**
 * generates some random junk for testing on the framebuffer.
 */
void randomizeFramebuffer(uint32_t buffer[]) {

	for (int i = 0; i < MATRIX_SIZE; i++) {
		buffer[i] = 0x00
			| ((gammaTable[rand() % 255]) << 0)
			| ((gammaTable[rand() % 255]) << 8)
			| ((gammaTable[rand() % 255]) << 16)
			| ((rand() % 255) << 24);
	}
}


/**
 * sets the row on the row gpio ports
 */
void setRow(int row) {
	// todo: perhaps a lookup table could give us a tiny boost here.

	if (row & 0b0001) MTX_PORT->BSRR = MTX_PA;
	else MTX_PORT->BRR = MTX_PA;

	if (row & 0b0010) MTX_PORT->BSRR = MTX_PB;
	else MTX_PORT->BRR = MTX_PB;

	if (row & 0b0100) MTX_PORT->BSRR = MTX_PC;
	else MTX_PORT->BRR = MTX_PC;

	if (row & 0b1000) MTX_PORT->BSRR = MTX_PD;
	else MTX_PORT->BRR = MTX_PD;
}

/**
 * loads rgb1 and rgb2 gpio ports with the given bitplane
 */
void setRGB(uint32_t rgb1, uint32_t rgb2, uint8_t plane) {
	// using bitshifting seems to be faster due to gcc optimization
	// than using a bitmask lookup table here.

	if (rgb1 & (1 << plane))        MTX_PORT->BSRR = MTX_PR0;
	else                            MTX_PORT->BRR  = MTX_PR0;

	if (rgb1 & (1 << (plane + 8))) 	MTX_PORT->BSRR = MTX_PG0;
	else                            MTX_PORT->BRR  = MTX_PG0;

	if (rgb1 & (1 << (plane + 16))) MTX_PORT->BSRR = MTX_PB0;
	else                            MTX_PORT->BRR  = MTX_PB0;

	if (rgb2 & (1 << plane))        MTX_PORT->BSRR = MTX_PR1;
	else                            MTX_PORT->BRR  = MTX_PR1;

	if (rgb2 & (1 << (plane + 8))) 	MTX_PORT->BSRR = MTX_PG1;
	else                            MTX_PORT->BRR  = MTX_PG1;

	if (rgb2 & (1 << (plane + 16))) MTX_PORT->BSRR = MTX_PB1;
	else                            MTX_PORT->BRR  = MTX_PB1;
}


/**
 * strobes / shows a line for a n*nop amount of time.
 */
void showLine(int amount) {
	STROBE;
	DISP_ON;
	for (int c=0; c<amount; c++) asm("nop");
	DISP_OFF;
}

void processBuffer(uint32_t src[], uint32_t dst[]){
	// apply GOF rules on src and store result in dst.
	for (int i=0; i<MATRIX_SIZE; i++){
		CellAction action = analyzeCell(i,src);
		if (COPY == action ){
			dst[i] = src[i];
		}
		else if (NEW == action ){
			dst[i] = ((gammaTable[rand() % 255]) << 0) | ((gammaTable[rand() % 255]) << 8) | ((gammaTable[rand() % 255]) << 16) | ((1) << 24);
			birthRate++;
		}
		else if (KILL == action ){
			dst[i] = 0x00ffffff & src[i];
		}
	}

	// fade out dead cells
	for (int i=0; i<MATRIX_SIZE; i++){
		if (! (0x01000000 & dst[i]) ){
			dst[i] =  (((dst[i]       & 0x000000ff) >> 1))       |
	              (((dst[i] >> 8  & 0x000000ff) >> 1) << 8)  |
	              (((dst[i] >> 16 & 0x000000ff) >> 1) << 16);
		}
	}
}

CellAction analyzeCell(int offset, uint32_t buffer[]){
	// skip the first row, first column, last column and last row to make alive neighbor
	// detection easier.
	if ( ( offset    < MATRIX_WIDTH) ||
	     ( offset    > MATRIX_SIZE - MATRIX_WIDTH ) ||
	     ( offset    % MATRIX_WIDTH == 0) ||
	     ((offset+1) % MATRIX_WIDTH == 0)) return KILL;

	int neighbors = 0;
	int alive = buffer[offset] & 0x01000000;

	if (buffer[offset-1] & 0x1000000) neighbors ++;
	if (buffer[offset+1] & 0x1000000) neighbors ++;

	offset -= MATRIX_WIDTH;
	if (buffer[offset-1] & 0x1000000) neighbors ++;
	if (buffer[offset]   & 0x1000000) neighbors ++;
	if (buffer[offset+1] & 0x1000000) neighbors ++;

	offset += MATRIX_WIDTH*2;
	if (buffer[offset-1] & 0x1000000) neighbors ++;
	if (buffer[offset]   & 0x1000000) neighbors ++;
	if (buffer[offset+1] & 0x1000000) neighbors ++;

	return (neighbors < 2) ? KILL :
	       (alive && (neighbors == 2 || neighbors == 3)) ? COPY :
	       (neighbors > 3) ? KILL :
	       (!alive && neighbors == 3) ? NEW :
	       COPY;
}

void setupRGBMatrixPorts() {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(MTX_RCCPB, ENABLE);                        // enable matrix GPIO port
	RCC_APB2PeriphClockCmd(LED_RCCPB, ENABLE);                        // enable LED GPIO port
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);              // enable AFIO module, needed to release (remap the AlternateFunction) the JTAG Pins

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 					// disable JTAG Debugging, we only need SWD. Releases PB3 and PB4

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

	GPIO_InitStructure.GPIO_Pin = MTX_PR0 | MTX_PG0 | MTX_PB0;        // RGB0
	GPIO_Init(MTX_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = MTX_PR1 | MTX_PG1 | MTX_PB1;        // RGB1
	GPIO_Init(MTX_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = MTX_PA | MTX_PB | MTX_PC | MTX_PD;  // ABCD
	GPIO_Init(MTX_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = MTX_PSTB | MTX_POE | MTX_PCLK;      // Strobe, OutputEnable, Clock
	GPIO_Init(MTX_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LED_P;                              // Status LED
	GPIO_Init(LED_PORT, &GPIO_InitStructure);
}


