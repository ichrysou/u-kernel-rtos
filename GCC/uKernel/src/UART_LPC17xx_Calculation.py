import sys

PCLK = 100000000 / 4
BR = 115200#9600
DL_EST = PCLK / (16 * BR)

def table(loc_fr_est):
    fr_field = [1.000, 1.067, 1.071, 1.077, 1.083, 1.091, 1.100, 1.111, 1.125, 1.133, 1.143, 1.154, 1.167, 1.182, 1.200,
    1.214, 1.222, 1.231, 1.250, 1.267, 1.273, 1.286, 1.300, 1.308, 1.333, 1.357, 1.364, 1.375, 1.385, 1.400, 1.417, 1.429,
    1.444, 1.455, 1.462, 1.467,1.500, 1.533, 1.538, 1.545, 1.556, 1.571, 1.583, 1.600, 1.615, 1.625, 1.636, 1.643, 1.667,
    1.692, 1.700, 1.714, 1.727, 1.733, 1.750, 1.769, 1.778, 1.786, 1.800, 1.818, 1.833, 1.846, 1.857, 1.867, 1.875, 1.889,
    1.900, 1.909, 1.917, 1.923, 1.929, 1.933]
    DivVal = [0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 2, 1, 3, 2, 3, 1, 4, 3, 2, 3, 4, 1, 5, 4, 3, 5, 2, 5, 3, 4, 5, 6, 7, 1, 8, 7, 6, 5, 4, 7, 3, 8, 5, 7, 9, 2, 9, 7, 5, 8, 11, 3, 10, 7, 11, 4, 9, 5, 11, 6, 13, 7, 8, 9, 10, 11, 12, 13, 14]
    MulVal = [1, 15, 14, 13, 12, 11, 10, 9, 8, 15, 7, 13, 6, 11, 5, 14, 9, 13, 4, 15, 11, 7, 10, 13, 3, 14, 11, 8, 13, 5, 12, 7, 9, 11, 13, 15, 2, 15, 13, 11, 9, 7, 12, 5, 13, 8, 11, 14, 3, 13, 10, 7, 11, 15, 4, 13, 9, 14, 5, 11, 6, 13, 7, 15, 8, 9, 10, 11, 12, 13, 14, 15]
 
    for i in range(len(fr_field)):
        if (loc_fr_est < fr_field[i]):
            break
    if (i > 0):

        i = (i - 1) if ((loc_fr_est - fr_field[i-1]) < (fr_field[i] - loc_fr_est)) else i

    else:
        i = 0
    retDiv = DivVal[i]
    retMul = MulVal[i]
    return (retDiv, retMul)


def UART_BaudRate(PCLK, UnDLM, UnDLL, DivAddVal, MulVal):
    BaudRate = PCLK / (16 * (256*UnDLM + UnDLL) * (1 + DivAddVal / float(MulVal)))
    BaudRate = int(BaudRate)
    return BaudRate

DL_EST = PCLK/(16*BR)

if ((PCLK % (16 * BR)) == 0):
    DIVADDVAL = 0
    MULVAL = 1
else:
    FR_EST = 1.5
    cntr = 0
    FR_range = [r/1000.0 for r in range(1000,1900)] 
    while (True):
        DL_EST = int(PCLK/(16*BR*FR_EST))
        FR_EST = float(PCLK)/(16*BR*DL_EST)
        if (1.1 < FR_EST) and (FR_EST < 1.9):
            break;
        else:
            FR_EST = FR_range[cntr]
            if cntr < len(FR_range):
                cntr = cntr + 1
            else:
                print "FR_EST not found!"
                break;

    print "fr_EST value: ", FR_EST

    DIVADDVAL = table(FR_EST)[0]
    MULVAL = table(FR_EST)[1]

DL_EST_15_8 = (int(DL_EST) & (0xFF << 8)) >> 8
DL_EST_7_0  = (int(DL_EST) & (0xFF))

DLM = DL_EST_15_8
DLL = DL_EST_7_0

print "DIVADDVAL:", DIVADDVAL 
print "MULVAL   :", MULVAL    
print "DLM      :", DLM       
print "DLL      :", DLL       

print "Desired Baud Rate", BR
print "Acquired Baud Rate", UART_BaudRate(PCLK, DLM, DLL, DIVADDVAL, MULVAL)

