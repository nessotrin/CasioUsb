#include "CalcConnector.h"

#include <Log.h>

libusb_device * CalcConnector::findCalcDevice(UsbContext * usbContext, unsigned short idVendor, unsigned short idProduct)
{

    Log::debug("Reading USB device list ...");
    
    libusb_device **deviceList = NULL;
    int deviceCount = libusb_get_device_list(usbContext->getlibusbContext(), &deviceList);
    
    if(deviceCount < 0)
    {
        Log::error("Can't get the list of USB devices !");
        return NULL;
    }

    libusb_device * device = NULL;

    for (size_t idx = 0; idx < deviceCount; ++idx)
    {
        Log::debug("Checking a USB peripheral ...");
        libusb_device *tmpDevice = deviceList[idx];
        libusb_device_descriptor deviceDescriptor;

        if(libusb_get_device_descriptor(deviceList[idx], &deviceDescriptor) != 0)
        {
            Log::info("Buggy USB peripheral in list !");
        }

        if(deviceDescriptor.idVendor == idVendor && deviceDescriptor.idProduct == idProduct)
        {
            device = deviceList[idx];
            Log::debug("Found the calculator over USB !");
            break;
        }
    }

    libusb_free_device_list(deviceList, 1);

    if(device == NULL)
    {
        Log::error("Can't find the calculator's USB peripheral !");
        return NULL;
    }
    
    return device;
}

libusb_device_handle * CalcConnector::openDevice(UsbContext * usbContext, libusb_device * device)
{
    Log::info("Openning socket to calc");

    libusb_device_handle * deviceHandle = NULL;

    int result = libusb_open(device, &deviceHandle);

    if(result != 0)
    {
        if(result == LIBUSB_ERROR_ACCESS)
        {
            Log::error("Not enought permission to open the calculator, run as root or add a rule to udev !");
            return NULL;
        }
        else if(result == LIBUSB_ERROR_NO_DEVICE)
        {
            Log::error("Device seems to have disconnected !");
            Log::info("This is a known bug on windows, waiting libusb to be fixed ...");
        }
        else
        {
            Log::error("Unknown error while opening the socket to the calculator !");
            return NULL;
        }
    }

    return deviceHandle;
}

libusb_device_handle * CalcConnector::connect(UsbContext * usbContext)
{
    Log::info("Openning socket to calc...");

    libusb_device * device = findCalcDevice(usbContext,0x07CF,0x6101); // CASIO'S USB ID -> vendor:0x07CF product:0x6101

    if(device  == NULL) 
    {
        Log::error("Failed to find the calc !");
        Log::info("Check that it's in receive mode ?");
        return NULL;
    }
    
    libusb_device_handle * deviceHandle = openDevice(usbContext, device);
    
    if(deviceHandle == NULL)
    {
        Log::error("Calc is detected but impossible to connect !");
        return NULL;
    }

    Log::info("Socket connected !");

    return deviceHandle;
}