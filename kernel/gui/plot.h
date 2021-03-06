#ifndef GUI_H
#define GUI_H

#include <gui/lv_conf.h>
#include <stdint.h>
#include <gui/gui.h>

#define VRAM_START 0
// #define VRAM_END   0xB0000
#define SCRNX LV_HOR_RES_MAX
#define SCRNY LV_VER_RES_MAX

#define __DATA__ __attribute__((section(".data")))

                      
// void InitScreen();

// void PlotPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
void PlotAddr(int addr, uint8_t color);


void sys_flush(ROI* roi);


#endif
