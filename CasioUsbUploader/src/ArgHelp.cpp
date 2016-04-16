#include "ArgHelp.h"

#include <stdio.h>

void ArgHelp::showHelp()
{
    printf("Available arguments: \n");
    printf("-f --filename     file to upload [REQUIRED]\n");
    printf("-F --foldername   which folder to use, [default: device's root]\n");
    printf("-d --devicename   which device to use, internal flash: fls0 [by default], external sd: crd0\n");
    printf("-w --overwrite    allow overwrite, [disabled by default]\n");
    printf("-l --loglevel     log level, 0 to 3 [default: 2]\n");
    printf("-q --quiet        quiet [disabled by default]\n");
    printf("-h --help         show this help\n");
}
