#include "CasioPacker.h"

#include <string.h>

#include <AsciiConverter.h>

#include <CasioChecksum.h>

#include <CasioEscaper.h>

#include <Log.h>

#include <stdio.h>
#include <stdlib.h>

bool CasioPacker::pack(CasioPacketInfo packetInfo, Buffer * output)
{
    output->setData((unsigned char *) malloc(4));
    if(output->getData() == NULL)
    {
        Log::error("Failed to alloc !");
        return true;
    }
    output->setSize(4);
    
    output->getData()[0] = packetInfo.type;
    AsciiConverter::numberToAscii(packetInfo.subtype,output->getData()+1,2);
    
    int extraSize = insertExtendedData(packetInfo.extendedData,output);
    if(extraSize == -1)
    {
        return true;
    }
    
    unsigned char checksum = CasioChecksum::compute(output);
    
    output->setData((unsigned char *)realloc(output->getData(),4+extraSize+2));
    if(output->getData() == NULL)
    {
        Log::error("Failed to realloc !");
        return true;
    }
    output->setSize(4+extraSize+2);
    
    AsciiConverter::numberToAscii(checksum,output->getData()+4+extraSize,2); // append

    
    return false;
}

/*
 * Appends the escaped extended data to a packet buffer
 * 
 * 
 * 
 */
int CasioPacker::insertExtendedData(Buffer * toInsert, Buffer * workingBuffer)
{
    int insertSize = 0;  
    
    if(toInsert->getSize() > 0)
    {
        CasioEscaper::escapeBuffer(toInsert,workingBuffer,8); //escape & copy => starts at 8 after (base) 4 + (data size) 4  

       	insertSize = workingBuffer->getSize() - 8; // get raw data size
        AsciiConverter::numberToAscii(insertSize,workingBuffer->getData()+4,4); //writes the size
    	insertSize += 4; //add size bytes back
    }

    AsciiConverter::numberToAscii(insertSize>0?1:0,workingBuffer->getData()+3,1); //write the extended data byte
    

    return insertSize;
}

bool CasioPacker::getExtendedData(Buffer * data, Buffer * input)
{
    Buffer escapedData(input->getData()+4,input->getSize()-4);
    
    if(CasioEscaper::restoreBuffer(&escapedData,data)) // Too small
    {
        return true;
    }
    
    return false;
}

int CasioPacker::getExtendedDataSize(Buffer * input)
{
    unsigned int isExtended;
    AsciiConverter::asciiToNumber(input->getData()+3,&isExtended,1);
    if(isExtended)
    {
        unsigned int extendedDataSize;
        AsciiConverter::asciiToNumber(input->getData()+4, &extendedDataSize,4);
        return extendedDataSize;
    }
    else
    {
        return 0;
    }
}

#define MAX_CASIO_PACKET_SIZE 512 // Unknown, it should be fine

bool CasioPacker::checkCorruption(Buffer * input)
{
    Buffer checksumBuffer(input->getData(),input->getSize()-2);
    unsigned char checksum = CasioChecksum::compute(&checksumBuffer);
    
    unsigned int originalChecksum;
    AsciiConverter::asciiToNumber(input->getData()+input->getSize()-2, &originalChecksum,2);
    
    if(originalChecksum != checksum)
    {
        Log::warning("Wrong checksum !");
        printf("packetsize %d\n",input->getSize());
        printf("%d vs %d\n",originalChecksum,checksum);
        

        unsigned int subtypeInt;
        AsciiConverter::asciiToNumber(input->getData()+1,&subtypeInt,2); // subtype
        printf("Type%d Subtype%d\n",input->getData()[0],subtypeInt);
        
        return true;
    }
    
    int extendedDataSize = getExtendedDataSize(input);
    if(extendedDataSize < 0 || extendedDataSize > MAX_CASIO_PACKET_SIZE)
    {
        Log::warning("Bad extended data size !");
        printf("%d\n",extendedDataSize);
        return true;
    }
    
    
    return false;
}
    
bool CasioPacker::unpack(CasioPacketInfo * packetInfo, Buffer * input)
{
    if(checkCorruption(input))
    {
        Log::warning("Corrupted packet !");
        return true;
    }
    
    packetInfo->type = input->getData()[0]; // Type
    
    unsigned int subtypeInt;
    AsciiConverter::asciiToNumber(input->getData()+1,&subtypeInt,2); // subtype
    packetInfo->subtype = (unsigned char) subtypeInt;
    
    
    if(getExtendedData(packetInfo->extendedData, input)) // extended
    {
        Log::error("Can't unpack, buffer too small !");
        return true;
    }
    
    return false;
}