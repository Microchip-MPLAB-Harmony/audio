/*******************************************************************************
  Opus Decoder Interface File

  Company:
    Microchip Technology Inc.

  File Name:
    opus_dec.c

  Summary:
   Contains the functional implementation of opus decoder interface.

  Description:
   This file contains the functional implementation of opus decoder interface.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2017 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/
// DOM-IGNORE-END
#include"../include/opus_defines.h"
#include "opus_dec.h"


/********************************* Local variables*****************************/

// OPUS file always play at 48Khz, see OPUS spec
static const uint16_t   OPUS_FS = 48000;
static SYS_FS_HANDLE    currentOpusFileHandle;
static sOpusStreamDcpt  pOpusDcpt[1]; 
static sOpusHeader      pOpusHdr[1];
static OpusDecoder      *pOpusDecoder;  
static opusDecDcpt      pDDcpt[1];

/**********************STATIC FUNCTIONS DECLRATIONS****************************/
static OPUS_ERROR_MSG OggReadPageHdr(sOggPageHdr *pOggHdr, 
                                        sOpusStreamDcpt *pOpusDcpt);
static int32_t        OggPageDataSize(sOggPageHdr* pOggPage);
static OPUS_ERROR_MSG OggSkipPage(sOggPageHdr* pOggPage);
static int            OggPageGetPktSize(sOpusStreamDcpt* pOpusDcpt, int* pPktComplete);
static OPUS_ERROR_MSG OggGetPacketData(sOpusStreamDcpt* pOpusDcpt, void* pBuff, 
                                        int nBytes, void* pResult);


/**********************PROFILING DECODE FUNCTION*******************************/
#undef PROFILING

#ifdef PROFILING
static uint32_t cycles_count = 0;
static uint32_t start, end;
static uint32_t worst=0, best=0xffffffff;
static total_packets = 0;
#endif


/********************OPUS DECODER FUNCTIONS IMPLEMENTATION*********************/
   
OPUS_ERROR_MSG OPUS_Initialize(const SYS_FS_HANDLE opus_file_handle){
    OPUS_ERROR_MSG ret = OPUS_SUCCESS; 
    uint32_t readRet;
    bool commentFound;
    sOggPageHdr *pOggPageHdr = &pOpusDcpt->pageHdr;
    int err;
    
    currentOpusFileHandle = opus_file_handle;
    while(ret == OPUS_SUCCESS)
    {
        if((ret=OggReadPageHdr(pOggPageHdr, 0)) != OPUS_SUCCESS)
		{
			break;
		}
        
        if(pOggPageHdr->headerFlags!=OGG_FLAG_BOS || pOggPageHdr->granulePos!=0 || pOggPageHdr->pageNo!=0 ||
                    pOggPageHdr->pageSegments!=1)
        {
            // looking into a wrong Ogg stream
            // try to skip this page in case we have a multiple Ogg container...
            ret=OggSkipPage(pOggPageHdr);
            continue;	// skipped current page...
        }
        
        // read OpusHead content
        readRet = SYS_FS_FileRead(currentOpusFileHandle, (uint8_t*)pOpusHdr, pOggPageHdr->segmentTbl[0]);
        
        if(readRet != pOggPageHdr->segmentTbl[0])
        {
            ret=OPUS_READ_ERROR;	// could not read the Stream
            break;
        }

        // verify magic signature field
        if(strncmp(pOpusHdr[0].signature, "OpusHead", 8))
        {
            continue;	// skip current page in case we have a multiple Ogg container...
        }
        
        // finally, we got a valid Opus header.
        // try to get to the data page
        // for now, we ignore the opus tags page

        commentFound=0;
        while(ret==OPUS_SUCCESS)
        {
            if((ret = OggReadPageHdr(pOggPageHdr,pOpusDcpt))!=OPUS_SUCCESS)
            {
                break;		// some error
            }

            // valid new page from the same stream
            if(!commentFound)
            {
                // this should be the opus comment block
                // THIS DOES NOT handle the case that comment header spans two or more pages.
                if(pOggPageHdr->granulePos==0 && pOggPageHdr->pageNo==1 && pOggPageHdr->segmentTbl[pOggPageHdr->pageSegments-1] < 255)
                {	// seems to be the valid comment
                    
                    // verify comment header magic signature
                    // strcmp() "OpusTags", skip this for efficiency 
                    commentFound=1;
                }

                ret=OggSkipPage(pOggPageHdr);
                continue;	// just skip it anyway
            }
            else if(pOggPageHdr->pageNo==2 && pOggPageHdr->granulePos)
            { // ok, we got the 1st data page
                break;
            }

            ret = OggSkipPage(pOggPageHdr);	// try to find another start of data page?
                                    // will fail, most likely
        }
        
        if(ret!=OPUS_SUCCESS)
        {
            break;	// some error occurred
        }
        
        
        // THIS ONLY HANDLES Channel mapping family 0
        if(pOpusHdr->channel_mapping == 0)
        {
            pOpusHdr->nb_streams = 1;
            pOpusHdr->nb_coupled = pOpusHdr->channels-1;
            pOpusHdr->stream_map[0] = 0;
            pOpusHdr->stream_map[1] = 1;
            
            pDDcpt->currPktNo = 0;
            pDDcpt->processedPktNo = 0;
            
            pOpusDcpt->pktIx = 0;
            pOpusDcpt->prevBytes = 0;
            pOpusDcpt->segIx = 0;
            
            
            pOpusDecoder = opus_decoder_create(48000, pOpusHdr->channels, &err);
            
            if(err != OPUS_OK){
//                fprintf(stderr, "Cannot create decoder: %s\n", opus_strerror(err));
                ret = OPUS_GENERAL_ERROR;
                break;
            }
            if (!pOpusDecoder)
            {
                ret = OPUS_GENERAL_ERROR;
//               fprintf (stderr, "Decoder initialization failed: %s\n", opus_strerror(err));
                break;
            }
        }else{
            
            ret = OPUS_GENERAL_ERROR;
            break;
        }
        break;
    } 
    return ret;
}

OPUS_ERROR_MSG OPUS_Decoder(const uint8_t *input, uint16_t inSize, uint16_t *read, 
                              int16_t *output, uint16_t *written, uint16_t outSize){

    OPUS_ERROR_MSG res = OPUS_SUCCESS;
    
    // opus_decode return value,
    // it is number of decoded pcm samples per channel,
    // thus, if this opus stream is stereo, the size of 
    // returned pcm value is out_samples*4. (out_samples*bitdepth*channel)
    int output_samples = 0;
#ifdef PROFILING
    total_packets++;
    asm volatile("mtc0   $0,$9");
    asm volatile("mfc0   %0, $9" : "=r"(start));
#endif
    
    if(pDDcpt->currPktNo==pDDcpt->processedPktNo+1)
    {   // in sync
        // regular decode
        // CAUTION: this decode function may return decoded data larger than output buffer size,
        // you'd better:
        //          1. has a output buffer size larger than or equal to OPUS_MAX_FRAME_SIZE*4,
        //             if the channel number is 2, or OPUS_MAX_FRAME_SIZE*2 if the channel number is 1.
        //          2. make sure each packet in opus file does not contain more than OPUS_MAX_FRAME_SIZE samples
        
        output_samples = opus_decode(pOpusDecoder, input, pDDcpt->nInBytes, output, OPUS_MAX_FRAME_SIZE, 0);
        
    }
    else
    { // lost frames, let decoder guess
        if(pOpusHdr->channels == 1)
            output_samples = opus_decode(pOpusDecoder, NULL, 0, output, outSize/2, 0);
        else if(pOpusHdr->channels == 2){
            output_samples = opus_decode(pOpusDecoder, NULL, 0, output, outSize/4, 0);
        }
    }

    if(output_samples <= 0)
    {
        res=OPUS_STREAM_ERROR;
        *written = 0;
        return res;
    }

    // Convert to stereo
    if(pOpusHdr->channels == 1)
    {
        // preskip
        if(pDDcpt->currPktNo == 1 && pOpusHdr->preskip != 0){
            
            output_samples = (pOpusHdr->preskip>output_samples) ? output_samples : output_samples - pOpusHdr->preskip;
            memcpy(output, output+pOpusHdr->preskip, 2*output_samples);
        }
        *written += 2*output_samples; // multiply by 2, mono mode

    }else if(pOpusHdr->channels == 2){   
        if(pDDcpt->currPktNo == 1 && pOpusHdr->preskip != 0){
            output_samples = (pOpusHdr->preskip>output_samples) ? output_samples : output_samples - pOpusHdr->preskip;
            memcpy(output, output+2*pOpusHdr->preskip, 4*output_samples);
        }
        *written += 4*output_samples; // multiply by 4, stereo mode
    } 
    *read = inSize;
    
#ifdef PROFILING
    asm volatile("mfc0   %0, $9" : "=r"(end));
    uint32_t tmp = end-start;
    if(tmp > worst){
        worst = tmp;
    }
    if(tmp < best){
        best = tmp;
    }
    
    cycles_count += (end - start);
#endif

    if(res==OPUS_SUCCESS)
    {
        pDDcpt->processedPktNo++;	// packet processed
        return res;
    }
    return res;
}


int32_t        OPUS_DiskRead(uint8_t *inBuff){
    OPUS_ERROR_MSG res;
    sOpusPktDcpt pktDcpt;
    int32_t readBytes = 0;

//    pDDcpt->cdc.nOutBytes=0;
    while(pDDcpt->processedPktNo==pDDcpt->currPktNo){
        readBytes = SYS_FS_FileTell(currentOpusFileHandle);
        pktDcpt.pktSeqNo = pDDcpt->currPktNo;
        res = OggGetPacketData(pOpusDcpt, inBuff, OPUS_INPUT_BUFFER_SIZE, &pktDcpt);
        readBytes = SYS_FS_FileTell(currentOpusFileHandle) - readBytes;
         // zero byte packet, read next packet
        if(readBytes != 0){
            
            pDDcpt->currPktNo=pktDcpt.pktSeqNo;
            pDDcpt->nInBytes = pktDcpt.pktBytes;
            return readBytes;
        }
        if(res!=OPUS_SUCCESS)
        {
            return -1;
        }
        
        pDDcpt->processedPktNo = pDDcpt->currPktNo;
//        return readBytes;
    }
    return -1;
}


/*********************************************************************/
int32_t OPUS_GetSamplingRate(){return OPUS_FS;}
bool    isOPUSdecoder_enabled(){return true;}
uint8_t OPUS_GetChannels(){return pOpusHdr->channels;}
void    OPUS_Cleanup()
{
    if(pOpusDecoder != NULL)
    {
        opus_decoder_destroy(pOpusDecoder); 
    }
    pOpusDecoder = NULL;
}

/*****************************STATIC FUNCTIONS*********************************/

static OPUS_ERROR_MSG OggReadPageHdr(sOggPageHdr *pOggHdr, sOpusStreamDcpt *pOpusDcpt){
    int		pageBytes=0;
	bool	useStreamId;
	int		streamId=0; 
    OPUS_ERROR_MSG res = OPUS_SUCCESS;
    uint32_t readRet;
	
    if((useStreamId=(pOpusDcpt!=0)))
	{
		streamId=pOpusDcpt->pageHdr.streamNo;
		if((pOpusDcpt->pageHdr.headerFlags&OGG_FLAG_EOS)!=0)
		{
			res=OPUS_STREAM_END;	// end of stream
		}
	}
    
    while(res == OPUS_SUCCESS)
	{
		pageBytes=sizeof(*pOggHdr)-sizeof(pOggHdr->segmentTbl);	// page with no segment table
        readRet = SYS_FS_FileRead(currentOpusFileHandle, (uint8_t*)pOggHdr, pageBytes);
		if(readRet != pageBytes)
		{
			res=OPUS_READ_ERROR;
			break;
		}
		else if(pOggHdr->pageCapture!=OGG_ID_MAGIC)
		{
			res=OPUS_STREAM_ERROR;	// not an Ogg Stream
			break;
		}
		else if((pageBytes=pOggHdr->pageSegments))
		{
            
            readRet = SYS_FS_FileRead(currentOpusFileHandle, (uint8_t*)pOggHdr->segmentTbl, pageBytes);
			if(readRet == pageBytes)
			{
				if(!useStreamId || pOggHdr->streamNo==streamId)
				{	// found it
					break;
				}
				else if(!(pOggHdr->headerFlags&OGG_FLAG_EOS))
				{
					res=OggSkipPage(pOggHdr);
					continue;
				}
				else
				{
					res=OPUS_STREAM_END;	// wrong stream, end of stream
					break;
				}
			}
			else
			{
				res=OPUS_READ_ERROR;
				break;
			}
		}
		else if(pOggHdr->headerFlags&OGG_FLAG_EOS)
		{
			res=OPUS_STREAM_END;
		}
		else
		{	
			res=OPUS_STREAM_ERROR;	// empty page ?
			break;
		}
	}

	return res;
}



static int32_t OggPageDataSize(sOggPageHdr* pOggPage){
    int32_t	ix;
	int32_t	dataBytes;
    
	for(ix=0, dataBytes=0; ix<pOggPage->pageSegments; ix++)
	{
		dataBytes+=pOggPage->segmentTbl[ix];
	}
	return dataBytes;
}

static OPUS_ERROR_MSG OggSkipPage(sOggPageHdr* pOggPage){
    OPUS_ERROR_MSG ret;

    int32_t	dataBytes=OggPageDataSize(pOggPage);

    int status;
    status = SYS_FS_FileSeek(currentOpusFileHandle, dataBytes, SYS_FS_SEEK_CUR);
    if( status == dataBytes )
    {
        ret=OPUS_SUCCESS;
    }else{
        ret=OPUS_READ_ERROR;
    }
    return ret;
}
// returns the number of bytes in the current packet, completed or not
// also returns true if the packet pointed by the current segIx is completed on this page
// updates the current segIx
static int OggPageGetPktSize(sOpusStreamDcpt* pOpusDcpt, int* pPktComplete){
    int segIx;
	int nBytes;
	int totBytes=0;

	*pPktComplete=0;	
	for(segIx=pOpusDcpt->segIx; segIx<pOpusDcpt->pageHdr.pageSegments; segIx++)
	{
		totBytes+=(nBytes=pOpusDcpt->pageHdr.segmentTbl[segIx]);
		if(nBytes<255)
		{
			*pPktComplete=1;
			segIx++;
			break;
		}
	}
	
	pOpusDcpt->segIx=segIx;
	return totBytes;
}

// This function read packet by packet

// read next data packet
// the read will extend across page boundary if the packet doesn't end on the current page
// in case of page lost, the correct packet number should be returned

// param: pBuff: input buffer pointer
static OPUS_ERROR_MSG OggGetPacketData(sOpusStreamDcpt* pOpusDcpt, void* pBuff, int nBytes, void* pResult)
{
	int		rdBytes;
	int		pktBytes;
	int		pktComplete;
	int		prevPgNo;
	int		loopCount;
	
	OPUS_ERROR_MSG	res = OPUS_SUCCESS;
	sOpusPktDcpt*	pPktDcpt=(sOpusPktDcpt*)pResult;


	pPktDcpt->pktBytes=0;
	loopCount=0;
	
	while(1)
	{
		pktBytes=OggPageGetPktSize(pOpusDcpt, &pktComplete);
		if(pktBytes==0 && pktComplete)
		{ // if loopCount==0, we have a 0 len pkt mid page
			res=loopCount?OPUS_SUCCESS:OPUS_STREAM_ERROR;
			break;
		}
		
		// either pktBytes or pktComplete==0
		if(pktBytes)
		{
			if(nBytes<pktBytes)
			{
				res=OPUS_BUFF_ERROR;	// should accommodate at least a packet
				break;
			}
            rdBytes = SYS_FS_FileRead(currentOpusFileHandle, (uint8_t*)pBuff, pktBytes);
			
			if(rdBytes != pktBytes)
			{
				res=OPUS_READ_ERROR;
				break;
			}
		}
		
		if(pktComplete)
		{
			pPktDcpt->pktBytes+=pktBytes;
			pPktDcpt->pktSeqNo=++pOpusDcpt->pktIx;	// was in sync
            
			// Found all data in this packet,
            // break this loop
            break;
		}
		
        
		// read in a new page
		pBuff=(char*)pBuff+pktBytes;
		nBytes-=pktBytes;
			
		prevPgNo=pOpusDcpt->pageHdr.pageNo;
		res=OggReadPageHdr(&pOpusDcpt->pageHdr, pOpusDcpt);
		if(res!=OPUS_SUCCESS)
		{
            res=OPUS_STREAM_END;	// eos	
			break;		// could not end in the middle of the packet!
		}
		pOpusDcpt->segIx=0;	// brand new page
        
		if(pOpusDcpt->pageHdr.pageNo!=prevPgNo+1)
		{ // we're out of sync
			// TODO: update pSpxDcpt->pktIx with lost packets: based on frameSamples, framesPerPkt and granulePos
			// for now, no op. We maintain sequential packet numbers!
		}
		loopCount++;
	}
	
	return res;
}