#include "FileLoader.h"

#include <stdio.h>
#include <stdlib.h>
#include <Log.h>


Buffer FileLoader::readFile(const char * filename)
{
    FILE * file;

    Log::debug("Openning the file ...");
    file = fopen(filename,"rb");
    if(file == NULL)
    {
        Log::error("Can't open the file !");
        Log::info("Check the name ?");
        printf("Filename: \"%s\"\n",filename);
        return Buffer(NULL,-1);
    }

    if(fseek(file, 0, SEEK_END))
    {
        Log::error("Can't read file: fseek failure");
        return Buffer(NULL,-1);
    }
    int fileSize = ftell(file);
    if(fseek(file, 0, SEEK_SET))
    {
        Log::error("Can't read file: fseek failure");
        return Buffer(NULL,-1);
    }

    if(fileSize == 0)
    {
        Log::warning("File is empty !");
        Log::info("Check the file ?");
    }

    unsigned char * data = (unsigned char *) malloc(fileSize);
    if(data == NULL)
    {
        Log::error("Can't alloc memory to read the file !");
        return Buffer(NULL,-1);
    }

    Log::debug("Reading the file ...");
    if(fread (data,1,fileSize,file) != fileSize)
    {
        Log::error("Missmatching file size after reading !");
        Log::info("Unknown error, is the file being modified ?");
        return Buffer(NULL,-1);
    }

    if(fclose(file))
    {
        Log::error("Failed to close the file !");
    }

    Log::info("File has been read !");
    
    return Buffer(data,fileSize);
}
