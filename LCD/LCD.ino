#include <LiquidCrystal.h>



LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int Contrast = 75;
void setup() {
  // put your setup code here, to run once:
    analogWrite(6, Contrast);
  lcd.begin(16, 2);
  // Print a message to the LCD.

}

void loop() {
  // put your main code here, to run repeatedly:
  
  lcd.setCursor(0, 1);
  lcd.print("tst");
}
