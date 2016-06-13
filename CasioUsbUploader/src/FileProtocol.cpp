#include "FileProtocol.h"

#include <CasioPacker.h>
#include <AsciiConverter.h>
#include <string.h>
#include <Log.h>

#include <stdio.h>
#include <algorithm>

bool FileProtocol::buildSendRequestPacket(Buffer * output, int dataSize, const char * foldername, const char * filename, const char * devicename, unsigned char mode)
{


    size_t folderNameSize = strlen(foldername);
    size_t fileNameSize = strlen(filename);
    size_t deviceNameSize = strlen(devicename);
    
    unsigned char * dataPtr;     //layout: mode+data_type+data_size+d[1-6]_size+d1+d2+d5
    
    int bufferSize = (int) 24+folderNameSize+fileNameSize+deviceNameSize;
    dataPtr = (unsigned char *) malloc(bufferSize);
    if(dataPtr == NULL)
    {
        Log::error("Failed to allocate the buffer !");
        return true;
    }
    Buffer extendedDataBuffer(dataPtr,bufferSize);
    
    AsciiConverter::numberToAscii(mode,extendedDataBuffer.getData(),2); //MODE (0x02 = overwrite ?)
    AsciiConverter::numberToAscii(0x05,extendedDataBuffer.getData()+2,2); //data type
    AsciiConverter::numberToAscii(dataSize,extendedDataBuffer.getData()+4,8); //data size
    
    AsciiConverter::numberToAscii(folderNameSize,extendedDataBuffer.getData()+12,2); //D1 -> folder name size
    AsciiConverter::numberToAscii(fileNameSize,extendedDataBuffer.getData()+14,2); //D2 -> file name size
    AsciiConverter::numberToAscii(0,extendedDataBuffer.getData()+16,2);
    AsciiConverter::numberToAscii(0,extendedDataBuffer.getData()+18,2);
    AsciiConverter::numberToAscii(deviceNameSize,extendedDataBuffer.getData()+20,2); //D5 -> peripheral name size
    AsciiConverter::numberToAscii(0,extendedDataBuffer.getData()+22,2);
    
    memcpy(extendedDataBuffer.getData()+24,foldername,folderNameSize);
    memcpy(extendedDataBuffer.getData()+24+folderNameSize,filename,fileNameSize);
    memcpy(extendedDataBuffer.getData()+24+folderNameSize+fileNameSize,devicename,deviceNameSize);
    
    
    CasioPacketInfo info;
    info.type = 0x1;
    info.subtype = 0x45;
    info.extendedData = &extendedDataBuffer;
    
    if(CasioPacker::pack(info,output))
    {
        Log::error("Failed to pack send file request !");
        free(dataPtr);
        return true;
    }

    free(dataPtr);
    return false;
}

bool FileProtocol::buildFilePartPacket(Buffer * output, Buffer * data, int partId, int partCount, int partSize)
{
    unsigned char * temporaryData = (unsigned char *) malloc(partSize+8);
    if(temporaryData == NULL)
    {
        Log::error("Failed to alloc buffer !");
        return true;
    }
    
    
    AsciiConverter::numberToAscii(partCount,temporaryData,4);
    AsciiConverter::numberToAscii(partId+1,temporaryData+4,4); // starts at 1
    
    int dataBeginning = partId*partSize;
    int dataSize = std::min(data->getSize()-dataBeginning,partSize); // do not overread the data
    
    memcpy(temporaryData+8,data->getData()+dataBeginning,dataSize);
    
    
    Buffer temporaryBuffer(temporaryData,dataSize+8);
    
    CasioPacketInfo info;
    info.type = 0x2;
    info.subtype = 0x45;
    info.extendedData = &temporaryBuffer;

    if(CasioPacker::pack(info,output))
    {
        Log::error("Failed to pack file part !");
        free(temporaryData);
        return true;
    }
    
    free(temporaryData);
    return false;
}