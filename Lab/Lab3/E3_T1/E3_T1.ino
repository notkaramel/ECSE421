/*
Task: Implement the reaction time game.
Components:
- Joystick
- The push button
- LCD

// Using TimerInterrupt for time limit (for March 4th edit)
https://github.com/khoih-prog/TimerInterrupt/blob/master/examples/TimerInterruptTest/TimerInterruptTest.ino
*/


#include <LiquidCrystal.h>
#include <TimerInterrupt.h>
// Wiring: 8 ~ RS | 9 ~ E | 10-13 ~ D4-D7
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

#define JX A0  // might differ the wiring for the module
#define JY A1  // to feel more natural on the breadboard
#define SW 3
#define BTN 2

// Custom arrow, generated from
// https://maxpromer.github.io/LCD-Character-Creator/
byte upArrow[] = { 0x04, 0x0E, 0x1F, 0x15, 0x04, 0x04, 0x04, 0x04 };
byte downArrow[] = { 0x04, 0x04, 0x04, 0x04, 0x15, 0x1F, 0x0E, 0x04 };
byte rightArrow[] = { 0x18, 0x0C, 0x06, 0x03, 0x03, 0x06, 0x0C, 0x18 };
byte leftArrow[] = { 0x03, 0x06, 0x0C, 0x18, 0x18, 0x0C, 0x06, 0x03 };
byte buttonSymbol[] = { 0x0E, 0x1F, 0x11, 0x15, 0x15, 0x11, 0x1F, 0x0E };

/*
The user is shown a symbol on the screen representing an action
Symbols: up, down, left, right, or button
*/
enum JOYSTICK { BUTTON,
                UP,
                RIGHT,
                DOWN,
                LEFT,
                NONE };

enum GAME { START,
            WAIT,
            END,
            IDLE };

enum JOYSTICK jstate = NONE, qstate = NONE;
enum GAME gstate = IDLE;

/*
Game design idea:
- gameRound = 0 
  - gstate = START
    - wait for delay[i]
    - show character quiz[i] & start timer
  - gstate = WAIT
    - IF capture joystick input & stop timer
      - gather time[i] = timerEnd - timerStart
      - gather result[i] = {0 (miss) | 1 (hit)}
      - { gstate = START; gameRound++ } if gameRound <= 5 else gstate = END
  - gstate = END
    - Display result
    - Set gstate = START
    - Wait for button to interrupt
*/

// Global Variables
uint8_t gameRound = -1;
unsigned int result[5], timer[5];
unsigned int randomDelayMs[5], quiz[5];
unsigned long timerStart, timerEnd;

void idleScreen() {
  lcd.clear();
  lcd.home();
  lcd.print("Press button");
  lcd.setCursor(0, 1);
  lcd.print("to start game!");
}

// Start the game
void gameStart() {
  lcd.clear();
  lcd.home();
  lcd.print("Ready: ");  // 7 char

  for (int i = 0; i < 5; i++) {
    randomDelayMs[i] = random(1000, 3000);
    Serial.println(randomDelayMs[i]);
    quiz[i] = random(0, 5);
    // reset the result and timer
    result[i] = 0;
    timer[i] = 0.0;
  }

  gstate = START;
  Serial.println("========================");
  gameRound = 0;
  timerStart = millis();
}

// Show result, end the game
void showResult() {
  unsigned int totalTime = 0, errors = 0;
  for (int i = 0; i < 5; i++) {
    totalTime += timer[i];
    if (!result[i]) errors++;
  }

  lcd.clear();
  lcd.home();
  lcd.print("Time (ms): ");  // 11 character
  lcd.print(totalTime);

  lcd.setCursor(0, 1);
  lcd.print("Errors: ");  // 8 char
  lcd.print(errors);
}

void setup() {
  lcd.begin(16, 2);

  // Create custom arrow keys
  lcd.createChar(0, buttonSymbol);
  lcd.createChar(1, upArrow);
  lcd.createChar(2, rightArrow);
  lcd.createChar(3, downArrow);
  lcd.createChar(4, leftArrow);

  // Start with IDLE Screen
  idleScreen();

  // Use an interrupt for the button and Arduino timer interrupt for the time limit.
  DDRD &= ~((1 << BTN) | (1 << SW));  // Set Button and Joystick's switch as inputs
  PORTD |= (1 << SW);                 // INPUT_PULLUP for switch press
  DDRC &= ~((1 << JX) | (1 << JY));   // Set joystick x and y as input

  attachInterrupt(digitalPinToInterrupt(BTN), gameStart, RISING);

  // Set random seed
  randomSeed(1241);

  Serial.begin(9600);
}


void loop() {
  switch (gstate) {
    case START:
      {
        // - wait for delay[i]
        // - show character quiz[i] & start timer
        timerEnd = millis();

        if (timerEnd - timerStart >= randomDelayMs[gameRound]) {
          // Serial.println("END, START, DELAY");
          // Serial.print(timerEnd);
          // Serial.print(timerStart);
          // Serial.print(randomDelayMs);
          qstate = quiz[gameRound];
          lcd.setCursor(7, 0);
          lcd.write(qstate);
          timerStart = millis();
          gstate = WAIT;
        }
        break;
      }
    case WAIT:
      {
        // will not wait for more than 5s

        int joyX = analogRead(JX),
            joyY = analogRead(JY),
            sw = digitalRead(SW);

        if (!sw) jstate = BUTTON;
        else if (joyX < 100) jstate = LEFT;
        else if (joyX > 900) jstate = RIGHT;
        else if (joyY > 900) jstate = UP;
        else if (joyY < 100) jstate = DOWN;
        else jstate = NONE;

        // IF exceed 5s waiting, to go IDLE screen
        if(millis() - timerStart >= 5000) {
          gstate = IDLE;
          idleScreen();
        }

        // IF capture joystick input
        //  - stop timer
        //  - gather time[i] = timerEnd - timerStart
        //  - gather result[i] = {0 (miss) | 1 (hit)}
        //  - { gstate = START; gameRound++ } if gameRound <= 5 else gstate = END
        if (jstate != NONE) {  // Conclude round
          lcd.setCursor(7, 0);
          lcd.print(" ");
          timerEnd = millis();
          timer[gameRound] = timerEnd - timerStart;
          result[gameRound] = (jstate == qstate);


          if (gameRound >= 4) gstate = END;
          else {
            gameRound++;
            gstate = START;
            timerStart = timerEnd = millis();
          }
        }
        break;
      }
    case END:
      // Show result
      showResult();
      gstate = IDLE;
      break;
    case IDLE:
      // Do nothing
      break;
  }
}
