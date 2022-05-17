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

//*****************************************************************************
// File: libq_q15_ExpAvg_q15_q15_q1d15.c
//*****************************************************************************

#include "audio/math/libq_c/libq_c.h"

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
//*****************************************************************************
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
