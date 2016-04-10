#ifndef _CASIOCHECKSUM_H_
#define _CASIOCHECKSUM_H_

#include <Buffer.h>

class CasioChecksum
{
public:
    static unsigned char compute(Buffer * bufferToCompute);

};

#endif // _CASIOCHECKSUM_H_
