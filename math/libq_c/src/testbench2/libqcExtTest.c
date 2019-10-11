/******************************************************************************
 *
 * File: fractextTest.c
 *
 * Description: ATI extended fixed-point library tests.
 *
 * Notes:
 *   1.)  There are four #ifdef values as it currently stands:
 *        OUTPUT_ERRORS - Prints error status.  This is independent of the
 *                        DETAIL_OUTPUT flag.
 *        DETAIL_OUTPUT - Prints status information about each test.  Also, if
 *                        on, it will print error status.
 *
 * Revision History:
 * 14 Jun 06 - DHY - inception
 * 19 Jun 06 - DHY - Pre-review comments
 * 07 Jul 06 - DHY - Doc review
 * 11 Jul 06 - DHY - Pre-review comments
 * 17 Jul 06 - DHY - Code review comments
 * 20 Jul 06 - DHY - Post review comments
 *
 ******************************************************************************
 * Acoustic Technologies, Inc., copyright 2006
 * All rights reserved.
 ******************************************************************************
 * ------------------------------
 * Source Safe Revision History:
 * ------------------------------
 * $Log: /SYSTEMS/libraries/fractext/fractextTest.c $
 *       
 *       6     7/20/06 3:44p Hdeng
 *       Post review comments
 *
 *       5     7/19/06 4:13p Hdeng
 *       Code review comments
 *
 *       4     7/11/06 9:06a Hdeng
 *       Pre-review comments
 *
 *       3     7/07/06 9:55a Hdeng
 *       Changes based on doc review
 *
 *       2     6/19/06 1:31p Hdeng
 *       Pre-review comments
 *
 *       1     6/16/06 1:53p Hdeng
 *       Initial release
 *
 * $NoKeywords: $
 ******************************************************************************/

/* Include Files for all Targets */
#include <stdio.h>
#include "atitypes.h"
#include "aticonvert.h"
#include "fractext.h"

/* Defines for error reporting */
#undef DUMP_ERRORS
#ifdef DETAIL_OUTPUT
#define DUMP_ERRORS
#endif
#ifdef OUTPUT_ERRORS
#define DUMP_ERRORS
#endif

/* Data structures for testing data */
/************************************
 * Input:  AtiFract32
 * Result: Accumulator
 ************************************/
typedef struct _TestDataDA {
  AtiFract32 op1;
  AtiAccum result;
} TestDataDA;

/************************************
 * Input:  Accumulator
 * Result: AtiFract32
 ************************************/
typedef struct _TestDataAD {
  AtiAccum op1;
  AtiFract32 result;
} TestDataAD;

/************************************
 * Input1: Accumulator
 * Input2: Accumulator
 * Input3: AtiInt16
 * Result: Accumulator
 ************************************/
typedef struct _TestDataAAIA {
  AtiAccum op1;
  AtiAccum op2;
  AtiInt16 op3;
  AtiAccum result;
} TestDataAAIA;

/************************************
 * Input1: Accumulator
 * Input2: AtiInt16
 * Input3: AtiInt16
 * Result: Accumulator
 ************************************/
typedef struct _TestDataAIIA {
  AtiAccum op1;
  AtiInt16 op2;
  AtiInt16 op3;
  AtiAccum result;
} TestDataAIIA;


/* Define Test Data for fedaDeposit() */
#define DEPOSITTESTNUM  4
const TestDataDA depositTestData[DEPOSITTESTNUM] =
{
  {0,             { 0,              0}}, /* 0 */
  {-1,            {-1,             -1}}, /* 1 */
  {ATIMAXFRACT32, { 0,  ATIMAXFRACT32}}, /* 2 */
  {ATIMINFRACT32, {-1,  ATIMINFRACT32}}, /* 3 */
};

/* Define Test Data for feadExtract() */
#define EXTRACTTESTNUM  8
const TestDataAD extractTestData[EXTRACTTESTNUM] =
{
  { { 0,             0},             0}, /* 0 */
  { {-1, ATIMINFRACT32}, ATIMINFRACT32}, /* 1 */
  { { 1,             0}, ATIMAXFRACT32}, /* 2 */
  { { 1,            -1}, ATIMAXFRACT32}, /* 3 */
  { {-1, ATIMAXFRACT32}, ATIMINFRACT32}, /* 4 */
  { {-2,            -1}, ATIMINFRACT32}, /* 5 */
  { { 0, ATIMINFRACT32}, ATIMAXFRACT32}, /* 6 */
  { {-1,             0}, ATIMINFRACT32}, /* 7 */
};

/* Define Test Data for feaaiaAdd() */
#define ADDTESTNUM  10
const TestDataAAIA addTestData[ADDTESTNUM] =
{

  { {         0,              1}, { 0,             2}, 8,
    {         0,              3} },                       /* 0 */
  { {        -1,             -1}, {-1,            -2}, 8,
    {        -1,             -3} },                       /* 1 */
  { {         0,  ATIMAXFRACT32}, { 0,             2}, 8,
    {         0, -ATIMAXFRACT32} },                       /* 2 */
  { {         0,             -1}, { 0,             2}, 8,
    {         1,              1} },                       /* 3 */
  { {        -1,  ATIMINFRACT32}, {-1, ATIMINFRACT32}, 8,
    {        -1,              0} },                       /* 4 */
  { {       127,             -1}, { 0,             2}, 8,
    {       127,             -1} },                       /* 5 */
  { {4294967168,              0}, {-1,            -1}, 8,
    {4294967168,              0} },                       /* 6 */
  { {         1,             -1}, { 0,             2}, 2,
    {         1,             -1} },                       /* 7 */
  { {         0,     2147483647}, { 0,             2}, 0,
    {         0,     2147483647} },                       /* 8 */
  { {         0,     2147483647}, { 0,             2}, 1,
    {         0,     2147483649} },                       /* 9 */
};

/* Define Test Data for feaiiaShl() */
#define SHLTESTNUM  11
const TestDataAIIA shlTestData[SHLTESTNUM] =
{
  {  {         0,             1},   1,	 8,
     {         0,             2} },          /* 0 */
  {  {         0,             1},  31,	 8,
     {         0, ATIMINFRACT32} },          /* 1 */
  {  {         0,             1},  32,	 8,
     {         1,             0} },          /* 2 */
  {  {         1,             1},   1,	 8,
     {         2,             2} },          /* 3 */
  {  {        -1, ATIMINFRACT32},   1,	 8,
     {        -1,             0} },          /* 4 */
  {  {       126,             0},   1,	 8,
     {       127,            -1} },          /* 5 */
  {  {         1,             1},  -1,	 8,
     {         0, ATIMINFRACT32} },          /* 6 */
  {  {        -1, ATIMINFRACT32},  -3,	 8,
     {        -1,   -268435456} },           /* 7 */
  {  {4294967168,             0},   2,   8,
     {4294967168,            0} },           /* 8 */
  {  {        -1,    2147483648},   2,   1,
     {        -1,            0} },           /* 9 */
  {  {         0,    2147483646},   2,   0,
     {         0,   2147483647} },           /* 10 */
};

/* Define Test Data for feaiiaShr() */
#define SHRTESTNUM  10
const TestDataAIIA shrTestData[SHRTESTNUM] =
{
  {  {        0,             1},   1, 8,  { 0,             0} }, /* 0 */
  {  {        1,             0},  31, 8,  { 0,             2} }, /* 1 */
  {  {        1,             0},  33, 8,  { 0,             0} }, /* 2 */
  {  {        1,             1},   1, 8,  { 0, ATIMINFRACT32} }, /* 3 */
  {  {       -1, ATIMINFRACT32},   1, 8,  {-1,   -1073741824} }, /* 4 */
  {  {305419896,             0},  32, 8,  { 0,     305419896} }, /* 5 */
  {  {        1,             1},  -1, 8,  { 2,             2} }, /* 6 */
  {  {       -1, ATIMINFRACT32},  -3, 8,  {-4,             0} }, /* 7 */
  {  {        0, ATIMAXFRACT32},   3, 0,  { 0,     268435455} }, /* 8 */
  {  {       -1, ATIMINFRACT32},   1, 1,  {-1,    3221225472} }, /* 9 */
};

/****************************************************************************
 *                                                                          *
 * testPass() function                                                      *
 *                                                                          *
 ****************************************************************************/
int testPass(void)
{
  int i, errcntr, totalerrs;
  AtiFract32 a;
  AtiAccum acc;

  totalerrs = 0;

 /*---------------------------------------------------------------*
  * fedaDeposit Test                                              *
  *   AtiAccum fedaDeposit(AtiFract32 a)                          *
  *---------------------------------------------------------------*/
#ifdef DETAIL_OUTPUT
  printf("fedaDeposit() tests started\n");
#endif
  errcntr = 0;
  for (i = 0; i < DEPOSITTESTNUM; i++)
  {
    acc = fedaDeposit(depositTestData[i].op1);
    if ( (acc.guard != depositTestData[i].result.guard) ||
      (acc.base != depositTestData[i].result.base) )
    {
#ifdef DUMP_ERRORS
      printf("Error fedaDeposit test %d: expected=0x%8.8lx 0x%8.8lx, \
             received=0x%8.8lx 0x%8.8lx\n",
             i, depositTestData[i].result.guard, depositTestData[i].result.base,
             acc.guard, acc.base);
#endif
      errcntr++;
    }
  }
  totalerrs += errcntr;
#ifdef DETAIL_OUTPUT
  printf("fedaDeposit test completed with %d errors\n", errcntr);
  printf("fedaDeposit() tests complete\n\n");
#endif

 /*---------------------------------------------------------------*
  * feadExtract Test                                              *
  *   AtiFract32 feadExtract(AtiAccum acc)                        *
  *---------------------------------------------------------------*/
#ifdef DETAIL_OUTPUT
  printf("feadExtract() tests started\n");
#endif
  errcntr = 0;
  for (i = 0; i < EXTRACTTESTNUM; i++)
  {
    a = feadExtract(extractTestData[i].op1);
    if ( a != extractTestData[i].result )
    {
#ifdef DUMP_ERRORS
      printf("Error feadExtracttest %d: expected=0x%8.8lx, received=0x%8.8lx",
              i, extractTestData[i].result, a);
#endif
      errcntr++;
    }
  }
  totalerrs += errcntr;
#ifdef DETAIL_OUTPUT
  printf("feadExtract test completed with %d errors\n", errcntr);
  printf("feadExtract() tests complete\n\n");
#endif

/*---------------------------------------------------------------*
 * feaaiaAdd Test                                                *
 *   AtiAccum feaaiaAdd(AtiAccum acc1, AtiAccum acc2,            *
 *                      AtiInt16 numOfGuardBit)                  *
 *---------------------------------------------------------------*/
#ifdef DETAIL_OUTPUT
  printf("feaaiaAdd() tests started\n");
#endif
  errcntr = 0;
  for (i = 0; i < ADDTESTNUM; i++)
  {
    acc = feaaiaAdd(addTestData[i].op1,addTestData[i].op2,addTestData[i].op3);
    if ( (acc.guard != addTestData[i].result.guard) ||
      (acc.base != addTestData[i].result.base) )
    {
#ifdef DUMP_ERRORS
      printf("Error feaaiaAdd test %d: expected=0x%8.8lx 0x%8.8lx, \
              received=0x%8.8lx 0x%8.8lx\n",
              i, addTestData[i].result.guard, addTestData[i].result.base,
              acc.guard, acc.base);
#endif
      errcntr++;
    }
  }
  totalerrs += errcntr;
#ifdef DETAIL_OUTPUT
  printf("feaaiaAdd test completed with %d errors\n", errcntr);
  printf("feaaiaAdd() tests complete\n\n");
#endif

/*---------------------------------------------------------------*
 * feaiiaShl Test                                                *
 *   AtiAccumulator feaiiaShl(AtiAccumulator acc, AtiInt16 b,    *
 *                           AtiInt16 numOfGuardBit)             *
 *---------------------------------------------------------------*/
#ifdef DETAIL_OUTPUT
  printf("feaiiaShl() tests started\n");
#endif
  errcntr = 0;
  for (i = 0; i < SHLTESTNUM; i++)
  {
    acc = feaiiaShl(shlTestData[i].op1,shlTestData[i].op2, shlTestData[i].op3);
    if ( (acc.guard != shlTestData[i].result.guard) ||
      (acc.base != shlTestData[i].result.base) )
    {
#ifdef DUMP_ERRORS
      printf("Error feaiiaShl test %d: expected=0x%8.8lx 0x%8.8lx, \
              received=0x%8.8lx 0x%8.8lx\n",
              i, shlTestData[i].result.guard, shlTestData[i].result.base,
              acc.guard, acc.base);
#endif
      errcntr++;
    }
  }
  totalerrs += errcntr;
#ifdef DETAIL_OUTPUT
  printf("feaiiaShl test completed with %d errors\n", errcntr);
  printf("feaiiaShl() tests complete\n\n");
#endif

  /*-------------------------------------------------------------*
   * feaiiaShr Test                                              *
   *   AtiAccumulator feaiiaShr(AtiAccum acc, AtiInt16 b,        *
   *                           AtiInt16 numOfGuardBit)           *
   *-------------------------------------------------------------*/
#ifdef DETAIL_OUTPUT
  printf("feaiiaShr() tests started\n");
#endif
  errcntr = 0;
  for (i = 0; i < SHRTESTNUM; i++)
  {
    acc = feaiiaShr(shrTestData[i].op1,shrTestData[i].op2,shrTestData[i].op3);
    if ( (acc.guard != shrTestData[i].result.guard) ||
      (acc.base != shrTestData[i].result.base) )
    {
#ifdef DUMP_ERRORS
      printf("Error feaiiaShr test %d: expected=0x%8.8lx 0x%8.8lx,\
              received=0x%8.8lx 0x%8.8lx\n",
              i, shrTestData[i].result.guard, shrTestData[i].result.base,
              acc.guard, acc.base);
#endif
      errcntr++;
    }
  }
  totalerrs += errcntr;
#ifdef DETAIL_OUTPUT
  printf("feaiiaShr test completed with %d errors\n", errcntr);
  printf("feaiiaShr() tests complete\n\n");
#endif

 /*---------------------------------------------------------------*
  * Finished testing, so return total errors for the pass         *
  *---------------------------------------------------------------*/
#ifdef DETAIL_OUTPUT
  printf("\nfractextTest test pass completed with %d total errors.\n",
          totalerrs);
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
  int i;
  int passErrors;
  unsigned long totalErrors = 0;

  /*---------------------------------------------------------------*
   * Perform Test on Extended Fraction Library/Intrisic Functions  *
   *---------------------------------------------------------------*/
#ifdef DETAIL_OUTPUT
 printf("\nExtended Fractional Testing is about to begin!\n\n");
#endif

  /* Run tests */
  passErrors = testPass();
  totalErrors += passErrors;

  /* Wait loop for debug only purposes to check variable updates */
  i = 0;
  while( i < 5)
  {
    i++;
  }

#ifdef DETAIL_OUTPUT
  printf("\nExtended Fractional Testing is finished!\n");
#endif

  return (0);
}