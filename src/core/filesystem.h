#pragma once

#include <LittleFS.h>


class FileSystem {
    private:
        bool started = false;
        bool romOpen = false;
        const char* path;
        const char* savePath;
        fs::File romFile;

    public:
        bool init(const char* path, const char* savePath);
        bool readRom(uint32_t offset, size_t bufferSize, uint8_t* buffer);
        bool readSave(size_t bufferSize, uint8_t* buffer);
        bool writeSave(size_t bufferSize, uint8_t* buffer);

    
};