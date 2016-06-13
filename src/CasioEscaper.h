#ifndef _CASIOESCAPER_H_
#define _CASIOESCAPER_H_

#include <Buffer.h>

class CasioEscaper
{
public:
    static bool escapeBuffer(Buffer * input, Buffer * output, int outputOffset);
    static bool restoreBuffer(Buffer * input, Buffer * output);

};

#endif // _CASIOESCAPER_H_
