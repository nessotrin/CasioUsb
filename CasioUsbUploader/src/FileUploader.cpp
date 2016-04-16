#include "FileUploader.h"

#include <FileProtocol.h>
#include <FileLoader.h>
#include <CasioPacker.h>
#include <CasioProtocolTools.h>

#include <stdio.h>
#include <math.h>

bool FileUploader::sendFileRequest(const char * foldername, int dataSize, const char * filename, const char * devicename, unsigned char mode, Socket * socket)
{
    unsigned char packedData[2048];
    Buffer packedBuffer(packedData,2048);
    
    FileProtocol::buildSendRequestPacket(&packedBuffer,dataSize,foldername,filename,devicename,mode);
    
    CasioPacketInfo info;
    
    if(CasioProtocolTools::sendPacketAndGetAnswer(socket,&packedBuffer,5,&info))
    {
        Log::error("Failed to send the send file request !");
        return true;
    }
    
    if(info.type == 0x06 && info.subtype == 0x00)
    {
        Log::info("File request accepted !");
        return false;
    }
    else if(info.type == 0x15 && info.subtype == 0x02)
    {
        Log::error("File already exists !");
        Log::info("use -w to overwrite ?");
        return true;
    }
    else if(info.type == 0x15 && info.subtype == 0x05)
    {
        Log::error("Memory full !");
        Log::info("optimize the memory ?");
        return true;
    }
    else
    {
        Log::error("Bad answer from the calc !");
        printf("DEBUG INFO: type:%#x subtype:%#x extended size: %d\n",info.type,info.subtype,info.extendedData->getSize());
        return true;
    }

}

bool FileUploader::sendFilePart(Buffer * data, Socket * socket, int partId, int partCount, int partSize)
{
    unsigned char packedData[2048];
    Buffer packedBuffer(packedData,2048);
    
    if(FileProtocol::buildFilePartPacket(&packedBuffer,data,partId,partCount,partSize))
    {
        Log::error("Failed to create the send part packet !");
        return true;
    }
    
    if(CasioProtocolTools::sendPacketAndAck(socket,&packedBuffer,10))
    {
        Log::error("Failed to send the part packet !");
        return true;
    }
    
    return false;
}

bool FileUploader::uploadFile(Socket * socket, const char * foldername, const char * filename, const char * devicename, unsigned char mode)
{
    Buffer fileData = FileLoader::readFile(filename);
    if(fileData.getSize() == -1)
    {
        Log::error("Failed to load the file !");
        return 1;
    }
    
    if(sendFileRequest(foldername,fileData.getSize(),filename,devicename,mode,socket))
    {
        Log::error("Failed the file send request !");
        return true;
    }
    
    if(fileData.getSize() == 0)
    {
        Log::error("Zero sized file not handled at this time !");
        return true;
    }
    
    int partCount = ceil(fileData.getSize()/256.0f);
    
    Log::info("Starting to upload ...");
        
    for(int i = 0 ; i < partCount ; i++)
    {
        Log::debug("Sending next part ...");
        if(sendFilePart(&fileData,socket,i,partCount,256))
        {
            return true;
        }
    }
    
    Log::info("File uploaded !");
    
    return false;
}
