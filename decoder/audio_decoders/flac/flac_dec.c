/*
 * Copyright (C) 2002-2009  Josh Coalson
 * Copyright (C) 2011-2014  Xiph.Org Foundation
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/*******************************************************************************
 FLAC Decoder Library Interface File

  Company:
    Microchip Technology Inc.

  File Name:
    flac_dec.c

  Summary:
	This header file consists of flac library interface function implementations.
    
  Description:
    FLAC decoder interface function implementations, it is not library source 
    code of flac, it is interface functions for easily use in Harmony audio framework.

*******************************************************************************/
// DOM-IGNORE-BEGIN
/*******************************************************************************
  Modifications copyright (c) 2018 Microchip Technology Inc
 *
 *  StreamDecoderClientData structure and stream decoder call back functions
 *  are modified from file decoders.c in test_libFLAC project, Xiph.Org Foundation
 * 
*******************************************************************************/
// DOM-IGNORE-END

#include "decoder/audio_decoders/flac/include/FLAC/stream_decoder.h"
#include "flac_dec.h"

/* Modification:
 * This structure declaration is a modification of 
 * structure StreamDecoderClientData from decoders.c file 
 * in test_libFLAC project.
 */
typedef struct {
	SYS_FS_HANDLE file;
	FLAC__bool ignore_errors;
	FLAC__bool error_occurred;
    FLAC__bool eof;
    uint16_t *decodebuf;
    uint16_t *decodesize;
    FLAC__StreamDecoder *decoder;
    FLAC__uint64 total_samples;
    FLAC__uint32 sample_rate;
    FLAC__uint32 channels;
    FLAC__uint32 bps;
    FLAC__uint32 min_blocksize;
    FLAC__uint32 max_blocksize;
    FLAC__uint32 min_framesize;
    FLAC__uint32 max_framesize;
    FLAC__uint64 flacfilesize_;
    FLAC__uint32 bitrate;
} StreamDecoderClientData;

static StreamDecoderClientData FlacData;


#ifdef OFFLINE_DEBUG 
static SYS_FS_HANDLE decodedFileHandle;
static uint64_t samplecount=0;
static bool writedone = false;
#endif

static void arrange(uint32_t *arr, int s, int e);
/* Modification:
 * 
 * This function is a modification of function stream_decoder_tell_callback_
 * from decoders.c file in test_libFLAC project.
 */
static FLAC__StreamDecoderTellStatus stream_decoder_tell_callback_(const FLAC__StreamDecoder *decoder, FLAC__uint64 *absolute_byte_offset, void *client_data)
{
	StreamDecoderClientData *dcd = (StreamDecoderClientData*)client_data;

	if(0 == dcd) {
		return FLAC__STREAM_DECODER_TELL_STATUS_ERROR;
	}

	if(dcd->error_occurred)
		return FLAC__STREAM_DECODER_TELL_STATUS_ERROR;

	*absolute_byte_offset = SYS_FS_FileTell(dcd->file);

	if(*absolute_byte_offset < 0) {
		dcd->error_occurred = true;
		return FLAC__STREAM_DECODER_TELL_STATUS_ERROR;
	}

	return FLAC__STREAM_DECODER_TELL_STATUS_OK;
}

/* Modification:
 * 
 * This function is a modification of function stream_decoder_length_callback_
 * from decoders.c file in test_libFLAC project.
 */
static FLAC__StreamDecoderLengthStatus stream_decoder_length_callback_(const FLAC__StreamDecoder *decoder, FLAC__uint64 *stream_length, void *client_data)
{
	StreamDecoderClientData *dcd = (StreamDecoderClientData*)client_data;

	if(0 == dcd) {
		return FLAC__STREAM_DECODER_LENGTH_STATUS_ERROR;
	}

	if(dcd->error_occurred)
		return FLAC__STREAM_DECODER_LENGTH_STATUS_ERROR;

	*stream_length = (FLAC__uint64)dcd->flacfilesize_;
	return FLAC__STREAM_DECODER_LENGTH_STATUS_OK;
}

/* Modification:
 * 
 * This function is a modification of function stream_decoder_eof_callback_
 * from decoders.c file in test_libFLAC project.
 */
static FLAC__bool stream_decoder_eof_callback_(const FLAC__StreamDecoder *decoder, void *client_data)
{
	StreamDecoderClientData *dcd = (StreamDecoderClientData*)client_data;
    bool eof;
    
	if(0 == dcd) {
		return true;
	}

	if(dcd->error_occurred)
		return true;

	dcd->eof = eof = SYS_FS_FileEOF(dcd->file);
    return eof;
}

/* Modification:
 * 
 * This function is a modification of function stream_decoder_error_callback_
 * from decoders.c file in test_libFLAC project.
 */
static void stream_decoder_error_callback_(const FLAC__StreamDecoder *decoder, FLAC__StreamDecoderErrorStatus status, void *client_data)
{
	StreamDecoderClientData *dcd = (StreamDecoderClientData*)client_data;

	if(0 == dcd) {
		return;
	}

	if(!dcd->ignore_errors) {
		dcd->error_occurred = true;
	}
}

/* Modification:
 * 
 * This function is a modification of function stream_decoder_seek_callback_
 * from decoders.c file in test_libFLAC project.
 */
static FLAC__StreamDecoderSeekStatus stream_decoder_seek_callback_(const FLAC__StreamDecoder *decoder, FLAC__uint64 absolute_byte_offset, void *client_data)
{
	StreamDecoderClientData *dcd = (StreamDecoderClientData*)client_data;

	if(0 == dcd) 
    {
		return FLAC__STREAM_DECODER_SEEK_STATUS_ERROR;
	}

	if(dcd->error_occurred)
	{
        return FLAC__STREAM_DECODER_SEEK_STATUS_ERROR;
    }

	if(SYS_FS_FileSeek(dcd->file, absolute_byte_offset, SEEK_SET) < 0) {
		dcd->error_occurred = true;
		return FLAC__STREAM_DECODER_SEEK_STATUS_ERROR;
	}

	return FLAC__STREAM_DECODER_SEEK_STATUS_OK;
}

/* Modification:
 * 
 * This function is a modification of function stream_decoder_read_callback_
 * from decoders.c file in test_libFLAC project.
 */
static FLAC__StreamDecoderReadStatus stream_decoder_read_callback_(const FLAC__StreamDecoder *decoder, FLAC__byte buffer[], size_t *bytes, void *client_data)
{
	StreamDecoderClientData *dcd = (StreamDecoderClientData*)client_data;
	const size_t requested_bytes = *bytes;

	if(0 == dcd) {
		return FLAC__STREAM_DECODER_READ_STATUS_ABORT;
	}

	if(dcd->error_occurred)
		return FLAC__STREAM_DECODER_READ_STATUS_ABORT;

	if(SYS_FS_FileEOF(dcd->file)) {
		*bytes = 0;
		return FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM;
	}
	else if(requested_bytes > 0) {
		*bytes = SYS_FS_FileRead(dcd->file, buffer, requested_bytes);
		if(*bytes == 0) {
			if(SYS_FS_FileEOF(dcd->file))
				return FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM;
			else
				return FLAC__STREAM_DECODER_READ_STATUS_ABORT;
		}
		else {
			return FLAC__STREAM_DECODER_READ_STATUS_CONTINUE;
		}
	}
	else
		return FLAC__STREAM_DECODER_READ_STATUS_ABORT; /* abort to avoid a deadlock */
}

/* Modification:
 * 
 * This function is a modification of function stream_decoder_metadata_callback_
 * from decoders.c file in test_libFLAC project.
 */
static void stream_decoder_metadata_callback_(const FLAC__StreamDecoder *decoder, const FLAC__StreamMetadata *metadata, void *client_data)
{
    StreamDecoderClientData *dcd = (StreamDecoderClientData*)client_data;
	if(metadata->type == FLAC__METADATA_TYPE_STREAMINFO) {
		dcd->total_samples = metadata->data.stream_info.total_samples;
		dcd->sample_rate = metadata->data.stream_info.sample_rate;
		dcd->channels = metadata->data.stream_info.channels;
		dcd->bps = metadata->data.stream_info.bits_per_sample;
        dcd->min_blocksize=metadata->data.stream_info.min_blocksize;
        dcd->max_blocksize = metadata->data.stream_info.max_blocksize;
        dcd->min_framesize = metadata->data.stream_info.min_framesize;
        dcd->max_framesize = metadata->data.stream_info.max_framesize;
	}
}

/* Modification:
 * 
 * This function is a modification of function stream_decoder_write_callback_
 * from decoders.c file in test_libFLAC project.
 */
static FLAC__StreamDecoderWriteStatus stream_decoder_write_callback_(const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 * const buffer[], void *client_data)
{
    StreamDecoderClientData *dcd = (StreamDecoderClientData*)client_data;
	uint32_t i = 0, j = 0;

    
	if(dcd->total_samples == 0) {
		return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
	}
    
	if(buffer[0] == NULL) {
		return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
	}
	if(buffer[1] == NULL) {
		return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
	}

	/* write decoded PCM samples */
    if(dcd->bps == 16)
    {
        for(i = 0,j = 0; i < frame->header.blocksize; i++,j+=2) 
        {
            dcd->decodebuf[j] =  (buffer[0][i]&(0x0000FFFF)); // left channel
            dcd->decodebuf[j+1] = (buffer[1][i]&(0x0000FFFF)); //right channel
        }
        *(dcd->decodesize) = frame->header.blocksize*4;
    }
    else if(dcd->bps == 24)
    {
        uint32_t *buf_ptr = (uint32_t*)dcd->decodebuf;
        
        // re-arrange from L1,L2,...,Ln,R1,R2,...,RN
        // to format L1,R1,L2,R2,...,Ln,Rn
        arrange(buf_ptr,0,(frame->header.blocksize+4)*2-1);
        
        *(dcd->decodesize) = frame->header.blocksize*8;
    }
#ifdef OFFLINE_DEBUG
    if(!writedone)
    {
        if (SYS_FS_FileWrite( decodedFileHandle, (const void *) dcd->decodebuf, *(dcd->decodesize)) == -1)
        {
        /* Write was not successful. Close the file
         * and error out.*/



        }
        samplecount += frame->header.blocksize;
        if(samplecount == dcd->total_samples)
        {
            // only write first block
            SYS_FS_FileClose(decodedFileHandle);
            writedone = true;
        } 
    }
    
#endif
	return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
}

bool FLAC_Initialize (SYS_FS_HANDLE fhandle, void *input_buffer)
{
	FLAC__StreamDecoderInitStatus init_status;
    FLAC__bool ok = true;
    
    FlacData.file = fhandle;
	FlacData.ignore_errors = false;
	FlacData.error_occurred = false;
    FlacData.eof = false;
    
    
#if defined(OFFLINE_DEBUG)
    decodedFileHandle = SYS_FS_FileOpen("/mnt/myDrive1/decoded.txt", (SYS_FS_FILE_OPEN_APPEND_PLUS));
    if(decodedFileHandle == SYS_FS_HANDLE_INVALID)
    {
        /* Could not open the file. Error out*/
        //Nop();

    }
#endif
    
    if((FlacData.decoder = FLAC__stream_decoder_new()) == NULL) {
		return false;
	}
    
    (void)FLAC__stream_decoder_set_md5_checking(FlacData.decoder, false);
    
    init_status = FLAC__stream_decoder_init_stream(FlacData.decoder, 
                                                    stream_decoder_read_callback_,
                                                    stream_decoder_seek_callback_,
                                                    stream_decoder_tell_callback_, 
                                                    stream_decoder_length_callback_, 
                                                    stream_decoder_eof_callback_, 
                                                    stream_decoder_write_callback_, 
                                                    stream_decoder_metadata_callback_, 
                                                    stream_decoder_error_callback_, 
                                                    &FlacData,
                                                    input_buffer
                                                    );
    // give the input buffer pointer to flac library
    
	if(init_status != FLAC__STREAM_DECODER_INIT_STATUS_OK) {
		return false;
	}
    
   
    if(!FLAC__stream_decoder_process_until_end_of_metadata(FlacData.decoder))
		return false;
    
    //bitrate = (sample rate) * (bit depth) * (number of channels) = kbits per second
    FlacData.bitrate = (FlacData.sample_rate * FlacData.bps * FlacData.channels);
    
    return ok;
}

bool FLAC_Decoder( uint8_t *input, uint16_t inSize, uint16_t *read, uint8_t *output, uint16_t *written )
{
    if(FlacData.decoder != NULL)
    {
        
        FlacData.decodebuf = (uint16_t *)output;
        FlacData.decodesize = written;
        
        bool ret = FLAC__stream_decoder_process_single(FlacData.decoder, output);
        
        return ret;
    }
    return false;
}

bool isFLACdecoder_enabled()
{
    return true;
}

uint8_t FLAC_GetChannels(){
    return 1; //FlacData.channels;
}

void FLAC_RegisterDecoderEventHandlerCallback(DecoderEventHandlerCB fptr)
{
    return;
}

int32_t FLAC_GetBitRate()
{
    return FlacData.bitrate;
}
int32_t FLAC_GetSamplingRate() 
{
    return FlacData.sample_rate;
}

uint8_t FLAC_GetBitdepth()
{
    return FlacData.bps;
}

int32_t FLAC_GetBlockSize()
{
    FLAC__StreamDecoderState state;
    if(FlacData.decoder != NULL)
    {
        state = FLAC__stream_decoder_get_state(FlacData.decoder);
        if((state == FLAC__STREAM_DECODER_END_OF_STREAM) || (state == FLAC__STREAM_DECODER_ABORTED) )
        {
            FLAC__stream_decoder_delete(FlacData.decoder);
            return -1;
        }
        else
        {
            return FlacData.max_blocksize;
        }            
    }
    return -1; 
}

uint32_t FLAC_GetDuration(void){
    if(FlacData.sample_rate != 0)
    {
        return FlacData.total_samples / FlacData.sample_rate;
    }
    else
    {
        return 0;
    }
}

void FLAC_Cleanup() 
{
    if(FlacData.decoder != NULL)
        FLAC__stream_decoder_delete(FlacData.decoder);
}

/**
 *  in array re-arrange, 
 *  re-arrange from L1,L2,...,Ln,R1,R2,...,RN
 *  to format L1,R1,L2,R2,...,Ln,Rn
 */
static void arrange(uint32_t *arr, int s, int e)
{
    // end condition
    if(e <= s || e-s<=2)
    {
        return;
    }
    int n = e - s + 1;

    int mid = (n) / 2;
    bool odd = (mid & 1) == 1 ? true : false;
    int mmid;
    if (odd)
    {
        mmid = mid / 2+1;
    }else
    {
        mmid = mid / 2;
    }


    int i,j;
    for (i =s+mid,j=0; i <s+mid+mmid; i++,j++)
    {
        uint32_t tmp = arr[i];
        arr[i] = arr[s+mid-mmid+j];
        arr[s + mid - mmid + j] = tmp;
    }
    if (odd) //odd
    {
        uint32_t tmp = arr[s+mid-1];
        arr[s+mid-1] = arr[s+mid];
        arr[s+mid] = tmp;

        arrange(arr, s, s+mid-2);
        arrange(arr, s+mid+1, e);
    }
    else
    {
        arrange(arr, s, s + mid - 1);
        arrange(arr, s + mid, e);
    }
}