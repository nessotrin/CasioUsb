#ifndef _LOG_H___
#define _LOG_H___

enum CASIOUSB_LOGLEVEL
{
    CASIOUSB_LOGLEVEL_ERROR = 0,
    CASIOUSB_LOGLEVEL_WARNING,
    CASIOUSB_LOGLEVEL_INFO,
    CASIOUSB_LOGLEVEL_DEBUG = 3,
};

class Log
{
private:
    static int loglevel;

public:

    Log();

    static void setLoglevel(int newCASIOUSB_LOGLEVEL);
    static int getLoglevel();
    
    static void debug(const char * messageToLog);
    static void info(const char * messageToLog);
    static void warning(const char * messageToLog);
    static void error(const char * messageToLog);

};

#endif // _LOG_H___
