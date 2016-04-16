#include <stdio.h>

#include <Socket.h>
#include <CalcConnector.h>
#include <Log.h>
#include <CasioUsbVersion.h>

#define VERSION_MAJOR 0
#define VERSION_MINOR 0
#define VERSION_RELEASE 1
#define VERSION_DATE_STRING "16 april 2016"


#define BUFFER_STEP_SIZE 64

int main(int argc, char **argv)
{
	printf("Casio Usb Logger [%s] V%d.%d.%d by Nessotrin\n",VERSION_DATE_STRING,VERSION_MAJOR,VERSION_MINOR,VERSION_RELEASE);
    CasioUsbVersion::printVersion();
    
    Log::setLoglevel(CASIOUSB_LOGLEVEL_DEBUG);

    UsbContext context;
    if(context.initUsbContext())
    {
        Log::error("Can't init the USB context !");
        return 1;
    }
    Socket calcSocket;
    if(calcSocket.setDeviceHandle(CalcConnector::connect(&context)))
    {
        Log::error("Can't connect to the calc !");
        return 1;
    }
    
    unsigned char bufferData[BUFFER_STEP_SIZE];
    Buffer buffer(bufferData,BUFFER_STEP_SIZE);
    
    while(1)
    {
        buffer.setSize(BUFFER_STEP_SIZE);
        if(calcSocket.receiveBuffer(&buffer))
        {
            Log::error("Failed to receive the log !");
            return 1;
        }
        for(int i = 0 ; i < buffer.getSize() ; i++) // Can't use %s 'cause we don't know if the message is complete
        {
            printf("%c",buffer.getData()[i]);
        }
    }
    
	return 0;
}
