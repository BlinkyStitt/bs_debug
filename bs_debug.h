#include <Arduino.h>

#ifdef DEBUG

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

void freeMemory(bool eol) {
  char top;
  Serial.print(F("Free Mem="));
#ifdef __arm__
  Serial.print(&top - reinterpret_cast<char*>(sbrk(0)));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  Serial.print(&top - __brkval);
#else  // __arm__
  Serial.print(__brkval ? &top - __brkval : &top - __malloc_heap_start);
#endif  // __arm__

  if (eol) {
    Serial.println();
  }
}

#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINT2(x, y) Serial.print(x, y)
#define DEBUG_PRINTLN(x) Serial.println(x)
#define DEBUG_PRINTLN2(x, y) Serial.println(x, y)
#define DEBUG_PRINTF(x, y) Serial.printf(x, y)

// TODO: this was working for smart-compass, but bs-music-maker didn't like it
void DEBUG_HEX8(uint8_t *data, uint8_t length, bool eol) {
  // prints 8-bit data in hex with leading zeroes
  char tmp[16];

  // hex16 needs %.4X
  sprintf(tmp, "%.2X", data[0]);

  DEBUG_PRINT(tmp);

  for (int i = 1; i < length; i++) {
    DEBUG_PRINT(F("-"));

    sprintf(tmp, "%.2X", data[i]);
    DEBUG_PRINT(tmp);
  }

  if (eol) {
    DEBUG_PRINTLN();
  }
}
// TODO: debug HEX16?

#else  // !DEBUG

#define DEBUG_PRINT(x)
#define DEBUG_PRINT2(x, y)
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINTLN2(x, y)
#define DEBUG_PRINTF(x, y)

void DEBUG_HEX8(uint8_t *data, uint8_t length, bool eol) {}

void freeMemory(bool eol) {}

#endif  // DEBUG

// TODO: rename to debugSerial?
void debug_serial(int bitrate = 115200, int delay_ms = 5000) {
// delay_ms: don't get locked out if something crashes
  #ifdef DEBUG
    Serial.begin(bitrate);

    delay(delay_ms);
    #ifdef DEBUG_SERIAL_WAIT
      while (!Serial) {
        ; // wait for serial port to connect
      }
    #endif  // DEBUG_SERIAL_WAIT

  #else  // no DEBUG
    delay(delay_ms);
  #endif  // DEBUG
}
