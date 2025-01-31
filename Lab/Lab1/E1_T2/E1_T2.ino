// Setting the location of the digital pins
#define R_PIN 3
#define G_PIN 5
#define B_PIN 6

#define BLINK_MS 83

/**
 * Write color in RGB digital value
 * @param r (0,1)
 * @param g (0,1)
 * @param b (0,1)
 */
void setColor(int r, int g, int b)
{
    digitalWrite(R_PIN, r);
    digitalWrite(G_PIN, g);
    digitalWrite(B_PIN, b);
}

void setup()
{
    pinMode(R_PIN, OUTPUT);
    pinMode(G_PIN, OUTPUT);
    pinMode(B_PIN, OUTPUT);
}


void loop()
{
    setColor(1, 0, 0);
    delay(BLINK_MS);
    setColor(0, 1, 0);
    delay(BLINK_MS);
    setColor(0, 0, 1);
    delay(BLINK_MS);
    setColor(1, 1, 0);
    delay(BLINK_MS);
    setColor(1, 0, 1);
    delay(BLINK_MS);
    setColor(0, 1, 1);
    delay(BLINK_MS);
}