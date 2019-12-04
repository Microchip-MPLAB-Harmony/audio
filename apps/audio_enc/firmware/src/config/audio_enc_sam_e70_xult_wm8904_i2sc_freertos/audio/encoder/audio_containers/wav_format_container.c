/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    wav_format_container.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#ifndef _WAV_FORMAT_CONTAINER_C
#define _WAV_FORMAT_CONTAINER_C
/* This section lists the other files that are included in this file.
 */
#include "include/wav_format_container.h"
/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */
/** Descriptive Data Item Name

  @Summary
    Brief one-line summary of the data item.
    
  @Description
    Full description, explaining the purpose and usage of data item.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
    
  @Remarks
    Any additional remarks
 */


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */

/** 
  @Function
    int ExampleLocalFunctionName ( int param1, int param2 ) 

  @Summary
    Brief one-line description of the function.

  @Description
    Full description, explaining the purpose and usage of the function.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.

  @Precondition
    List and describe any required preconditions. If there are no preconditions,
    enter "None."

  @Parameters
    @param param1 Describe the first parameter to the function.
    
    @param param2 Describe the second parameter to the function.

  @Returns
    List (if feasible) and describe the return values of the function.
    <ul>
      <li>1   Indicates an error occurred
      <li>0   Indicates an error did not occur
    </ul>

  @Remarks
    Describe any special behavior not described above.
    <p>
    Any additional remarks.

  @Example
    @code
    if(ExampleFunctionName(1, 2) == 0)
    {
        return 3;
    }
 */



/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

int32_t wav_riff_fill_header(void *pheader, 
        EncoderType encoderType, 
        StreamInfo *si, 
        int encodedAudioSize)
{
    WAVHEADER *wavheader = (WAVHEADER*) pheader;
    /**configured data part**/
    wavheader->samplesPerSec = si->sample_rate;
    // in fact it's bits per channel
    wavheader->bitsPerSample = si->bit_depth;
    wavheader->dataLen = si->bit_depth;
    wavheader->bytesPerSec = si->bps / 8;
    wavheader->datasize = encodedAudioSize;
    wavheader->filesize = WAV_HEADER_SIZE/*wav header size*/ + wavheader->datasize;
    wavheader->numOfChan = si->channel;
    wavheader->blockAlign = (wavheader->numOfChan)*(wavheader->dataLen) / 8;
    
    switch(encoderType)
    {
        case PCM:
            wavheader->type_frmt = 1; 
            break;
        case ADPCM:
            wavheader->type_frmt = 0x0040;
            break;
        default:
            wavheader->type_frmt = 1;
            break;
    }
    

    // can be extended to any other encoders 
        
    /**fixed data part**/
    wavheader->format = 0x46464952; //"RIFF"
    wavheader->filetype = 0x45564157; // "WAVE"
    wavheader->frmtchunk_marker = 0x20746d66; // "fmt "
    wavheader->Subchunk2ID = 0x61746164; //"data"
    return WAV_HEADER_SIZE;
}
/* *****************************************************************************
 End of File
 */
#endif