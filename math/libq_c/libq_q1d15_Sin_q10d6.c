//*****************************************************************************
//
// File: libq_q1d15_Sin_q10d6.c
//
// Description: This function approximates the sine of an angle.
//
//*****************************************************************************/

#include "libq_c.h"

#define ONEOVERTHREESIXTYQM7D23  Fl2FxPnt16(1.0/360,23)

//**********************************************************************
//
// libq_q1d15_Sin_q10d6()
//
// Description:
//   This function approximates the sine of an angle in degrees 
//   using the following algorithm: sin(x) = 3.140625x + 0.02026367x^2 -
//   5.325196x^3 + 0.5446778x^4 + 1.800293x^5.  The approximation
//   is accurate for any value of x from 0 degrees to 90 degrees.
//
// Arguments:
//   FxPnt16 angleQ10d6
//     [in] The angle in degrees for which the sine is computed
//
// Return Value:
//   q15.  sinQ1d15, the value of the computed sine.
//
//**********************************************************************
q15 libq_q1d15_Sin_q10d6(q15 angleQ10d6)
{
    int16_t i, signFlag;
    q15 positiveAngleQ10d6;/* Positive angle for quadrant
                                     determination */
    q15 scaledAngleQ1d15;  /* Angle scaled to 1.15 */
    q15 sinQ1d15;          /* This is the computed sine value */
  
    //These are the coefficients for the sine polynomial. They have been
    //scaled down by 8 to fit into a Q1.15 format 
    static const q15 sinCoefQ1d15[] = {Fl2Fract16(0.2250366),
                                       Fl2Fract16(0.0680847),
                                       Fl2Fract16(-0.6656495),
                                       Fl2Fract16(0.002532959),
                                       Fl2Fract16(0.392578)};
  
    positiveAngleQ10d6 = angleQ10d6;
    signFlag = 0;
  
    //If negative, make angle positive and set the sign flag. 
    if (angleQ10d6 < 0)
    {
        positiveAngleQ10d6 = libq_q15_Negate_q15(angleQ10d6);
        angleQ10d6 = libq_q15_Negate_q15(angleQ10d6);
        signFlag = 1;
    }
  
    if ((positiveAngleQ10d6 >= NINETYQ10D6) &&
        (positiveAngleQ10d6 < ONEEIGHTYQ10D6))
    {
        //2nd quadrant, sine(x) = sine(180-x) 
        angleQ10d6 = libq_q15_Sub_q15_q15(ONEEIGHTYQ10D6, positiveAngleQ10d6);
    }
    else if ((positiveAngleQ10d6 >= ONEEIGHTYQ10D6) &&
             (positiveAngleQ10d6 < TWOSEVENTYQ10D6))
    {
        //3rd quadrant sine(x) = -sine(x-180) 
        angleQ10d6 = libq_q15_Sub_q15_q15(positiveAngleQ10d6,ONEEIGHTYQ10D6);
    }
    else if (positiveAngleQ10d6 >= TWOSEVENTYQ10D6)
    {
        //4th quadrant sine(x) = -sine(360-x) 
        angleQ10d6 = libq_q15_Sub_q15_q15(THREESIXTYQ10D6,positiveAngleQ10d6);
    }
    //Compute scaled angle, i.e. convert to 1.15 
    scaledAngleQ1d15 = libq_q15_ExtractH_q31(
                           libq_q31_ShiftLeft_q31_i16(
                               libq_q31_Mult2_q15_q15(angleQ10d6,
                                                      ONEOVERTHREESIXTYQM7D23),
                                                      2));
  
    //Initialize sine to value of the first coefficient 
    sinQ1d15 = sinCoefQ1d15[0];
  
    //Compute sin using Taylor polynomial
    for (i = 1; i < sizeof(sinCoefQ1d15)/sizeof(sinCoefQ1d15[0]); i++)
    {
      //sinQ1d15 = scaledAngleQ1d15*sinQ1d15 + sinCoefQ1d15[i] 
      sinQ1d15 = libq_q15_Add_q15_q15(sinCoefQ1d15[i],
                            libq_q15_MultipyR2_q15_q15(sinQ1d15,
                                                       scaledAngleQ1d15));
    }
    //Perform the final multiplication; saves one iteration
    sinQ1d15 = libq_q15_MultipyR2_q15_q15(sinQ1d15,scaledAngleQ1d15);
  
    //sin = 8*sin, because coefficients were scaled down by 8
    sinQ1d15 = libq_q15_ShiftLeft_q15_i16(sinQ1d15,3);
  
    //sin(x) = -sin(x) in lower half plane
    if (((positiveAngleQ10d6 > ONEEIGHTYQ10D6) && (signFlag == 0)) ||
        ((positiveAngleQ10d6 < ONEEIGHTYQ10D6) && (signFlag == 1)))
    {
      sinQ1d15 =  libq_q15_Negate_q15(sinQ1d15);
    }
  
    return sinQ1d15;

} 


//******************************************************************************
// libq_q20d12_Sin_q20d12()
//
// Description:
//   A sine approximation via a third-order approx.
//
//   @param x    Angle (scaled to 2^15/(pi/2) = 2^13*4/(pi/2) units/cycle/)
//   @return     Sine value (Q12) 
//   - See more at: http://www.coranac.com/2009/07/sines/#sthash.JfBZosA7.dpuf
//
//   This function approximates the sin of angle in radians  using the
//   following third order apprx. 
//
// Algorithm:
//
//   s3(x) = (3/pi)x + (4/pi^3)x^3
//
//   with rescaled angle: z = x/(pi/2) 
//                        2^n = pi/2; n bits
//
//   Gives: 
//
//   s3(z) = (1/2)z(3-z^2)*2^A (scaled output)
//
//   Inner shifts of n and p gives:
//           
//   s3(z) = x(3*2^p - x^2*2^(p-2n)) / x^(A-n-1-p)
//
//         =x(3*2^p - x^2^r)s^s
//
//   Where r = 2n-p and s=n+p+1-A gives the best p as 15
//
//   Quadrants: 00,01,10,11; z%4 gives the value
//
//
// Arguments:
//
//   q31 angleQ20d12
//     [in] Fixed point fractional angle in radians on range +/- pi/2
//
// Return Value:
//   q31 sinQ20d12, the value of the computed sine.
//
//******************************************************************************
q31 libq_q20d12_Sin_q20d12(q31 x)
{
    //S(x) = angQN * ( (3<<p) - (x*x>>r) ) >> s

    static const int qN = 13;    //Q format for quarter cycle input 
    static const int qA = 12;    //Q format for output
    static const int qP = 15;    //Best Q format for parentheses intermediate
    int qR = 2*qN-qP;    //r = 2n-p 
    int qS = qN+qP+1-qA; //s = A-1-p-n

    //Dimensionless input (0,2) in Q13
    //Convert to z in Q13 to z in Q30 (2^13*4 units/per cycle) 
    //x = x<<(30-qN);  //Shift to full s32 range (Q13->Q30 leaving 1 int mag bit)
    
    //Radian input (0 to 2pi) in Q12
    //Q13 angle input in radians * 2^19/(pi/2) gives z in Q30, i.e. 19=(30-13) + 12 
    //x *= 83443;   
    
    //Radian input (0 to 2pi) in Q12
    //Q12 angle input in radians * 2^20/(pi/2) gives z in Q30, i.e. 20=(30-12) + 12 
    x *= 166886;   

    if( (x^(x<<1)) < 0)     // test for quadrant 1 or 2
    {
        x = (1<<31) - x;
    }

    x= x>>(30-qN); //Shift back to original Q format

    return ( x * ( (3<<qP) - (x*x>>qR) ) >> qS);
} 