#include "CasioProtocolTools.h"

#include <Log.h>
#include <SocketTools.h>
#include <CasioPacker.h>

#include <stdio.h>
#include <stdlib.h>

bool CasioProtocolTools::connectProtocol(Socket * socket, int maxRetry)
{
    Buffer packet(NULL,0);
    Buffer emptyData(NULL,0);
    
    
    
    unsigned char gapFillingData[] = {0};
    Buffer gapFillingBuffer(gapFillingData,1);
    SocketTools::sendBufferOrTimeout(socket,&gapFillingBuffer,10); // HACK: The first packet always seems to be lost, skips the timeout
        

    int trycount = 0;
    
    CasioPacketInfo packetInfo;
    packetInfo.type = 5;
    packetInfo.subtype = 0;
    packetInfo.extendedData = &emptyData;
    
    if(CasioPacker::pack(packetInfo,&packet))
    {
        Log::error("Failed to pack the connect request !");
        return true;
    }
    
    while(trycount < maxRetry)
    {
        
        Log::info("Sending connection packet ...");
        
        if(sendPacketAndAck(socket,&packet,CONNECT_SEND_TRYCOUNT))
        {
            Log::error("Failed to send and ACK connection packet !");
            continue;
        }
        else
        {
            Log::info("Protocol connected !");
            free(packet.getData());
            return false;
        }
        
        trycount++;
    }

    free(packet.getData());
    return true;
}
bool CasioProtocolTools::disconnectProtocol(Socket * socket, int maxRetry)
{
    Buffer emptyExtraBuffer(NULL,0);
    CasioPacketInfo info;
    info.type = 0x18;
    info.subtype = 0x0;
    info.extendedData = &emptyExtraBuffer;
    
    Buffer packet(NULL,0);
    
    if(CasioPacker::pack(info,&packet))
    {
        Log::error("Failed to pack the disconnect signal!");
        return true;
    }
        
    if(SocketTools::sendBufferOrTimeout(socket,&packet,maxRetry))
    {
        Log::error("Failed to send the disconnect packet !");
        free(packet.getData());
        return true;
    }
    
    free(packet.getData());
    return false;
}
bool CasioProtocolTools::sendPacketAndGetAnswer(Socket * socket, Buffer * packet, int maxRetry, CasioPacketInfo * result)
{
    int trycount = 0;
    
    unsigned char answerPacketData[MAX_RECEIVED_PACKET_SIZE];
    Buffer answerPacketBuffer(answerPacketData,MAX_RECEIVED_PACKET_SIZE);

    unsigned char answerData[MAX_RECEIVED_PACKET_SIZE];
    Buffer answerBuffer(answerData,MAX_RECEIVED_PACKET_SIZE);

    while(trycount < maxRetry)
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
        else
        {
            Log::debug("Receive timeout, re-sending ...");
        }
        trycount++;
    }
    
    return true;
}

bool CasioProtocolTools::sendPacketAndAck(Socket * socket, Buffer * packet, int maxRetry)
{
    CasioPacketInfo info;
    
    if(sendPacketAndGetAnswer(socket,packet,maxRetry,&info))
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
bool CasioProtocolTools::sendAck(Socket * socket, int maxRetry)
{
    unsigned char ackData[6];
    Buffer ackPacket(ackData,6);
    
    Buffer emptyExtraData(NULL,0);
    
    CasioPacketInfo info;
    info.type = 0x06;
    info.subtype = 0x0;
    info.extendedData = &emptyExtraData;
    
    CasioPacker::pack(info,&ackPacket);

    if(SocketTools::sendBufferOrTimeout(socket,&ackPacket,maxRetry))
    {
        return true;
    }
    
    return false;
}
