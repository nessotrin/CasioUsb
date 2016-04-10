#include "CasioChecksum.h"

#include <stdio.h>

unsigned char CasioChecksum::compute(Buffer * bufferToCompute)
{
    unsigned char value = 0;
    
    for(int i = 1 ; i < bufferToCompute->getSize() ; i++) //Warning: start at 1 !
    {
        value += bufferToCompute->getData()[i];
    }
    
    return (~value)+1;
}
