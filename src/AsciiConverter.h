#ifndef _ASCIICONVERTER_H_
#define _ASCIICONVERTER_H_

class AsciiConverter
{
public:
    static void numberToAscii(unsigned int number, unsigned char * outputBuffer, int byteCount);
    static void asciiToNumber(unsigned char * inputBuffer, unsigned int * outputNumber, int byteCount);
};

#endif // _ASCIICONVERTER_H_
