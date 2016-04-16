#include "CasioUsbLoggerVersion.h"

#include <CasioUsbVersion.h>
#include <stdio.h>

void CasioUsbLoggerVersion::printVersion()
{
    CasioUsbVersion::printVersion();
    printf("Casio Usb Logger [%s] V%d.%d.%d by Nessotrin\n",CASIOUSBLOGGER_VERSION_DATE_STRING,CASIOUSBLOGGER_VERSION_MAJOR,CASIOUSBLOGGER_VERSION_MINOR,CASIOUSBLOGGER_VERSION_RELEASE);
}


