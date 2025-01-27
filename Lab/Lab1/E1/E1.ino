/*
-- Exercise 1: Working with an RGB LED --
-- Instruction from the lab manual --

Your sensor kit comes with two RGB LED modules. You may use either of them (or both!). Plug the module into your breadboard, then connect each connector to an appropriate pin on your Arduino board ("—" to GND, then each "R", "G", and "B" to a digital pin).

As you will see in your readings, it is important to add a resistor in series to prevent the LED from frying; fortunately, this resistor is already built into the sensor module.

Remember to replace the address of the Arduino’s on-board LED, LED_BUILTIN, referenced in the code, with the “slot” numbers in which your LED's "R", "G", and "B" pins are connected, to reflect the addresses of your external LED's pins.

> TODO: Task: Using the blink sketch as an example, blink your external LED in white color using `delay(amount)`, where amount is measured in milliseconds.
> TODO: Task: Modify your previous code to make the LED cycle through 6 distinct colors every 500ms.
*/

// Setting the location of the digital pins
#define R_PIN 3
#define G_PIN 5
#define B_PIN 6

/**
 * Write color in RGB value
 * @param r (0,1)
 */
void setColor(int r, int g, int b)
{
    digitalWrite(R_PIN, r);
    digitalWrite(G_PIN, g);
    digitalWrite(B_PIN, b);
}

void task1()
{
    setColor(1, 1, 1);
    delay(500);
    setColor(0, 0, 0);
    delay(500);
}

void task2()
{
    setColor(1, 0, 0);
    delay(500);
    setColor(0, 1, 0);
    delay(500);
    setColor(0, 0, 1);
    delay(500);
    setColor(1, 1, 0);
    delay(500);
    setColor(1, 0, 1);
    delay(500);
    setColor(0, 1, 1);
    delay(500);
    setColor(1, 1, 1);
    delay(500);
}

void setup()
{
    Serial.begin(9600);
    pinMode(R_PIN, OUTPUT);
    pinMode(G_PIN, OUTPUT);
    pinMode(B_PIN, OUTPUT);
}

void loop()
{
    // task1();
    task2();
}