#include "CasioPacker.h"

#include <string.h>

#include <AsciiConverter.h>

#include <CasioChecksum.h>

#include <CasioEscaper.h>

#include <Log.h>

#include <stdio.h>

#define MAX_CASIO_PACKET_SIZE 1024

bool CasioPacker::pack(CasioPacketInfo packetInfo, Buffer * output)
{
    output->getData()[0] = packetInfo.type;
    AsciiConverter::numberToAscii(packetInfo.subtype,output->getData()+1,2);

    
    int extraSize = insertExtendedData(packetInfo.extendedData,output);
    if(extraSize == -1)
    {
        return true;
    }
    
    
    Buffer checksumBuffer(output->getData(),4+extraSize);
    unsigned char checksum = CasioChecksum::compute(&checksumBuffer);
    
    
    AsciiConverter::numberToAscii(checksum,output->getData()+4+extraSize,2);
    
    output->setSize(4+extraSize+2);
    
    /* DEBUG
    printf("Packet dump: ");
    for(int i = 0 ; i < output->getSize() ; i++)
    {
        printf(" (%d) %#x [%c] ",i-8,output->getData()[i],output->getData()[i]);
    }
    printf("\n");
    */
    
    return false;
}

int CasioPacker::insertExtendedData(Buffer * data, Buffer * workingBuffer)
{
    unsigned char escapedDataData[MAX_CASIO_PACKET_SIZE*2];
    Buffer escapedData(escapedDataData,MAX_CASIO_PACKET_SIZE*2);
    
    
    if(CasioEscaper::escapeBuffer(data,&escapedData))
    {
        Log::error("insertExtendedData: IMPOSSIBLE: too big for escape !");
        printf("DEBUG INFO: Data size %d, working buffer size %d\n",data->getSize(),workingBuffer->getSize());
        return -1;
    }

    AsciiConverter::numberToAscii(escapedData.getSize()>0?1:0,workingBuffer->getData()+3,1);
    
    
    
    if(escapedData.getSize())
    {
        AsciiConverter::numberToAscii(escapedData.getSize(),workingBuffer->getData()+4,4);
        memcpy(workingBuffer->getData()+8,escapedData.getData(),escapedData.getSize());
        
        return 4+escapedData.getSize();
    }
    else
    {
        return 0;
    }
}

bool CasioPacker::getExtendedData(Buffer * data, Buffer * input)
{
    unsigned char escapedDataData[MAX_CASIO_PACKET_SIZE*2];
    Buffer escapedData(escapedDataData,MAX_CASIO_PACKET_SIZE*2);
    
    escapedData.setSize(getExtendedDataSize(input));
    
    if(escapedData.getSize() == 0)
    {
        return false;
    }

    memcpy(escapedData.getData(),input->getData()+8,escapedData.getSize());
    
    if(CasioEscaper::restoreBuffer(&escapedData,data)) // Too small
    {
        return true;
    }
    
    return false;
}

int CasioPacker::getExtendedDataSize(Buffer * input)
{
    int isExtended;
    AsciiConverter::asciiToNumber(input->getData()+3,&isExtended,1);
    if(isExtended)
    {
        int extendedDataSize;
        AsciiConverter::asciiToNumber(input->getData()+4, &extendedDataSize,4);
        return extendedDataSize;
    }
    else
    {
        return 0;
    }
}

bool CasioPacker::checkCorruption(Buffer * input)
{
    Buffer checksumBuffer(input->getData(),input->getSize()-2);
    unsigned char checksum = CasioChecksum::compute(&checksumBuffer);
    
    int originalChecksum;
    AsciiConverter::asciiToNumber(input->getData()+input->getSize()-2, &originalChecksum,2);
    
    if(originalChecksum != checksum)
    {
        Log::warning("Wrong checksum !");
        printf("packetsize %d\n",input->getSize());
        printf("%d vs %d\n",originalChecksum,checksum);
        

        int subtypeInt;
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
    
    int subtypeInt;
    AsciiConverter::asciiToNumber(input->getData()+1,&subtypeInt,2); // subtype
    packetInfo->subtype = subtypeInt;
    
    
    if(getExtendedData(packetInfo->extendedData, input)) // extended
    {
        Log::error("Can't unpack, buffer too small !");
        return true;
    }
    
    return false;
}