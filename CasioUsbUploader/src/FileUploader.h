#ifndef FILEUPLOADER_H
#define FILEUPLOADER_H

#include <Socket.h>

class FileUploader
{
public:
    static bool uploadFile(Socket * socket, const char * foldername, const char * filename, const char * devicename, unsigned char mode);
private:
    static bool sendFilePart(Buffer * data, Socket * socket, int partId, int partCount, int partSize);
    static bool sendFileRequest(const char * foldername, int dataSize, const char * filename, const char * devicename, unsigned char mode, Socket * socket);

};

#endif // FILEUPLOADER_H
