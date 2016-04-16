#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <libusb-1.0/libusb.h>

#include <UsbContext.h>

#include <Log.h>

#include <Buffer.h>

#define CASIOUSB_SOCKET_TIMEOUT 2000

#define CASIOUSB_ENDPOINT_OUT (0x01)
#define CASIOUSB_ENDPOINT_IN (0x82)


enum CASIOUSB_SOCKET_RESULT
{
    CASIOUSB_SOCKET_OK = 0,
    CASIOUSB_SOCKET_ERROR,
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
    CASIOUSB_SOCKET_RESULT sendBuffer(Buffer * bufferToSend);
    CASIOUSB_SOCKET_RESULT receiveBuffer(Buffer * bufferToFill);
    bool bulkTransfereBuffer(Buffer * bufferToUse, unsigned char endpoint);
};

#endif // _SOCKET_H_
