#include "AsciiConverter.h"

#include <stdio.h>

void AsciiConverter::numberToAscii(int number, unsigned char * outputBuffer, int byteCount)
{
    for(int i = 0 ; i < byteCount ; i++)
    {
        unsigned char value = (((unsigned int)number) >> (4*i)) & 0xf;
        if(value < 10)
        {
            value+= 0x30;
        }
        else
        {
            value+= 0x41 -10;
        }
        outputBuffer[byteCount-i-1] = value;
    }
}
void AsciiConverter::asciiToNumber(unsigned char * inputBuffer, int * outputNumber, int byteCount)
{
    *outputNumber = 0;
    for(int i = 0 ; i < byteCount ; i++)
    {
        unsigned char value = inputBuffer[byteCount-i-1];
        if(value <= 0x39)
        {
            value -= 0x30;
        }
        else
        {
            value -= (0x41-10);
        }
        *outputNumber |= value << (4*i);
    }
}

