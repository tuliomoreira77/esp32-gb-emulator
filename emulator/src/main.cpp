#include <Arduino.h>
#include <LittleFS.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#include "core/motherboard.h"
#include "core/screen.h"

#define LED_PIN 2
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

/*
TODO -> Bus - Rom Banking, Memory Banking, Rom read
TODO -> OLED Display Driver
TODO -> Button Driver
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

void errorBlink() {
  for(int i=0; i < 40; i++) {
    digitalWrite(LED_PIN, HIGH); 
    delay(100);                 
    digitalWrite(LED_PIN, LOW); 
    delay(100);
  }
}

void successBlink() {
  for(int i=0; i < 5; i++) {
    digitalWrite(LED_PIN, HIGH); 
    delay(500);                 
    digitalWrite(LED_PIN, LOW); 
    delay(200);
  }
}

void drawFrameLoop(void*) {
  for(;;) {
    if(screen->canDraw) {
      screen->drawFrame();
      screen->canDraw = false;
    }
    vTaskDelay(pdMS_TO_TICKS(32));
  }
  
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  joypad = new Joypad();
  screen = new Screen();
  motherboard = new Motherboard(joypad, screen);
  
  screen->init();
  
  bool romRead = readRom("/tetris.gb");
  if (!romRead) {
    errorBlink();
    return;
  }

  motherboard->insertCartridge(rom);

  xTaskCreatePinnedToCore(drawFrameLoop, "display", 8192, nullptr, 3, nullptr, 0);

  successBlink();
}

void loop() {
  while(1) {
    unsigned long iTime = millis();
    for(unsigned long count =0; count < 1000000; count++) {
      motherboard->runCycle();
    }
    Serial.println(millis() - iTime);
  }
}
