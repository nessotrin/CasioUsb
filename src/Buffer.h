#ifndef _BUFFER_H_
#define _BUFFER_H_

class Buffer
{
public:
    Buffer();
    Buffer(unsigned char * newData, int newSize);

    unsigned char * getData();
    void setData(unsigned char * newData);
    int getSize();
    void setSize(int newSize);
    
private:
    unsigned char * data;
    int size;
};

#endif // _BUFFER_H_
