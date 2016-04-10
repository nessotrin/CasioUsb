#include "CasioEscaper.h"

bool CasioEscaper::escapeBuffer(Buffer * input, Buffer * output)
{
    int writePos = 0;
    int readPos = 0;
    while(readPos < input->getSize())
    {
        if(writePos >= output->getSize())
        {
            return true;
        }
        if(input->getData()[readPos] < 0x1F || input->getData()[readPos] == 0x5C)
        {
            output->getData()[writePos] = 0x5C;
            writePos++;
            if(writePos >= output->getSize())
            {
                return true;
            }
            output->getData()[writePos] = input->getData()[readPos]+0x20;
        }
        else
        {
            output->getData()[writePos] = input->getData()[readPos];
        }
        writePos++;
        readPos++;
    }
    
    output->setSize(writePos);
    
    return false;
}
bool CasioEscaper::restoreBuffer(Buffer * input, Buffer * output)
{
    int writePos = 0;
    int readPos = 0;
    while(readPos < input->getSize())
    {
        if(input->getData()[readPos] == 0x5C)
        {
            readPos++;
        }
        if(writePos >= output->getSize())
        {
            return true;
        }
        output->getData()[writePos] = input->getData()[readPos];
        readPos++;
    }
    
    output->setSize(writePos);
    
    return false;
}
