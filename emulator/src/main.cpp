#include <Arduino.h>
#include <LittleFS.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "core/motherboard.h"
#include "core/screen.h"

/*
TODO -> Bus - Rom Banking, Memory Banking, Rom read
*/

static uint8_t rom[0x8000];
Motherboard* motherboard = nullptr;
Joypad* joypad = nullptr;
Screen* screen = nullptr;


bool readFileChunked(const char* path, uint8_t* destination, size_t destinationSize, size_t chunkSize) {
    size_t outBytesRead = 0;

    File file = LittleFS.open(path, FILE_READ);
    if (!file) {
        return false;
    }

    size_t fileSize = file.size();
    if (fileSize > destinationSize) {
        file.close();
        return false;
    }

    while (file.available()) {
        size_t remaining = fileSize - outBytesRead;
        size_t toRead = min(chunkSize, remaining);

        size_t bytesRead = file.read(destination + outBytesRead, toRead);
        if (bytesRead == 0) {
            file.close();
            return false;
        }

        outBytesRead += bytesRead;
    }

    file.close();
    return true;
}

bool readRom(const char* path) {
  if (!LittleFS.begin(true)) {
      return false;
  }

  bool romRead = readFileChunked(path, rom, 0x8000, 2048);

  if (!romRead) {
    return false;
  }

  return true;
}

void setup() {
  //Serial.begin(115200);

  joypad = new Joypad();
  screen = new Screen();
  motherboard = new Motherboard(joypad, screen);
  
  screen->init();
  joypad->init();
  
  bool romRead = readRom("/tetris.gb");
  if (!romRead) {
    return;
  }

  motherboard->insertCartridge(rom);
}

void loop() {
  motherboard->runCycle();
  /*
  unsigned long iTime = millis();
  for(unsigned long count =0; count < 1000000; count++) {
    motherboard->runCycle();
  }
  Serial.println(millis() - iTime);
  */
}
