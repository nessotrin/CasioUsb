#ifndef _PACKET_H_
#define _PACKET_H_

#include <Buffer.h>

struct CasioPacketInfo
{
    unsigned char type;
    unsigned char subtype;
    Buffer * extendedData;
};

#define MINIMAL_PACKED_BUFFER_SIZE 6

class CasioPacker
{
public:
    static bool pack(CasioPacketInfo packetInfo, Buffer * output);
    static bool unpack(CasioPacketInfo * packetInfo, Buffer * input);

private:
    static int insertExtendedData(Buffer * data, Buffer * workingBuffer);
    static bool getExtendedData(Buffer * data, Buffer * input);
    static int getExtendedDataSize(Buffer * input);
    static bool checkCorruption(Buffer * input);
};

#endif // _PACKET_H_
