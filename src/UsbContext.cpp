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
        Log::error("Can't init libusb !");
        Log::info("Check your system, CasioUsb probably ins't responsible for this");
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