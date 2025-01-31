/*
- TODO: Task: Use the rotary encoder to select the buzzing frequency of the buzzer. Using the serial plotter and the microphone, find the approximate frequency at which the buzzer appears to be the loudest from the microphone's point of view.
*/

#define CLK_PIN 2
#define DT_PIN 4
#define SW_PIN 7

#define BUZZER 9
#define MIC A0

int counter = 0;
int clkState, dtState, swState;
int mic = 0;

void setup()
{
    // Initialize the serial communication
    Serial.begin(9600);

    // Initialize the rotary encoder
    pinMode(CLK_PIN, INPUT);
    pinMode(DT_PIN, INPUT);
    pinMode(SW_PIN, INPUT_PULLUP);

    // Initialize the buzzer
    pinMode(BUZZER, OUTPUT);

    // Initialize the microphone
    pinMode(MIC, INPUT);
}

void loop()
{

    // Read the state of the CLK and DT pins
    clkState = digitalRead(CLK_PIN);
    dtState = digitalRead(DT_PIN);
    swState = digitalRead(SW_PIN);
    mic = analogRead(MIC);

    // Clockwise detection when CLK changes before DT
    if (clkState == 1 && dtState == 0) { // Decrease counter
        counter++;
        delay(100);
    } else if (clkState == 0 && dtState == 1) { // Increase counter
        counter--;
        delay(100);
    }
    
    tone(BUZZER, 440 + counter*10);
    Serial.print(440 + counter*10);
    Serial.print(" ");
    Serial.println(mic);
}