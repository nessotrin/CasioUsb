#include "AsciiConverter.h"

#include <stdio.h>

void AsciiConverter::numberToAscii(unsigned int number, unsigned char * outputBuffer, int byteCount)
{
    for(int i = 0 ; i < byteCount ; i++)
    {
        unsigned char value = (number >> (4*i)) & 0xf;
        if(value < 10)
        {
            value += ((unsigned char) 0x30);
        }
        else
        {
            value += (unsigned char) 0x37; //0x41 -> letters      -10 -> only part over 9
        }
        outputBuffer[byteCount-i-1] = value;
    }
}
void AsciiConverter::asciiToNumber(unsigned char * inputBuffer, unsigned int * outputNumber, int byteCount)
{
    *outputNumber = 0;
    for(int i = 0 ; i < byteCount ; i++)
    {
        unsigned char value = inputBuffer[byteCount-i-1];
        if(value <= 0x39)
        {
            value -= (unsigned char) 0x30;
        }
        else
        {
            value -= (unsigned char) 0x37; //0x41 -> letters      -10 -> only part over 9
        }
        *outputNumber |= value << (4*i);
    }
}

