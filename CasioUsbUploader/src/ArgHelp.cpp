#include "ArgHelp.h"

#include <stdio.h>

void ArgHelp::showHelp()
{
    printf("Available arguments: \n");
    printf("-f FILENAME (REQUIRED)\n");
    printf("-F FOLDERNAME (default to root)\n");
    printf("-d DEVICENAME (internal flash: fls0 DEFAULT, external sd: crd0)\n");
    printf("-w (allow overwrite, disabled by default)\n");
    printf("-l (log level, 0 to 3, default to 2)\n");
    printf("-q (quiet, disabled by default)\n");
}
