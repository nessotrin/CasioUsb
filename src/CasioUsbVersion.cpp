#include "CasioUsbVersion.h"

#include <stdio.h>

void CasioUsbVersion::printVersion()
{
    printf("Lib Casio USB [%s] V%d.%d.%d by Nessotrin\n",CASIOUSB_VERSION_DATE_STRING,CASIOUSB_VERSION_MAJOR,CASIOUSB_VERSION_MINOR,CASIOUSB_VERSION_RELEASE);
}


