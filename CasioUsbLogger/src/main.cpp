#include <stdio.h>

#include <Socket.h>
#include <CalcConnector.h>
#include <Log.h>
#include <CasioUsbLoggerVersion.h>
#include <ArgReader.h>

#define BUFFER_STEP_SIZE 64

int main(int argc, char **argv)
{
    Log::setLoglevel(CASIOUSB_LOGLEVEL_INFO);

    ArgReader argReader;
    if(argReader.readArg(argc,argv))
    {
        return 1;
    }
    
    if(!argReader.isQuiet())
    {
        CasioUsbLoggerVersion::printVersion();
    }

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
