#include "FileProtocol.h"

#include <CasioPacker.h>
#include <AsciiConverter.h>
#include <string.h>
#include <Log.h>

#include <stdio.h>
#include <algorithm>

bool FileProtocol::buildSendRequestPacket(Buffer * output, int dataSize, const char * foldername, const char * filename, const char * devicename, unsigned char mode)
{
    unsigned char data[1024];     //layout: mode+data_type+data_size+d[1-6]_size+d1+d2+d5
    Buffer buffer(data,1024);
    
    AsciiConverter::numberToAscii(mode,buffer.getData(),2); //MODE (0x02 = overwrite ?)
    AsciiConverter::numberToAscii(0x05,buffer.getData()+2,2); //data type
    AsciiConverter::numberToAscii(dataSize,buffer.getData()+4,8); //data size

    int folderNameSize = strlen(foldername);
    int fileNameSize = strlen(filename);
    int deviceNameSize = strlen(devicename);
    
    AsciiConverter::numberToAscii(folderNameSize,buffer.getData()+12,2); //D1 -> folder name size
    AsciiConverter::numberToAscii(fileNameSize,buffer.getData()+14,2); //D2 -> file name size
    AsciiConverter::numberToAscii(0,buffer.getData()+16,2);
    AsciiConverter::numberToAscii(0,buffer.getData()+18,2);
    AsciiConverter::numberToAscii(deviceNameSize,buffer.getData()+20,2); //D5 -> peripheral name size
    AsciiConverter::numberToAscii(0,buffer.getData()+22,2);
    
    memcpy(buffer.getData()+24,foldername,folderNameSize);
    memcpy(buffer.getData()+24+folderNameSize,filename,fileNameSize);
    memcpy(buffer.getData()+24+folderNameSize+fileNameSize,devicename,deviceNameSize);
    
    buffer.setSize(24+folderNameSize+fileNameSize+deviceNameSize);
    
    CasioPacketInfo info;
    info.type = 0x1;
    info.subtype = 0x45;
    info.extendedData = &buffer;
    
    if(CasioPacker::pack(info,output))
    {
        Log::error("Failed to pack send file request !");
        return true;
    }

    return false;
}

bool FileProtocol::buildFilePartPacket(Buffer * output, Buffer * data, int partId, int partCount, int partSize)
{
    unsigned char temporaryData[2048];
    
    if(output->getSize() < partSize+8)
    {
        Log::error("buildFilePartPacket: impossible: Buffer too small !");
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
        return true;
    }
    
    return false;
}