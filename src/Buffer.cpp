#include "Buffer.h"

Buffer::Buffer()
{
    
}

Buffer::Buffer(unsigned char * newData, int newSize)
{
    data = newData;
    size = newSize;
}


unsigned char * Buffer::getData()
{
    return data;
}

void Buffer::setData(unsigned char * newData)
{
    data = newData;
}

int Buffer::getSize()
{
    return size;
}

void Buffer::setSize(int newSize)
{
    size = newSize;
}