#ifndef _SINX_LUT_91_     // Guards against multiple inclusion#define _SINX_LUT_91_#define FIRST_QUADRANT_SIN_LUT_LENGTH 91static const fract_q15_t _1stQuadrantSin_LUT[] =  //1x91 LUT, Sin(x) in Q0.15{ 0x0000, 0x023C, 0x0478, 0x06B3, 0x08EE, 0x0B28, 0x0D61, 0x0F99, 0x11D0,  0x1406 , 0x163A, 0x186C, 0x1A9D, 0x1CCB, 0x1EF7, 0x2121, 0x2348, 0x256C,  0x278E, 0x29AC, 0x2BC7, 0x2DDF, 0x2FF3, 0x3203, 0x3410, 0x3618, 0x381D,  0x3A1C, 0x3C18 , 0x3E0E, 0x4000, 0x41ED, 0x43D4, 0x45B7, 0x4794, 0x496B,  0x4B3D, 0x4D08, 0x4ECE, 0x508E, 0x5247, 0x53FA, 0x55A6, 0x574C, 0x58EB,  0x5A82, 0x5C13, 0x5D9D , 0x5F1F, 0x609A, 0x620E, 0x637A, 0x64DE, 0x663A,  0x678E, 0x68DA, 0x6A1E, 0x6B5A, 0x6C8D, 0x6DB8, 0x6EDA, 0x6FF4, 0x7104,  0x720D, 0x730C, 0x7402, 0x74EF , 0x75D3, 0x76AE, 0x7780, 0x7848, 0x7907,  0x79BC, 0x7A68, 0x7B0B, 0x7BA3, 0x7C33, 0x7CB8, 0x7D34, 0x7DA6, 0x7E0E,  0x7E6D, 0x7EC1, 0x7F0C, 0x7F4C, 0x7F83 , 0x7FB0, 0x7FD3, 0x7FEC, 0x7FFB,  0x7FFF };#endif//def _SINX_LUT_91_
