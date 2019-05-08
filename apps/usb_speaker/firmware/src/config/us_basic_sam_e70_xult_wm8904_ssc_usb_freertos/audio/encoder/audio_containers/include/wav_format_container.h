/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    wav_format_container.h

  @Summary
    The "standard" Microsoft .WAV file format.

  @Description
     a subset of the general Microsoft RIFF format.
 */
/* ************************************************************************** */

#ifndef _WAV_FORMAT_CONTAINER_H    /* Guard against multiple inclusion */
#define _WAV_FORMAT_CONTAINER_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */

// audio container needs information from encoder to encapsulate an audio file


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include "encoder.h"
    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Section: Constants                                                         */
    /* ************************************************************************** */
    /* ************************************************************************** */

    /*  A brief description of a section can be given directly below the section
        banner.
     */


    /* ************************************************************************** */
    /** Descriptive Constant Name

      @Summary
        Brief one-line summary of the constant.
    
      @Description
        Full description, explaining the purpose and usage of the constant.
        <p>
        Additional description in consecutive paragraphs separated by HTML 
        paragraph breaks, as necessary.
        <p>
        Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
    
      @Remarks
        Any additional remarks
     */

    // WAV container header size
#define WAV_HEADER_SIZE 44


    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************

    /*  A brief description of a section can be given directly below the section
        banner.
     */


    // *****************************************************************************

    /** Descriptive Data Type Name

      @Summary
        Brief one-line summary of the data type.
    
      @Description
        Full description, explaining the purpose and usage of the data type.
        <p>
        Additional description in consecutive paragraphs separated by HTML 
        paragraph breaks, as necessary.
        <p>
        Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.

      @Remarks
        Any additional remarks
        <p>
        Describe enumeration elements and structure and union members above each 
        element or member.
     */
    /* Not the best place to put this struct */
    typedef struct {
        int format;     // 4 bytes "RIFF" in ASCII format
        int filesize;   // 4 bytes, Size of the overall file.
        int filetype;   // 4 bytes "WAVE
        int frmtchunk_marker; // Format chunk marker. Includes trailing null, "fmt "
        int dataLen;    // Length of format data as listed above, for example, 16
        short type_frmt; // Type of format (1 is PCM) - 2 byte integer
        short numOfChan; // stereo or mono
        int samplesPerSec; // sample rate
        int bytesPerSec; // byte rate, == SampleRate * NumChannels * BitsPerSample/8
        short blockAlign; // == NumChannels * BitsPerSample/8
                          // The number of bytes for one sample including
                          // all channels.
        short bitsPerSample; 
        int Subchunk2ID; // "data"
        unsigned int datasize; //Size of the data section. 
    } WAVHEADER;

//    int32_t wav_riff_fill_header(void *pheader, uint32_t size);
    int32_t wav_riff_fill_header(void *pheader, 
        EncoderType encoderType, 
        StreamInfo *si, 
        int encodedAudioSize);


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _WAV_FORMAT_CONTAINER_H */

/* *****************************************************************************
 End of File
 */
