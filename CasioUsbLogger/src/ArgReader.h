#ifndef _ARGREADER_H_
#define _ARGREADER_H_

#include <string.h>

class ArgReader
{
private:
    bool quiet;

    bool isArgPresent(char * argToCheck, char * shortArg, char * longArg);
    void errorMissingArg(char * argMissused);
    
public:
    ArgReader();
    bool readArg(int argc, char **argv);
    bool isQuiet();

};

#endif // _ARGREADER_H_
