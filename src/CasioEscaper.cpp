#include "CasioEscaper.h"

#include <stdlib.h>

#include <Log.h>

#define CASIO_ESCAPER_STEP_ALLOC 128

/* Adds escaping caracters to an existing buffer in a existing or newly allocated one (through NULL), writing starts at outputOffset
 * 
 * 0x09 0x50 becomes 0x5C 0x09 0x50 
 *                     |
 *                  Escaper
 */ 
bool CasioEscaper::escapeBuffer(Buffer * input, Buffer * output, int outputOffset)
{
    if(input->getSize() == 0)
    {
        output->setSize(0+outputOffset);
        return false;
    }
    
    int writePos = outputOffset;
    int readPos = 0;
    
    while(readPos < input->getSize())
    {
        if(writePos-1 >= output->getSize()) // Allocating more memory (-1 to have at least 2 free bytes)
        {
            output->setData((unsigned char *)realloc(output->getData(),output->getSize()+CASIO_ESCAPER_STEP_ALLOC));
            if(output->getData() == NULL)
            {
                Log::error("Failed to realloc buffer !");
                return true;
            }
            output->setSize(output->getSize()+CASIO_ESCAPER_STEP_ALLOC);
        }
            
        if(input->getData()[readPos] < 0x1F || input->getData()[readPos] == 0x5C)
        {
            output->getData()[writePos] = 0x5C;
            writePos++;

            output->getData()[writePos] = input->getData()[readPos]+0x20; // one extra byte
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

/* Removes escaping caracters to an existing buffer in a existing or newly allocated one (through NULL)
 * 
 *  0x5C 0x09 0x50 becomes 0x5C 0x09 0x50 
 *   |
 * Escaper
 */ 
bool CasioEscaper::restoreBuffer(Buffer * input, Buffer * output)
{
    if(input->getSize() == 0)
    {
        output->setSize(0);
        return false;
    }
    
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
            output->setData((unsigned char *)realloc(output->getData(),output->getSize()+CASIO_ESCAPER_STEP_ALLOC));
            if(output->getData() == NULL)
            {
                Log::error("Failed to realloc buffer !");
                return true;
            }
            output->setSize(output->getSize()+CASIO_ESCAPER_STEP_ALLOC);
        }
        output->getData()[writePos] = input->getData()[readPos];
        readPos++;
    }
    
    output->setSize(writePos);
    
    return false;
}
