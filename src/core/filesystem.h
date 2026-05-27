#pragma once

#include <LittleFS.h>


class FileSystem {
    private:
        bool romOpen = false;
        fs::File romFile;

    public:
        bool init(const char* path);
        bool readRom(uint32_t offset, size_t bufferSize, uint8_t* buffer);
    
};