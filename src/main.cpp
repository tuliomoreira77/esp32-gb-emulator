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


//BASIC UI ELEMENTS
const static int UI_LINES = 4;
struct UIState {
  UIElement* elements;
  uint8_t lastDPadState;
  uint8_t lastButtonState;
  int cursor;
};
void initialUILoop();
void buildUIElements(UIElement elements[], FileDescriptor files[], int size);
void handleDPad(UIState* state);


void setup() {
  //alloc memory first to prevent heap fragmentation 
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
  memMap.gameRom  = (uint8_t*) heap_caps_malloc(2 * 1024 * 1024, MALLOC_CAP_SPIRAM);

  
  screen = new Screen();
  screen->init();

  joypad = new Joypad();
  joypad->init();

  fileSystem = new FileSystem();
  motherboard = new Motherboard(joypad, screen, fileSystem, &memMap);
  
  Serial.begin(115200);
  
  bool fileSystemOk = fileSystem->init();

  if(!fileSystemOk) {
    while (true) { delay(1000); }
  }

  initialUILoop();

  fileSystem->initRom("/pokemon_pcrystal.gbc", "/pokemon_pcrystal.sav");
  memset(memMap.extMem, 0, 0x8000);
  fileSystem->readSave(0x8000, memMap.extMem);

  time_t rtcRegister = fileSystem->readRTC();
  struct timeval tv;
  tv.tv_sec = rtcRegister + 3600;
  tv.tv_usec = 0;
  settimeofday(&tv, NULL);

  fileSystem->readRom(0, 2 * 1024 * 1024, memMap.gameRom);
  motherboard->insertCartridge();
}

void loop() {

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
  for(uint32_t count =0; count < 4000000;) {
    count = count + motherboard->runCycle();
  }
  //Serial.println(millis() - iTime);

}

void initialUILoop() {
  FileDescriptor files[UI_LINES];
  UIElement elements[UI_LINES];
  int size = fileSystem->listFolder(files, UI_LINES, 0);

  buildUIElements(elements, files, size);

  UIState state = {
    elements,
    0b1111,
    0b1111,
    0
  };

  screen->requestDrawUI();
  while (true) {
    elements[state.cursor].selected = false;
    handleDPad(&state);
    elements[state.cursor].selected = true;

    if(joypad->getButtons() != state.lastButtonState) {
      state.lastButtonState = joypad->getButtons();
      if((state.lastButtonState & 0b0001) == 0) {
        screen->endDrawUI();
        break;
      }
    }
    
    screen->drawGenericUI(elements, UI_LINES);
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void handleDPad(UIState* state) {
  if(joypad->getDPad() != state->lastDPadState) {
    state->lastDPadState = joypad->getDPad();
    if((state->lastDPadState & 0b0100) == 0 && state->cursor > 0) {
      state->cursor -= 1;
    } else if((state->lastDPadState & 0b1000) == 0 && state->cursor < UI_LINES-1) {
      state->cursor += 1;
    }
  }
}

void buildUIElements(UIElement elements[], FileDescriptor files[], int size) {
  for(int i=0; i < size; i++) {
    elements[i].selected = false;
    elements[i].text = files[i].name;
    switch (files[i].ext)
    {
      case ROM:
        elements[i].action = LOAD_GAME;
        break;
      
      default:
        elements[i].action = NO_ACTION;
        break;
    }
  }
}

