#include "ArgReader.h"

#include <string.h>
#include <Log.h>
#include <ArgHelp.h>
#include <stdlib.h>
#include <stdio.h>

ArgReader::ArgReader()
{
    quiet = false;
}

bool ArgReader::isArgPresent(char * argToCheck, char * shortArg, char * longArg)
{
    return strcmp(argToCheck, shortArg) == 0 || strcmp(argToCheck, longArg) == 0;
}

void ArgReader::errorMissingArg(char * argMissused)
{
    char * warningTextBase = " without following argument!";
    char * errorMessage = (char *) malloc(strlen(argMissused)+strlen(warningTextBase));
    if(errorMessage == NULL)
    {
        Log::error("warnMissingArg: failed to allocate memory !");
        exit(1);
    }
    strcpy(errorMessage,argMissused);
    strcpy(errorMessage+strlen(argMissused),warningTextBase);
    Log::error(errorMessage);
    free(errorMessage);

    ArgHelp::showHelp();
}

bool ArgReader::readArg(int argc, char** argv)
{
    for(int i = 1 ; i < argc ; i++)
    {
        
        if(isArgPresent(argv[i],(char*)"-l",(char*)"--loglevel"))
        {
            if(i+1 >= argc || argv[i+1][0] == '-')
            {
                errorMissingArg(argv[i]);
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
        
        if(isArgPresent(argv[i],(char*)"-q",(char*)"--quiet"))
        {
            quiet = true;
        }

        if(isArgPresent(argv[i],(char*)"-h",(char*)"--help"))
        {
            ArgHelp::showHelp();
            return true;
        }
    }
    
    if(quiet)
    {
        Log::setLoglevel(-1); //Quiet
    }
    
    return false;
}

bool ArgReader::isQuiet()
{
    return quiet;
}
