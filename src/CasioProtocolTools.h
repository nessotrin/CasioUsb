#ifndef _CASIOPROTOCOLTOOLS_H_
#define _CASIOPROTOCOLTOOLS_H_

#define CONNECT_SEND_TRYCOUNT 100
#define CONNECT_RECEIVE_TRYCOUNT 10

#define GENERAL_SEND_TRYCOUNT 10
#define GENERAL_RECEIVE_TRYCOUNT 10

#include <Socket.h>
#include <CasioPacker.h>


class CasioProtocolTools
{
public:
    static bool connectProtocol(Socket * socket, int maxRetry);
    static bool disconnectProtocol(Socket * socket, int maxRetry);
    
    static bool sendPacketAndGetAnswer(Socket * socket, Buffer * packet, int timeout, CasioPacketInfo * result);
    static bool sendPacketAndAck(Socket * socket, Buffer * packet, int timeout);
    static bool sendAck(Socket * socket, int timeout);

};

#endif // _CASIOPROTOCOLTOOLS_H_
