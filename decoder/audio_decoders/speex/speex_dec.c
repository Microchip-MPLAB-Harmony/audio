/*******************************************************************************
  Speex Decoder Library Implementation

  Company:
    Microchip Technology Inc.

  File Name:
    speex_dec.c

  Summary:
   Contains the functional implementation of speex decoder.

  Description:
   This file contains the functional implementation of speex decoder.
 *******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <string.h>
#include "system/fs/sys_fs.h"
#include "speex_dec.h"

static int32_t          speex_bitrate;
static int32_t          speex_bitspersample;
static sSpeexHeader     pSpxHdr[1];
//static progressDcpt     pPrgDcpt[1];
static sSpxStreamDcpt          pSpxDcpt[1];
static spxDecDcpt              pDDcpt[1];



static SYS_FS_HANDLE currentSPXFileHandle;
//static void convertMono2Stereo(int16_t *buf, size_t size);
static SPEEX_ERROR_MSG SPEEX_OggSkipPage(sOggPageSegHdr* pOggPage);
static int SPEEX_OggPagePackets(sOggPageSegHdr* pOggPage);
static int32_t SPEEX_OggPageDataSize(sOggPageSegHdr* pOggPage);
static SPEEX_ERROR_MSG SPEEX_OggReadPageHdr(sOggPageSegHdr *pOggHdr, sSpxStreamDcpt* pSpxDcpt);
static int SPEEX_OggPageGetPktSize(sSpxStreamDcpt* pDcpt, int* pPktComplete);
static void SpxCleanUpDecoder(spxDecDcpt* h);
static SPEEX_ERROR_MSG SpxInitDecoder(int procSamples);
static SPEEX_ERROR_MSG SPEEX_ReadData(sSpxStreamDcpt* pSpxDcpt, void* pBuff, int nBytes, void* pResult);

SPEEX_ERROR_MSG SPEEX_Initialize(SYS_FS_HANDLE spx_file_handle)
{
    SPEEX_ERROR_MSG ret = 1;
    uint32_t readRet;
    bool commentFound;
    int		nPkts, pgSize;
	int		compressFact, avgPktSize;
    eSpxFlags	spxFlags;
    sOggPageSegHdr*	pOggPageHdr = &pSpxDcpt->pageHdr;
    currentSPXFileHandle = spx_file_handle;
    while(ret == SPEEX_SUCCESS)
    {
        if((ret=SPEEX_OggReadPageHdr(pOggPageHdr, 0)) != SPEEX_SUCCESS)
		{
			break;   
		}

        if(pOggPageHdr->headerFlags!=OGG_FLAG_BOS || pOggPageHdr->granulePos!=0 || pOggPageHdr->pageNo!=0 ||
                    pOggPageHdr->pageSegments!=1 || pOggPageHdr->segmentTbl[0]!=sizeof(sSpeexHeader))
        {
            // looking into a wrong Ogg stream
            // try to skip this page in case we have a multiple Ogg container...
            ret=SPEEX_OggSkipPage(pOggPageHdr);
            continue;	// skipped current page...
        }
            
        // read speex header
        readRet = SYS_FS_FileRead(currentSPXFileHandle, (uint8_t*)pSpxHdr, sizeof(*pSpxHdr));

        if(readRet != sizeof(*pSpxHdr))
        {
            ret=SPEEX_READ_ERROR;	// could not read the Stream
            break;
        }

        if(strncmp(pSpxHdr->speexString, OGG_ID_SPEEX, SPEEX_STRING_LENGTH) || pSpxHdr->headerSize!=sizeof(*pSpxHdr))
        {
            continue;	// skip current page in case we have a multiple Ogg container...
        }

        // finally, we got a valid Speex header.
        // try to get to the  data page
        // for now, we ignore the comment page

        commentFound=0;
        while(ret==SPEEX_SUCCESS)
        {
            if((ret = SPEEX_OggReadPageHdr(pOggPageHdr,pSpxDcpt))!=SPEEX_SUCCESS)
            {
                break;		// some error
            }

            // valid new page from the same stream
            if(!commentFound)
            {
                // this should be the Speex comment block
                if(pOggPageHdr->granulePos==0 && pOggPageHdr->pageNo==1 && pOggPageHdr->pageSegments==1)
                {	// seems to be the valid comment
                    commentFound=1;
                }

                ret=SPEEX_OggSkipPage(pOggPageHdr);
                continue;	// just skip it anyway
            }
            else if(pOggPageHdr->pageNo==2 && pOggPageHdr->granulePos)
            { // ok, we got the 1st data page
                break;
            }

            ret = SPEEX_OggSkipPage(pOggPageHdr);	// try to find another start of data page?
                                    // will fail, most likely
        }
        
        if(ret!=SPEEX_SUCCESS)
		{
			break;	// some error occurred
		}
        
        // store the info we need from the header
        pSpxDcpt->runDcpt.streamNo=pOggPageHdr->streamNo;
        pSpxDcpt->runDcpt.framesPerPacket=pSpxHdr->framesPerPacket;
        pSpxDcpt->runDcpt.frameSamples=pSpxHdr->frameSamples;
        spxFlags=0;
        if(pSpxHdr->wBand)
        {
            spxFlags|=SPX_FLAG_WB;
        }
        if(pSpxHdr->vbr)
        {
            spxFlags|=SPX_FLAG_VBR;
        }
        pSpxDcpt->runDcpt.spxFlags=spxFlags;
        
        pgSize=SPEEX_OggPageDataSize(pOggPageHdr);
		nPkts=SPEEX_OggPagePackets(pOggPageHdr);
		avgPktSize=nPkts?pgSize/nPkts:pgSize;		// TODO: this works just for CBR
								// have to be changed for VBR!

		pSpxDcpt->segIx=pSpxDcpt->pktIx=0;
        
        
        // since the spxHdr.bitRate is not updated we try to estimate something:
        // NOTE: the calculation can be done using the SPEEX_GET_BITRATE.
        // However, at this moment we didn't feed any data into the Speex decoder
        // compression factor is multiplied by 100 for better approx
        compressFact=(pSpxHdr->framesPerPacket*pSpxHdr->frameSamples*200)/(avgPktSize);	// (2bytes*100)/1byte
        speex_bitrate=(pSpxHdr->sampleRate*16*100)/compressFact;	// in bps
    //    pSpxHdr->bitRate = speex_bitrate;
        speex_bitspersample = (16*100+compressFact-1)/compressFact;		// round up
    //    pSHndl->info.lEndian=1;				// always in Ogg files
        
        break;
    }
    
    if(ret != SPEEX_SUCCESS)
    {
        return ret;
    }
    ret = SpxInitDecoder(1);
    return ret;
}

SPEEX_ERROR_MSG SPEEX_Decoder(uint8_t *input, uint16_t inSize, uint16_t *read, int16_t *output, uint16_t *written)
{
    int		frameIx;
    int		decFault;
    
    int8_t *outptr = (int8_t*)output;
    SPEEX_ERROR_MSG res = SPEEX_SUCCESS;
    
    for(frameIx=0; frameIx < pDDcpt->framesPerPacket; frameIx++)
    {
        // decode a frame
        if(pDDcpt->currPktNo==pDDcpt->processedPktNo+1)
        { // in sync
            decFault=speex_decode_int(pDDcpt->cdc.spxState, &pDDcpt->cdc.spxBits, (short*)outptr);
        }
        else
        { // lost frames, let decoder guess
            decFault=speex_decode_int(pDDcpt->cdc.spxState, 0, (short*)outptr);
        }

        if(decFault==-1)
        {
            res=SPEEX_STREAM_END;	// eos
            pDDcpt->cdc.nOutBytes += pDDcpt->frameSize;	// update the bytes in the output buffer
            *written += pDDcpt->frameSize;
            return res;
        }
        else if(decFault==-2)
        {	//error in the input stream ?
            res=SPEEX_STREAM_ERROR;
            return res;
        }

        /*if(speex_bits_remaining(pDcpt->cdc.spxState)<0)
        {	// overflow...
            res=AUDIO_RES_DSTREAM_ERROR;	// TODO
            break;
        }*/				
        
        // Convert to stereo
//        convertMono2Stereo((int16_t*)outptr, pDDcpt->frameSize/2);

        outptr += pDDcpt->frameSize; // update output ptr
        pDDcpt->cdc.nOutBytes += pDDcpt->frameSize;	// update the bytes in the output buffer
        *written += pDDcpt->frameSize; // multiply by 2, stereo mode
        
    }

    if(res==SPEEX_SUCCESS)
    {
        pDDcpt->processedPktNo++;	// packet processed

        if(pDDcpt->cdc.nOutBytes+pDDcpt->outFrameSize > pDDcpt->cdc.outBuffSize)			
        {	// no room for another frame, output buffer has to be flushed
//            pDDcpt->cdc.pOutFrame=pDcpt->cdc.outBuff;	// reset pointers
            return res;
        }

        //TODO: progress dcpt
//        if(pPrgDcpt)	
//        {
//            if(++pPrgDcpt->progressCnt==pPrgDcpt->progressStep)
//            {
//                (*pPrgDcpt->progressFnc)(pPrgDcpt->progressCnt);
//                pPrgDcpt->progressCnt=0;
//            }
//        }
    }
    return res;
}


SPEEX_ERROR_MSG SPEEX_OggReadPageHdr(sOggPageSegHdr *pOggHdr, sSpxStreamDcpt* pSpxDcpt){
//    int		nBytes;
	int		pageBytes;
	bool	useStreamId;
	int		streamId=0; 
    SPEEX_ERROR_MSG res = SPEEX_SUCCESS;
    uint32_t readRet;
	
    if((useStreamId=(pSpxDcpt!=0)))
	{
		streamId=pSpxDcpt->pageHdr.streamNo;
		if((pSpxDcpt->pageHdr.headerFlags&OGG_FLAG_EOS)!=0)
		{
			res=SPEEX_STREAM_END;	// end of stream
		}
	}
    
    while(res == SPEEX_SUCCESS)
	{
		pageBytes=sizeof(*pOggHdr)-sizeof(pOggHdr->segmentTbl);	// page with no segment table
        readRet = SYS_FS_FileRead(currentSPXFileHandle, (uint8_t*)pOggHdr, pageBytes);
		if(readRet != pageBytes)
		{
			res=SPEEX_READ_ERROR;
			break;
		}
		else if(pOggHdr->pageCapture!=OGG_ID_MAGIC)
		{
			res=SPEEX_STREAM_ERROR;	// not an Ogg Stream
			break;
		}
		else if((pageBytes=pOggHdr->pageSegments))
		{
            readRet = SYS_FS_FileRead(currentSPXFileHandle, (uint8_t*)pOggHdr->segmentTbl, pageBytes);
			if(readRet == pageBytes)
			{
				if(!useStreamId || pOggHdr->streamNo==streamId)
				{	// found it
					break;
				}
				else if(!(pOggHdr->headerFlags&OGG_FLAG_EOS))
				{
					res=SPEEX_OggSkipPage(pOggHdr);
					continue;
				}
				else
				{
					res=SPEEX_STREAM_END;	// wrong stream, end of stream
					break;
				}
			}
			else
			{
				res=SPEEX_READ_ERROR;
				break;
			}
		}
		else if(pOggHdr->headerFlags&OGG_FLAG_EOS)
		{
			res=SPEEX_STREAM_END;
		}
		else
		{	
			res=SPEEX_STREAM_ERROR;	// empty page ?
			break;
		}
	}

	return res;
}
uint8_t SPEEX_GetChannels(){
    return pSpxHdr->nChannels;
}
//static void convertMono2Stereo(int16_t *buf, size_t size){
//    int16_t i = 0;
//    for(i = size - 1; i >= 0; i--)
//    {
//        memcpy(&buf[2*i], &buf[i], sizeof(int16_t));
//        memcpy(&buf[2*i+1], &buf[i], sizeof(int16_t));
//    }
//}

static SPEEX_ERROR_MSG SPEEX_OggSkipPage(sOggPageSegHdr* pOggPage){

    SPEEX_ERROR_MSG ret;

    int32_t	dataBytes=SPEEX_OggPageDataSize(pOggPage);

    int status;
    status = SYS_FS_FileSeek(currentSPXFileHandle, dataBytes, SYS_FS_SEEK_CUR);
    if( status == dataBytes )
    {
        ret=SPEEX_SUCCESS;
    }else{
        ret=SPEEX_READ_ERROR;
    }
    
    
    return ret;
}

// returns the number of data bytes in an Ogg page
static int32_t SPEEX_OggPageDataSize(sOggPageSegHdr* pOggPage){
    int32_t	ix;
	int32_t	dataBytes;
    
	for(ix=0, dataBytes=0; ix<pOggPage->pageSegments; ix++)
	{
		dataBytes+=pOggPage->segmentTbl[ix];
	}
	return dataBytes;
}


// returns the number of completed packets in an Ogg page
// packets are counted on the page they actually end, not on the page they begin.
static int SPEEX_OggPagePackets(sOggPageSegHdr* pOggPage)
{
	int	ix;
	int	nPackets=0;
	
			
	for(ix=0; ix<pOggPage->pageSegments; ix++)
	{
		if(pOggPage->segmentTbl[ix]<255)
		{
			nPackets++;
		}
	}

	return nPackets;
}


// returns the number of bytes in the current packet, completed or not
// also returns true if the packet pointed by the current segIx is completed on this page
// updates the current segIx
static int SPEEX_OggPageGetPktSize(sSpxStreamDcpt* pDcpt, int* pPktComplete)
{
	int segIx;
	int nBytes;
	int totBytes=0;

	*pPktComplete=0;	
	for(segIx=pDcpt->segIx; segIx<pDcpt->pageHdr.pageSegments; segIx++)
	{
		totBytes+=(nBytes=pDcpt->pageHdr.segmentTbl[segIx]);
		if(nBytes<255)
		{
			*pPktComplete=1;
			segIx++;
			break;
		}
	}
	
	pDcpt->segIx=segIx;
	return totBytes;
}


int32_t SPEEX_DiskRead(uint8_t *inBuff){
    SPEEX_ERROR_MSG res;
    sSpxPktDcpt	pktDcpt;
    int32_t readBytes = 0;
    pktDcpt.pktSeqNo = 0;
    pDDcpt->cdc.nOutBytes=0;
    if(pDDcpt->processedPktNo==pDDcpt->currPktNo){
        readBytes = SYS_FS_FileTell(currentSPXFileHandle);
        res = SPEEX_ReadData(pSpxDcpt, inBuff, SPEEX_INPUT_BUFFER_SIZE, &pktDcpt);
        readBytes = SYS_FS_FileTell(currentSPXFileHandle) - readBytes;
        if(res!=SPEEX_SUCCESS)
        {
            return -1;
        }
        pDDcpt->currPktNo=pktDcpt.pktSeqNo;

//        prevPktSize = pktDcpt.pktBytes;
        speex_bits_read_from(&pDDcpt->cdc.spxBits, (char *)inBuff, pktDcpt.pktBytes);
        return readBytes;
    }
    return -1;
}

// This function read packet by packet

// read next data packet
// the read will extend across page boundary if the packet doesn't end on the current page
// in case of page lost, the correct packet number should be returned

// param: pBuff: input buffer pointer
SPEEX_ERROR_MSG SPEEX_ReadData(sSpxStreamDcpt* pSpxDcpt, void* pBuff, int nBytes, void* pResult)
{
	int		rdBytes;
	int		pktBytes;
	int		pktComplete;
	int		prevPgNo;
	int		loopCount;
	
	SPEEX_ERROR_MSG	res = SPEEX_SUCCESS;
//	sSpxStreamDcpt*	pSpxDcpt=(sSpxStreamDcpt*)pStr->pHDcpt;	// get the speex state machine info
	sSpxPktDcpt*	pPktDcpt=(sSpxPktDcpt*)pResult;


	pPktDcpt->pktBytes=0;
	loopCount=0;
	
	while(1)
	{
		pktBytes=SPEEX_OggPageGetPktSize(pSpxDcpt, &pktComplete);
		if(pktBytes==0 && pktComplete)
		{ // if loopCount==0, we have a 0 len pkt mid page
			res=loopCount?SPEEX_SUCCESS:SPEEX_STREAM_ERROR;
			break;
		}
		
		// either pktBytes or pktComplete==0
		if(pktBytes)
		{
			if(nBytes<pktBytes)
			{
				res=SPEEX_BUFF_ERROR;	// should accommodate at least a packet
				break;
			}
            rdBytes = SYS_FS_FileRead(currentSPXFileHandle, (uint8_t*)pBuff, pktBytes);
			
			if(rdBytes != pktBytes)
			{
				res=SPEEX_READ_ERROR;
				break;
			}
		}
		
		if(pktComplete)
		{
			pPktDcpt->pktBytes+=pktBytes;
			pPktDcpt->pktSeqNo=++pSpxDcpt->pktIx;	// was in sync
			break;
		}
		
		// read in a new page
		pBuff=(char*)pBuff+pktBytes;
		nBytes-=pktBytes;
			
		prevPgNo=pSpxDcpt->pageHdr.pageNo;
		res=SPEEX_OggReadPageHdr(&pSpxDcpt->pageHdr, pSpxDcpt);
		if(res!=SPEEX_SUCCESS)
		{
			if(res==SPEEX_STREAM_END)
			{
				res=SPEEX_SUCCESS;	// eos
			}
			break;		// could not end in the middle of the packet!
		}
		pSpxDcpt->segIx=0;	// brand new page
		if(pSpxDcpt->pageHdr.pageNo!=prevPgNo+1)
		{ // we're out of sync
			// TODO: update pSpxDcpt->pktIx with lost packets: based on frameSamples, framesPerPkt and granulePos
			// for now, no op. We maintain sequential packet numbers!
		}
		loopCount++;
	}
	
	return res;
}


static SPEEX_ERROR_MSG SpxInitDecoder(int procSamples)
{
    
	sSpxRunDcpt	runDcpt;
	int		outFrameSamples;
	int		sampleRate;
	int		inBuffSize;
	int		compression;
	int		prcptEnh;
//	spxDecDcpt*	pDDcpt;

    runDcpt = pSpxDcpt->runDcpt;
	

//	pDDcpt=(spxDecDcpt*)calloc(1, sizeof(*pDDcpt));
//	if(!pDDcpt)
//	{
//		return APP_OUT_OF_MEM_ERROR;
//	}

//	AudioStreamGetRunInfo(AudioGetInStream(), &runDcpt, sizeof(runDcpt));
	
	speex_bits_init(&pDDcpt->cdc.spxBits);
#ifdef SPEEX_WIDEBAND	
	pDDcpt->cdc.spxState=speex_decoder_init((runDcpt.spxFlags&SPX_FLAG_WB)?&speex_wb_mode:&speex_nb_mode);
#else
	pDDcpt->cdc.spxState=speex_decoder_init(&speex_nb_mode);	
#endif	// SPEEX_WIDEBAND

	if(!pDDcpt->cdc.spxState)
	{	// allocation problems
		SpxCleanUpDecoder(pDDcpt);
		return SPEEX_OUT_OF_MEM_ERROR;		
	}
		
	
	prcptEnh=(runDcpt.spxFlags&SPX_FLAG_PRCPT_ENH)!=0;
	speex_decoder_ctl(pDDcpt->cdc.spxState, SPEEX_SET_ENH, &prcptEnh);

	sampleRate=pSpxHdr->sampleRate;
	speex_decoder_ctl(pDDcpt->cdc.spxState, SPEEX_SET_SAMPLING_RATE, &sampleRate);

//    speex_decoder_ctl(pDDcpt->cdc.spxState, SPEEX_GET_FRAME_SIZE, &decoderFrameSize);

	compression=(pSpxHdr->sampleRate*16)/speex_bitrate;
	inBuffSize=(runDcpt.framesPerPacket*runDcpt.frameSamples*sizeof(short))/compression;
	inBuffSize=(inBuffSize*SPX_CODEC_BUFF_MUL)/SPX_CODEC_BUFF_DIV;
	pDDcpt->cdc.inBuffSize=(inBuffSize+7)&0xfffffff8;	// 8 multiple

	outFrameSamples=runDcpt.framesPerPacket*runDcpt.frameSamples;	// Always process full packets
	// round up input size to an integer number of frames taken by the encoder
	pDDcpt->cdc.outBuffSize=((procSamples+outFrameSamples-1)/outFrameSamples)*outFrameSamples;
	
	pDDcpt->cdc.outBuffSize*=sizeof(short);
	pDDcpt->outFrameSize=outFrameSamples*sizeof(short);	// convert to bytes


    /*
     * Should check if inBuffSize is larger than decoder input buffer that allocated in application level
     */
//    if(pDDcpt->cdc.inBuffSize > DECODER_MAX_INPUT_BUFFER_SIZE || pDDcpt->outFrameSize*2 > DECODER_MAX_OUTPUT_BUFFER_SIZE)
//    {
//        return APP_BUFF_ERROR;
//    }

	pDDcpt->nInBytes=pDDcpt->cdc.nOutBytes=0;	// show input buffer empty
//	pDDcpt->cdc.pOutFrame= NULL;//pDDcpt->cdc.outBuff;	// set the out pointers

	pDDcpt->framesPerPacket=runDcpt.framesPerPacket;
	pDDcpt->frameSize=runDcpt.frameSamples*sizeof(short);
	pDDcpt->processedPktNo=pDDcpt->currPktNo=0;
	
	return SPEEX_SUCCESS;

}

void SPEEX_Cleanup(){
    SpxCleanUpDecoder(pDDcpt);
}

static void SpxCleanUpDecoder(spxDecDcpt* pDDcpt)
{
	if((pDDcpt))
	{
		speex_bits_destroy(&pDDcpt->cdc.spxBits);
		if(pDDcpt->cdc.spxState)
		{
			speex_decoder_destroy(pDDcpt->cdc.spxState);
		}
	}
}

bool isSPEEXdecoder_enabled()
{
    return true;
}

int32_t SPEEX_GetSamplingRate(){
    return pSpxHdr->sampleRate;
}

int32_t SPEEX_GetBitrate(){
    return speex_bitrate;
}
