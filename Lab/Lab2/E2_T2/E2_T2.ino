/*
Exercise 2: The Smart Home
Question: Is this system best modelled as a Mealy machine or Moore machine?
Explain your choice.

Task: Create a state diagram for the above specifications. Using your diagram,
create the state-transition logic table. Include the current states, as well as
the inputs and outputs.

Task: Design the smart home controller in Arduino using switch cases, which
should be representative of each state in your state diagram. We recommend
creating functions to organize your code.

Specifications:

- Monitor the temperature: turn on the heating when it is below temperature X
and turn on cooling when it is above temperature Y.
- Monitor the **light sensor** and the **motion sensor**:
    - If there is no light but there is motion, turn on the lights (modeled by
the LED)
    - if there is some light, do nothing.
    - If there is lots of light, roll down the shades (use the relay as a mock
output). Assume the shades take 8 seconds to roll down and 10 seconds to roll
up.
    - (clarification: feb. 13) The shades should roll up when there isn't "a lot
of light" anymore.

    - Turn off the lights if there is no motion after Z seconds of no movement.

- A long press of the joystick opens the settings to configure the temperatures
and the light timeout. Design a menu system to let the user change these
variables. (clarification: feb. 13) This menu should be operated using the
joystick and displayed on the LCD.
- (clarification: feb. 13) The system should still operate while the menu is
open.
- Show the states and system status on the screen (they can be hidden when
navigating through the menu system).

Motion sensor reference:
https://www.instructables.com/How-to-Use-a-PIR-Motion-Sensor-With-Arduino/


*/

// --- WIRING ---
#define JoyX A0
#define JoyY A1
#define JoySW 2

#define PhotoRes A2
#define Motion 3

#define LED 4

#define Relay 7

#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 10, 11, 12, 13); // LCD: pin 8 to 13 for RS, E, D4-D7

#define oneSecond 1000
#define refresh 50 // ms, data refresh rate

// Instantiate global variables
unsigned long currTime = 0, prevTime = 0, longPressEnd = 0, longPressStart = 0;
unsigned long tempX = 20, tempY = 30, timeT = 2; // degree C, degree C, seconds

// Controlling 3 state machines:
// menuState: Control the display
// lightState: Control the output light
// shadeState: Control the relay
int8_t menuState = 0, prevMenuState = -1, lightState = 0, shadeState = 0;
unsigned int lightCurrTime = 0, lightPrevTime = 0;

// Menu State -1: Status and state of the system
void stateStatus() {
  lcd.setCursor(0, 0);

  lcd.print("L: ");
  switch (lightState) {
  case 0: // No light
    lcd.print("None");
    break;
  case 1: // Low light
    lcd.print("Low");
    break;
  case 2: // High light
    lcd.print("High");
    break;
  }

  lcd.setCursor(8, 0);
  lcd.print("M: ");
  lcd.print(digitalRead(Motion));

  lcd.setCursor(0, 1);
  lcd.print("Shade: ");
  switch (shadeState) {
  case 0:
    lcd.print("Idle");
    break;
  case 1:
    lcd.print("Down");
    break;
  case 2:
    lcd.print("Up");
    break;
  }
}

// State 0: Show the main menu
void menu0conf() {
  lcd.setCursor(0, 0);
  lcd.print("< Temperature");
  lcd.setCursor(10, 1);
  lcd.print("Time >");
}

// State 1
// Show the temperature selection menu
void menu1TempSelect() {
  lcd.setCursor(0, 0);
  lcd.print("< X to heat   |>");
  lcd.setCursor(0, 1);
  lcd.print("<< Y to cool");
}

// State 2, X temperature selection menu
void menu2X() {
  lcd.setCursor(0, 0);
  lcd.print("X = ");
  lcd.print(tempX);
  lcd.print(" degC");

  lcd.setCursor(0, 1);
  lcd.print("UP  DOWN   | Y <");
}

// State 3, Y selection menu
void menu3Y() {
  lcd.setCursor(0, 0);
  lcd.print("Y = ");
  lcd.print(tempY);
  lcd.print(" degC");

  lcd.setCursor(0, 1);
  lcd.print("UP  DOWN   | X >");
}

// State 4, time adjustment menu
void menu4T() {
  lcd.setCursor(0, 0);
  lcd.print("T = ");
  lcd.print(timeT);
  lcd.print(" seconds");

  lcd.setCursor(0, 1);
  lcd.print("UP  DOWN     | <");
}

// Log all the values for debugging purposes
void log(int joyX, int joyY, int joySW, int photores, int motion, int led,
         int relay) {
  Serial.print("JoyX: ");
  Serial.print(joyX);
  Serial.print("\tJoyY: ");
  Serial.print(joyY);
  Serial.print("\tJoySW: ");
  Serial.print(joySW);

  Serial.print("\tLight: ");
  Serial.print(photores);
  Serial.print("\tMotion: ");
  Serial.print(motion);
  Serial.print("\tLED: ");
  Serial.print(led);
  Serial.print("\tRelay: ");
  Serial.print(relay);

  Serial.print(" | ");
  Serial.print("\tX: ");
  Serial.print(tempX);
  Serial.print("\tY: ");
  Serial.print(tempY);
  Serial.print("\tT: ");
  Serial.println(timeT);
}

void ledOn() { digitalWrite(LED, HIGH); }

void ledOff() { digitalWrite(LED, LOW); }

void shade(char direction) {
  // if (direction == "up")
  // ... else if (direction == "down")
  digitalWrite(Relay, HIGH);
}

void shadeDone() { digitalWrite(Relay, LOW); }

// ---------- Controls -------------
void joyDown() {
  switch (menuState) {
  case 2:
    if (tempX > 0)
      tempX--;
    menu2X();
    delay(200);
    break;
  case 3:
    if (tempX < tempY)
      tempY--;
    menu3Y();
    delay(200);
    break;
  case 4:
    if (timeT > 0)
      timeT--;
    menu4T();
    delay(200);
    break;
  }
}

void joyUp() {
  switch (menuState) {
  case 2:
    if (tempX < tempY)
      tempX++;
    menu2X();
    delay(200);
    break;
  case 3:
    if (tempX <= tempY)
      tempY++;
    menu3Y();
    delay(200);
    break;
  case 4:
    timeT++;
    menu4T();
    delay(200);
    break;
  }
}

void joyRight() {
  switch (menuState) {
  case 0:
    menuState = 4;
    delay(500);
    lcd.clear();
    break;
  case 1:
    menuState = 0;
    delay(500);
    lcd.clear();
    break;
  case 2: // Temp X to Temp Menu
    menuState = 1;
    delay(500);
    lcd.clear();
    break;
  case 3: // Temp Y to Temp X
    menuState = 2;
    delay(500);
    lcd.clear();
    break;
  default:
    break;
  }
}

void joyLeft() {
  switch (menuState) {
  case 0:
    menuState = 1;
    delay(500);
    lcd.clear();
    break;
  case 1:
    menuState = 2;
    delay(500);
    lcd.clear();
    break;
  case 2:
    menuState = 3;
    delay(500);
    lcd.clear();
    break;
  case 4: // Back
    menuState = 0;
    delay(500);
    lcd.clear();
    break;
  default:
    break;
  }
}

void joyPressed() {
  switch (menuState) {
  // do nothing cases
  case -1:
    break;
  case 0:
    break;

  // far from main configuration menu
  default:
    menuState = 0;
    delay(500);
    lcd.clear();
    break;
  }
}

void joyLongPressed() {
  switch (menuState) {
  case -1:
    menuState = 0;
    delay(500);
    lcd.clear();
    break;
  default:
    menuState = -1;
    delay(500);
    lcd.clear();
    break;
  }
}

// Set up all input and output pins
// Set up LCD, Serial port
void setup() {
  pinMode(JoyX, INPUT);
  pinMode(JoyY, INPUT);
  pinMode(JoySW, INPUT_PULLUP);

  pinMode(PhotoRes, INPUT);
  pinMode(Motion, INPUT);

  pinMode(LED, OUTPUT);
  pinMode(Relay, OUTPUT);
  lcd.begin(16, 2);
  Serial.begin(115200);

  menuState = -1;
  prevMenuState = -2;
}

void loop() {
  if (menuState != prevMenuState) {
    switch (menuState) {
    case -1:
      stateStatus();
      break;
    case 0:
      menu0conf();
      break;
    case 1:
      menu1TempSelect();
      break;
    case 2:
      menu2X();
      break;
    case 3:
      menu3Y();
      break;
    case 4:
      menu4T();
      break;
    default:
      break;
    }
    prevMenuState = menuState;
  }

  int joyX, joyY, joySW, photores, motion, led, relay;
  joyX = analogRead(JoyX);
  joyY = analogRead(JoyY);
  joySW = digitalRead(JoySW);

  photores = analogRead(PhotoRes);
  motion = digitalRead(Motion);
  led = HIGH;
  relay = 0;
  currTime = millis();

  // Optimized by ChatGPT o3-mini from previously written code
  if (joySW == 0 &&
      longPressStart ==
          0) { // Button pressed for the first time, start the press timer
    longPressStart = currTime;
  }

  if (joySW == 0) { // Button is being held, update the release time
    longPressEnd = currTime;
  }

  if (joySW == 1 && longPressStart != 0) {
    if (longPressEnd - longPressStart >= 1000) {
      joyLongPressed(); // Long press action
    } else {
      joyPressed(); // Normal press action
    }

    // Reset after release
    longPressStart = 0;
    longPressEnd = 0;
  }

  if (joyX < 300)
    joyRight(); // X go right
  if (joyX > 700)
    joyLeft(); // X go left (horizontal)
  if (joyY > 750)
    joyDown(); // Y go down
  if (joyY < 300)
    joyUp(); // Y go up (vertical)

  if (motion) {
    lightState = 1;
  }

  // if (joySW == 0) {  // Button is being held, update the release time
  //   longPressEnd = currTime;
  // }

  // if (joySW == 1 && longPressStart != 0) {
  //   if (longPressEnd - longPressStart >= 1000) {
  //     joyLongPressed();  // Long press action
  //   } else {
  //     joyPressed();  // Normal press action
  //   }

  //   // Reset after release
  //   longPressStart = 0;
  //   longPressEnd = 0;
  // }

  // if (motion) {
  //   ledOn();
  // }
  // if (lightCurrTime < timeT * 1000) {
  //   lightCurrTime = millis();
  // } else {
  //   ledOff();
  //   lightCurrTime = 0;
  // }

  if (currTime - prevTime >= refresh) {
    log(joyX, joyY, joySW, photores, motion, led, relay);
    prevTime = currTime;
    if (menuState == -1)
      stateStatus();
  }
}
