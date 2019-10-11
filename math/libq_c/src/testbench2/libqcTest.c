/******************************************************************************
 *
 * File: fractTest.c
 *
 * Description: Aggregation of both known internal fractional library tests.
 *
 * Notes:
 *   1.)  There are four #ifdef values as it currently stands:
 *        EX_TIMER - If on, collects the time needed to run over several
 *                   thousand iterations. This only works with Windows targets.
 *        OUTPUT_ERRORS - Prints error status.  This is independent of the
 *                        DETAIL_OUTPUT flag.
 *        DETAIL_OUTPUT - Prints status information about each test.  Also, if
 *                        on, it will print error status.
 *        MULTIPASS -     Runs the tests multiple times.
 *
 *   2.)  Warning!!! This test does operate correctly on the C54xxSimulator
 *        (sim5410.cfg), or the C5416DeviceSimulator (SIM5416.cfg) in Code
 *        Composer 2.2.  It fails some of the frdsssMsuR and frdssdMsu tests
 *        when the C54 Intrinsics are used.  It does not saturate correctly.
 *        It does however pass on the real C5410 and C5416 Hardware.
 *
 * Revision History:
 * 13 Oct 03 - SC - Inital checkin
 * 15 Oct 03 - DP - Added sourcesafe info to header and MULTIPASS define.
 *                  Moved the individual hardcoded tests into the data
 *                  structures for standard loop testing.  Added tests
 *                  for the routines not previously tested and removed
 *                  testing of frdsSat routine.  Fixed bad expected s.
 * 16 Oct 03 - DP - Changed 0x8000 values etc, to ATIMIN types.
 * 13 Nov 03 - DP - Add more shift tests.
 * 30 Mar 04 - FHS - added fractmat library tests
 *  6 Apr 04 - FHS - restored to fract library testing only (removed fractmat)
 * 09 Aug 04 - SBS - Branched from Scorpion 2.
 *-----------------------------------------------------------------------------
 * 29 Aug 07 - JLA - Migrated from Gila database.
 * 29 Aug 07 - JLA - Deleted timing code to remove library dependencies so that
 *                   it will more likely compile on various platforms.  Also
 *                   added output to a text file in addition to the output to
 *                   the screen.
 *
 ******************************************************************************
 * Acoustic Technologies, Inc., copyright 2002-2007
 * All rights reserved.
 ******************************************************************************
 * ------------------------------
 * Source Safe Revision History:
 * ------------------------------
 * $Log: /SYSTEMS/libraries/fract/testBench/fractTest.c $
 *       
 *       2     8/31/07 10:07a Jallen
 *       Added revision history comments that I should have put there before.
 *
 *       1     8/30/07 9:24a Jallen
 *       Initial Release - migrated from the Gila database.
 *
 *       1     8/09/04 1:03p Ssuppappola
 *       Branched from Scorpion 2.
 *
 * $NoKeywords: $
 ******************************************************************************/

/* Include Files for all Targets */
#include <stdio.h>
#include "atitypes.h"
#include "fract.h"

/* Defines for error reporting */
#undef DUMP_ERRORS
#ifdef DETAIL_OUTPUT
#define DUMP_ERRORS
#endif
#ifdef OUTPUT_ERRORS
#define DUMP_ERRORS
#endif

/* Variables used for holding fractional return values */
volatile AtiFract32  fr32;
volatile AtiFract16  fr16;
volatile AtiInt16    ires;
volatile AtiFract16  test16;


typedef struct _TestDataSS {
  AtiFract16 op1;
  AtiFract16 result;
} TestDataSS;

typedef struct _TestDataDD {
  AtiFract32 op1;
  AtiFract32 result;
} TestDataDD;

typedef struct _TestDataDS {
  AtiFract32 op1;
  AtiFract16 result;
} TestDataDS;

typedef struct _TestDataSD {
  AtiFract16 op1;
  AtiFract32 result;
} TestDataSD;

typedef struct _TestDataSSS {
  AtiFract16 op1;
  AtiFract16 op2;
  AtiFract16 result;
} TestDataSSS;

typedef struct _TestDataSSD {
  AtiFract16 op1;
  AtiFract16 op2;
  AtiFract32 result;
} TestDataSSD;

typedef struct _TestDataDDD {
  AtiFract32 op1;
  AtiFract32 op2;
  AtiFract32 result;
} TestDataDDD;

typedef struct _TestDataSI {
  AtiFract16 op1;
  AtiInt16   result;
} TestDataSI;

typedef struct _TestDataDI {
  AtiFract32 op1;
  AtiInt16   result;
} TestDataDI;

typedef struct _TestDataSIS {
  AtiFract16 op1;
  AtiInt16   op2;
  AtiFract16 result;
} TestDataSIS;

typedef struct _TestDataDID {
  AtiFract32 op1;
  AtiInt16   op2;
  AtiFract32 result;
} TestDataDID;

typedef struct _TestDataDSSD {
  AtiFract32 op1;
  AtiFract16 op2;
  AtiFract16 op3;
  AtiFract32 result;
} TestDataDSSD;

typedef struct _TestDataDSSS {
  AtiFract32 op1;
  AtiFract16 op2;
  AtiFract16 op3;
  AtiFract16 result;
} TestDataDSSS;

/* Define Test Data for frssAbs() */
#define ABSTESTNUM  10
const TestDataSS absTestData[ABSTESTNUM] =
{
  { ATIMINFRACT16,  ATIMAXFRACT16 },  // 0
  {            -3,              3 },  // 1
  {             0,              0 },  // 2
  {             2,              2 },  // 3
  {          -459,            459 },  // 4
  { ATIMAXFRACT16,  ATIMAXFRACT16 },  // 5
  { ATIMINFRACT16,  ATIMAXFRACT16 },  // 6
  {        -32767,  ATIMAXFRACT16 },  // 7
  {         32766,          32766 },  // 8
  {        -32766,          32766 }   // 9
};
/* Define Test Data for frddAbs() */
#define LABSTESTNUM  10
const TestDataDD labsTestData[LABSTESTNUM] =
{
  { ATIMINFRACT32,  ATIMAXFRACT32 },  // 0
  {            -3,              3 },  // 1
  {             0,              0 },  // 2
  {             2,              2 },  // 3
  {          -459,            459 },  // 4
  { ATIMAXFRACT32,  ATIMAXFRACT32 },  // 5
  { ATIMINFRACT32,  ATIMAXFRACT32 },  // 6
  {   -2147483647,  ATIMAXFRACT32 },  // 7
  {    2147483646,     2147483646 },  // 8
  {   -2147483646,     2147483646 }   // 9
};
/* Define Test Data for frssNegate() */
#define NEGTESTNUM  4
const TestDataSS negTestData[NEGTESTNUM] =
{
  { ATIMINFRACT16,  ATIMAXFRACT16 },  // 0
  { ATIMAXFRACT16,         -32767 },  // 1
  {           256,           -256 },  // 2
  {          -256,            256 }   // 3
};
/* Define Test Data for frddNegate() */
#define LNEGTESTNUM  4
const TestDataDD lnegTestData[LNEGTESTNUM] =
{
  { ATIMINFRACT32,  ATIMAXFRACT32 },  // 0
  { ATIMAXFRACT32,    -2147483647 },  // 1
  {      16777216,      -16777216 },  // 2
  {     -16777216,       16777216 }   // 3
};
/* Define Test Data for frdsRound() */
#define RNDTESTNUM  11
const TestDataDS rndTestData[RNDTESTNUM] =
{
  { ATIMINFRACT32,  ATIMINFRACT16 },  // 0
  {   -2147418113,         -32767 },  // 1
  { ATIMAXFRACT32,  ATIMAXFRACT16 },  // 2
  {    2147418111,  ATIMAXFRACT16 },  // 3
  {    2147385343,          32766 },  // 4
  {    1074724863,          16399 },  // 5
  {        131071,              2 },  // 6
  {            -1,              0 },  // 7
  {        -65537,             -1 },  // 8
  {         65535,              1 },  // 9
  {         32767,              0 }   // 10
};
/* Define Test Data for frsdDepositH() */
#define DEPOSHTESTNUM  3
const TestDataSD deposhTestData[DEPOSHTESTNUM] =
{
  {        -21555,    -1412628480 },  // 0
  { ATIMINFRACT16,  ATIMINFRACT32 },  // 1
  { ATIMAXFRACT16,     2147418112 }   // 2
};
/* Define Test Data for frsdDepositL() */
#define DEPOSLTESTNUM  3
const TestDataSD deposlTestData[DEPOSLTESTNUM] =
{
  {        -21555,         -21555 },  // 0
  { ATIMINFRACT16,         -32768 },  // 1
  { ATIMAXFRACT16,          32767 }   // 2
};
/* Define Test Data for frdsExtractH() */
#define EXTHTESTNUM  4
const TestDataDS exthTestData[EXTHTESTNUM] =
{
  {     305419896,           4660 },  // 0
  { ATIMAXFRACT32,  ATIMAXFRACT16 },  // 1
  { ATIMINFRACT32,  ATIMINFRACT16 },  // 2
  {            -1,             -1 }   // 3
};
/* Define Test Data for frdsExtractL() */
#define EXTLTESTNUM  4
const TestDataDS extlTestData[EXTLTESTNUM] =
{
  {     305419896,          22136 },  // 0
  { ATIMAXFRACT32,             -1 },  // 1
  { ATIMINFRACT32,              0 },  // 2
  {            -1,             -1 }   // 3
};
/* Define Test Data for frsssAdd() */
#define ADDTESTNUM  31
const TestDataSSS addTestData[ADDTESTNUM] =
{
  { ATIMINFRACT16,          -256, ATIMINFRACT16 },  // 0
  { ATIMAXFRACT16,          7936, ATIMAXFRACT16 },  // 1
  {          4095,            16,          4111 },  // 2
  { ATIMAXFRACT16, ATIMAXFRACT16, ATIMAXFRACT16 },  // 3
  {             1, ATIMAXFRACT16, ATIMAXFRACT16 },  // 4
  { ATIMINFRACT16, ATIMINFRACT16, ATIMINFRACT16 },  // 5
  {             0,             0,             0 },  // 6
  {             7,             4,            11 },  // 7
  {             4,             6,            10 },  // 8
  {             1,             1,             2 },  // 9
  {            -7,             4,            -3 },  // 10
  {             4,            -6,            -2 },  // 11
  {            -1,            -3,            -4 },  // 12
  {             7,            -4,             3 },  // 13
  {            -4,             6,             2 },  // 14
  {             0,            -1,            -1 },  // 15
  { ATIMAXFRACT16,             1, ATIMAXFRACT16 },  // 16
  { ATIMAXFRACT16, ATIMAXFRACT16, ATIMAXFRACT16 },  // 17
  {            -1, ATIMAXFRACT16,         32766 },  // 18
  {         32766,             2, ATIMAXFRACT16 },  // 19
  {             1,         32766, ATIMAXFRACT16 },  // 20
  { ATIMINFRACT16,            -1, ATIMINFRACT16 },  // 21
  { ATIMINFRACT16, ATIMINFRACT16, ATIMINFRACT16 },  // 22
  {             1, ATIMINFRACT16,        -32767 },  // 23
  {        -32767,            -2, ATIMINFRACT16 },  // 24
  {            -1,        -32766,        -32767 },  // 25
  {        -32767,            -1, ATIMINFRACT16 },  // 26
  { ATIMINFRACT16, ATIMAXFRACT16,            -1 },  // 27
  { ATIMAXFRACT16, ATIMINFRACT16,            -1 },  // 28
  {             0, ATIMINFRACT16, ATIMINFRACT16 },  // 29
  {             0, ATIMAXFRACT16, ATIMAXFRACT16 }   // 30
};
/* Define Test Data for frdddAdd() */
#define LADDTESTNUM 28
const TestDataDDD laddTestData[LADDTESTNUM] =
{
  { ATIMINFRACT32,            -1, ATIMINFRACT32 },  // 0
  { ATIMINFRACT32, ATIMINFRACT32, ATIMINFRACT32 },  // 1
  {     536870912, ATIMAXFRACT32, ATIMAXFRACT32 },  // 2
  {             0,             0,             0 },  // 3
  {             7,             4,            11 },  // 4
  {             4,             6,            10 },  // 5
  {             1,             1,             2 },  // 6
  {            -7,             4,            -3 },  // 7
  {             4,            -6,            -2 },  // 8
  {            -1,            -3,            -4 },  // 9
  {             7,            -4,             3 },  // 10
  {            -4,             6,             2 },  // 11
  {             0,            -1,            -1 },  // 12
  { ATIMAXFRACT32,             1, ATIMAXFRACT32 },  // 13
  { ATIMAXFRACT32, ATIMAXFRACT32, ATIMAXFRACT32 },  // 14
  {            -1, ATIMAXFRACT32,    2147483646 },  // 15
  {    2147483646,             2, ATIMAXFRACT32 },  // 16
  {             1,    2147483645,    2147483646 },  // 17
  { ATIMINFRACT32,            -1, ATIMINFRACT32 },  // 18
  { ATIMINFRACT32, ATIMINFRACT32, ATIMINFRACT32 },  // 19
  {             1, ATIMINFRACT32,   -2147483647 },  // 20
  {   -2147483647,            -2, ATIMINFRACT32 },  // 21
  {            -1,   -2147483646,   -2147483647 },  // 22
  {   -2147483647,            -1, ATIMINFRACT32 },  // 23
  { ATIMINFRACT32, ATIMAXFRACT32,            -1 },  // 24
  { ATIMAXFRACT32, ATIMINFRACT32,            -1 },  // 25
  {             0, ATIMINFRACT32, ATIMINFRACT32 },  // 26
  {             0, ATIMAXFRACT32, ATIMAXFRACT32 }   // 27
};
/* Define Test Data for frsssSub() */
#define SUBTESTNUM  29
const TestDataSSS subTestData[SUBTESTNUM] =
{
  {           256,           512,          -256 },  // 0
  {           512,           256,           256 },  // 1
  {            -1, ATIMINFRACT16, ATIMAXFRACT16 },  // 2
  { ATIMAXFRACT16, ATIMINFRACT16, ATIMAXFRACT16 },  // 3
  {             0,             0,             0 },  // 4
  {             7,             4,             3 },  // 5
  {             4,             6,            -2 },  // 6
  {             1,             0,             1 },  // 7
  {            -7,             4,           -11 },  // 8
  {             4,            -6,            10 },  // 9
  {            -1,            -3,             2 },  // 10
  {             7,            -4,            11 },  // 11
  {            -4,             6,           -10 },  // 12
  {             1, ATIMINFRACT16, ATIMAXFRACT16 },  // 13
  { ATIMAXFRACT16, ATIMAXFRACT16,             0 },  // 14
  { ATIMAXFRACT16,             0, ATIMAXFRACT16 },  // 15
  { ATIMAXFRACT16,            -1, ATIMAXFRACT16 },  // 16
  { ATIMAXFRACT16,             1,         32766 },  // 17
  {             1, ATIMAXFRACT16,        -32766 },  // 18
  {             0, ATIMAXFRACT16,        -32767 },  // 19
  { ATIMINFRACT16,            -1,        -32767 },  // 20
  { ATIMINFRACT16,             1, ATIMINFRACT16 },  // 21
  { ATIMINFRACT16, ATIMINFRACT16,             0 },  // 22
  { ATIMINFRACT16, ATIMAXFRACT16, ATIMINFRACT16 },  // 23
  { ATIMAXFRACT16, ATIMINFRACT16, ATIMAXFRACT16 },  // 24
  {             1, ATIMINFRACT16, ATIMAXFRACT16 },  // 25
  {            -1, ATIMINFRACT16, ATIMAXFRACT16 },  // 26
  {        -32767,             2, ATIMINFRACT16 },  // 27
  {             0, ATIMINFRACT16, ATIMAXFRACT16 }   // 28
};
/* Define Test Data for frdddSub() */
#define LSUBTESTNUM  29
const TestDataDDD lsubTestData[LSUBTESTNUM] =
{
  {       1048576,       2097152,      -1048576 },  // 0
  {       2097152,       1048576,       1048576 },  // 1
  {            -1, ATIMINFRACT32, ATIMAXFRACT32 },  // 2
  { ATIMAXFRACT32, ATIMINFRACT32, ATIMAXFRACT32 },  // 3
  {             0,             0,             0 },  // 4
  {             7,             4,             3 },  // 5
  {             4,             6,            -2 },  // 6
  {             1,             0,             1 },  // 7
  {            -7,             4,           -11 },  // 8
  {             4,            -6,            10 },  // 9
  {            -1,            -3,             2 },  // 10
  {             7,            -4,            11 },  // 11
  {            -4,             6,           -10 },  // 12
  {             1, ATIMINFRACT32, ATIMAXFRACT32 },  // 13
  { ATIMAXFRACT32, ATIMAXFRACT32,             0 },  // 14
  { ATIMAXFRACT32,             0, ATIMAXFRACT32 },  // 15
  { ATIMAXFRACT32,            -1, ATIMAXFRACT32 },  // 16
  { ATIMAXFRACT32,             1,    2147483646 },  // 17
  {             1, ATIMAXFRACT32,   -2147483646 },  // 18
  {             0, ATIMAXFRACT32,   -2147483647 },  // 19
  { ATIMINFRACT32,            -1,   -2147483647 },  // 20
  { ATIMINFRACT32,             1, ATIMINFRACT32 },  // 21
  { ATIMINFRACT32, ATIMINFRACT32,             0 },  // 22
  { ATIMINFRACT32, ATIMAXFRACT32, ATIMINFRACT32 },  // 23
  { ATIMAXFRACT16, ATIMINFRACT32, ATIMAXFRACT32 },  // 24
  {             1, ATIMINFRACT32, ATIMAXFRACT32 },  // 25
  {            -1, ATIMINFRACT32, ATIMAXFRACT32 },  // 26
  {   -2147483647,             2, ATIMINFRACT32 },  // 27
  {             0, ATIMINFRACT32, ATIMAXFRACT32 }   // 28
};
/* Define Test Data for frsiNorm() */
#define NORMTESTNUM  28
const TestDataSI normTestData[NORMTESTNUM] =
{
  {          -256,             7 },  // 0
  {         21760,             0 },  // 1
  {             1,            14 },  // 2
  {          3855,             3 },  // 3
  { ATIMAXFRACT16,             0 },  // 4
  { ATIMINFRACT16,             0 },  // 5
  {             1,            14 },  // 6
  {             2,            13 },  // 7
  {             3,            13 },  // 8
  {             4,            12 },  // 9
  {             5,            12 },  // 10
  {          2048,             3 },  // 11
  {          4096,             2 },  // 12
  {          8192,             1 },  // 13
  {          8192,             1 },  // 14
  {         16383,             1 },  // 15
  {         16384,             0 },  // 16
  {         16384,             0 },  // 17
  {         20478,             0 },  // 18
  { ATIMAXFRACT16,             0 },  // 19
  {            -1,            15 },  // 20
  {            -2,            14 },  // 21
  {            -3,            13 },  // 22
  {            -4,            13 },  // 23
  {            -5,            12 },  // 24
  {         20479,             0 },  // 25
  { ATIMINFRACT16,             0 },  // 26
  {             0,             0 }   // 27
};
/* Define Test Data for frdiNorm() */
#define LNORMTESTNUM  28
const TestDataDI lnormTestData[LNORMTESTNUM] =
{
  {     -16777216,             7 },  // 0
  {    1426063360,             0 },  // 1
  {             1,            30 },  // 2
  {     252641280,             3 },  // 3
  { ATIMAXFRACT32,             0 },  // 4
  { ATIMINFRACT32,             0 },  // 5
  {             1,            30 },  // 6
  {             2,            29 },  // 7
  {             3,            29 },  // 8
  {             4,            28 },  // 9
  {             5,            28 },  // 10
  {     134217728,             3 },  // 11
  {     268435456,             2 },  // 12
  {     536870912,             1 },  // 13
  {     536870913,             1 },  // 14
  {    1073741823,             1 },  // 15
  {    1073741824,             0 },  // 16
  {    1073741825,             0 },  // 17
  {    1342177278,             0 },  // 18
  { ATIMAXFRACT32,             0 },  // 19
  {            -1,            31 },  // 20
  {            -2,            30 },  // 21
  {            -3,            29 },  // 22
  {            -4,            29 },  // 23
  {            -5,            28 },  // 24
  {    1342177279,             0 },  // 25
  { ATIMINFRACT32,             0 },  // 26
  {             0,             0 }   // 27
};
/* Define Test Data for frsisShl() */
#define SHLTESTNUM  19
const TestDataSIS shlTestData[SHLTESTNUM] =
{
  {        -24576,             8, ATIMINFRACT16 },  // 0
  {          2730,             9, ATIMAXFRACT16 },  // 1
  {          2730,            10, ATIMAXFRACT16 },  // 2
  {          2730,            -9,             5 },  // 3
  {             1,             1,             2 },  // 4
  {             4,             2,            16 },  // 5
  {           136,             5,          4352 },  // 6
  {             1,            -1,             0 },  // 7
  {             4,            -2,             1 },  // 8
  {          4352,            -5,           136 },  // 9
  {            -1,          4711, ATIMINFRACT16 },  // 10
  {             1,          4711, ATIMAXFRACT16 },  // 11
  {         -4711,          4711, ATIMINFRACT16 },  // 12
  {          4711,          4711, ATIMAXFRACT16 },  // 13
  {          4711,         -4711,             0 },  // 14
  {         -4711,         -4711,            -1 },  // 15
  { ATIMAXFRACT16,             1, ATIMAXFRACT16 },  // 16
  {            -1,             1,            -2 },  // 17
  { ATIMINFRACT16,             1, ATIMINFRACT16 }   // 18
};
/* Define Test Data for frdidShl() */
#define LSHLTESTNUM  57
const TestDataDID lshlTestData[LSHLTESTNUM] =
{
  {    -536870913,             3, ATIMINFRACT32 },  // 0
  {     412607665,             8, ATIMAXFRACT32 },  // 1
  {    -268435456,             8, ATIMINFRACT32 },  // 2
  {    -536870913,            -3,     -67108865 },  // 3
  {    -536870913,           -16,         -8193 },  // 4
  {    -536870913,           -32,            -1 },  // 5
  {             1,             1,             2 },  // 6
  {             4,             2,            16 },  // 7
  {           136,             5,          4352 },  // 8
  {             1,            -1,             0 },  // 9
  {             4,            -2,             1 },  // 10
  {          4352,            -5,           136 },  // 11
  {            -1,          4711, ATIMINFRACT32 },  // 12
  {             1,          4711, ATIMAXFRACT32 },  // 13
  {         -4711,          4711, ATIMINFRACT32 },  // 14
  {          4711,          4711, ATIMAXFRACT32 },  // 15
  {          4711,         -4711,             0 },  // 16
  {         -4711,         -4711,            -1 },  // 17
  { ATIMAXFRACT32,             1, ATIMAXFRACT32 },  // 18
  {            -1,             1,            -2 },  // 19
  { ATIMINFRACT32,             1, ATIMINFRACT32 },   // 20
  {    -536870913,            -1,    -268435457 },  // 21
  {    -536870913,            -2,    -134217729 },  // 22
  {    -536870913,            -4,     -33554433 },  // 23
  {    -536870913,            -5,     -16777217 },  // 24
  {    -536870913,            -6,      -8388609 },  // 25
  {    -536870913,            -7,      -4194305 },  // 26
  {    -536870913,            -8,      -2097153 },  // 27
  {    -536870913,            -9,      -1048577 },  // 28
  {    -536870913,           -10,       -524289 },  // 29
  {    -536870913,           -11,       -262145 },  // 30
  {    -536870913,           -12,       -131073 },  // 31
  {    -536870913,           -13,        -65537 },  // 32
  {    -536870913,           -14,        -32769 },  // 33
  {    -536870913,           -15,        -16385 },  // 34
  {     412607665,             1,     825215330 },  // 35
  {     412607665,             2,    1650430660 },  // 36
  {     412607665,             3, ATIMAXFRACT32 },  // 37
  {          4711,             1,          9422 },  // 38
  {          4711,             2,         18844 },  // 39
  {          4711,             3,         37688 },  // 40
  {          4711,             4,         75376 },  // 41
  {          4711,             5,        150752 },  // 42
  {          4711,             6,        301504 },  // 43
  {          4711,             7,        603008 },  // 44
  {          4711,             8,       1206016 },  // 45
  {          4711,             9,       2412032 },  // 46
  {          4711,            10,       4824064 },  // 47
  {          4711,            11,       9648128 },  // 48
  {          4711,            12,      19296256 },  // 49
  {          4711,            13,      38592512 },  // 50
  {          4711,            14,      77185024 },  // 51
  {          4711,            15,     154370048 },  // 52
  {          4711,            16,     308740096 },  // 53
  {          4711,            17,     617480192 },  // 54
  {          4711,            18,    1234960384 },  // 55
  {          4711,            19, ATIMAXFRACT32 }   // 56
};
/* Define Test Data for frsisShr() */
#define SHRTESTNUM  37
const TestDataSIS shrTestData[SHRTESTNUM] =
{
  {             1,             3,             0 },  // 0
  {          -256,            16,            -1 },  // 1
  {            -1,            -5,           -32 },  // 2
  {          2730,            10,             2 },  // 3
  {          2730,           -10, ATIMAXFRACT16 },  // 4
  {             1,             1,             0 },  // 5
  {             4,             2,             1 },  // 6
  {          4352,             5,           136 },  // 7
  {             1,            -1,             2 },  // 8
  {             1,            -2,             4 },  // 9
  {           136,            -5,          4352 },  // 10
  {            -1,          4711,            -1 },  // 11
  {             1,          4711,             0 },  // 12
  {         -4711,          4711,            -1 },  // 13
  {          4711,          4711,             0 },  // 14
  { ATIMAXFRACT16,             1,         16383 },  // 15
  { ATIMINFRACT16,             1,        -16384 },  // 16
  {         26505,             0,         26505 },  // 17
  {         26505,             4,          1656 },  // 18
  {         26505,             8,           103 },  // 19
  {         26505,            12,             6 },  // 20
  {         26505,            14,             1 },  // 21
  {         26505,            15,             0 },  // 22
  {         26505,            16,             0 },  // 23
  {         26505,            31,             0 },  // 24
  {         26505,            32,             0 },  // 25
  {         26505,            64,             0 },  // 26
  {        -26506,             0,        -26506 },  // 27
  {        -26506,             4,         -1657 },  // 28
  {        -26506,             8,          -104 },  // 29
  {        -26506,            12,            -7 },  // 30
  {        -26506,            14,            -2 },  // 31
  {        -26506,            15,            -1 },  // 32
  {        -26506,            16,            -1 },  // 33
  {        -26506,            31,            -1 },  // 34
  {        -26506,            32,            -1 },  // 35
  {        -26506,            64,            -1 }   // 36
};
/* Define Test Data for frdidShr() */
#define LSHRTESTNUM  44
const TestDataDID lshrTestData[LSHRTESTNUM] =
{
  {    -536870913,            -3, ATIMINFRACT32 },  // 0
  {     412607665,            -8, ATIMAXFRACT32 },  // 1
  {    -268435456,            -8, ATIMINFRACT32 },  // 2
  {    -536870913,             3,     -67108865 },  // 3
  {    -536870913,            16,         -8193 },  // 4
  {    -536870913,            32,            -1 },  // 5
  {             1,             1,             0 },  // 6
  {             4,             2,             1 },  // 7
  {          4352,             5,           136 },  // 8
  {             1,            -1,             2 },  // 9
  {             1,            -2,             4 },  // 10
  {           136,            -5,          4352 },  // 11
  {            -1,          4711,            -1 },  // 12
  {             1,          4711,             0 },  // 13
  {         -4711,          4711,            -1 },  // 14
  {          4711,          4711,             0 },  // 15
  { ATIMAXFRACT32,             1,    1073741823 },  // 16
  { ATIMINFRACT32,             1,   -1073741824 },  // 17
  {    1737036340,             0,    1737036340 },  // 18
  {    1737036340,             4,     108564771 },  // 19
  {    1737036340,             8,       6785298 },  // 20
  {    1737036340,            12,        424081 },  // 21
  {    1737036340,            16,         26505 },  // 22
  {    1737036340,            20,          1656 },  // 23
  {    1737036340,            24,           103 },  // 24
  {    1737036340,            28,             6 },  // 25
  {    1737036340,            30,             1 },  // 26
  {    1737036340,            31,             0 },  // 27
  {    1737036340,            32,             0 },  // 28
  {    1737036340,            33,             0 },  // 29
  {    1737036340,            64,             0 },  // 30
  {   -1737036341,             0,   -1737036341 },  // 31
  {   -1737036341,             4,    -108564772 },  // 32
  {   -1737036341,             8,      -6785299 },  // 33
  {   -1737036341,            12,       -424082 },  // 34
  {   -1737036341,            16,        -26506 },  // 35
  {   -1737036341,            20,         -1657 },  // 36
  {   -1737036341,            24,          -104 },  // 37
  {   -1737036341,            28,            -7 },  // 38
  {   -1737036341,            30,            -2 },  // 39
  {   -1737036341,            31,            -1 },  // 40
  {   -1737036341,            32,            -1 },  // 41
  {   -1737036341,            33,            -1 },  // 42
  {   -1737036341,            64,            -1 }   // 43
};
/* Define Test Data for frsisShrR() */
#define SHRRTESTNUM  37
const TestDataSIS shrrTestData[SHRRTESTNUM] =
{
  {             1,             3,             0 },  // 0
  {          -256,            16,             0 },  // 1
  {            -1,            -5,           -32 },  // 2
  {          2730,            10,             3 },  // 3
  {          2730,           -10, ATIMAXFRACT16 },  // 4
  {             1,             1,             1 },  // 5
  {             4,             2,             1 },  // 6
  {          4352,             5,           136 },  // 7
  {             1,            -1,             2 },  // 8
  {             1,            -2,             4 },  // 9
  {           136,            -5,          4352 },  // 10
  {            -1,          4711,             0 },  // 11
  {             1,          4711,             0 },  // 12
  {         -4711,          4711,             0 },  // 13
  {          4711,          4711,             0 },  // 14
  { ATIMAXFRACT16,             1,         16384 },  // 15
  { ATIMINFRACT16,             1,        -16384 },  // 16
  {         26505,             0,         26505 },  // 17
  {         26505,             4,          1657 },  // 18
  {         26505,             8,           104 },  // 19
  {         26505,            12,             6 },  // 20
  {         26505,            14,             2 },  // 21
  {         26505,            15,             1 },  // 22
  {         26505,            16,             0 },  // 23
  {         26505,            31,             0 },  // 24
  {         26505,            32,             0 },  // 25
  {         26505,            64,             0 },  // 26
  {        -26506,             0,        -26506 },  // 27
  {        -26506,             4,         -1657 },  // 28
  {        -26506,             8,          -104 },  // 29
  {        -26506,            12,            -6 },  // 30
  {        -26506,            14,            -2 },  // 31
  {        -26506,            15,            -1 },  // 32
  {        -26506,            16,             0 },  // 33
  {        -26506,            31,             0 },  // 34
  {        -26506,            32,             0 },  // 35
  {        -26506,            64,             0 }   // 36
};
/* Define Test Data for frdidShrR() */
#define LSHRRTESTNUM  44
const TestDataDID lshrrTestData[LSHRRTESTNUM] =
{
  {    -536870913,            -3, ATIMINFRACT32 },  // 0
  {     412607665,            -8, ATIMAXFRACT32 },  // 1
  {    -268435456,            -8, ATIMINFRACT32 },  // 2
  {    -536870913,             3,     -67108864 },  // 3
  {    -536870913,            16,         -8192 },  // 4
  {    -536870913,            32,             0 },  // 5
  {             1,             1,             1 },  // 6
  {             4,             2,             1 },  // 7
  {          4352,             5,           136 },  // 8
  {             1,            -1,             2 },  // 9
  {             1,            -2,             4 },  // 10
  {           136,            -5,          4352 },  // 11
  {            -1,          4711,             0 },  // 12
  {             1,          4711,             0 },  // 13
  {         -4711,          4711,             0 },  // 14
  {          4711,          4711,             0 },  // 15
  { ATIMAXFRACT32,             1,    1073741824 },  // 16
  { ATIMINFRACT32,             1,   -1073741824 },  // 17
  {    1737036340,             0,    1737036340 },  // 18
  {    1737036340,             4,     108564771 },  // 19
  {    1737036340,             8,       6785298 },  // 20
  {    1737036340,            12,        424081 },  // 21
  {    1737036340,            16,         26505 },  // 22
  {    1737036340,            20,          1657 },  // 23
  {    1737036340,            24,           104 },  // 24
  {    1737036340,            28,             6 },  // 25
  {    1737036340,            30,             2 },  // 26
  {    1737036340,            31,             1 },  // 27
  {    1737036340,            32,             0 },  // 28
  {    1737036340,            33,             0 },  // 29
  {    1737036340,            64,             0 },  // 30
  {   -1737036341,             0,   -1737036341 },  // 31
  {   -1737036341,             4,    -108564771 },  // 32
  {   -1737036341,             8,      -6785298 },  // 33
  {   -1737036341,            12,       -424081 },  // 34
  {   -1737036341,            16,        -26505 },  // 35
  {   -1737036341,            20,         -1657 },  // 36
  {   -1737036341,            24,          -104 },  // 37
  {   -1737036341,            28,            -6 },  // 38
  {   -1737036341,            30,            -2 },  // 39
  {   -1737036341,            31,            -1 },  // 40
  {   -1737036341,            32,             0 },  // 41
  {   -1737036341,            33,             0 },  // 42
  {   -1737036341,            64,             0 }   // 43
};
/* Define Test Data for frsssMult() */
#define MULTESTNUM  22
const TestDataSSS mulTestData[MULTESTNUM] =
{
  {            0,              0,             0 },  // 0
  {          256,            256,             2 },  // 1
  {         4711,          16384,          2355 },  // 2
  {           -1,              0,             0 },  // 3
  {         -256,            256,            -2 },  // 4
  {          256,           -256,            -2 },  // 5
  {         -256,           -256,             2 },  // 6
  {        -4711,          16384,         -2356 },  // 7
  {         4711,         -16384,         -2356 },  // 8
  {        -4711,         -16384,          2355 },  // 9
  { ATIMAXFRACT16, ATIMAXFRACT16,         32766 },  // 10
  { ATIMAXFRACT16,         16384,         16383 },  // 11
  {         16384, ATIMAXFRACT16,         16383 },  // 12
  { ATIMAXFRACT16,             1,             0 },  // 13
  { ATIMAXFRACT16,             2,             1 },  // 14
  { ATIMAXFRACT16,             3,             2 },  // 15
  { ATIMINFRACT16, ATIMINFRACT16, ATIMAXFRACT16 },  // 16
  { ATIMINFRACT16, ATIMAXFRACT16,        -32767 },  // 17
  { ATIMAXFRACT16, ATIMINFRACT16,        -32767 },  // 18
  { ATIMINFRACT16,             1,            -1 },  // 19
  { ATIMINFRACT16,             2,            -2 },  // 20
  { ATIMINFRACT16,             3,            -3 }   // 21
};
/* Define Test Data for frssdMult() */
#define LMULTESTNUM  18
const TestDataSSD lmulTestData[LMULTESTNUM] =
{
  {             0,             0,             0 },  // 0
  {             2,             3,            12 },  // 1
  {          4711,             5,         47110 },  // 2
  {            -2,             3,           -12 },  // 3
  {             2,            -3,           -12 },  // 4
  {            -2,            -3,            12 },  // 5
  {         -4711,             5,        -47110 },  // 6
  {          4711,            -5,        -47110 },  // 7
  {         -4711,            -5,         47110 },  // 8
  { ATIMAXFRACT16, ATIMAXFRACT16,    2147352578 },  // 9
  { ATIMAXFRACT16,        -32767,   -2147352578 },  // 10
  {        -32767, ATIMAXFRACT16,   -2147352578 },  // 11
  { ATIMAXFRACT16,             2,        131068 },  // 12
  { ATIMAXFRACT16,             1,         65534 },  // 13
  {             1, ATIMAXFRACT16,         65534 },  // 14
  { ATIMAXFRACT16,             0,             0 },  // 15
  {             0, ATIMAXFRACT16,             0 },  // 16
  { ATIMINFRACT16, ATIMINFRACT16, ATIMAXFRACT32 }   // 17
};
/* Define Test Data for frsssMultR() */
#define MULRTESTNUM  28
const TestDataSSS mulrTestData[MULRTESTNUM] =
{
  {             0,             0,             0 },  // 0
  {           256,           256,             2 },  // 1
  {          4711,         16384,          2356 },  // 2
  {            -1,             0,             0 },  // 3
  {          -256,           256,            -2 },  // 4
  {           256,          -256,            -2 },  // 5
  {          -256,          -256,             2 },  // 5
  {         -4711,         16384,         -2355 },  // 6
  {          4711,        -16384,         -2355 },  // 7
  {         -4711,        -16384,          2356 },  // 8
  { ATIMAXFRACT16, ATIMAXFRACT16,         32766 },  // 9
  { ATIMAXFRACT16,         32766,         32765 },  // 10
  {         32766, ATIMAXFRACT16,         32765 },  // 11
  { ATIMAXFRACT16,         16384,         16384 },  // 12
  {         16384, ATIMAXFRACT16,         16384 },  // 13
  { ATIMAXFRACT16,         16385,         16384 },  // 14
  {         16385, ATIMAXFRACT16,         16384 },  // 15
  { ATIMAXFRACT16,             2,             2 },  // 16
  { ATIMAXFRACT16,             1,             1 },  // 17
  {             1, ATIMAXFRACT16,             1 },  // 18
  { ATIMAXFRACT16,             0,             0 },  // 19
  {             0, ATIMAXFRACT16,             0 },  // 20
  { ATIMINFRACT16, ATIMINFRACT16, ATIMAXFRACT16 },  // 21
  { ATIMINFRACT16, ATIMAXFRACT16,        -32767 },  // 22
  { ATIMAXFRACT16, ATIMINFRACT16,        -32767 },  // 23
  { ATIMINFRACT16,             1,            -1 },  // 24
  { ATIMINFRACT16,             2,            -2 },  // 25
  { ATIMINFRACT16,             3,            -3 }   // 26
};
/* Define Test Data for frsssDivS() */
/* Note input operands must be positive */
#define DIVTESTNUM  42
const TestDataSSS divTestData[DIVTESTNUM] =
{
  {          4660,          4660, ATIMAXFRACT16 },  // 0
  {          8738,          4369, ATIMAXFRACT16 },  // 1
  { ATIMAXFRACT16,          4369, ATIMAXFRACT16 },  // 2
  { ATIMAXFRACT16, ATIMAXFRACT16, ATIMAXFRACT16 },  // 3
  {          1024,          2048,         16384 },  // 4
  {         21846,         20480, ATIMAXFRACT16 },  // 5
  {           291,         20480,           465 },  // 6
  {          4660,          6826,         22370 },  // 7
  {             1,             1, ATIMAXFRACT16 },  // 8
  {          4711,          4711, ATIMAXFRACT16 },  // 9
  {             5,            10,         16384 },  // 10
  {             5,            20,          8192 },  // 11
  {             5,            40,          4096 },  // 12
  { ATIMAXFRACT16, ATIMAXFRACT16, ATIMAXFRACT16 },  // 13
  {         16384, ATIMAXFRACT16,         16384 },  // 14
  {             1, ATIMAXFRACT16,             1 },  // 15
  {             1,             2,         16384 },  // 16
  {        -24400,          8651, ATIMINFRACT16 },  // 17
  {        -21241,         21317,        -32651 },  // 18
  {        -25670,        -16799, ATIMAXFRACT16 },  // 19
  {         -9883,         10145,        -31921 },  // 20
  {        -21390,         24676,        -28404 },  // 21
  {          2542,         -6446,        -12922 },  // 22
  {         -5451,        -20134,          8871 },  // 23
  {          -536,         28873,          -608 },  // 24
  {        -21846,        -20644, ATIMAXFRACT16 },  // 25
  {        -29025,         -9680, ATIMAXFRACT16 },  // 26
  {        -14143,        -16808,         27572 },  // 27
  {        -21798,        -19879, ATIMAXFRACT16 },  // 28
  {         -3393,         -4899,         22694 },  // 29
  {         -2340,        -21733,          3528 },  // 30
  {         11377,        -28997,        -12856 },  // 31
  {          5667,        -23515,         -7896 },  // 32
  {        -26724,         -5771, ATIMAXFRACT16 },  // 33
  { ATIMINFRACT16,        -23766, ATIMAXFRACT16 },  // 34
  { ATIMINFRACT16,        -32354, ATIMAXFRACT16 },  // 35
  { ATIMINFRACT16,         31869, ATIMINFRACT16 },  // 36
  { ATIMINFRACT16,        -22794, ATIMAXFRACT16 },  // 37
  {        -28859, ATIMINFRACT16,         28859 },  // 38
  {         26757, ATIMINFRACT16,        -26757 },  // 39
  {        -28969, ATIMINFRACT16,         28969 },  // 40
  {         21894, ATIMINFRACT16,        -21894 }   // 41
};

/* Define Test Data for frdssdMac() */
#define MACTESTNUM  9
const TestDataDSSD macTestData[MACTESTNUM] =
{
  {    1515870810,          4096,          4096,    1549425242 },  // 0
  {    1515890085,          8191,          8191,    1650075047 },  // 1
  {    1516240895,          8191,          8191,    1650425857 },  // 2
  {    1515870810,         23130,         23130, ATIMAXFRACT32 },  // 3
  {             0,         23130,         23130,    1069993800 },  // 4
  { ATIMAXFRACT32, ATIMAXFRACT16, ATIMAXFRACT16, ATIMAXFRACT32 },  // 5
  { ATIMAXFRACT32, ATIMINFRACT16, ATIMINFRACT16, ATIMAXFRACT32 },  // 6
  {             0, ATIMAXFRACT16, ATIMAXFRACT16,    2147352578 },  // 7
  {             0, ATIMINFRACT16, ATIMINFRACT16, ATIMAXFRACT32 }   // 8
};
/* Define Test Data for frdsssMacR() */
#define MACRTESTNUM  9
const TestDataDSSS macrTestData[MACRTESTNUM] =
{
  {    1515870810,         4096,           4096,         23642 },  // 0
  {    1515890085,         8191,           8191,         25178 },  // 1
  {    1516240895,         8191,           8191,         25184 },  // 2
  {    1515870810,        23130,          23130, ATIMAXFRACT16 },  // 3
  {             0,        23130,          23130,         16327 },  // 4
  { ATIMAXFRACT32, ATIMAXFRACT16, ATIMAXFRACT16, ATIMAXFRACT16 },  // 5
  { ATIMAXFRACT32, ATIMINFRACT16, ATIMINFRACT16, ATIMAXFRACT16 },  // 6
  {             0, ATIMAXFRACT16, ATIMAXFRACT16,         32766 },  // 7
  {             0, ATIMINFRACT16, ATIMINFRACT16, ATIMAXFRACT16 }   // 8
};

/* Define Test Data for frdssdMsu() */
#define MSUTESTNUM  9
const TestDataDSSD msuTestData[MSUTESTNUM] =
{
  {    1515870810,          4096,          4096,    1482316378 },  // 0
  {    1515890085,          8191,          8191,    1381705123 },  // 1
  {    1516240895,          8191,          8191,    1382055933 },  // 2
  {    1515870810,         23130,         23130,     445877010 },  // 3
  {             0,         23130,         23130,   -1069993800 },  // 4
  { ATIMAXFRACT32, ATIMAXFRACT16, ATIMAXFRACT16,        131069 },  // 5
  { ATIMAXFRACT32, ATIMINFRACT16, ATIMINFRACT16,             0 },  // 6
  {             0, ATIMAXFRACT16, ATIMAXFRACT16,   -2147352578 },  // 7
  {             0, ATIMINFRACT16, ATIMINFRACT16,   -2147483647 }   // 8
};
/* Define Test Data for frdsssMsuR() */
#define MSURTESTNUM  9
const TestDataDSSS msurTestData[MSURTESTNUM] =
{
  {    1515870810,          4096,          4096,         22618 },  // 0
  {    1515890085,          8191,          8191,         21083 },  // 1
  {    1516240895,          8191,          8191,         21088 },  // 2
  {    1515870810,         23130,         23130,          6804 },  // 3
  {             0,         23130,         23130,        -16327 },  // 4
  { ATIMAXFRACT32, ATIMAXFRACT16, ATIMAXFRACT16,             2 },  // 5
  { ATIMAXFRACT32, ATIMINFRACT16, ATIMINFRACT16,             0 },  // 6
  {             0, ATIMAXFRACT16, ATIMAXFRACT16,        -32766 },  // 7
  {             0, ATIMINFRACT16, ATIMINFRACT16, ATIMINFRACT16 }   // 8
};



/****************************************************************************
 *                                                                          *
 * testPass() function                                                      *
 *                                                                          *
 ****************************************************************************/
int testPass(FILE *hOut)
{
  int i, errcntr, totalerrs;
  AtiFract16 fr16;
  AtiFract32 fr32;
  AtiInt16   ires;

  totalerrs = 0;

 /*---------------------------------------------------------------*
  * frssAbs Test                                                  *
  *   AtiFract16 frssAbs(AtiFract16)                              *
  *---------------------------------------------------------------*/
#ifdef DETAIL_OUTPUT
  printf("frssAbs() tests started\n");
  fprintf(hOut, "frssAbs() tests started\n");
#endif
  errcntr = 0;
  for (i = 0; i < ABSTESTNUM; i++)
  {
    fr16 = frssAbs(absTestData[i].op1);
    if (fr16 != absTestData[i].result)
    {
#ifdef DUMP_ERRORS
      printf("Error frssAbs test %d: expected=0x%4.4hx, received=0x%4.4hx\n",
             i, absTestData[i].result, fr16);
      fprintf(hOut, "Error frssAbs test %d: expected=0x%4.4hx, received=0x%4.4hx\n",
             i, absTestData[i].result, fr16);
#endif
      errcntr++;
    }
  }
  totalerrs += errcntr;
#ifdef DETAIL_OUTPUT
  printf("frssAbs test completed with %d errors\n", errcntr);
  fprintf(hOut, "frssAbs test completed with %d errors\n", errcntr);
  printf("frssAbs() tests complete\n\n");
  fprintf(hOut, "frssAbs() tests complete\n\n");
#endif

 /*---------------------------------------------------------------*
  * frddAbs Test                                                  *
  *   AtiFract32 frddAbs(AtiFract32)                              *
  *---------------------------------------------------------------*/
#ifdef DETAIL_OUTPUT
  printf("frddAbs() tests started\n");
  fprintf(hOut, "frddAbs() tests started\n");
#endif
  errcntr = 0;
  for (i = 0; i < LABSTESTNUM; i++)
  {
    fr32 = frddAbs(labsTestData[i].op1);
    if (fr32 != labsTestData[i].result)
    {
#ifdef DUMP_ERRORS
      printf("Error frddAbs test %d: expected=0x%8.8lx, received=0x%8.8lx\n",
             i, labsTestData[i].result, fr32);
      fprintf(hOut, "Error frddAbs test %d: expected=0x%8.8lx, received=0x%8.8lx\n",
             i, labsTestData[i].result, fr32);
#endif
      errcntr++;
    }
  }
  totalerrs += errcntr;
#ifdef DETAIL_OUTPUT
  printf("frddAbs test completed with %d errors\n", errcntr);
  fprintf(hOut, "frddAbs test completed with %d errors\n", errcntr);
  printf("frddAbs() tests complete\n\n");
  fprintf(hOut, "frddAbs() tests complete\n\n");
#endif

 /*---------------------------------------------------------------*
  * frssNegate Test                                               *
  *   AtiFract16 frssNegate(AtiFract16)                           *
  *---------------------------------------------------------------*/
#ifdef DETAIL_OUTPUT
  printf("frssNegate() tests started\n");
  fprintf(hOut, "frssNegate() tests started\n");
#endif
  errcntr = 0;
  for (i = 0; i < NEGTESTNUM; i++)
  {
    fr16 = frssNegate(negTestData[i].op1);
    if (fr16 != negTestData[i].result)
    {
#ifdef DUMP_ERRORS
      printf("Error frssNegate test %d: expected=0x%4.4hx, received=0x%4.4hx\n",
             i, negTestData[i].result, fr16);
      fprintf(hOut, "Error frssNegate test %d: expected=0x%4.4hx, received=0x%4.4hx\n",
             i, negTestData[i].result, fr16);
#endif
      errcntr++;
    }
  }
  totalerrs += errcntr;
#ifdef DETAIL_OUTPUT
  printf("frssNegate test completed with %d errors\n", errcntr);
  fprintf(hOut,"frssNegate test completed with %d errors\n", errcntr);
  printf("frssNegate() tests complete\n\n");
  fprintf(hOut, "frssNegate() tests complete\n\n");
#endif

 /*---------------------------------------------------------------*
  * frddNegate Test                                               *
  *   AtiFract32 frddNegate(AtiFract32)                           *
  *---------------------------------------------------------------*/
#ifdef DETAIL_OUTPUT
  printf("frddNegate() tests started\n");
  fprintf(hOut, "frddNegate() tests started\n");
#endif
  errcntr = 0;
  for (i = 0; i < LNEGTESTNUM; i++)
  {
    fr32 = frddNegate(lnegTestData[i].op1);
    if (fr32 != lnegTestData[i].result)
    {
#ifdef DUMP_ERRORS
      printf("Error frddNegate test %d: expected=0x%8.8lx, received=0x%8.8lx\n",
             i, lnegTestData[i].result, fr32);
      fprintf(hOut, "Error frddNegate test %d: expected=0x%8.8lx, received=0x%8.8lx\n",
             i, lnegTestData[i].result, fr32);
#endif
      errcntr++;
    }
  }
  totalerrs += errcntr;
#ifdef DETAIL_OUTPUT
  printf("frddNegate test completed with %d errors\n", errcntr);
  fprintf(hOut, "frddNegate test completed with %d errors\n", errcntr);
  printf("frddNegate() tests complete\n\n");
  fprintf(hOut, "frddNegate() tests complete\n\n");
#endif

 /*---------------------------------------------------------------*
  * frdsRound Test                                                *
  *   AtiFract16 frdsRound(AtiFract32)                            *
  *---------------------------------------------------------------*/
#ifdef DETAIL_OUTPUT
  printf("frdsRound() tests started\n");
  fprintf(hOut, "frdsRound() tests started\n");
#endif
  errcntr = 0;
  for (i = 0; i < RNDTESTNUM; i++)
  {
    fr16 = frdsRound(rndTestData[i].op1);
    if (fr16 != rndTestData[i].result)
    {
#ifdef DUMP_ERRORS
      printf("Error frdsRound test %d: expected=0x%4.4hx, received=0x%4.4hx\n",
             i, rndTestData[i].result, fr16);
      fprintf(hOut, "Error frdsRound test %d: expected=0x%4.4hx, received=0x%4.4hx\n",
             i, rndTestData[i].result, fr16);
#endif
      errcntr++;
    }
  }
  totalerrs += errcntr;
#ifdef DETAIL_OUTPUT
  printf("frdsRound test completed with %d errors\n", errcntr);
  fprintf(hOut, "frdsRound test completed with %d errors\n", errcntr);
  printf("frdsRound() tests complete\n\n");
  fprintf(hOut, "frdsRound() tests complete\n\n");
#endif

 /*---------------------------------------------------------------*
  * frsdDepositH Test                                             *
  *   AtiFract32 frsdDepositH(AtiFract16)                         *
  *---------------------------------------------------------------*/
#ifdef DETAIL_OUTPUT
  printf("frsdDepositH() tests started\n");
  fprintf(hOut, "frsdDepositH() tests started\n");
#endif
  errcntr = 0;
  for (i = 0; i < DEPOSHTESTNUM; i++)
  {
    fr32 = frsdDepositH(deposhTestData[i].op1);
    if (fr32 != deposhTestData[i].result)
    {
#ifdef DUMP_ERRORS
      printf("Error frsdDepositH test %d: expected=0x%8.8lx, received=0x%8.8lx\n",
             i, deposhTestData[i].result, fr32);
      fprintf(hOut, "Error frsdDepositH test %d: expected=0x%8.8lx, received=0x%8.8lx\n",
             i, deposhTestData[i].result, fr32);
#endif
      errcntr++;
    }
  }
  totalerrs += errcntr;
#ifdef DETAIL_OUTPUT
  printf("frsdDepositH test completed with %d errors\n", errcntr);
  fprintf(hOut, "frsdDepositH test completed with %d errors\n", errcntr);
  printf("frsdDepositH() tests complete\n\n");
  fprintf(hOut, "frsdDepositH() tests complete\n\n");
#endif

 /*---------------------------------------------------------------*
  * frsdDepositL Test                                             *
  *   AtiFract32 frsdDepositL(AtiFract16)                         *
  *---------------------------------------------------------------*/
#ifdef DETAIL_OUTPUT
  printf("frsdDepositL() tests started\n");
  fprintf(hOut, "frsdDepositL() tests started\n");
#endif
  errcntr = 0;
  for (i = 0; i < DEPOSLTESTNUM; i++)
  {
    fr32 = frsdDepositL(deposlTestData[i].op1);
    if (fr32 != deposlTestData[i].result)
    {
#ifdef DUMP_ERRORS
      printf("Error frsdDepositL test %d: expected=0x%8.8lx, received=0x%8.8lx\n",
             i, deposlTestData[i].result, fr32);
      fprintf(hOut, "Error frsdDepositL test %d: expected=0x%8.8lx, received=0x%8.8lx\n",
             i, deposlTestData[i].result, fr32);
#endif
      errcntr++;
    }
  }
  totalerrs += errcntr;
#ifdef DETAIL_OUTPUT
  printf("frsdDepositL test completed with %d errors\n", errcntr);
  fprintf(hOut, "frsdDepositL test completed with %d errors\n", errcntr);
  printf("frsdDepositL() tests complete\n\n");
  fprintf(hOut, "frsdDepositL() tests complete\n\n");
#endif

 /*---------------------------------------------------------------*
  * frdsExtractH Test                                             *
  *   AtiFract16 frdsExtractH(AtiFract32)                         *
  *---------------------------------------------------------------*/
#ifdef DETAIL_OUTPUT
  printf("frdsExtractH() tests started\n");
  fprintf(hOut, "frdsExtractH() tests started\n");
#endif
  errcntr = 0;
  for (i = 0; i < EXTHTESTNUM; i++)
  {
    fr16 = frdsExtractH(exthTestData[i].op1);
    if (fr16 != exthTestData[i].result)
    {
#ifdef DUMP_ERRORS
      printf("Error frdsExtractH test %d: expected=0x%4.4hx, received=0x%4.4hx\n",
             i, exthTestData[i].result, fr16);
      fprintf(hOut, "Error frdsExtractH test %d: expected=0x%4.4hx, received=0x%4.4hx\n",
             i, exthTestData[i].result, fr16);
#endif
      errcntr++;
    }
  }
  totalerrs += errcntr;
#ifdef DETAIL_OUTPUT
  printf("frdsExtractH test completed with %d errors\n", errcntr);
  fprintf(hOut, "frdsExtractH test completed with %d errors\n", errcntr);
  printf("frdsExtractH() tests complete\n\n");
  fprintf(hOut, "frdsExtractH() tests complete\n\n");
#endif

 /*---------------------------------------------------------------*
  * frdsExtractL Test                                             *
  *   AtiFract16 frdsExtractL(AtiFract32)                         *
  *---------------------------------------------------------------*/
#ifdef DETAIL_OUTPUT
  printf("frdsExtractL() tests started\n");
  fprintf(hOut, "frdsExtractL() tests started\n");
#endif
  errcntr = 0;
  for (i = 0; i < EXTLTESTNUM; i++)
  {
    fr16 = frdsExtractL(extlTestData[i].op1);
    if (fr16 != extlTestData[i].result)
    {
#ifdef DUMP_ERRORS
      printf("Error frdsExtractL test %d: expected=0x%4.4hx, received=0x%4.4hx\n",
             i, extlTestData[i].result, fr16);
      fprintf(hOut, "Error frdsExtractL test %d: expected=0x%4.4hx, received=0x%4.4hx\n",
             i, extlTestData[i].result, fr16);
#endif
      errcntr++;
    }
  }
  totalerrs += errcntr;
#ifdef DETAIL_OUTPUT
  printf("frdsExtractL test completed with %d errors\n", errcntr);
  fprintf(hOut, "frdsExtractL test completed with %d errors\n", errcntr);
  printf("frdsExtractL() tests complete\n\n");
  fprintf(hOut, "frdsExtractL() tests complete\n\n");
#endif

 /*---------------------------------------------------------------*
  * frsssAdd Test                                                 *
  *   AtiFract16 frsssAdd(AtiFract16, AtiFract16)                 *
  *---------------------------------------------------------------*/
#ifdef DETAIL_OUTPUT
  printf("frsssAdd() tests started\n");
  fprintf(hOut, "frsssAdd() tests started\n");
#endif
  errcntr = 0;
  for (i = 0; i < ADDTESTNUM; i++)
  {
    fr16 = frsssAdd(addTestData[i].op1, addTestData[i].op2);
    if (fr16 != addTestData[i].result)
    {
#ifdef DUMP_ERRORS
      printf("Error frsssAdd test %d: expected=0x%4.4hx, received=0x%4.4hx\n",
             i, addTestData[i].result, fr16);
      fprintf(hOut, "Error frsssAdd test %d: expected=0x%4.4hx, received=0x%4.4hx\n",
             i, addTestData[i].result, fr16);
#endif
      errcntr++;
    }
  }
  totalerrs += errcntr;
#ifdef DETAIL_OUTPUT
  printf("frsssAdd test completed with %d errors\n", errcntr);
  fprintf(hOut, "frsssAdd test completed with %d errors\n", errcntr);
  printf("frsssAdd() tests complete\n\n");
  fprintf(hOut, "frsssAdd() tests complete\n\n");
#endif

 /*---------------------------------------------------------------*
  * frdddAdd Test                                                 *
  *   AtiFract32 frdddAdd(AtiFract32, AtiFract32)                 *
  *---------------------------------------------------------------*/
#ifdef DETAIL_OUTPUT
  printf("frdddAdd() tests started\n");
  fprintf(hOut, "frdddAdd() tests started\n");
#endif
  errcntr = 0;
  for (i = 0; i < LADDTESTNUM; i++)
  {
    fr32 = frdddAdd(laddTestData[i].op1, laddTestData[i].op2);
    if (fr32 != laddTestData[i].result)
    {
#ifdef DUMP_ERRORS
      printf("Error frdddAdd test %d: expected=0x%8.8lx, received=0x%8.8lx\n",
             i, laddTestData[i].result, fr32);
      fprintf(hOut, "Error frdddAdd test %d: expected=0x%8.8lx, received=0x%8.8lx\n",
             i, laddTestData[i].result, fr32);
#endif
      errcntr++;
    }
  }
  totalerrs += errcntr;
#ifdef DETAIL_OUTPUT
  printf("frdddAdd test completed with %d errors\n", errcntr);
  fprintf(hOut, "frdddAdd test completed with %d errors\n", errcntr);
  printf("frdddAdd() tests complete\n\n");
  fprintf(hOut, "frdddAdd() tests complete\n\n");
#endif

 /*---------------------------------------------------------------*
  * frsssSub Test                                                 *
  *   AtiFract16 frsssSub(AtiFract16, AtiFract16)                 *
  *---------------------------------------------------------------*/
#ifdef DETAIL_OUTPUT
  printf("frsssSub() tests started\n");
  fprintf(hOut, "frsssSub() tests started\n");
#endif
  errcntr = 0;
  for (i = 0; i < SUBTESTNUM; i++)
  {
    fr16 = frsssSub(subTestData[i].op1, subTestData[i].op2);
    if (fr16 != subTestData[i].result)
    {
#ifdef DUMP_ERRORS
      printf("Error frsssSub test %d: expected=0x%4.4hx, received=0x%4.4hx\n",
             i, subTestData[i].result, fr16);
      fprintf(hOut, "Error frsssSub test %d: expected=0x%4.4hx, received=0x%4.4hx\n",
             i, subTestData[i].result, fr16);
#endif
      errcntr++;
    }
  }
  totalerrs += errcntr;
#ifdef DETAIL_OUTPUT
  printf("frsssSub test completed with %d errors\n", errcntr);
  fprintf(hOut, "frsssSub test completed with %d errors\n", errcntr);
  printf("frsssSub() tests complete\n\n");
  fprintf(hOut, "frsssSub() tests complete\n\n");
#endif

 /*---------------------------------------------------------------*
  * frdddSub Test                                                 *
  *   AtiFract32 frdddSub(AtiFract32, AtiFract32)                 *
  *---------------------------------------------------------------*/
#ifdef DETAIL_OUTPUT
  printf("frdddSub() tests started\n");
  fprintf(hOut, "frdddSub() tests started\n");
#endif
  errcntr = 0;
  for (i = 0; i < LSUBTESTNUM; i++)
  {
    fr32 = frdddSub(lsubTestData[i].op1, lsubTestData[i].op2);
    if (fr32 != lsubTestData[i].result)
    {
#ifdef DUMP_ERRORS
      printf("Error frdddSub test %d: expected=0x%8.8lx, received=0x%8.8lx\n",
             i, lsubTestData[i].result, fr32);
      fprintf(hOut, "Error frdddSub test %d: expected=0x%8.8lx, received=0x%8.8lx\n",
             i, lsubTestData[i].result, fr32);
#endif
      errcntr++;
    }
  }
  totalerrs += errcntr;
#ifdef DETAIL_OUTPUT
  printf("frdddSub test completed with %d errors\n", errcntr);
  fprintf(hOut, "frdddSub test completed with %d errors\n", errcntr);
  printf("frdddSub() tests complete\n\n");
  fprintf(hOut, "frdddSub() tests complete\n\n");
#endif

 /*---------------------------------------------------------------*
  * frsiNorm Test                                                 *
  *   AtiInt16   frsiNorm(AtiFract16)                             *
  *---------------------------------------------------------------*/
#ifdef DETAIL_OUTPUT
  printf("frsiNorm() tests started\n");
  fprintf(hOut, "frsiNorm() tests started\n");
#endif
  errcntr = 0;
  for (i = 0; i < NORMTESTNUM; i++)
  {
    ires = frsiNorm(normTestData[i].op1);
    if (ires != normTestData[i].result)
    {
#ifdef DUMP_ERRORS
      printf("Error frsiNorm test %d: expected=%hx, received=%hx\n",
             i, normTestData[i].result, ires);
      fprintf(hOut, "Error frsiNorm test %d: expected=%hx, received=%hx\n",
             i, normTestData[i].result, ires);
#endif
      errcntr++;
    }
  }
  totalerrs += errcntr;
#ifdef DETAIL_OUTPUT
  printf("frsiNorm test completed with %d errors\n", errcntr);
  fprintf(hOut, "frsiNorm test completed with %d errors\n", errcntr);
  printf("frsiNorm() tests complete\n\n");
  fprintf(hOut, "frsiNorm() tests complete\n\n");
#endif

 /*---------------------------------------------------------------*
  * frdiNorm Test                                                 *
  *   AtiInt16   frdiNorm(AtiFract32)                             *
  *---------------------------------------------------------------*/
#ifdef DETAIL_OUTPUT
  printf("frdiNorm() tests started\n");
  fprintf(hOut, "frdiNorm() tests started\n");
#endif
  errcntr = 0;
  for (i = 0; i < LNORMTESTNUM; i++)
  {
    ires = frdiNorm(lnormTestData[i].op1);
    if (ires != lnormTestData[i].result)
    {
#ifdef DUMP_ERRORS
      printf("Error frdiNorm test %d: expected=%hx, received=%hx\n",
             i, lnormTestData[i].result, ires);
      fprintf(hOut, "Error frdiNorm test %d: expected=%hx, received=%hx\n",
             i, lnormTestData[i].result, ires);
#endif
      errcntr++;
    }
  }
  totalerrs += errcntr;
#ifdef DETAIL_OUTPUT
  printf("frdiNorm test completed with %d errors\n", errcntr);
  fprintf(hOut, "frdiNorm test completed with %d errors\n", errcntr);
  printf("frdiNorm() tests complete\n\n");
  fprintf(hOut, "frdiNorm() tests complete\n\n");
#endif

 /*---------------------------------------------------------------*
  * frsisShl Test                                                 *
  *   AtiFract16 frsisShl(AtiFract16, AtiInt16)                   *
  *---------------------------------------------------------------*/
#ifdef DETAIL_OUTPUT
  printf("frsisShl() tests started\n");
  fprintf(hOut, "frsisShl() tests started\n");
#endif
  errcntr = 0;
  for (i = 0; i < SHLTESTNUM; i++)
  {
    fr16 = frsisShl(shlTestData[i].op1, shlTestData[i].op2);
    if (fr16 != shlTestData[i].result)
    {
#ifdef DUMP_ERRORS
      printf("Error frsisShl test %d: expected=0x%4.4hx, received=0x%4.4hx\n",
             i, shlTestData[i].result, fr16);
      fprintf(hOut, "Error frsisShl test %d: expected=0x%4.4hx, received=0x%4.4hx\n",
             i, shlTestData[i].result, fr16);
#endif
      errcntr++;
    }
  }
  totalerrs += errcntr;
#ifdef DETAIL_OUTPUT
  printf("frsisShl test completed with %d errors\n", errcntr);
  fprintf(hOut, "frsisShl test completed with %d errors\n", errcntr);
  printf("frsisShl() tests complete\n\n");
  fprintf(hOut, "frsisShl() tests complete\n\n");
#endif

/*---------------------------------------------------------------*
 * frdidShl Test                                                 *
 *   AtiFract32 frdidShl(AtiFract32, AtiInt16)                   *
 *---------------------------------------------------------------*/
#ifdef DETAIL_OUTPUT
  printf("frdidShl() tests started\n");
  fprintf(hOut, "frdidShl() tests started\n");
#endif
  errcntr = 0;
  for (i = 0; i < LSHLTESTNUM; i++)
  {
    fr32 = frdidShl(lshlTestData[i].op1, lshlTestData[i].op2);
    if (fr32 != lshlTestData[i].result)
    {
#ifdef DUMP_ERRORS
      printf("Error frdidShl test %d: expected=0x%8.8lx, received=0x%8.8lx\n",
             i, lshlTestData[i].result, fr32);
      fprintf(hOut, "Error frdidShl test %d: expected=0x%8.8lx, received=0x%8.8lx\n",
             i, lshlTestData[i].result, fr32);
#endif
      errcntr++;
    }
  }
  totalerrs += errcntr;
#ifdef DETAIL_OUTPUT
  printf("frdidShl test completed with %d errors\n", errcntr);
  fprintf(hOut, "frdidShl test completed with %d errors\n", errcntr);
  printf("frdidShl() tests complete\n\n");
  fprintf(hOut, "frdidShl() tests complete\n\n");
#endif

 /*---------------------------------------------------------------*
  * frsisShr Test                                                 *
  *   AtiFract16 frsisShr(AtiFract16, AtiInt16)                   *
  *---------------------------------------------------------------*/
#ifdef DETAIL_OUTPUT
  printf("frsisShr() tests starting\n");
  fprintf(hOut, "frsisShr() tests starting\n");
#endif
  errcntr = 0;
  for (i = 0; i < SHRTESTNUM; i++)
  {
    fr16 = frsisShr(shrTestData[i].op1, shrTestData[i].op2);
    if (fr16 != shrTestData[i].result)
    {
#ifdef DUMP_ERRORS
      printf("Error frsisShr test %d: expected=0x%4.4hx, received=0x%4.4hx\n",
             i, shrTestData[i].result, fr16);
      fprintf(hOut, "Error frsisShr test %d: expected=0x%4.4hx, received=0x%4.4hx\n",
             i, shrTestData[i].result, fr16);
#endif
      errcntr++;
    }
  }
  totalerrs += errcntr;
#ifdef DETAIL_OUTPUT
  printf("frsisShr test completed with %d errors\n", errcntr);
  fprintf(hOut, "frsisShr test completed with %d errors\n", errcntr);
  printf("frsisShr() tests complete\n\n");
  fprintf(hOut, "frsisShr() tests complete\n\n");
#endif

 /*---------------------------------------------------------------*
  * frdidShr Test                                                 *
  *   AtiFract32 frdidShr(AtiFract32, AtiInt16)                   *
  *---------------------------------------------------------------*/
#ifdef DETAIL_OUTPUT
  printf("frdidShr() tests starting\n");
  fprintf(hOut, "frdidShr() tests starting\n");
#endif
  errcntr = 0;
  for (i = 0; i < LSHRTESTNUM; i++)
  {
    fr32 = frdidShr(lshrTestData[i].op1, lshrTestData[i].op2);
    if (fr32 != lshrTestData[i].result)
    {
#ifdef DUMP_ERRORS
      printf("Error frdidShr test %d: expected=0x%8.8lx, received=0x%8.8lx\n",
             i, lshrTestData[i].result, fr32);
      fprintf(hOut, "Error frdidShr test %d: expected=0x%8.8lx, received=0x%8.8lx\n",
             i, lshrTestData[i].result, fr32);
#endif
      errcntr++;
    }
  }
  totalerrs += errcntr;
#ifdef DETAIL_OUTPUT
  printf("frdidShr test completed with %d errors\n", errcntr);
  fprintf(hOut, "frdidShr test completed with %d errors\n", errcntr);
  printf("frdidShr() tests complete\n\n");
  fprintf(hOut, "frdidShr() tests complete\n\n");
#endif

 /*---------------------------------------------------------------*
  * frsisShrR Test                                                *
  *   AtiFract16 frsisShrR(AtiFract16, AtiInt16)                  *
  *---------------------------------------------------------------*/
#ifdef DETAIL_OUTPUT
  printf("frsisShrR() tests starting\n");
  fprintf(hOut, "frsisShrR() tests starting\n");
#endif
  errcntr = 0;
  for (i = 0; i < SHRRTESTNUM; i++)
  {
    fr16 = frsisShrR(shrrTestData[i].op1, shrrTestData[i].op2);
    if (fr16 != shrrTestData[i].result)
    {
#ifdef DUMP_ERRORS
      printf("Error frsisShrR test %d: expected=0x%4.4hx, received=0x%4.4hx\n",
             i, shrrTestData[i].result, fr16);
      fprintf(hOut, "Error frsisShrR test %d: expected=0x%4.4hx, received=0x%4.4hx\n",
             i, shrrTestData[i].result, fr16);
#endif
      errcntr++;
    }
  }
  totalerrs += errcntr;
#ifdef DETAIL_OUTPUT
  printf("frsisShrR test completed with %d errors\n", errcntr);
  fprintf(hOut, "frsisShrR test completed with %d errors\n", errcntr);
  printf("frsisShrR() tests complete\n\n");
  fprintf(hOut, "frsisShrR() tests complete\n\n");
#endif

 /*---------------------------------------------------------------*
  * frdidShrR Test                                                *
  *   AtiFract32 frdidShrR(AtiFract32, AtiInt16)                  *
  *---------------------------------------------------------------*/
#ifdef DETAIL_OUTPUT
  printf("frdidShrR() tests starting\n");
  fprintf(hOut, "frdidShrR() tests starting\n");
#endif
  errcntr = 0;
  for (i = 0; i < LSHRRTESTNUM; i++)
  {
    fr32 = frdidShrR(lshrrTestData[i].op1, lshrrTestData[i].op2);
    if (fr32 != lshrrTestData[i].result)
    {
#ifdef DUMP_ERRORS
      printf("Error frdidShrR test %d: expected=0x%8.8lx, received=0x%8.8lx\n",
             i, lshrrTestData[i].result, fr32);
      fprintf(hOut, "Error frdidShrR test %d: expected=0x%8.8lx, received=0x%8.8lx\n",
             i, lshrrTestData[i].result, fr32);
#endif
      errcntr++;
    }
  }
  totalerrs += errcntr;
#ifdef DETAIL_OUTPUT
  printf("frdidShrR test completed with %d errors\n", errcntr);
  fprintf(hOut, "frdidShrR test completed with %d errors\n", errcntr);
  printf("frdidShrR() tests complete\n\n");
  fprintf(hOut, "frdidShrR() tests complete\n\n");
#endif

 /*---------------------------------------------------------------*
  * frsssMult Test                                                *
  *   AtiFract16 frsssMult(AtiFract16, AtiFract16)                *
  *---------------------------------------------------------------*/
#ifdef DETAIL_OUTPUT
  printf("frsssMult() tests started\n");
  fprintf(hOut, "frsssMult() tests started\n");
#endif
  errcntr = 0;
  for (i = 0; i < MULTESTNUM; i++)
  {
    fr16 = frsssMult(mulTestData[i].op1, mulTestData[i].op2);
    if (fr16 != mulTestData[i].result)
    {
#ifdef DUMP_ERRORS
      printf("Error frsssMult test %d: expected=0x%4.4hx, received=0x%4.4hx\n",
             i, mulTestData[i].result, fr16);
      fprintf(hOut, "Error frsssMult test %d: expected=0x%4.4hx, received=0x%4.4hx\n",
             i, mulTestData[i].result, fr16);
#endif
      errcntr++;
    }
  }
  totalerrs += errcntr;
#ifdef DETAIL_OUTPUT
  printf("frsssMult test completed with %d errors\n", errcntr);
  fprintf(hOut, "frsssMult test completed with %d errors\n", errcntr);
  printf("frsssMult() tests complete\n\n");
  fprintf(hOut, "frsssMult() tests complete\n\n");
#endif

 /*---------------------------------------------------------------*
  * frssdMult Test                                                *
  *   AtiFract32 frssdMult(AtiFract16, AtiFract16)                *
  *---------------------------------------------------------------*/
#ifdef DETAIL_OUTPUT
  printf("frssdMult() tests started\n");
  fprintf(hOut, "frssdMult() tests started\n");
#endif
  errcntr = 0;
  for (i = 0; i < LMULTESTNUM; i++)
  {
    fr32 = frssdMult(lmulTestData[i].op1, lmulTestData[i].op2);
    if (fr32 != lmulTestData[i].result)
    {
#ifdef DUMP_ERRORS
      printf("Error frssdMult test %d: expected=0x%8.8lx, received=0x%8.8lx\n",
             i, lmulTestData[i].result, fr32);
      fprintf(hOut, "Error frssdMult test %d: expected=0x%8.8lx, received=0x%8.8lx\n",
             i, lmulTestData[i].result, fr32);
#endif
      errcntr++;
    }
  }
  totalerrs += errcntr;
#ifdef DETAIL_OUTPUT
  printf("frssdMult test completed with %d errors\n", errcntr);
  fprintf(hOut, "frssdMult test completed with %d errors\n", errcntr);
  printf("frssdMult() tests complete\n\n");
  fprintf(hOut, "frssdMult() tests complete\n\n");
#endif

 /*---------------------------------------------------------------*
  * frsssMultR Test                                               *
  *   AtiFract16 frsssMultR(AtiFract16, AtiFract16)               *
  *---------------------------------------------------------------*/
#ifdef DETAIL_OUTPUT
  printf("frsssMultR() tests started\n");
  fprintf(hOut, "frsssMultR() tests started\n");
#endif

  errcntr = 0;
  for (i = 0; i < MULRTESTNUM; i++)
  {
    fr16 = frsssMultR(mulrTestData[i].op1, mulrTestData[i].op2);
    if (fr16 != mulrTestData[i].result)
    {
#ifdef DUMP_ERRORS
      printf("Error frsssMultR test %d: expected=0x%4.4hx, received=0x%4.4hx\n",
             i, mulrTestData[i].result, fr16);
      fprintf(hOut, "Error frsssMultR test %d: expected=0x%4.4hx, received=0x%4.4hx\n",
             i, mulrTestData[i].result, fr16);
#endif
      errcntr++;
    }
  }
  totalerrs += errcntr;
#ifdef DETAIL_OUTPUT
  printf("frsssMultR test completed with %d errors\n", errcntr);
  fprintf(hOut, "frsssMultR test completed with %d errors\n", errcntr);
  printf("frsssMultR() tests complete\n\n");
  fprintf(hOut, "frsssMultR() tests complete\n\n");
#endif

 /*---------------------------------------------------------------*
  * frsssDivS Test                                                *
  *   AtiFract16 frsssDivS(AtiFract16, AtiFract16)                *
  *---------------------------------------------------------------*/
#ifdef DETAIL_OUTPUT
  printf("frsssDivS() tests started\n");
  fprintf(hOut, "frsssDivS() tests started\n");
#endif
  errcntr = 0;
  for (i = 0; i < DIVTESTNUM; i++)
  {
    fr16 = frsssDivS(divTestData[i].op1, divTestData[i].op2);
    if (fr16 != divTestData[i].result)
    {
#ifdef DUMP_ERRORS
      printf("Error frsssDivS test %d: expected=0x%4.4hx, received=0x%4.4hx\n",
             i, divTestData[i].result, fr16);
      fprintf(hOut, "Error frsssDivS test %d: expected=0x%4.4hx, received=0x%4.4hx\n",
             i, divTestData[i].result, fr16);
#endif
      errcntr++;
    }
  }
  totalerrs += errcntr;
#ifdef DETAIL_OUTPUT
  printf("frsssDivS test completed with %d errors\n", errcntr);
  fprintf(hOut, "frsssDivS test completed with %d errors\n", errcntr);
  printf("frsssDivS() tests complete\n\n");
  fprintf(hOut, "frsssDivS() tests complete\n\n");
#endif

 /*---------------------------------------------------------------*
  * frdssdMac Test                                                *
  *   AtiFract32 frdssdMac(AtiFract32, AtiFract16, AtiFract16)    *
  *---------------------------------------------------------------*/
#ifdef DETAIL_OUTPUT
  printf("frdssdMac() tests started\n");
  fprintf(hOut, "frdssdMac() tests started\n");
#endif
  errcntr = 0;
  for (i = 0; i < MACTESTNUM; i++)
  {
    fr32 = frdssdMac(macTestData[i].op1, macTestData[i].op2, macTestData[i].op3);
    if (fr32 != macTestData[i].result)
    {
#ifdef DUMP_ERRORS
      printf("Error frdssdMac test %d: expected=0x%8.8lx, received=0x%8.8lx\n",
             i, macTestData[i].result, fr32);
      fprintf(hOut, "Error frdssdMac test %d: expected=0x%8.8lx, received=0x%8.8lx\n",
             i, macTestData[i].result, fr32);
#endif
      errcntr++;
    }
  }
  totalerrs += errcntr;
#ifdef DETAIL_OUTPUT
  printf("frdssdMac test completed with %d errors\n", errcntr);
  fprintf(hOut, "frdssdMac test completed with %d errors\n", errcntr);
  printf("frdssdMac() tests complete\n\n");
  fprintf(hOut, "frdssdMac() tests complete\n\n");
#endif

 /*---------------------------------------------------------------*
  * frdsssMacR Test                                               *
  *   AtiFract16 frdsssMacR(AtiFract32, AtiFract16, AtiFract16)   *
  *---------------------------------------------------------------*/
#ifdef DETAIL_OUTPUT
  printf("frdsssMacR() tests started\n");
  fprintf(hOut, "frdsssMacR() tests started\n");
#endif
  errcntr = 0;
  for (i = 0; i < MACRTESTNUM; i++)
  {
    fr16 = frdsssMacR(macrTestData[i].op1, macrTestData[i].op2, macrTestData[i].op3);
    if (fr16 != macrTestData[i].result)
    {
#ifdef DUMP_ERRORS
      printf("Error frdsssMacR test %d: expected=0x%4.4hx, received=0x%4.4hx\n",
             i, macrTestData[i].result, fr16);
      fprintf(hOut, "Error frdsssMacR test %d: expected=0x%4.4hx, received=0x%4.4hx\n",
             i, macrTestData[i].result, fr16);
#endif
      errcntr++;
    }
  }
  totalerrs += errcntr;
#ifdef DETAIL_OUTPUT
  printf("frdsssMacR test completed with %d errors\n", errcntr);
  fprintf(hOut, "frdsssMacR test completed with %d errors\n", errcntr);
  printf("frdsssMacR() tests complete\n\n");
  fprintf(hOut, "frdsssMacR() tests complete\n\n");
#endif

 /*---------------------------------------------------------------*
  * frdssdMsu Test                                                *
  *   AtiFract32 frdssdMsu(AtiFract32, AtiFract16, AtiFract16)    *
  *---------------------------------------------------------------*/
#ifdef DETAIL_OUTPUT
  printf("frdssdMsu() tests started\n");
  fprintf(hOut, "frdssdMsu() tests started\n");
#endif
  errcntr = 0;
  for (i = 0; i < MSUTESTNUM; i++)
  {
    fr32 = frdssdMsu(msuTestData[i].op1, msuTestData[i].op2, msuTestData[i].op3);
    if (fr32 != msuTestData[i].result)
    {
#ifdef DUMP_ERRORS
      printf("Error frdssdMsu test %d: expected=0x%8.8lx, received=0x%8.8lx\n",
             i, msuTestData[i].result, fr32);
      fprintf(hOut, "Error frdssdMsu test %d: expected=0x%8.8lx, received=0x%8.8lx\n",
             i, msuTestData[i].result, fr32);
#endif
      errcntr++;
    }
  }
  totalerrs += errcntr;
#ifdef DETAIL_OUTPUT
  printf("frdssdMsu test completed with %d errors\n", errcntr);
  fprintf(hOut, "frdssdMsu test completed with %d errors\n", errcntr);
  printf("frdssdMsu() tests complete\n\n");
  fprintf(hOut, "frdssdMsu() tests complete\n\n");
#endif

 /*---------------------------------------------------------------*
  * frdsssMsuR Test                                               *
  *   AtiFract16 frdsssMsuR(AtiFract32, AtiFract16, AtiFract16)   *
  *---------------------------------------------------------------*/
#ifdef DETAIL_OUTPUT
  printf("frdsssMsuR() tests started\n");
  fprintf(hOut, "frdsssMsuR() tests started\n");
#endif
  errcntr = 0;
  for (i = 0; i < MSURTESTNUM; i++)
  {
    fr16 = frdsssMsuR(msurTestData[i].op1, msurTestData[i].op2, msurTestData[i].op3);
    if (fr16 != msurTestData[i].result)
    {
#ifdef DUMP_ERRORS
      printf("Error frdsssMsuR test %d: expected=0x%4.4hx, received=0x%4.4hx\n",
             i, msurTestData[i].result, fr16);
      fprintf(hOut, "Error frdsssMsuR test %d: expected=0x%4.4hx, received=0x%4.4hx\n",
             i, msurTestData[i].result, fr16);
#endif
      errcntr++;
    }
  }
  totalerrs += errcntr;
#ifdef DETAIL_OUTPUT
  printf("frdsssMsuR test completed with %d errors\n", errcntr);
  fprintf(hOut, "frdsssMsuR test completed with %d errors\n", errcntr);
  printf("frdsssMsuR() tests complete\n\n");
  fprintf(hOut, "frdsssMsuR() tests complete\n\n");
#endif


 /*---------------------------------------------------------------*
  * Finished testing, so return total errors for the pass         *
  *---------------------------------------------------------------*/
#ifdef DETAIL_OUTPUT
  printf("\n");
  fprintf(hOut, "\n");
  printf("fractTest test pass completed with %d total errors.\n\n\n", totalerrs);
  fprintf(hOut, "fractTest test pass completed with %d total errors.\n\n\n", totalerrs);
#endif
  return( totalerrs );
}



/****************************************************************************
 *                                                                          *
 * main() function                                                          *
 *                                                                          *
 ****************************************************************************/
int main()
{
  int passErrors;
  unsigned long totalErrors = 0;
  FILE *hOut;

  hOut = fopen("fractLibTestResult.txt", "w");

  /*---------------------------------------------------------------*
   * Perform Test on Fraction Library/Intrisic Functions           *
   *---------------------------------------------------------------*/
#ifdef DETAIL_OUTPUT
 printf("\nFractional Testing is about to begin!\n\n");
#endif

#ifdef MULTIPASS
  /* Run all tests multiple times */
  for (i = 0; i < 10000; i++)
  {
    passErrors = testPass(hOut);
    totalErrors += passErrors;
  }
#else
  /* Run one pass of the tests */
  passErrors = testPass(hOut);
  totalErrors += passErrors;
#endif

#ifdef DETAIL_OUTPUT
  printf("\nFractional Testing is finished!\n\n");
#endif

  return (0);
}


