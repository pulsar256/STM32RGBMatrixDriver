#ifndef __MAIN_H
#define __MAIN_H

void setupRGBMatrixPorts(void);
void displayBuffer(uint32_t buffer[]);
void randomizeFramebuffer(uint32_t buffer[]);
void setRow(int row);
void setRGB(uint32_t rgb1, uint32_t rgb2, uint8_t plane);
void showLine(int amount);

#endif
