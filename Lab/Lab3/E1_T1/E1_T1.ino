/*
Task: Write a program that rings the buzzer every time the button is
pressed, using an interrupt and ISR to control the buzzer. (Edit March 4) You
should not be polling the button in the `loop()` method. Implement a two-tone
buzz (or feel free to get more creative with your chime!).
*/

#define BUTTON 2
#define BUZZ 5
bool on = false;

// Interupt Service Routine
void isr() {
  on = !on;
}

void setup() {
  pinMode(BUZZ, OUTPUT);
  pinMode(BUTTON, INPUT);
  attachInterrupt(digitalPinToInterrupt(BUTTON), isr, CHANGE);
}

void loop() {
  if(on) {
    tone(BUZZ, 370);
    delay(500);
    tone(BUZZ, 493);
    delay(500);
    tone(BUZZ, 740);
    delay(500);
  } else {
    noTone(BUZZ);
  }
}
