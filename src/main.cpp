#include <Arduino.h>
#include <LittleFS.h>
#include <sys/time.h>
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

void imprimirStatusMemoria() {
    size_t livreTotal = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    size_t maiorBloco = heap_caps_get_largest_free_block(MALLOC_CAP_8BIT);
    
    // Cálculo simples de fragmentação:
    // Quanto menor a porcentagem de "maiorBloco" em relação ao "livreTotal",
    // mais fragmentada está a memória.
    float fragmentacao = 0.0;
    if (livreTotal > 0) {
        fragmentacao = (1.0 - ((float)maiorBloco / (float)livreTotal)) * 100.0;
    }

    Serial.println("--- Status da Memória ESP32 ---");
    Serial.printf("Memória Livre Total: %u bytes\n", livreTotal);
    Serial.printf("Maior Bloco Contíguo: %u bytes\n", maiorBloco);
    Serial.printf("Fragmentação Estimada: %.2f%%\n", fragmentacao);
    Serial.println("-------------------------------");
}


void setup() {
  //Need to break patterns to alloc memory first to prevent heap fragmentation 
  memMap.extMem = (uint8_t*) heap_caps_malloc(0x8000, MALLOC_CAP_8BIT);
  memMap.bank0 = (uint8_t*) heap_caps_malloc(0x8000, MALLOC_CAP_8BIT);
  memMap.bank1 = &memMap.bank0[0x4000];

  uint8_t* contiguousBlock = (uint8_t*) heap_caps_malloc(0x6280, MALLOC_CAP_8BIT);
  memMap.vram0 = &contiguousBlock[0];
  memMap.vram1 = &contiguousBlock[0x2000];
  memMap.workRam0 = &contiguousBlock[0x4000];
  memMap.workRam1 = &contiguousBlock[0x5000];
  memMap.highMemory = &contiguousBlock[0x6000];
  memMap.colorRam = &contiguousBlock[0x6200];

  memMap.workRam2 = (uint8_t*) heap_caps_malloc(0x6000, MALLOC_CAP_8BIT);
  memMap.workRam3 = &memMap.workRam2[0x1000];
  memMap.workRam4 = &memMap.workRam2[0x2000];
  memMap.workRam5 = &memMap.workRam2[0x3000];
  memMap.workRam6 = &memMap.workRam2[0x4000];
  memMap.workRam7 = &memMap.workRam2[0x5000];

  memMap.bank2 = (uint8_t*) heap_caps_malloc(0x4000, MALLOC_CAP_8BIT);
  memMap.bank3 = (uint8_t*) heap_caps_malloc(0x4000, MALLOC_CAP_8BIT);
  memMap.bank4 = (uint8_t*) heap_caps_malloc(0x4000, MALLOC_CAP_8BIT);
  memMap.bank5 = (uint8_t*) heap_caps_malloc(0x4000, MALLOC_CAP_8BIT);
  memMap.bank6 = (uint8_t*) heap_caps_malloc(0x4000, MALLOC_CAP_8BIT);
  memMap.bank7 = (uint8_t*) heap_caps_malloc(0x4000, MALLOC_CAP_8BIT);
  //memMap.bank8 = (uint8_t*) heap_caps_malloc(0x4000, MALLOC_CAP_8BIT);

  screen = new Screen();
  screen->init();

  joypad = new Joypad();
  joypad->init();

  fileSystem = new FileSystem();
  motherboard = new Motherboard(joypad, screen, fileSystem, &memMap);
  //Serial.begin(115200);

  bool fileSystemOk = fileSystem->init("/pokemon_gold.gbc", "/pokemon_gold.sav");

  if(!fileSystemOk) {
    while (true) { delay(1000); }
  }

  memset(memMap.extMem, 0, 0x8000);
  fileSystem->readSave(0x8000, memMap.extMem);

  time_t rtcRegister = fileSystem->readRTC();
  struct timeval tv;
  tv.tv_sec = rtcRegister + 3600;
  tv.tv_usec = 0;
  settimeofday(&tv, NULL);

  motherboard->insertCartridge();
  
}

void loop() {
  //motherboard->runCycle();

  if (joypad->saveGame) {
    screen->requestDrawUI();
    vTaskDelay(pdMS_TO_TICKS(100));
    screen->drawSaveUI();
    fileSystem->writeSave(0x8000, memMap.extMem);
    fileSystem->writeRTC(time(NULL));
    vTaskDelay(pdMS_TO_TICKS(2000));
    screen->endDrawUI();
    joypad->saveGame = false;
  }
  
  //unsigned long iTime = millis();
  for(uint32_t count =0; count < 1000000;) {
    count = count + motherboard->runCycle();
  }
  //Serial.println(millis() - iTime);
}