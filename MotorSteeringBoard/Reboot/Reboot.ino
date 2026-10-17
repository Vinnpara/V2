#include <EEPROM.h>

void setup() {
  // 1. Clear the EEPROM (Wipe saved names/IDs)
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
  
  // 2. The flash memory is automatically overwritten 
  // with this empty sketch when you hit "Upload"
}

void loop() {
  // Empty loop: Your Uno will now do absolutely nothing when powered on
}
