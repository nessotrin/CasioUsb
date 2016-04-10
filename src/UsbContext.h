#ifndef _USBCONTEXT_H_
#define _USBCONTEXT_H_

#include <libusb-1.0/libusb.h>

#include <Log.h>

class UsbContext
{
public:
    UsbContext();
    ~UsbContext();
    
    bool initUsbContext();
    void deinitUsbContext();
    
    libusb_context * getlibusbContext();
    
private:
    Log logger;
    libusb_context * libusbContext;

};

#endif // _USBCONTEXT_H_
