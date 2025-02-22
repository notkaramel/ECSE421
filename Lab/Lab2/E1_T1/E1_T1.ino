/*
Task 1: Timer Functions

Task: Design a binary-format counter using one of the two functions mentioned above. The counter should be initialized at 0 and increment up every second. Display the counter on the LCD. 

Question: According to Arduino's documentation, micros() will overflow after approximately 70 minutes. Use this to determine the number of bits used by micros(). Show your calculations.

Instructions from the lab webpage
*/

#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

#define oneSecond 1000
unsigned long currTime = 0, counter = 0, prevTime = 0;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);  // Set up the LCD's number of columns and rows
  lcd.setCursor(0, 0);
  lcd.print("Binary counter");

  // Start at 0
  lcd.setCursor(0, 1);
  lcd.print(counter, BIN);
}


void loop() {
  currTime = millis();

  if (currTime - prevTime >= oneSecond) {
    prevTime = currTime;

    Serial.print("Time: ");
    Serial.println(currTime);  // prints time since program started

    counter++;
    lcd.setCursor(0, 1);
    lcd.print(counter, BIN);
  }
}