// Welch, Wright, & Morrow, 
// Real-time Digital Signal Processing, 2017

/* coeff_HP.c                          */
/* FIR filter coefficients             */
/* exported by MATLAB using FIR_DUMP2C */

#include "coeff.h"
#include "coeff_HP.h"

float B_HP[N+1] = {
-0.000140684160,	/* h[0] */
-0.000152795472,	/* h[1] */
0.000183608678,	/* h[2] */
0.000244940436,	/* h[3] */
-0.000218953309,	/* h[4] */
-0.000365042157,	/* h[5] */
0.000239774902,	/* h[6] */
0.000514767028,	/* h[7] */
-0.000237905270,	/* h[8] */
-0.000694361566,	/* h[9] */
0.000204112112,	/* h[10] */
0.000902310563,	/* h[11] */
-0.000128335980,	/* h[12] */
-0.001135019633,	/* h[13] */
0.000000000000,	/* h[14] */
0.001386537558,	/* h[15] */
0.000191615037,	/* h[16] */
-0.001648332168,	/* h[17] */
-0.000456941700,	/* h[18] */
0.001909130494,	/* h[19] */
0.000805658226,	/* h[20] */
-0.002154829653,	/* h[21] */
-0.001246200526,	/* h[22] */
0.002368479104,	/* h[23] */
0.001785274772,	/* h[24] */
-0.002530327494,	/* h[25] */
-0.002427392999,	/* h[26] */
0.002617917869,	/* h[27] */
0.003174454683,	/* h[28] */
-0.002606202965,	/* h[29] */
-0.004025396657,	/* h[30] */
0.002467636438,	/* h[31] */
0.004975931920,	/* h[32] */
-0.002172174039,	/* h[33] */
-0.006018395041,	/* h[34] */
0.001687086800,	/* h[35] */
0.007141707933,	/* h[36] */
-0.000976438761,	/* h[37] */
-0.008331474980,	/* h[38] */
0.000000000000,	/* h[39] */
0.009570211080,	/* h[40] */
0.001288776865,	/* h[41] */
-0.010837700319,	/* h[42] */
-0.002946350938,	/* h[43] */
0.012111476994,	/* h[44] */
0.005045401885,	/* h[45] */
-0.013367414938,	/* h[46] */
-0.007686046332,	/* h[47] */
0.014580405712,	/* h[48] */
0.011016464747,	/* h[49] */
-0.015725101677,	/* h[50] */
-0.015272954203,	/* h[51] */
0.016776696311,	/* h[52] */
0.020863962331,	/* h[53] */
-0.017711711763,	/* h[54] */
-0.028566288115,	/* h[55] */
0.018508762513,	/* h[56] */
0.040057473772,	/* h[57] */
-0.019149264392,	/* h[58] */
-0.059725292797,	/* h[59] */
0.019618059959,	/* h[60] */
0.103713444904,	/* h[61] */
-0.019903934336,	/* h[62] */
-0.317508587540,	/* h[63] */
0.520000000000,	/* h[64] */
-0.317508587540,	/* h[65] */
-0.019903934336,	/* h[66] */
0.103713444904,	/* h[67] */
0.019618059959,	/* h[68] */
-0.059725292797,	/* h[69] */
-0.019149264392,	/* h[70] */
0.040057473772,	/* h[71] */
0.018508762513,	/* h[72] */
-0.028566288115,	/* h[73] */
-0.017711711763,	/* h[74] */
0.020863962331,	/* h[75] */
0.016776696311,	/* h[76] */
-0.015272954203,	/* h[77] */
-0.015725101677,	/* h[78] */
0.011016464747,	/* h[79] */
0.014580405712,	/* h[80] */
-0.007686046332,	/* h[81] */
-0.013367414938,	/* h[82] */
0.005045401885,	/* h[83] */
0.012111476994,	/* h[84] */
-0.002946350938,	/* h[85] */
-0.010837700319,	/* h[86] */
0.001288776865,	/* h[87] */
0.009570211080,	/* h[88] */
0.000000000000,	/* h[89] */
-0.008331474980,	/* h[90] */
-0.000976438761,	/* h[91] */
0.007141707933,	/* h[92] */
0.001687086800,	/* h[93] */
-0.006018395041,	/* h[94] */
-0.002172174039,	/* h[95] */
0.004975931920,	/* h[96] */
0.002467636438,	/* h[97] */
-0.004025396657,	/* h[98] */
-0.002606202965,	/* h[99] */
0.003174454683,	/* h[100] */
0.002617917869,	/* h[101] */
-0.002427392999,	/* h[102] */
-0.002530327494,	/* h[103] */
0.001785274772,	/* h[104] */
0.002368479104,	/* h[105] */
-0.001246200526,	/* h[106] */
-0.002154829653,	/* h[107] */
0.000805658226,	/* h[108] */
0.001909130494,	/* h[109] */
-0.000456941700,	/* h[110] */
-0.001648332168,	/* h[111] */
0.000191615037,	/* h[112] */
0.001386537558,	/* h[113] */
0.000000000000,	/* h[114] */
-0.001135019633,	/* h[115] */
-0.000128335980,	/* h[116] */
0.000902310563,	/* h[117] */
0.000204112112,	/* h[118] */
-0.000694361566,	/* h[119] */
-0.000237905270,	/* h[120] */
0.000514767028,	/* h[121] */
0.000239774902,	/* h[122] */
-0.000365042157,	/* h[123] */
-0.000218953309,	/* h[124] */
0.000244940436,	/* h[125] */
0.000183608678,	/* h[126] */
-0.000152795472,	/* h[127] */
-0.000140684160,	/* h[128] */
};
