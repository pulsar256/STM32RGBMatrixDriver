#ifndef __MAIN_H
#define __MAIN_H

typedef enum { KILL, COPY, NEW } CellAction;

void setupRGBMatrixPorts(void);
void displayBuffer(uint32_t buffer[]);
void randomizeFramebuffer(uint32_t buffer[]);
void setRow(int row);
void setRGB(uint32_t rgb1, uint32_t rgb2, uint8_t plane);
void showLine(int amount);
void processBuffer(uint32_t src[], uint32_t dst[]);
CellAction analyzeCell(int offset, uint32_t buffer[]);

#endif
