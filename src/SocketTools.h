#ifndef _SOCKETTOOLS_H_
#define _SOCKETTOOLS_H_

#include <Socket.h>

#define MAX_RECEIVED_PACKET_SIZE 4096

class SocketTools
{
public:
    static bool sendBufferOrTimeout(Socket * socket, Buffer * buffer, int maxRetry);
    static bool receiveBufferOrTimeout(Socket * socket, Buffer * buffer, int maxRetry, int minimum);

};

#endif // _SOCKETTOOLS_H_
