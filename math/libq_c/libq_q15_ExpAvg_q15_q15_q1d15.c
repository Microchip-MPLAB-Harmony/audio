//*****************************************************************************
// File: libq_q15_ExpAvg_q15_q15_q1d15.c
//*****************************************************************************/

#include "libq_c.h"

//*****************************************************************************
//
// libq_q15_ExpAvg_q15_q15_q1d15()
//
// Description:
//   Exponential averaging implements a smoothing function based on the form:
//       avg[i+1] = avg[i] * lamda + new * (1-lamda)
//       avg[i+1] = (avg[i] - new) * lamda + new
//
// Arguments:
//   q15 prevAvg16  [in] Previous exponential average (Q1.15))
//   q15 newMeas16  [in] New value to be added to average (Q1.15))
//   q15 lamdaQ1d15 [in] Exp. Averaging constant (Q1.15))
//
// Return Value: 
//   q15 newAvg16 (Q1.15))
//
//*****************************************************************************/
q15 libq_q15_ExpAvg_q15_q15_q1d15(q15 prevAvg16,
                     q15 newMeas16,
                     q15 lamdaQ1d15)
{
  q15 newAvg16;   

  // avg[i+1] = (avg[i] - new) * lamda + new 
  newAvg16 = libq_q15_Sub_q15_q15(prevAvg16, newMeas16);
  newAvg16 = libq_q15_MultipyR2_q15_q15(lamdaQ1d15, newAvg16);
  newAvg16 = libq_q15_Add_q15_q15(newAvg16, newMeas16);

  return(newAvg16);
}