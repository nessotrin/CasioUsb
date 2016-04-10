#ifndef _FILEPROTOCOL_H_
#define _FILEPROTOCOL_H_

#include <Buffer.h>

class FileProtocol
{
public:
    static bool buildSendRequestPacket(Buffer * output, int dataSize, const char * foldername, const char * filename, const char * devicename, unsigned char mode);
    static bool buildFilePartPacket(Buffer * output, Buffer * data, int partId, int partCount, int partSize);

};

#endif // _FILEPROTOCOL_H_
