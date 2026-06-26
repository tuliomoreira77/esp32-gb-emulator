#include "filesystem.h"

bool FileSystem::init(const char* path, const char* savePath) {
    started = LittleFS.begin(false);
    if(started) {
        this->path = path;
        this->savePath = savePath;
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

bool FileSystem::readSave(size_t bufferSize, uint8_t* buffer) {
    if(started) {
        fs::File file = LittleFS.open(savePath, FILE_READ);
        if(file) {
            file.read(buffer, bufferSize);
            file.close();
            return true;
        }
        return false;
    }
    return false;
}

bool FileSystem::writeSave(size_t bufferSize, uint8_t* buffer) {
    if(started) {
        fs::File file = LittleFS.open(savePath, FILE_WRITE);
        file.write(buffer, bufferSize);
        file.close();
        return true;
    }
    return false;
}