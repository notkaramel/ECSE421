/*
Keypad reference code: https://github.com/Chris--A/Keypad/blob/master/examples/CustomKeypad/CustomKeypad.ino
Ultrasonic reference code: https://github.com/ErickSimoes/Ultrasonic/blob/master/examples/UltrasonicSimple/UltrasonicSimple.ino
Linear Hall reference code: https://arduinomodules.info/ky-024-linear-magnetic-hall-module/
Buzzer reference code: https://sensorkit.arduino.cc/sensorkit/module/lessons/lesson/04-the-buzzer
*/

#include <Keypad.h>
#include <Ultrasonic.h>

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte colPins[COLS] = { 5, 4, 3, 2 };
byte rowPins[ROWS] = { A3, A2, A1, A0 };

#define LED_R 6
#define LED_G 7

#define BUZZER 10
#define MAGNETIC 11
Ultrasonic ultrasonic(12, 13);

Keypad kp = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// Sensor inputs
int distanceCM, tamperDetection;

// System variables
int presence = 0, tamper = 0, N = 0, pinStatus = 0, systemStatus = 0, attempts = 0;
char input[5], PIN[5] = "3333A";

// Timer tracking for no delay
int refreshTime = 100;  // ms
unsigned long tCurr = 0, tPrev = 0, tTamper = 0, tPin = 0, tPanic = 0, tBlink = 0;

void setup() {
  pinMode(MAGNETIC, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  Serial.begin(9600);
}

// ============= KEYPAD =============
void clearInput() {
  for (int i = 0; i < 5; i++) {
    input[i] = ' ';
  }
  N = 0;
}

/*
Returns a value for pinStatus
*/
bool verifyPIN() {
  for (int i = 0; i < 5; i++) {
    if (input[i] != PIN[i]) {
      clearInput();
      return false;
    }
  }
  clearInput();
  return true;
}

// ============= BUZZER =============
// Three state of Buzzer: none, warning, panic
void warningTone() {
  tone(BUZZER, 300);
}

void panicTone() {
  tone(BUZZER, 500, 400);
  tone(BUZZER, 800, 1000);
}

void stopTone() {
  noTone(BUZZER);
}

// ============= LED ============= //
void green() {
  digitalWrite(LED_R, LOW);
  digitalWrite(LED_G, HIGH);
}

void red() {
  digitalWrite(LED_R, HIGH);
  digitalWrite(LED_G, LOW);
}

void orange() {
  digitalWrite(LED_R, HIGH);
  digitalWrite(LED_G, HIGH);
}

bool blinkToggle = false;
void blinkingRed() {
  if (tBlink == 0) tBlink = tCurr;  // set starting time stamp

  if (tCurr - tBlink >= 200) {
    blinkToggle = !blinkToggle;
    tBlink = tCurr;
  }

  if (blinkToggle) {
    // Red for some time
    digitalWrite(LED_R, HIGH);
    digitalWrite(LED_G, LOW);
  } else {
    // Blank for some time
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_G, LOW);
  }
}

// ============= CONTROL SYSTEM ============= //
unsigned long tDectection = 0;
bool wait = false;

void controlSystem() {
  switch (systemStatus) {
    case 0:  // Disarmed, green light
      if (pinStatus == 1) {
        systemStatus = 2;
        pinStatus = 0;
      } else if (pinStatus == 2) {
        systemStatus = 1;
        pinStatus = 0;
        attempts++;
      }

      green();
      stopTone();
      break;
    case 1:  // Warning state
      // Serial.print(tamper);
      // Serial.print(", ");
      // Serial.print(presence);
      // Serial.print(", ");
      if (pinStatus == 1) {
        systemStatus = 0;
        pinStatus = 0;
        attempts = 0;
      } else if (pinStatus == 2 && attempts == 2) {
        systemStatus = 3;
        pinStatus = 0;
      } else if (pinStatus == 2) {
        systemStatus = 1;
        attempts++;
        pinStatus = 0;
      }

      if (tCurr - tDectection > 5200) {
        // Serial.println("WAIT IS FALSE");
        wait = false;
      }

      if (!wait && tamper) {
        // Serial.print(tamper);
        // Serial.println("");
        if (tCurr - tDectection < 15000) {
          systemStatus = 3;
        }

        if (presence && tCurr - tDectection < 15000) {
          systemStatus = 3;
        }

        if (tCurr - tDectection >= 15000) {
          tDectection = 0;
        }
      }

      warningTone();
      orange();
      break;
    case 2:  // Armed state
      if (pinStatus == 1) {
        systemStatus = 0;
        attempts = 0;
        pinStatus = 0;
      }

      if (pinStatus == 2 && attempts == 2) {  // TODO: also check for number of attempts
        systemStatus = 3;
        pinStatus = 0;
      }

      if (pinStatus == 2) {  // TODO: also check for number of attempts
        attempts++;
        pinStatus = 0;
      }


      if (tamper || presence) {  // if there is presence or tamper, send to warning stage
        systemStatus = 1;
        tDectection = tCurr;
        wait = true;
      }

      stopTone();
      red();
      break;
    case 3:  // ALARM, panic mode
      if (pinStatus == 1) {
        systemStatus = 0;
        pinStatus = 0;
        attempts = 0;
        tDectection = 0;
      }
      panicTone();
      blinkingRed();
      break;
  }
}

// ============= LOOP ============= //
void loop() {
  tCurr = millis();

  // Keypad subsystem looping
  if (N == 5) {
    if (verifyPIN()) {
      pinStatus = 1;
      // toggle between disarmed (0) and armed (2)
      // if other states, go to disarmed
    } else {
      pinStatus = 2;
    }
  }

  char userInput = kp.getKey();
  if (userInput) {
    if (userInput == '*') clearInput();
    else if (N <= 4) {
      input[N] = userInput;
      N++;
    }
  }


  tamperDetection = digitalRead(MAGNETIC);
  if (tamperDetection) {
    tamper = 1;
    tTamper = tCurr;
  } else if (tamperDetection == 0 && tCurr - tTamper >= 5000) {
    tamper = 0;
    tTamper = -1;
  }


  controlSystem();
  // Serial.print(presence);
  Serial.println(input);


  if (tCurr - tPrev >= refreshTime) {
    tPrev = tCurr;
    distanceCM = ultrasonic.read();

    if (distanceCM < 100) {
      presence = 1;
    } else {
      presence = 0;
    }
  }
}