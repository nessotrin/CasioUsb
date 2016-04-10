#ifndef _LOG_H___
#define _LOG_H___

enum LOGLEVEL
{
    LOGLEVEL_ERROR = 0,
    LOGLEVEL_WARNING,
    LOGLEVEL_INFO,
    LOGLEVEL_DEBUG = 3,
};

class Log
{
private:
    static int loglevel;

public:

    Log();

    static void setLoglevel(int newLoglevel);
    static int getLoglevel();
    
    static void debug(const char * messageToLog);
    static void info(const char * messageToLog);
    static void warning(const char * messageToLog);
    static void error(const char * messageToLog);

};

#endif // _LOG_H___
