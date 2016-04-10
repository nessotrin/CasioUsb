#include "Socket.h"

#include <stdio.h>

#include <Log.h>

Socket::Socket()
{
    isConnected = 0;
}

Socket::~Socket()
{
    if(isConnected)
    {
        libusb_close(deviceHandle);
    }
}

bool Socket::setDeviceHandle(libusb_device_handle* newDeviceHandle)
{
    if(newDeviceHandle == NULL)
    {
        return true;
    }
    deviceHandle = newDeviceHandle;
    isConnected = 1;
    return false;
}

SOCKET_RESULT Socket::sendBuffer(Buffer * bufferToSend)
{
    if(bulkTransfereBuffer(bufferToSend, ENDPOINT_OUT))
    {
        Log::error("receiveBuffer: Error sending bulk");
        bufferToSend->setSize(0);
        return SOCKET_ERROR;
    }
    
    return SOCKET_OK;
}

SOCKET_RESULT Socket::receiveBuffer(Buffer * bufferToFill)
{
    if(bulkTransfereBuffer(bufferToFill, ENDPOINT_IN))
    {
        Log::error("receiveBuffer: Error receiving bulk");
        bufferToFill->setSize(0);
        return SOCKET_ERROR;
    }
    
    return SOCKET_OK;
}

bool Socket::bulkTransfereBuffer(Buffer * bufferToUse, unsigned char endpoint)
{
    if(!isConnected)
    {
        Log::error("bulkTransfere: Not connected !");
        return SOCKET_ERROR;
    }

    int processed;
    int result = libusb_bulk_transfer(deviceHandle,
                                               endpoint,
                                               bufferToUse->getData(),
                                               bufferToUse->getSize(),
                                               &processed, SOCKET_TIMEOUT);
    bufferToUse->setSize(processed);
    
    if(result != 0)
    {
        Log::error("bulkTransfereBuffer: Error in bulk transfere");
        return true;
    }
    
    return false;
}