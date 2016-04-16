#include "ArgHelp.h"

#include <stdio.h>

#include <CasioUsbLoggerVersion.h>

void ArgHelp::showHelp()
{
    CasioUsbLoggerVersion::printVersion();
    printf("Available arguments: \n");
    printf("-l --loglevel     log level, 0 to 3 [default: 2]\n");
    printf("-q --quiet        quiet [disabled by default]\n");
    printf("-h --help         show this help\n");
}
