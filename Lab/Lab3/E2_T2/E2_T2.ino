/*
== Generating a PWM signal ==

Using the PWM signal that we are now generating,
we want to be able to dim an LED. 

Task: Using one of the timers for the PWM and the rotary encoder to control its duty cycle, implement a program letting you control the brightness of an LED by rotating the rotary encoder. Do not use pinMode() or analogWrite().

In your deliverable make sure to discuss the following: 
- How you designed the interrupts
- Any challenges you faced
- If you were in a lab with access to more tools how would you test to ensure that your PWM code is working? 
*/

#define CLK PD2
#define DT PD4
#define SW PD3
#define LED PD6 // Using Timer0

enum State {
  IDLE,
  TURNING
};

enum State state = IDLE;

uint8_t brightness = 0;

void setLight(int percent) {
  int dutyCycle = int(0xff * percent/100);

  // -- CODE generated by ChatGPT -- 
  // Configure Timer0 for Fast PWM Mode (8-bit)
  TCCR0A |= _BV(COM0A1) | _BV(WGM00) | _BV(WGM01); // Fast PWM, non-inverting
  TCCR0B |= _BV(CS01); // Prescaler = 8 (gives ~976Hz frequency)

  OCR0A = dutyCycle; // Set duty cycle (0-255)
}

void encoderISR() {
  switch (state) {
    case IDLE:
      if (PIND & (1 << DT) && brightness < 95) {
        brightness += 5;
      } else {
        if (brightness >= 5)
        brightness -= 5;
      }
      setLight(brightness);
      Serial.print("Brightness: ");
      Serial.println(brightness);
      
      state = TURNING;
      break;
    
    case TURNING:
      state = IDLE;
      break;
  }
}

void switchISR() {
  brightness = 0;

  Serial.print("Reset LED");
}

void setup() {
  Serial.begin(9600);

  DDRD |= (1 << LED);
  DDRD &= ~((1 << CLK) | (1 << DT) | (1 << SW));  // Set PD2, PD3, PD4 as inputs
  PORTD |= (1 << SW);                                     // INPUT_PULLUP for switch press

  // Interrupt on clock signal changes
  attachInterrupt(digitalPinToInterrupt(CLK), encoderISR, RISING);
  // Interrupt on button press, falling edge
  attachInterrupt(digitalPinToInterrupt(SW), switchISR, RISING);
  state = IDLE;


  // Code source: https://www.instructables.com/Arduino-Timer-Interrupts/
  //set timer0 interrupt at 2kHz
  // TCCR0A = 0;// set entire TCCR0A register to 0
  // TCNT0  = 0;//initialize counter value to 0
  // set compare match register for 2khz increments
  // OCR0A = 124;// = (16*10^6) / (2000*64) - 1 (must be <256)
}

void loop() {
}
