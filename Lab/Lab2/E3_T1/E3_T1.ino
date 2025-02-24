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
#define LED_B 8

#define BUZZER 10
#define MAGNETIC 11
Ultrasonic ultrasonic(12, 13);

Keypad kp = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// Sensor inputs
int distanceCM, tamperDetection;

// System variables
int presence = 0, tamper = 0, N = 0, pinStatus = 0, systemStatus = 0;
char input[6], PIN[5] = "2143C";

// Timer tracking for no delay
int refreshTime = 100;  // ms
unsigned long tCurr = 0, tPrev = 0, tTamper = 0, tPin = 0, tPanic = 0, tRed = 0;

void setup() {
  pinMode(MAGNETIC, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  Serial.begin(9600);
}

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
      return false;
    }
  }
  return true;
}


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

// LED colouring
void green() {
  digitalWrite(LED_R, LOW);
  digitalWrite(LED_G, HIGH);
  digitalWrite(LED_B, LOW);
}

void red() {
  digitalWrite(LED_R, HIGH);
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_B, LOW);
}

void orange() {
  digitalWrite(LED_R, HIGH);
  digitalWrite(LED_G, HIGH);
  digitalWrite(LED_B, LOW);
}

void blinkingRed() {
  // Red for some time
  digitalWrite(LED_R, HIGH);
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_B, LOW);

  delay(200);

  // Blank for some time
  digitalWrite(LED_R, LOW);
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_B, LOW);

  delay(200);
}



void loop() {
  tCurr = millis();
  char userInput = kp.getKey();
  if (userInput) {
    if (userInput == '*') clearInput();
    else {
      if (N == 5) {
        if (verifyPIN()) {
          pinStatus = 1;
          // toggle between disarmed (0) and armed (2)
          // if other states, go to disarmed
        }
      }

      if (N <= 4) {
        Serial.print("\tUserInput = ");
        Serial.print(userInput);
        input[N] = userInput;
        N++;
      }

      Serial.print("\t ~ \t");
      Serial.println(input);
    }
  }


  if (tCurr - tPrev >= refreshTime) {

    distanceCM = ultrasonic.read();
    if (distanceCM < 100) {
      presence = 1;
    } else {
      presence = 0;
    }

    tamperDetection = digitalRead(MAGNETIC);
    if (tamperDetection) {
      tamper = 1;
      tTamper = tCurr;
    } else if (tamperDetection == 0 && tCurr - tTamper >= 5000) {
      tamper = 0;
      tTamper = -1;
    }
    // else {
    // (tamperDetection == 0 && tCurr - tTamper < 5000)
    // do nothing, tCurr is always increment
    // }

    tPrev = tCurr;
  }
}