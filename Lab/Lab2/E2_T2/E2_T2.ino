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
#define ThermoStat 5
#define Cooler 6
#define Shade 7

#define DHTTYPE DHT11

#include <DHT.h>
#include <LiquidCrystal.h>

DHT dht(ThermoStat, DHTTYPE);
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);  // LCD: pin 8 to 13 for RS, E, D4-D7

#define oneSecond 1000
#define refresh 50  // ms, data refresh rate

#define lowLight 600   // analog input value when it's dark (more current)
#define highLight 300  // light = less current

// Instantiate global variables
unsigned long currTime = 0, prevTime = 0, longPressEnd = 0, longPressStart = 0;
unsigned long tempX = 20, tempY = 30, timeT = 2;  // degree C, degree C, seconds
int tShadeDown = 8, tShadeUp = 10;

// Controlling 3 state machines:
// menuState: Control the display
// lightState: Control the output light
// shadeState: Control the relay
int8_t menuState = 0, prevMenuState = -1, lightState = 0, shadeState = 0;
unsigned int lightCurrTime = 0, lightPrevTime = 0, tShadeCurr = 0, tShadePrev = 0;

// Light bulb, shade
int bulb = 0, shadeInMotion = 0, temp = 10;
char *shadeDirection = "----";

byte lightBulb[] = {
  0b01110,
  0B10001,
  0B10101,
  0B10001,
  0B10001,
  0B01010,
  0B01110,
  0B01110,
};


// Menu State -1: Status and state of the system
void stateStatus() {
  lcd.setCursor(0, 0);

  lcd.print("L: ");
  switch (lightState) {
    case 0:  // No light
      lcd.print("----");
      break;
    case 1:  // Low light
      lcd.print("Low ");
      break;
    case 2:  // High light
      lcd.print("High");
      break;
  }

  lcd.print("  S: ");
  lcd.print(shadeDirection);
  lcd.setCursor(0, 1);
  lcd.print("M:");
  lcd.print(digitalRead(Motion));

  lcd.print(" ");
  lcd.write(byte(0));
  lcd.print(":");
  lcd.print(bulb);
  lcd.print(" ");

  lcd.print(" ");
  lcd.print("T:");
  lcd.print(temp);
  lcd.print(" C");
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

void ledOn() {
  digitalWrite(LED, HIGH);
  bulb = 1;
}

void ledOff() {
  digitalWrite(LED, LOW);
  bulb = 0;
}

// if (direction == "up")
// ... else if (direction == "down")
void shade(char* direction) {
  shadeInMotion = 1;
  if (direction == "up") {
    shadeState = 2;
  } else if (direction == "down") {
    shadeState = 1;
  } else {
    shadeState = 0;
  }
  digitalWrite(Shade, HIGH);
}

void shadeDone() {
  shadeInMotion = 0;
  shadeState = 0;
  digitalWrite(Shade, LOW);
}

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
    case 2:  // Temp X to Temp Menu
      menuState = 1;
      delay(500);
      lcd.clear();
      break;
    case 3:  // Temp Y to Temp X
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
    case 4:  // Back
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
      lcd.clear();
      break;
  }
}

void joyLongPressed() {
  switch (menuState) {
    case -1:
      menuState = 0;
      lcd.clear();
      break;
    default:
      menuState = -1;
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
  pinMode(Shade, OUTPUT);
      dht.begin();

  lcd.begin(16, 2);
  Serial.begin(115200);

  menuState = -1;
  prevMenuState = -2;
  lcd.createChar(0, lightBulb);
}

void loop() {
  currTime = millis();
  unsigned int joyX, joyY, joySW, lightSensor, motion;
  joyX = analogRead(JoyX);
  joyY = analogRead(JoyY);
  joySW = digitalRead(JoySW);
  temp = int(dht.readTemperature());

  lightSensor = 900 - analogRead(PhotoRes);
  motion = digitalRead(Motion);

  // Detect changes in menu state
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

  // Optimized by ChatGPT o3-mini from previously written code.
  // Button pressed for the first time, start the press timer
  if (joySW == 0 && longPressStart == 0) {
    longPressStart = currTime;
  }

  // Button is being held, update the release time
  if (joySW == 0) {
    longPressEnd = currTime;
  }

  if (joySW == 1 && longPressStart != 0) {
    if (longPressEnd - longPressStart >= 1000) {
      joyLongPressed();  // Long press action
    } else {
      joyPressed();  // Normal press action
    }

    // Reset after release
    longPressStart = 0;
    longPressEnd = 0;
  }

  // Joystick direction movement
  if (joyX < 300) joyRight();  // X go right
  if (joyX > 700) joyLeft();   // X go left (horizontal)
  if (joyY > 750) joyDown();   // Y go down
  if (joyY < 300) joyUp();     // Y go up (vertical)

  // Serial.print(lightCurrTime);
  // Serial.print(" - ");
  // Serial.print(lightPrevTime);
  // Serial.print(" = ");
  // Serial.println(lightCurrTime - lightPrevTime);
  switch (lightState) {
    case 0:
      // If there is motion, turn on the LED and set light timer variable
      if (motion == 1) {
        ledOn();
        lightCurrTime = currTime;
        lightPrevTime = currTime;
      }

      if (lightSensor > 300 && lightSensor < 600) {
        lightState = 1;  // no light to low light
      }
      break;
    case 1:
      if (lightSensor < 300) {
        lightState = 0;
      }
      if (lightSensor > 600) {  // Roll the shade down and change light state
        lightState = 2;
        shade("down");
        tShadeCurr = currTime;
        tShadePrev = currTime;
      }

      if (shadeInMotion && tShadeCurr - tShadePrev < tShadeUp * 1000) {
        tShadeCurr = currTime;
      }

      if (shadeInMotion && tShadeCurr - tShadePrev >= tShadeUp * 1000) {
        shadeDone();
        tShadeCurr = 0;
        tShadePrev = 0;
      }

      break;
    case 2:
      if (lightSensor > 300 && lightSensor < 600) {  // Roll the shade up and change light state
        lightState = 1;                              // no light to low light
        shade("up");
        tShadeCurr = currTime;
        tShadePrev = currTime;
      }

      if (shadeInMotion && tShadeCurr - tShadePrev < tShadeDown * 1000) {
        tShadeCurr = currTime;
      }

      if (shadeInMotion && tShadeCurr - tShadePrev >= tShadeDown * 1000) {
        shadeDone();
        tShadeCurr = 0;
        tShadePrev = 0;
      }
      break;
  }


  if (motion == 0 && bulb == 1) {
    lightCurrTime = currTime;
    // Not update lightPrevTime
  }
  // No more motion, the light will turn off after timeT seconds
  if (motion == 0 && lightCurrTime - lightPrevTime >= timeT * 1000) {
    ledOff();
    lightCurrTime = 0;
    lightPrevTime = 0;
  }

  switch (shadeState) {
    case 0:
      shadeDirection = "Idle";
      break;
    case 1:
      shadeDirection = "Down";
      break;
    case 2:
      shadeDirection = "Up  ";
      break;
  }


  if (currTime - prevTime >= refresh) {
    prevTime = currTime;
    if (menuState == -1)
      stateStatus();
  }
}
