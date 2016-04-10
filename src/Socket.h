#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <libusb-1.0/libusb.h>

#include <UsbContext.h>

#include <Log.h>

#include <Buffer.h>

#define SOCKET_TIMEOUT 2000

#define ENDPOINT_OUT (0x01)
#define ENDPOINT_IN (0x82)


enum SOCKET_RESULT
{
    SOCKET_OK = 0,
    SOCKET_ERROR,
};

class Socket
{
private:
    libusb_device_handle * deviceHandle;
    bool isConnected;

public:
    Socket();
    ~Socket();


    bool setDeviceHandle(libusb_device_handle * newDeviceHandle);
    SOCKET_RESULT sendBuffer(Buffer * bufferToSend);
    SOCKET_RESULT receiveBuffer(Buffer * bufferToFill);
    bool bulkTransfereBuffer(Buffer * bufferToUse, unsigned char endpoint);
};

#endif // _SOCKET_H_
