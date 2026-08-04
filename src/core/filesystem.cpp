#include "filesystem.h"


bool FileSystem::init() {
    started = LittleFS.begin(false);
    return started;
}

bool FileSystem::initRom(const char* path, const char* savePath) {
    if(started) {
        this->path = path;
        this->savePath = savePath;
        romFile = LittleFS.open(path, FILE_READ);
        romOpen = romFile;
    }

    return started && romOpen;
}

int FileSystem::listFolder(FileDescriptor* buffer, int limit, int offset) {
    File dir = LittleFS.open("/");
    File file = dir.openNextFile();

    for(int i=(0-offset); i < limit; i++) {
        if(!file) {
            dir.close();
            return i;
        }

        if(i >= 0) {
            strncpy(buffer[i].name, file.name(), sizeof(buffer[i].name) - 1);
            buffer[i].name[sizeof(buffer[i].name) - 1] = '\0';
            buffer[i].ext = DATA;
            if(strstr(file.name(), ".sav") != nullptr) {
                buffer[i].ext = SAVE;
            } else if(strstr(file.name(), ".gb") != nullptr || strstr(file.name(), ".gbc") != nullptr) {
                buffer[i].ext = ROM;
            }
        }
        file.close();
        file = dir.openNextFile();
    }

    dir.close();
    return limit;
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

time_t FileSystem::readRTC() {
    fs::File file = LittleFS.open("/rtc.data", FILE_READ);
    if (file) {
        time_t rtcRegister;
        file.read((uint8_t*) &rtcRegister, sizeof(rtcRegister));
        file.close();
        return rtcRegister;
    }
    return 1782570000u;
}

void FileSystem::writeRTC(time_t val) {
    fs::File file = LittleFS.open("/rtc.data", FILE_WRITE);
    if (file) {
        file.write((uint8_t*) &val, sizeof(val));
        file.close();
    }
}