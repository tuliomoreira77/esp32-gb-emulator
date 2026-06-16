#include <Arduino.h>
#include <LittleFS.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "core/motherboard.h"
#include "core/screen.h"
#include "core/memory_map.h"

MemoryMap memMap;
Motherboard* motherboard = nullptr;
Joypad* joypad = nullptr;
Screen* screen = nullptr;
FileSystem* fileSystem = nullptr;


void setup() {
  //Need to break patterns to alloc memory first to prevent heap fragmentation 
  memMap.bank0 = (uint8_t*) heap_caps_malloc(0x4000, MALLOC_CAP_8BIT);
  memMap.bank1 = (uint8_t*) heap_caps_malloc(0x4000, MALLOC_CAP_8BIT);
  memMap.highMemory = (uint8_t*) heap_caps_malloc(0x8000, MALLOC_CAP_8BIT);
  memMap.extMem = (uint8_t*) heap_caps_malloc(0x8000, MALLOC_CAP_8BIT);
  memset(memMap.extMem, 0, 0x8000);

  memMap.bank2 = (uint8_t*) heap_caps_malloc(0x4000, MALLOC_CAP_8BIT);
  memMap.bank3 = (uint8_t*) heap_caps_malloc(0x4000, MALLOC_CAP_8BIT);
  memMap.bank4 = (uint8_t*) heap_caps_malloc(0x4000, MALLOC_CAP_8BIT);
  memMap.bank5 = (uint8_t*) heap_caps_malloc(0x4000, MALLOC_CAP_8BIT);
  memMap.bank6 = (uint8_t*) heap_caps_malloc(0x4000, MALLOC_CAP_8BIT);
  memMap.bank7 = (uint8_t*) heap_caps_malloc(0x4000, MALLOC_CAP_8BIT);
  memMap.bank8 = (uint8_t*) heap_caps_malloc(0x4000, MALLOC_CAP_8BIT);

  screen = new Screen();
  screen->init();

  joypad = new Joypad();
  joypad->init();

  fileSystem = new FileSystem();
  motherboard = new Motherboard(joypad, screen, fileSystem, &memMap);
  Serial.begin(115200);

  bool fileSystemOk = fileSystem->init("/pokemon_red.gb");

  if(!fileSystemOk) {
    while (true) { delay(1000); }
  }

  motherboard->insertCartridge();
  
}

void loop() {
  motherboard->runCycle();
  
  unsigned long iTime = millis();
  for(unsigned long count =0; count < 1000000;) {
    count = count + motherboard->runCycle();
  }
  Serial.println(millis() - iTime);
  
}
