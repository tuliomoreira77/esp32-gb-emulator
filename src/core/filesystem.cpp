#include "filesystem.h"

bool FileSystem::init(const char* path) {
    bool started = LittleFS.begin(false);
    if(started) {
        romFile = LittleFS.open(path, FILE_READ);
        romOpen = romFile;
    }

    return started && romOpen;
}

bool FileSystem::readRom(uint32_t offset, size_t bufferSize, uint8_t* buffer) {
    romFile.seek(offset, SeekMode::SeekSet);
    romFile.read(buffer, bufferSize);
    return true;
}