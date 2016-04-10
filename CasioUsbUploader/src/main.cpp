#include <stdio.h>
#include <string.h>

#include <CalcConnector.h>
#include <Log.h>
#include <Buffer.h>
#include <CasioUsbVersion.h>
#include <CasioProtocolTools.h>
#include <CasioPacker.h>

#include <FileUploader.h>
#include <FileProtocol.h>
#include <FileLoader.h>
#include <ArgReader.h>


#define VERSION_MAJOR 0
#define VERSION_MINOR 0
#define VERSION_RELEASE 1
#define VERSION_DATE_STRING "09 april 2016"


int main(int argc, char **argv)
{
	printf("Casio Usb Uploader [%s] V%d.%d.%d by Nessotrin\n",VERSION_DATE_STRING,VERSION_MAJOR,VERSION_MINOR,VERSION_RELEASE);
    CasioUsbVersion::printVersion();

    Log::setLoglevel(LOGLEVEL_INFO);

    ArgReader argReader;
    if(argReader.readArg(argc,argv))
    {
        return 1;
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
    
    if(CasioProtocolTools::connectProtocol(&calcSocket, 5))
    {
        Log::error("Can't connect the protocol !");
        return 1;
    }
    
    if(FileUploader::uploadFile(&calcSocket
                               ,argReader.getFolderName()
                               ,argReader.getFileName()
                               ,argReader.getDeviceName()
                               ,argReader.getAllowOverwrite()?0x02:0x00))
    {
        Log::error("Failed to upload the file !");
    }
    
    if(CasioProtocolTools::disconnectProtocol(&calcSocket, 5))
    {
        Log::error("Can't disconnect the protocol !");
        return 1;
    }
        
	return 0;
}
