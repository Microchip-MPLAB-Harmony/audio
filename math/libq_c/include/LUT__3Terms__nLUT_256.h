/*******************************************************************************
* Copyright (C) 2022 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

#ifndef _LIBQC_H_LUT__3TERMS__NLUT_256_     // Guards against multiple inclusion
#define _LIBQC_H_LUT__3TERMS__NLUT_256_

// SixteenBitResults = 1,  nLUT = 256
// Max Abs Error: 7.38672e-05, 1/Max: 13537.8, Avg: 2.13971e-05, 1/Avg: 46735.3
// Max Rel Error: 0.000146843, 1/Max: 6809.98, Avg: 3.20995e-05, 1/Avg: 31153.2
// LIBQC_2toMinusX Lookup Table, nTaylorTerms = 3, nLUT = 256, epsilonLUT = 0.5*(0:1:nLUT)/nLUT;
#define N_2toMinusX_LUT 256
static const fract_q31_t  _2toMinusX_LUT[] = //1x257 lookup table
   {       0,          7,         19,         42,         81,        141,        227,        345,
         501,        699,        944,       1243,       1599,       2019,       2508,       3070,
        3712,       4438,       5253,       6163,       7173,       8289,       9514,      10855,
       12317,      13904,      15623,      17477,      19473,      21615,      23909,      26359,
       28971,      31750,      34701,      37829,      41139,      44636,      48326,      52212,
       56302,      60598,      65108,      69834,      74784,      79961,      85370,      91017,
       96907,     103044,     109434,     116082,     122992,     130169,     137619,     145347,
      153357,     161654,     170244,     179131,     188320,     197816,     207624,     217748,
      228195,     238968,     250073,     261514,     273296,     285425,     297904,     310740,
      323936,     337497,     351429,     365736,     380423,     395495,     410957,     426813,
      443068,     459727,     476795,     494276,     512176,     530499,     549249,     568432,
      588053,     608115,     628624,     649585,     671002,     692880,     715223,     738037,
      761325,     785093,     809346,     834088,     859323,     885057,     911293,     938037,
      965294,     993067,    1021362,    1050183,    1079535,    1109422,    1139849,    1170821,
     1202342,    1234416,    1267049,    1300245,    1334008,    1368343,    1403255,    1438747,
     1474826,    1511494,    1548757,    1586620,    1625086,    1664160,    1703847,    1744152,
     1785078,    1826630,    1868813,    1911631,    1955088,    1999190,    2043940,    2089343,
     2135404,    2182126,    2229515,    2277574,    2326309,    2375723,    2425821,    2476607,
     2528086,    2580262,    2633140,    2686724,    2741017,    2796026,    2851753,    2908204,
     2965382,    3023292,    3081939,    3141326,    3201458,    3262340,    3323975,    3386368,
     3449524,    3513446,    3578138,    3643606,    3709853,    3776884,    3844702,    3913313,
     3982720,    4052928,    4123941,    4195763,    4268398,    4341851,    4416125,    4491226,
     4567157,    4643922,    4721526,    4799973,    4879267,    4959411,    5040412,    5122271,
     5204995,    5288586,    5373049,    5458389,    5544608,    5631712,    5719705,    5808590,
     5898372,    5989055,    6080643,    6173140,    6266550,    6360878,    6456126,    6552301,
     6649404,    6747442,    6846416,    6946333,    7047195,    7149008,    7251774,    7355498,
     7460183,    7565835,    7672457,    7780052,    7888626,    7998181,    8108722,    8220254,
     8332779,    8446302,    8560827,    8676358,    8792899,    8910454,    9029026,    9148620,
     9269239,    9390888,    9513571,    9637291,    9762052,    9887859,   10014715,   10142623,
    10271589,   10401616,   10532707,   10664867,   10798100,   10932409,   11067798,   11204271,
    11341832,   11480486,   11620234,   11761083,   11903034,   12046093,   12190263,   12335548,
    12481951,   12629477,   12778129,   12927911,   13078828,   13230881,   13384076,   13538417,
    13693906 };

#endif//def _LIBQC_H_LUT__3TERMS__NLUT_256_
