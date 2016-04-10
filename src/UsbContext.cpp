#include "UsbContext.h"

UsbContext::UsbContext()
{
    libusbContext = NULL;
}

UsbContext::~UsbContext()
{
    
}

bool UsbContext::initUsbContext()
{
    if(libusb_init(&libusbContext) != 0)
    {
        Log::error("can't init libusb !\n");
        return true;
    }
    return false;
}

void UsbContext::deinitUsbContext()
{
    libusb_exit(libusbContext);
    libusbContext = NULL;
}
    
libusb_context * UsbContext::getlibusbContext()
{
    if(libusbContext == NULL)
    {
        Log::error("getlibusbContext: libusbContext => NULL !\n");
    }
    return libusbContext;
}