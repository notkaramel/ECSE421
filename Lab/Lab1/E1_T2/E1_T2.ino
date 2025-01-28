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