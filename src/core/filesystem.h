#pragma once

#include <LittleFS.h>
#include <sys/time.h>
#include <cstring>


enum Extension {
    DATA,
    SAVE,
    ROM
};

struct FileDescriptor {
    char name[30];
    Extension ext;
};

class FileSystem {
    private:
        bool started = false;
        bool romOpen = false;
        const char* path;
        const char* savePath;
        fs::File romFile;
        uint8_t* psRamRomCache;

    public:
        bool init();
        bool initRom(const char* path, const char* savePath);
        bool readRom(uint32_t offset, size_t bufferSize, uint8_t* buffer);
        bool readSave(size_t bufferSize, uint8_t* buffer);
        bool writeSave(size_t bufferSize, uint8_t* buffer);
        time_t readRTC();
        void writeRTC(time_t val);
        int listFolder(FileDescriptor* buffer, int limit, int offset);
};