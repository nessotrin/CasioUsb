#include "CasioProtocolTools.h"

#include <Log.h>
#include <SocketTools.h>
#include <CasioPacker.h>

#include <stdio.h>

bool CasioProtocolTools::connectProtocol(Socket * socket, int maxRetry)
{
    unsigned char dataBuffer[1024];
    Buffer packetBuffer(dataBuffer,1024);
    Buffer emptyData(NULL,0);
    
    
    
    unsigned char gapFillingData[] = {0};
    Buffer gapFillingBuffer(gapFillingData,1);
    SocketTools::sendBufferOrTimeout(socket,&gapFillingBuffer,10); // HACK: The first packet always seems to be lost, allows for faster connection
        

    int trycount = 0;
    
    while(trycount < maxRetry)
    {
        CasioPacketInfo packetInfo;
        packetInfo.type = 5;
        packetInfo.subtype = 0;
        packetInfo.extendedData = &emptyData;
        
        if(CasioPacker::pack(packetInfo,&packetBuffer))
        {
            Log::error("Failed to pack the connect request !");
            return true;
        }

        Log::info("Sending connection packet ...");
        
        if(sendPacketAndAck(socket,&packetBuffer,CONNECT_SEND_TRYCOUNT))
        {
            Log::error("Failed to send and ACK connection packet !");
            continue;
        }
        
        Log::info("Protocol connected !");
        return false;
    }

    return true;
}
bool CasioProtocolTools::disconnectProtocol(Socket * socket, int maxRetry)
{
    Buffer emptyExtraBuffer(NULL,0);
    CasioPacketInfo info;
    info.type = 0x18;
    info.subtype = 0x0;
    info.extendedData = &emptyExtraBuffer;
    
    unsigned char dataBuffer[1024];
    Buffer packetBuffer(dataBuffer,1024);
    
    if(CasioPacker::pack(info,&packetBuffer))
    {
        Log::error("Failed to pack the disconnect signal!");
        return true;
    }
        
    if(SocketTools::sendBufferOrTimeout(socket,&packetBuffer,maxRetry))
    {
        Log::error("Failed to send the disconnect packet !");
        return true;
    }
    
    return false;
}
bool CasioProtocolTools::sendPacketAndGetAnswer(Socket * socket, Buffer * packet, int timeout, CasioPacketInfo * result)
{
    int trycount = 0;
    
    unsigned char answerPacketData[MAX_RECEIVED_PACKET_SIZE];
    Buffer answerPacketBuffer(answerPacketData,MAX_RECEIVED_PACKET_SIZE);

    unsigned char answerData[MAX_RECEIVED_PACKET_SIZE];
    Buffer answerBuffer(answerData,MAX_RECEIVED_PACKET_SIZE);

    while(trycount < timeout)
    {
        Log::debug("Sending message ...");
        
        if(SocketTools::sendBufferOrTimeout(socket,packet,GENERAL_SEND_TRYCOUNT))
        {
            return true;
        }
            
        Log::debug("Sending waiting for ack message ...");
            
        SocketTools::receiveBufferOrTimeout(socket,&answerPacketBuffer,GENERAL_RECEIVE_TRYCOUNT,6);
        if(answerPacketBuffer.getSize())
        {
            Log::debug("Reading ack message ...");
                    
            CasioPacketInfo info;
            info.extendedData = &answerBuffer;
            
            if(CasioPacker::unpack(&info,&answerPacketBuffer))
            {
                return true;
            }
            
            if(info.type == 0x15 && info.subtype == 0x01) // RE-SEND REQUEST
            {
                continue;
            }
            else
            {
                *result = info;
                return false;
            }
        }
        trycount++;
    }
    
    return true;
}

bool CasioProtocolTools::sendPacketAndAck(Socket * socket, Buffer * packet, int timeout)
{
    CasioPacketInfo info;
    
    if(sendPacketAndGetAnswer(socket,packet,timeout,&info))
    {
        Log::error("Failed to send or get an answer from the calc !");
        return true;
    }
    
    if(info.type == 0x06 && info.subtype == 0x00)
    {
        return false;
    }
    else
    {
        Log::error("Unexpected answer while reading ACK !");
        return true;
    }
}
bool CasioProtocolTools::sendAck(Socket * socket, int timeout)
{
    unsigned char ackData[6];
    Buffer ackPacket(ackData,6);
    
    Buffer emptyExtraData(NULL,0);
    
    CasioPacketInfo info;
    info.type = 0x06;
    info.subtype = 0x0;
    info.extendedData = &emptyExtraData;
    
    CasioPacker::pack(info,&ackPacket);

    if(SocketTools::sendBufferOrTimeout(socket,&ackPacket,timeout))
    {
        return true;
    }
    
    return false;
}
