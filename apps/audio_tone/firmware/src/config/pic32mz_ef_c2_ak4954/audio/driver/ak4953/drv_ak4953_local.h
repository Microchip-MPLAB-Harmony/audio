/*******************************************************************************
  AK4953 Codec Driver Interface

  Company:
    Microchip Technology Inc.

  File Name:
    drv_AK4953_local.h

  Summary:
    AK4953 Codec driver local declarations and definitions

  Description:
    This file contains the AK4953 Codec driver's local declarations and definitions.
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
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
// DOM-IGNORE-END

#ifndef _DRV_AK4953_LOCAL_H
#define _DRV_AK4953_LOCAL_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "configuration.h"
#include "drv_ak4953.h"
#include "system/int/sys_int.h"
#include "system/ports/sys_ports.h"
#include "osal/osal.h"
#include "driver/i2s/drv_i2s.h"
#include "driver/i2c/drv_i2c.h"
#include "system/time/sys_time.h"




// *****************************************************************************
// *****************************************************************************
// Section: Constants and Macros
// *****************************************************************************
// *****************************************************************************
/* AK4953 Driver I2C communication macros

  Summary:
    AK4953 driver I2C communication.

  Description:
    These constants provide AK4953 driver I2C communication information. 
    This includes the I2C address for the codec and other codec specific I2C macros if any.
    

  Remarks:
    AK4953 needs a dummy addr and byte to be sent to I2C module to start the communications.
 */

#define AK4953_I2C_ADDR (0x12)                  // was (0x24) -- switch to 7-bit addr for H3
#define AK4953_I2C_DUMMY_ADDR                   0x00
#define AK4953_I2C_DUMMY_BYTE                   0x00



// *****************************************************************************
/* AK4953 command queue size

  Summary
    Specifies AK4953 command queue size.

  Description
    This type specifies the AK4953 command queue size.

  Remarks:
  	None.
*/

#define AK4953_COMMAND_QUEUE_BUFFER 5

/* AK4953 Driver Version Macros

  Summary:
    AK4953 driver version.

  Description:
    These constants provide AK4953 driver version information. The driver
    version is:
    DRV_AK4953_VERSION_MAJOR.DRV_AK4953_VERSION_MINOR[.<DRV_AK4953_VERSION_PATCH>][DRV_AK4953_VERSION_TYPE]
    It is represented in DRV_I2S_VERSION as:
    MAJOR *10000 + MINOR * 100 + PATCH, so as to allow comparisons.
    It is also represented in string format in DRV_AK4953_VERSION_STR.
    DRV_AK4953_VERSION_TYPE provides the type of the release when the
    release is a(alpha) or b(beta). The interfaces DRV_AK4953_VersionGet
    and DRV_AK4953_VersionStrGet provide interfaces to the access the
    version and the version string.

  Remarks:
    Modify the return value of DRV_I2S_VersionStrGet and the
    DRV_AK4953_VERSION_MAJOR, DRV_AK4953_VERSION_MINOR,
    DRV_AK4953_VERSION_PATCH, and DRV_AK4953_VERSION_TYPE.
 */

#define _DRV_AK4953_VERSION_MAJOR         0
#define _DRV_AK4953_VERSION_MINOR         1
#define _DRV_AK4953_VERSION_PATCH         0
#define _DRV_AK4953_VERSION_TYPE          a
#define _DRV_AK4953_VERSION_STR           "0.1.0a"


// *****************************************************************************
/* AK4953 DMA transfer abort

  Summary:
    AK4953 DMA transfer abort

  Description:
    This constant indicates that the AK4953 DMA transfer is aborted.

  Remarks:
    None.
*/
#define DRV_AK4953_DMA_TRANSFER_ABORT /*DOM-IGNORE-BEGIN*/((uint32_t)(-1))/*DOM-IGNORE-END*/


// *****************************************************************************
/* AK4953 DMA transfer error

  Summary:
    AK4953 DMA transfer error

  Description:
    This constant indicates that the AK4953 DMA transfer has an address error.

  Remarks:
    None.
*/
#define DRV_AK4953_DMA_TRANSFER_ERROR /*DOM-IGNORE-BEGIN*/((uint32_t)(-1))/*DOM-IGNORE-END*/


// *****************************************************************************
/* AK4953 Master Clock from REFCLOCK

  Summary:
    AK4953 Master Clock from REFCLOCK

  Description:
    The following constants gives computation of divisor (RODIV) and
    trim value (ROTRIM) constants for generating master clock value to
    AK4953. The source t0 generate master clock is REFCLOCK.

  Remarks:
    None.
*/
#define DRV_AK4953_MCLK_RODIV(source, rate, ratio)  ((source / rate / (ratio << 1)) )
#define DRV_AK4953_MCLK_ROTRIM(source, rate, ratio) (((source / rate) & ((ratio << 1)-1)) )


// *****************************************************************************
/* AK4953 Control register bit fields

  Summary:
    AK4953 Control register bit fields

  Description:
    The following constants gives computation for writing to a particular bit or
    field in the AK4953 control registers

  Remarks:
    None.
*/
#define DRV_AK4953_CONTROL_REG_BIT_WRITE(regValue, pos, newvale)    \
                            ((regValue & ~(1<<(pos)) ) | ((0x1&(newvale))<<(pos)))

#define DRV_AK4953_CONTROL_REG_FIELD_WRITE(reg,mask,pos,val)        \
                            ((reg) & ~(mask)) | ((mask)&((val)<<(pos)))


// *****************************************************************************
// *****************************************************************************
// Section: CODEC 4953 Register Map
// *****************************************************************************
// *****************************************************************************

/*  Register Map for 4953

  Summary:
    Register Map for 4953, Registers listed have address 00H, 01H, 02H, 03H
	and so on, respectively

  Description:
    Registers in AK 4953 for programming and controlling various functions
	and features of the codec.
    Abbreviations for the Registers for 4953 are be used in individual bit names for
	identification purposes. e.g. Register AK4953A_REG_PWR_MGMT1, will have
        individual bits named as PWRMGMT1_PMADL_UP or PWRMGMT1_LSV_UP to indicate
        that the bits belong to the AK4953A_REG_PWR_MGMT1 register.


  Remarks: None
*/
typedef enum{
								
	AK4953A_REG_PWR_MGMT1=0,	//0		<--
	AK4953A_REG_PWR_MGMT2,		//1
	AK4953A_REG_SIG_SLCT1,		//2
	AK4953A_REG_SIG_SLCT2,		//3
        AK4953A_REG_SIG_SLCT3,		//4
	AK4953A_REG_MODE_CTRL1,		//5
	AK4953A_REG_MODE_CTRL2,		//6
        AK4953A_REG_MODE_CTRL3,		//7
        AK4953A_REG_DIG_MIC,            //8
	AK4953A_REG_TMR_SLCT,		//9
        AK4953A_REG_ALC_TMR_SLCT,       //A
        AK4953A_REG_ALC_MODE_CTRL1,     //B
	AK4953A_REG_ALC_MODE_CTRL2,	//C
        AK4953A_REG_ALC_MODE_CTRL3,	//D
        AK4953A_REG_ALC_VLM,            //E
	AK4953A_REG_LIN_VOL_CTRL,       //F
        AK4953A_REG_RIN_VOL_CTRL,       //10
        AK4953A_REG_LOP_VOL_CTRL,       //11
        AK4953A_REG_ROP_VOL_CTRL,       //12
        AK4953A_REG_LDIG_VOL_CTRL,       //13
        AK4953A_REG_RDIG_VOL_CTRL,       //14
	AK4953A_REG_BEEP_FREQ,          //15
	AK4953A_REG_BEEP_ON_TIME,       //16
	AK4953A_REG_BEEP_OFF_TIME,      //17
	AK4953A_REG_BEEP_RPT_CNT,       //18
	AK4953A_REG_BEEP_VLM_CTRL,	//19
	AK4953A_REG_DFLTR_SLCT_1,       //1C
	AK4953A_REG_DFLTR_MODE,         //1D
	AK4953A_REG_HPF2_0_SET,         //1E
        AK4953A_REG_HPF2_1_SET,   	//1F
        AK4953A_REG_HPF2_2_SET,   	//20
        AK4953A_REG_HPF2_3_SET,   	//21
        AK4953A_REG_LPF_0_SET,   	//22
        AK4953A_REG_LPF_1_SET,   	//23
        AK4953A_REG_LPF_2_SET,   	//24
        AK4953A_REG_LPF_3_SET,   	//25
        AK4953A_REG_DFLTR_SLCT_2,       //30
        AK4953A_REG_E1_0_SET,            //32
        AK4953A_REG_E1_1_SET,            //33
        AK4953A_REG_E1_2_SET,            //34
        AK4953A_REG_E1_3_SET,            //35
        AK4953A_REG_E1_4_SET,            //36
        AK4953A_REG_E1_5_SET,            //37
        AK4953A_REG_E2_0_SET,            //38
        AK4953A_REG_E2_1_SET,            //39
        AK4953A_REG_E2_2_SET,            //3A
        AK4953A_REG_E2_3_SET,            //3B
        AK4953A_REG_E2_4_SET,            //3C
        AK4953A_REG_E2_5_SET,            //3D
        AK4953A_REG_E3_0_SET,            //3E
        AK4953A_REG_E3_1_SET,            //3F
        AK4953A_REG_E3_2_SET,            //40
        AK4953A_REG_E3_3_SET,            //41
        AK4953A_REG_E3_4_SET,            //42
        AK4953A_REG_E3_5_SET,            //43
        AK4953A_REG_E4_0_SET,            //44
        AK4953A_REG_E4_1_SET,            //45
        AK4953A_REG_E4_2_SET,            //46
        AK4953A_REG_E4_3_SET,            //47
        AK4953A_REG_E4_4_SET,            //48
        AK4953A_REG_E4_5_SET,            //49
        AK4953A_REG_E5_0_SET,            //4A
        AK4953A_REG_E5_1_SET,            //4B
        AK4953A_REG_E5_2_SET,            //4C
        AK4953A_REG_E5_3_SET,            //4D
        AK4953A_REG_E5_4_SET,            //4E
        AK4953A_REG_E5_5_SET,            //4F
	
}DRV_AK4953_CONTROL_REGISTER;

/* Register bit fields. */
//Register Name: Power Management 1
#define PWRMGMT1_PMADL_UP    	(1 << 0)
#define PWRMGMT1_PMADR_UP	(1 << 1)
#define PWRMGMT1_PMDAC_UP	(1 << 2)
#define PWRMGMT1_LSV_UP         (1 << 3)
#define PWRMGMT1_PMSPK_UP	(1 << 4)
#define PWRMGMT1_PMBP_UP	(1 << 5)
#define PWRMGMT1_PMVCM_UP	(1 << 6)
#define PWRMGMT1_PMPFIL_UP	(1 << 7)

//Register Name: Power Management 2
#define PWRMGMT2_PMPLL	    (1 << 0)
#define PWRMGMT2_MCKO	    (1 << 1)
#define PWRMGMT2_MS_MASTER  (1 << 3)
#define PWRMGMT2_PMHPR_UP   (1 << 4)
#define PWRMGMT2_PMHPL_UP   (1 << 5)

//Register Name: Signal Select 1
#define SIGSLCT1_MGAIN0_SET		(1 << 0)
#define SIGSLCT1_MGAIN1_SET		(1 << 1)
#define SIGSLCT1_MGAIN2_SET		(1 << 2)
#define SIGSLCT1_PMMP_UP		(1 << 3)
#define SIGSLCT1_MPSEL_UP		(1 << 4)
#define SIGSLCT1_DACS_ON		(1 << 5)
#define SIGSLCT1_SPPSN_UP		(1 << 7)

//Register Name: Signal Select 2
#define SIGSLCT2_INL0_ON		(1 << 0)
#define SIGSLCT2_INR0_ON		(1 << 1)
#define SIGSLCT2_INL1_ON		(1 << 2)
#define SIGSLCT2_INR1_ON		(1 << 3)
#define SIGSLCT2_SPKG0_ON               (1 << 6)
#define SIGSLCT2_SPKG1_ON               (1 << 7)


//Register Name: Signal Select 3
#define SIGSLCT3_HPM_UP		(1 << 2)
#define SIGSLCT3_MOFF_UP        (1 << 3)
#define SIGSLCT3_PTS0_UP        (1 << 4)
#define SIGSLCT3_PTS1_UP	(1 << 5)

//Register Name: Mode Control 1
#define MODECTRL1_DIF0_UP       (1 << 0)
#define MODECTRL1_DIF1_UP       (1 << 1)
#define MODECTRL1_HPZ_UP        (1 << 2)
#define MODECTRL1_BCK0_UP       (1 << 3)
#define MODECTRL1_PLL0_UP       (1 << 4)
#define MODECTRL1_PLL1_UP       (1 << 5)
#define MODECTRL1_PLL2_UP       (1 << 6)
#define MODECTRL1_PLL3_UP       (1 << 7)

//Register Name: Mode Control 2
#define MODECTRL2_FS0_UP       (1 << 0)
#define MODECTRL2_FS1_UP       (1 << 1)
#define MODECTRL2_FS2_UP        (1 << 2)
#define MODECTRL2_FS3_UP       (1 << 3)
#define MODECTRL2_DS_UP       (1 << 4)
#define MODECTRL2_CPCK_UP       (1 << 5)
#define MODECTRL2_PS0_UP       (1 << 6)
#define MODECTRL2_PS1_UP       (1 << 7)

//Register Name: Mode Control 3
#define MODECTRL3_DEM0_UP       (1 << 0)
#define MODECTRL3_DEM1_UP       (1 << 1)
#define MODECTRL3_IVOLC_UP        (1 << 2)
#define MODECTRL3_OVOLC_UP       (1 << 3)
#define MODECTRL3_DVOLC_UP       (1 << 4)
#define MODECTRL3_SMUTE_UP       (1 << 5)
#define MODECTRL3_THDET_UP       (1 << 6)
#define MODECTRL3_READ_UP       (1 << 7)

//Register Name: Timer Select
#define TMRSLCT_DVTM0_UP	(1 << 0)
#define TMRSLCT_DVTM1_UP	(1 << 01
#define TMRSLCT_ADRST0		(2 << 0)
#define TMRSLCT_AFRST1		(0 << 2)
#define TMRSLCT_WTM_256FS		(1 << 2)
#define TMRSLCT_WTM_512FS		(2 << 2)
#define TMRSLCT_WTM_1024FS		(3 << 2)
#define TMRSLCT_WTM_2048FS		(16 << 2)
#define TMRSLCT_WTM_4096FS		(17 << 2)
#define TMRSLCT_WTM_8192FS		(18 << 2)
#define TMRSLCT_WTM_16384FS		(19 << 2)
#define TMRSLCT_ZTM_128FS		(0 << 4)
#define TMRSLCT_ZTM_256FS		(1 << 4)
#define TMRSLCT_ZTM_512FS		(2 << 4)
#define TMRSLCT_ZTM_1024FS		(3 << 4)
#define TMRSLCT_DVTM_256FS		(1 << 7)

#define ALCCTRL1_LMTH0_SET		(1 << 0)
#define ALCCTRL1_RGAIN0_SET		(1 << 1)
#define ALCCTRL1_LMAT_0P375		(0 << 2)
#define ALCCTRL1_LMAT_0P750		(1 << 2)
#define ALCCTRL1_LMAT_1P125		(2 << 2)
#define ALCCTRL1_LMAT_1P500		(3 << 2)
#define ALCCTRL1_ZELMN_DISABLE	(1 << 4)
#define ALCCTRL1_ALC_ENABLE		(1 << 5)

#define ALCCTRL2_REF(n)			(n)

#define LINPGAGAIN_IVL(n)		(n)
#define RINPGAGAIN_IVL(n)		(n)

#define LDACVOL_DVL(n)			(n)
#define RDACVOL_DVL(n)			(n)

#define ALCCTRL3_VBAT_0P64AVDD	(1 << 1)
#define ALCCTRL3_LMTH1_SET		(1 << 6)
#define ALCCTRL3_RGAIN1_SET		(1 << 7)

#define MODECTRL3_DEM_44K1HZ	(0 << 0)
#define MODECTRL3_DEM_OFF		(1 << 0)
#define MODECTRL3_DEM_48KHZ		(2 << 0)
#define MODECTRL3_DEM_32KHZ		(3 << 0)
#define MODECTRL3_BST_OFF		(0 << 2)
#define MODECTRL3_BST_MIN		(1 << 2)
#define MODECTRL3_BST_MID		(2 << 2)
#define MODECTRL3_BST_MAX		(3 << 2)
#define MODECTRL3_DVOLC_DEP		(1 << 4)
#define MODECTRL3_SMUTE_ENABLE	(1 << 5)
#define MODECTRL3_LOOP_ENABLE	(1 << 6)

#define MODECTRL4_DAC_ON		(1 << 0)
#define MODECTRL4_MINH_ON		(1 << 1)
#define MODECTRL4_HPM_MONO		(1 << 2)
#define MODECTRL4_IVOLC_DEP		(1 << 3)

#define PWRMGMT3_PMADR_UP	    (1 << 0)
#define PWRMGMT3_INL0_SET	    (1 << 1)
#define PWRMGMT3_INR0_SET	    (1 << 2)
#define PWRMGMT3_MDIF1_DIFFIN   (1 << 3)
#define PWRMGMT3_MDIF2_DIFFIN   (1 << 4)
#define PWRMGMT3_HPG_3P6DB	    (1 << 5)
#define PWRMGMT3_INL1_SET	    (1 << 6)
#define PWRMGMT3_INR1_SET	    (1 << 7)

#define DFLTRSLCT_FIL3_ENABLE	(1 << 2)
#define DFLTRSLCT_EQ_ENABLE		(1 << 3)
#define DFLTRSLCT_FIL1_ENABLE	(1 << 4)
#define DFLTRSLCT_GN_0DB		(0 << 6)
#define DFLTRSLCT_GN_12DB		(1 << 6)
#define DFLTRSLCT_GN_24DB		(2 << 6)


#define FLTR30SET_F3A(nA)		(0xFF & nA)
#define FLTR31SET_F3A(nA)		((0x3F00 & nA) >> 8)
#define FLTR31SET_F3AS_LPF		(1 << 7)
#define FLTR32SET_F3B(nB)		(0xFF & nB)
#define FLTR33SET_F3B(nB)		((0x3F00 & nB) >> 8)
#define EQ0SET_EQA(nA)			(0xFF & nA)
#define EQ1SET_EQA(nA)			((0xFF00 & nA) >> 8)
#define EQ2SET_EQB(nB)			(0xFF & nB)
#define EQ3SET_EQB(nB)			((0x3F00 & nB) >> 8)
#define EQ4SET_EQC(nC)			(0xFF & nC)
#define EQ5SET_EQC(nC)			((0xFF00 & nC) >> 8)
#define FLTR10SET_F1A(nA)		(0xFF & nA)
#define FLTR11SET_F1A(nA)		((0x3F00 & nA) >> 8)
#define FLTR11SET_F1AS_LPF		(1 << 7)
#define FLTR12SET_F1B(nB)		(0xFF & nB)
#define FLTR13SET_F1B(nB)		((0x3F00 & nB) >> 8)


#define PWRMGMT4_PMMICL_UP	    (1 << 0)
#define PWRMGMT4_PMMICR_UP	    (1 << 1)
#define PWRMGMT4_PMAINL2_UP	    (1 << 2)
#define PWRMGMT4_PMAINR2_UP	    (1 << 3)
#define PWRMGMT4_PMAINL3_UP	    (1 << 4)
#define PWRMGMT4_PMAINR3_UP	    (1 << 5)
#define PWRMGMT4_PMAINL4_UP	    (1 << 6)
#define PWRMGMT4_PMAINR4_UP	    (1 << 7)

#define MODECTRL5_LODIF_DIFFOUT	(1 << 0)
#define MODECTRL5_AIN3_STEREOIN	(1 << 1)
#define MODECTRL5_MIX_MONO		(1 << 2)
#define MODECTRL5_L4DIF_DIFFIN	(1 << 3)
#define MODECTRL5_MICL3_MICAMP	(1 << 4)
#define MODECTRL5_MICR3_MICAMP	(1 << 5)

#define OUTMIXERCTRL_LINL2_ON	(1 << 0)
#define OUTMIXERCTRL_RINL2_ON	(1 << 1)
#define OUTMIXERCTRL_LINL3_ON	(1 << 2)
#define OUTMIXERCTRL_RINL3_ON	(1 << 3)
#define OUTMIXERCTRL_LINL4_ON	(1 << 4)
#define OUTMIXERCTRL_RINL4_ON	(1 << 5)
#define OUTMIXERCTRL_LOM3_MONO	(1 << 6)
#define OUTMIXERCTRL_LOM_MONO	(1 << 7)

#define HPMIXERCTRL_LINH2_ON	(1 << 0)
#define HPMIXERCTRL_RINH2_ON	(1 << 1)
#define HPMIXERCTRL_LINH3_ON	(1 << 2)
#define HPMIXERCTRL_RINH3_ON	(1 << 3)
#define HPMIXERCTRL_LINH4_ON	(1 << 4)
#define HPMIXERCTRL_RINH4_ON	(1 << 5)
#define HPMIXERCTRL_HPM3_MONO	(1 << 6)

// *****************************************************************************
/* AK4953 Channel Attenuation

  Summary:
    AK4953 Channel Attenuation

  Description:
    The following constants left and right channel attenuation

  Remarks:
    None.
*/
#define DRV_AK4953_LATT_ATT(n)                      (n)
#define DRV_AK4953_RATT_ATT(n)                      (n)
#define DRV_AK4953_ATT_MASK                         0xFF

// *****************************************************************************
/* AK4953 Volume Mapping 

  Summary:
    AK4953 Volume Mapping

  Description:
    Volume gain for Audio applications using Codec / DAC needs to be scaled as 
    the dB range supported by the Codec is too large for human hearing.
    The allowed range of the codec stretches from +12dB to -115 dB.
    However for most applications, the entire dB range is not audible.
    So we first define the modified dB range for the driver and then normalize 
    the volume value for that range.
    by converting max gain in dB to max gain in counts, 0x00 to 0xFF
    The MAximum and minimum values specified can be a floating point number.  
    It can be negative as well.
    (It is converted to codec gain counts before being used in the code.)
   
  Remarks:
    None.
*/

#define MAX_AUDIO_OUTPUT_GAIN_IN_dB   12.0
#define MIN_AUDIO_OUTPUT_GAIN_IN_dB  -60.0
#define MAX_AUDIO_GAIN  ( MAX_AUDIO_OUTPUT_GAIN_IN_dB >   12 ?   12 : MAX_AUDIO_OUTPUT_GAIN_IN_dB )
#define MIN_AUDIO_GAIN  ( MIN_AUDIO_OUTPUT_GAIN_IN_dB < -115 ? -115 : MIN_AUDIO_OUTPUT_GAIN_IN_dB )

#define MAX_VOLUME_CODE_FLOAT ( 256.0 - 2.0*(12.0-MAX_AUDIO_GAIN) )
#define MAX_VOLUME_CODE_ROUND (uint8_t) MAX_VOLUME_CODE_FLOAT 
#define MAX_VOLUME_CODE       ( MAX_VOLUME_CODE_FLOAT > 255 ? 255 : MAX_VOLUME_CODE_ROUND )

#define MIN_VOLUME_CODE_FLOAT ( 256.0 - 2.0*(12.0-MIN_AUDIO_GAIN) )
#define MIN_VOLUME_CODE_ROUND (int8_t)MIN_VOLUME_CODE_FLOAT 
#define MIN_VOLUME_CODE       ( MIN_VOLUME_CODE_ROUND )
// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
/* AK4953 Supported control commands

  Summary:
    AK4953 Supported control commands

  Description:
    This enumeration identifies AK4953 supported control commands

  Remarks:
    None.
*/
typedef enum
{
    DRV_AK4953_COMMAND_NONE,
    DRV_AK4953_COMMAND_INIT_START,
    DRV_AK4953_COMMAND_INIT_CLK_PDN_SET,
    DRV_AK4953_COMMAND_INIT_MODE_CTRL_SET,
    DRV_AK4953_COMMAND_DFLTR_MODE_SET,
    DRV_AK4953_COMMAND_INIT_AUDIOFORMAT,
    DRV_AK4953_COMMAND_INIT_END,
    DRV_AK4953_COMMAND_SAMPLING_RATE_SET,
    DRV_AK4953_COMMAND_VOLUME_SET_CHANNEL_LEFT,
    DRV_AK4953_COMMAND_VOLUME_SET_CHANNEL_RIGHT,
    DRV_AK4953_COMMAND_VOLUME_SET_CHANNEL_LEFT_ONLY,
    DRV_AK4953_COMMAND_VOLUME_SET_CHANNEL_RIGHT_ONLY,
    DRV_AK4953_COMMAND_VOLUME_SET_CHANNELS_INIT,
    DRV_AK4953_COMMAND_MUTE_ON,
    DRV_AK4953_COMMAND_MUTE_OFF,
    DRV_AK4953_COMMAND_DIGITAL_BLOCK_CONTROL_SET,
    DRV_AK4953_COMMAND_MONO_STEREO_MIC_SET,
    DRV_AK4953_COMMAND_INT_EXT_MIC_SET,
    DRV_AK4953_COMMAND_MIC_SET,
    DRV_AK4953_COMMAND_SEND,
    DRV_AK4953_COMMAND_COMPLETE
}DRV_AK4953_COMMAND;


// *****************************************************************************
/* AK4953 supported sampling rates

  Summary:
    AK4953 supported sampling rates

  Description:
    This enumeration identifies AK4953 supported sampling rates

  Remarks:
    None.
*/
typedef enum
{
    DRV_AK4953_SAMPLERATE_192000HZ = 192000,
    DRV_AK4953_SAMPLERATE_176400HZ = 176400,
    DRV_AK4953_SAMPLERATE_120000HZ = 120000,
    DRV_AK4953_SAMPLERATE_96000HZ =  96000,
    DRV_AK4953_SAMPLERATE_88200HZ =  88200,
    DRV_AK4953_SAMPLERATE_60000HZ =  60000,
    DRV_AK4953_SAMPLERATE_48000HZ =  48000,
    DRV_AK4953_SAMPLERATE_44100HZ =  44100,
    DRV_AK4953_SAMPLERATE_32000HZ =  32000,
    DRV_AK4953_SAMPLERATE_24000HZ =  24000,
    DRV_AK4953_SAMPLERATE_16000HZ =  16000,
    DRV_AK4953_SAMPLERATE_8000HZ =   8000,
    DRV_AK4953_SAMPLERATE_DEFAULT =  44100
} DRV_AK4953_SAMPLERATE;


// *****************************************************************************
/* AK4953 Driver task states

  Summary
    Lists the different states that AK4953 Driver task routine can have.

  Description
    This enumeration lists the different states that AK4953 Driver task routine can have.

  Remarks:
    None.
*/

typedef enum
{
    DRV_AK4953_TASK_STATE_CONTROL,
    DRV_AK4953_TASK_STATE_DATA_IN,
    DRV_AK4953_TASK_STATE_DATA_OUT,
} DRV_AK4953_TASK;


/**********************************************
 * Driver Client Obj
 **********************************************/
typedef struct _DRV_AK4953_CLIENT_OBJ_STRUCT
{
    /* Indicates that this object is in use */
    bool inUse;

    /* Indicate whether the client is open in
     * read,write or read/write mode */
    DRV_IO_INTENT ioIntent;

    /* Call back function for this client */
    DRV_AK4953_BUFFER_EVENT_HANDLER  pEventCallBack;


    /* Client data(Event Context) that will be
     * returned at callback */
    uintptr_t hClientArg;

    /* pointer to the driver that own this object */
    void* hDriver;

} DRV_AK4953_CLIENT_OBJ;


/***********************************************
 * Driver object structure. One object per
 * hardware instance
 **********************************************/

typedef struct _DRV_AK4953_OBJ_STRUCT
{

    /* Status of this driver instance */
    SYS_STATUS status;

    /* Indicates this object is in use */
    bool inUse;

    /* Flag to indicate that the hardware instance is used
     *  in exclusive access mode */
    bool isExclusive;

    /* Number of clients possible with the hardware instance */
    uint8_t numClients;

    /* Identifies data module(I2S) driver ID for
     * data interface of CODEC */
    SYS_MODULE_INDEX i2sDriverModuleIndex;

    /* Identifies data module(I2C) driver ID for
     * control interface of CODEC */
    SYS_MODULE_INDEX i2cDriverModuleIndex;
    /* Identifies data module(I2S) driver open handle */
    DRV_HANDLE i2sDriverHandle;

    DRV_HANDLE i2sDriverClientHandleRead;

    DRV_HANDLE i2sDriverClientHandleWrite;

    /* Identifies control module timer ID for
     * control interface of CODEC */
    SYS_MODULE_INDEX tmrDriverModuleIndex;

    /* Identifies control module(Timer) driver open handle */    
    DRV_HANDLE tmrDriverHandle;

    /* Sampling rate */
    uint32_t samplingRate;
    
    /* Identifies the Audio data format */
    DRV_AK4953_AUDIO_DATA_FORMAT audioDataFormat;

     /* Keeps track if the driver is in interrupt
     * context */
    bool isInInterruptContext;

    /* Hardware instance mutex */
    OSAL_MUTEX_DECLARE(mutexDriverInstance);

    /* AK4953 hardware object task state */
    DRV_AK4953_TASK task;

    /* ----------------------------------------------------*/
    /* Control interface specific Implementation variables */
    /* ----------------------------------------------------*/

    /* Volume for volume command */
    uint8_t volume[DRV_AK4953_NUMBER_OF_CHANNELS];

    /* Volume set under process */
    bool isVolumeSetUnderProcess;

    /* Command under execution */
    DRV_AK4953_COMMAND command;

    /* Command value being transmitted */
    uint32_t controlCommand;

    /* Status of command under execution */
    bool controlCommandStatus;

    /* Number of bits transfered for a control command */
    uint8_t countBit;

    /* Array holding the last programmed command value */
    uint8_t lastRegValue[90];

    /* Command complete callback function */
    DRV_AK4953_COMMAND_EVENT_HANDLER commandCompleteCallback;

    /* command complete event context */
    uintptr_t commandContextData;

    uint16_t mclk_multiplier;
    
    uintptr_t drvI2CMasterHandle;
    uintptr_t drvI2CBuffHandle;
    
    bool readComplete;
    bool writeComplete;
    
    DRV_HANDLE hWriteTransfer;
    DRV_HANDLE hReadTransfer;
    
} DRV_AK4953_OBJ;


// *****************************************************************************
/* AK4953 Driver Global Instances Object

  Summary:
    Object used to keep track of data that is common to all instances of the
    AK4953 driver.

  Description:
    This object is used to keep track of any data that is common to all
    instances of the AK4953 driver.

  Remarks:
    None.
*/

typedef struct
{
    /* Set to true if all members of this structure
       have been initialized once */
    bool membersAreInitialized;

    /* Mutex to protect client object pool */
    OSAL_MUTEX_DECLARE(mutexClientObjects);

} DRV_AK4953_COMMON_DATA_OBJ;


// *****************************************************************************
/* AK4953 Driver Control Command Object

  Summary:
    Object used to keep track of control data.

  Description:
    Object used to keep track of control data.

  Remarks:
    None.
*/
typedef struct{
    uint8_t command;
    uint8_t control_data[3];
    uint8_t array_size;
    DRV_HANDLE drvI2CBufHandle;
}AK4953_COMMAND;

// *****************************************************************************
/* AK4953 Driver Control Command Queue

  Summary:
    Object used to keep track of AK4953 Command Queue.

  Description:
    Object used to keep track of AK4953 Command Queue.

  Remarks:
    None.
*/
typedef struct{
    AK4953_COMMAND commandBuffer[AK4953_COMMAND_QUEUE_BUFFER];
    int8_t queueIn;
    int8_t queueOut;
    int8_t status; // Empty:0, Full:1, Normal:2
}AK4953_COMMAND_QUEUE;

/**************************************
 * Local functions.
 *************************************/
// *****************************************************************************
/* Function:
    void DRV_AK4953_VolumeReMapping( DRV_AK4953_OBJ* drvObj, uint8_t volume)

  Summary:
    Volume remapping to reverse the codec volume value to dB mapping which currently works reverse

  Description:
    Volume remapping to reverse the codec volume value to dB mapping which currently works reverse

  Precondition:
    DRV_AK4953_OBJ - driver object should be available

  Parameters:
    object          - Driver object handle, returned from the
                      DRV_AK4953_Initialize routine
    volume          - 0 - 255 value supported by the codec for volume adjustment

  Returns:
    None.

  Remarks:

*/
static void DRV_AK4953_VolumeReMapping( DRV_AK4953_OBJ* drvObj, DRV_AK4953_CHANNEL channel, uint8_t volume);


// *****************************************************************************
 /*
  Function:
    static void _DRV_AK4953_MasterClockSet(uint32_t samplingRate, uint16_t mclk_multiplier)

  Summary:
    Generates the master clock(to AK4953) from REFCLOCK  for the given
    sampling rate.

  Description:
    Generates the master clock(to AK4953) from REFCLOCK  for the given
    sampling rate.

  Remarks:
    None
*/
static void _DRV_AK4953_MasterClockSet(uint32_t samplingRate, uint16_t mclk_multiplier);


// *****************************************************************************
 /*
  Function:
    static void _DRV_AK4953_ConrolRegisterSet (DRV_AK4953_OBJ *drvObj,
                uint8_t *controlData, uint32_t size )

  Summary:
    Send control commands bytes to AK4953

  Description:
    Send control commands bytes to AK4953

  Remarks:
    None
*/
static uintptr_t _DRV_AK4953_ConrolRegisterSet
(
    DRV_AK4953_OBJ *drvObj,
    uint8_t *controlData,
    uint32_t size
);


// *****************************************************************************
 /*
  Function:
    static void _DRV_AK4953_ConrolRegisterReSet (DRV_AK4953_OBJ *drvObj,
                 uint8_t *data, uint32_t size )

  Summary:
    Prepares the first every communication control command to be sent to AK4953. Also starts the
    timer to initiate the control command transfer.

  Description:
    Prepares the first every communication control command to be sent to AK4953.
    Codec AK4953 requires a dummy address 0x00 and dummy data byte 0x00 to be sent
    over I2C as the first signal to the codec to prepare for receiving more data over I2C.

  Remarks:
    None
*/

static uintptr_t _DRV_AK4953_ConrolRegisterReSet (DRV_AK4953_OBJ *drvObj,
                uint8_t *data, uint32_t size );


// *****************************************************************************
 /*
  Function:
    static void _DRV_AK4953_ControlTasks(DRV_AK4953_OBJ *drvObj)

  Summary:
    Implements the state maching for the Audio control interface of AK4953

  Description:
    Implements the state maching for the Audio control interface of AK4953

  Remarks:
    None
*/
static void _DRV_AK4953_ControlTasks(DRV_AK4953_OBJ *drvObj);






// *****************************************************************************
 /*
  Function:
    static void _DRV_AK4953_I2SBufferEventHandler(DRV_I2S_BUFFER_EVENT event,
        DRV_I2S_BUFFER_HANDLE bufferHandle, uintptr_t contextHandle)

  Summary:
    Implements the handler for i2s buffer request completion.

  Description:
    Implements the handler for i2s buffer request completion.

  Remarks:
    None
*/
static void _DRV_AK4953_I2SBufferEventHandler
(
    DRV_I2S_BUFFER_EVENT event,
    DRV_I2S_BUFFER_HANDLE bufferHandle,
    uintptr_t contextHandle
);

 /*
  Function:
        static void _DRV_AK4953_CommandQueueGetSlot
        (
        )

  Summary:
    Get a free slot from AK4953 command queue.

  Description:
    Get a free slot from AK4953 command queue.

  Remarks:
    None
*/
static AK4953_COMMAND* _DRV_AK4953_CommandQueueGetSlot();
 /*
  Function:
        static void _DRV_AK4953_CommandQueuePop
        (
        )

  Summary:
    Pop up the top slot of AK4953 command queue.

  Description:
    Pop up the top slot of AK4953 command queue.

  Remarks:
    None
*/
static AK4953_COMMAND* _DRV_AK4953_CommandQueuePop();
 /*
  Function:
        static void _DRV_AK4953_CommandQueueTop
        (
        )

  Summary:
    Return the top slot of AK4953 command queue.

  Description:
    Return the top slot of AK4953 command queue.

  Remarks:
    None
*/
static AK4953_COMMAND* _DRV_AK4953_CommandQueueTop();
 /*
  Function:
        static uint8_t _DRV_AK4953_CONTROL_REG_FIELD_WRITE_Wrapper
        (
            DRV_AK4953_OBJ *drvObj, 
            uint8_t reg_addr, 
            uint8_t mask, 
            uint8_t pos, 
            uint8_t val
        )

  Summary:
    A wrapper function of DRV_AK4953_CONTROL_REG_FIELD_WRITE macro.

  Description:
    A wrapper function of DRV_AK4953_CONTROL_REG_FIELD_WRITE macro.

  Remarks:
    None
*/
static uint8_t _DRV_AK4953_CONTROL_REG_FIELD_WRITE_Wrapper(DRV_AK4953_OBJ *drvObj, uint8_t reg_addr, uint8_t mask, uint8_t pos, uint8_t val);

 /*
  Function:
        static uint8_t _DRV_AK4953_CONTROL_REG_BIT_WRITE_Wrapper
        (
            DRV_AK4953_OBJ *drvObj, 
            uint8_t reg_addr, 
            uint8_t pos, 
            uint8_t val
        )

  Summary:
    A wrapper function of DRV_AK4953_CONTROL_REG_BIT_WRITE macro.

  Description:
    A wrapper function of DRV_AK4953_CONTROL_REG_BIT_WRITE macro.

  Remarks:
    None
*/
static uint8_t _DRV_AK4953_CONTROL_REG_BIT_WRITE_Wrapper(DRV_AK4953_OBJ *drvObj, uint8_t reg_addr, uint8_t pos, uint8_t val);
#endif // #ifndef _DRV_AK4953_LOCAL
/*******************************************************************************
 End of File
*/