#ifndef _ARGREADER_H_
#define _ARGREADER_H_

#include <string.h>

class ArgReader
{
private:
    const char * fileName;
    const char * folderName;
    const char * deviceName;
    bool allowOverwrite;

    bool isArgPresent(char * argToCheck, char * shortArg, char * longArg);
    void errorMissingArg(char * argMissused);
    
public:
    ArgReader();
    bool readArg(int argc, char **argv);
    const char * getFileName();
    const char * getFolderName();
    const char * getDeviceName();
    bool getAllowOverwrite();

};

#endif // _ARGREADER_H_
