#include "ArgReader.h"

#include <string.h>
#include <Log.h>
#include <ArgHelp.h>
#include <stdlib.h>

ArgReader::ArgReader()
{
    fileName = NULL;
    folderName = NULL;
    deviceName = NULL;
    allowOverwrite = false;
}

#include <stdio.h>

bool ArgReader::readArg(int argc, char** argv)
{
    for(int i = 1 ; i < argc ; i++)
    {
        if(strcmp(argv[i], "-f") == 0)
        {
            if(i+1 >= argc)
            {
                printf("%d %d\n",i,argc);
                Log::error("-f without argument!");
                ArgHelp::showHelp();
                return true;
            }
            fileName = argv[i+1];
            if(strlen(fileName) > 12)
            {
                Log::warning("File name might be too long (12 max) !");
            }
        }
        if(strcmp(argv[i], "-F") == 0)
        {
            if(i+1 >= argc)
            {
                Log::error("-F without argument!");
                ArgHelp::showHelp();
                return true;
            }
            folderName = argv[i+1];
            if(strlen(folderName) > 8)
            {
                Log::warning("Folder name might be too long (8 max) !");
            }
        }
        if(strcmp(argv[i], "-d") == 0)
        {
            if(i+1 >= argc)
            {
                Log::error("-d without argument!");
                ArgHelp::showHelp();
                return true;
            }
            deviceName = argv[i+1];
            if(strlen(folderName) > 4)
            {
                Log::warning("Device name might be too long (4 max) !");
            }
        }
        if(strcmp(argv[i], "-l") == 0)
        {
            if(i+1 >= argc)
            {
                Log::error("-l without argument!");
                ArgHelp::showHelp();
                return true;
            }
            int newLoglevel = (int)strtol(argv[i+1], (char **)NULL, 10);
            if(newLoglevel < 0 || newLoglevel > 3)
            {
                
                Log::error("-l out of range!");
                ArgHelp::showHelp();
                return true;
            }
            Log::setLoglevel(newLoglevel);
        }
        if(strcmp(argv[i], "-q") == 0)
        {
            Log::setLoglevel(-1); //Quiet
        }
        if(strcmp(argv[i], "-w") == 0)
        {
            allowOverwrite = true;
        }
    }
    
    
    if(fileName == NULL)
    {
        ArgHelp::showHelp();
        return true;
    }

    if(folderName == NULL) // default to root
    {
        folderName = "";
    }
    
    if(deviceName == NULL) // default to internal
    {
        deviceName = "fls0";
    }
    
    return false;
}

const char * ArgReader::getFolderName()
{
    return folderName;
}
const char * ArgReader::getFileName()
{
    return fileName;
}
const char * ArgReader::getDeviceName()
{
    return deviceName;
}
bool ArgReader::getAllowOverwrite()
{
    return allowOverwrite;
}