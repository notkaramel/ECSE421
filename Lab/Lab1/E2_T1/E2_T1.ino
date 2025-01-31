/**
Task: Write a program that outputs a counter on the serial monitor. The counter is initialized at 0 and is incremented by 1 for each clockwise increment of the rotary encoder. Similarly, it is decremented by 1 for each counterclockwise increment. The counter should be reset to 0 when the encoder is pressed down like a button.
*/

#define CLK_PIN 2
#define DT_PIN 4
#define SW_PIN 7

int counter = 0;
int clkState, dtState, swState;

void setup()
{
    // Initialize the serial communication and the pins
    Serial.begin(9600);
    pinMode(CLK_PIN, INPUT);
    pinMode(DT_PIN, INPUT);
    pinMode(SW_PIN, INPUT_PULLUP);
}

void loop()
{
    // Read the state of the CLK and DT pins
    clkState = digitalRead(CLK_PIN);
    dtState = digitalRead(DT_PIN);
    swState = digitalRead(SW_PIN);

    if (swState == 0)
    {
        Serial.println("Reset counter");
        counter = 0;
        delay(500);
    }

    // Clockwise detection when CLK changes before DT
    if (clkState == 1 && dtState == 0) { // Decrease counter
        counter++;
        Serial.print("Counter: ");
        Serial.println(counter);
        delay(200);
    } else if (clkState == 0 && dtState == 1) { // Increase counter
        counter--;
        // Output the counter value
        Serial.print("Counter: ");
        Serial.println(counter);
        delay(200);
    }
}