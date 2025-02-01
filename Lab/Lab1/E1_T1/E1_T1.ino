// Setting the location of the digital pins
#define R_PIN 3
#define G_PIN 5
#define B_PIN 6

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
    // Blinking white light every 500ms
    setColor(1, 1, 1);
    delay(500);
    setColor(0, 0, 0);
    delay(500);
}