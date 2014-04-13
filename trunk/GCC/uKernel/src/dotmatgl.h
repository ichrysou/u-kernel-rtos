#ifndef __DOTMATGL_H__
#define __DOTMATGL_H__

#include "lpc17xx_clkpwr.h"
#include "lpc17xx_gpio.h"
#define MAT_MAX_COL 7
#define MAT_MAX_ROW 5
#define COL_PORT	LPC_GPIO0
#define ROW_PORT	LPC_GPIO2
#define VAL_PORT	ROW_PORT
#define COL_PORT_PINS (0x07F << 4)
#define ROW_PORT_PINS (0x1F << 1)
#define COL_POLARITY 0
#define ROW_POLARITY 1


void plot_col(int col);
void plot_row(int row);
void plot_val(unsigned int val);
void plot_xy(int col, int row);
void unplot_col(int col);
void unplot_row(int row);
void unplot_xy(int col, int row);


#endif
