#include "SocketTools.h"

bool SocketTools::sendBufferOrTimeout(Socket * socket, Buffer * buffer, int timeout)
{
    int tryCount = 0;
    int writeSize = 0;
    while(writeSize < buffer->getSize())
    {
        Buffer appendBuffer(buffer->getData()+writeSize,buffer->getSize()-writeSize);
        socket->sendBuffer(&appendBuffer);
        writeSize += appendBuffer.getSize();
        tryCount++;
        if(tryCount >= timeout)
        {
            return true;
        }
    }
    return false;
}

bool SocketTools::receiveBufferOrTimeout(Socket * socket, Buffer * buffer, int timeout, int minimum)
{
    int readSize = 0;
    int tryCount = 0;
    while(tryCount < timeout)
    {
        Buffer appendBuffer(buffer->getData()+readSize,buffer->getSize()-readSize);
        socket->receiveBuffer(&appendBuffer);
        
        readSize += appendBuffer.getSize();
        
        if(readSize >= minimum)
        {
            break;
        }
        
        tryCount++;
    }
    
    buffer->setSize(readSize);
    
    if(readSize < minimum)
    {
        return true;
    }
 
    return false;
}