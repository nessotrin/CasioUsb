#ifndef _CALCCONNECTOR_H_
#define _CALCCONNECTOR_H_

#include <libusb-1.0/libusb.h>

#include <UsbContext.h>

class CalcConnector
{
public:
    static libusb_device_handle * connect(UsbContext * usbContext);
    
    static libusb_device_handle * openDevice(UsbContext * usbContext, libusb_device * device);
    static libusb_device * findCalcDevice(UsbContext * usbContext, unsigned short idVendor, unsigned short idProduct);
};

#endif // _CALCCONNECTOR_H_
