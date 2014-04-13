#include "lpc17xx_gpio.h"
#include "dotmatgl.h"

//typedef digit_t unsigned char[];
//digit_t digits[10] = {{0x1F, 0x11, 0x11, 0x1F} ,{0x0, 0x1F, 0x0}, {0x17, 0x15, 0x15, 0x1d}, {}}

void dotmat_init()
{
	GPIO_SetDir(2, ROW_PORT_PINS, 1);
	GPIO_ClearValue(2, ROW_PORT_PINS);

	GPIO_SetDir(0, COL_PORT_PINS, 1);
	GPIO_SetValue(0, COL_PORT_PINS);

}

void plot_col(int col)
{
	if (col < MAT_MAX_COL)
		COL_PORT->FIOPIN &= ~((0x10 << col));

}
void unplot_col(int col)
{
	if (col < MAT_MAX_COL)
		COL_PORT->FIOPIN |= (0x10 << col);/*0x7F0*/
}

void plot_row(int row)
{
	if (row < MAT_MAX_ROW)
		ROW_PORT->FIOPIN |= (0x1 << 1) << row;
}

void unplot_row(int row)
{
	if (row < MAT_MAX_ROW)
			ROW_PORT->FIOPIN &= ~((0x1 << 1) << row);
}

void plot_val(unsigned int val)
{

	VAL_PORT->FIOPIN = val << 1;
}

void plot_xy(int col, int row)
{
	plot_col(col);
	plot_row(row);
}
void unplot_xy(int col, int row)
{
	unplot_col(col);
	unplot_row(row);

}



