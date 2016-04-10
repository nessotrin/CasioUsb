#include "Log.h"
#include <stdio.h>

int Log::loglevel = 0;

Log::Log()
{

}

void Log::debug(const char * messageToLog)
{
    if(Log::loglevel >= LOGLEVEL_DEBUG)
    {
        printf("[DEBUG]: %s\n",messageToLog);
    }
}
void Log::info(const char * messageToLog)
{
    if(Log::loglevel >= LOGLEVEL_INFO)
    {
        printf("[INFO]: %s\n",messageToLog);
    }
}
void Log::warning(const char * messageToLog)
{
    if(Log::loglevel >= LOGLEVEL_WARNING)
    {
        printf("[WARNING]: %s\n",messageToLog);
    }
}
void Log::error(const char * messageToLog)
{
    if(Log::loglevel >= LOGLEVEL_ERROR)
    {
        printf("[ERROR]: %s\n",messageToLog);
    }
}

void Log::setLoglevel(int newLoglevel)
{
    Log::loglevel = newLoglevel;
}
int Log::getLoglevel()
{
    return Log::loglevel;
}