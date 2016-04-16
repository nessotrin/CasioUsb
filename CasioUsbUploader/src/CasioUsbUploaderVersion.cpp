#include "CasioUsbUploaderVersion.h"

#include <CasioUsbVersion.h>
#include <stdio.h>

void CasioUsbUploaderVersion::printVersion()
{
    CasioUsbVersion::printVersion();
    printf("Casio Usb Uploader [%s] V%d.%d.%d by Nessotrin\n",CASIOUSBUPLOADER_VERSION_DATE_STRING,CASIOUSBUPLOADER_VERSION_MAJOR,CASIOUSBUPLOADER_VERSION_MINOR,CASIOUSBUPLOADER_VERSION_RELEASE);
}


