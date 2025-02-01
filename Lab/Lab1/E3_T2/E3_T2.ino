/*
Exercise 3: The Weather
Task: Repeat the previous task but display the temperature and humidity on the LCD display.

DHT instructions & code example: https://randomnerdtutorials.com/complete-guide-for-dht11dht22-humidity-and-temperature-sensor-with-arduino/

LCD instructions & code example from the lab instruction.
*/

#include <DHT.h>
#include<LiquidCrystal.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(9600);
    dht.begin();
    lcd.begin(16, 2); // Set up the LCD's number of columns and rows

    // write the humidity and temperature labels
    lcd.setCursor(0,0);
    lcd.print("H (%)");

    lcd.setCursor(8,0);
    lcd.print("T (*C)");
}

void loop() {
    float h = dht.readHumidity();
    float t = dht.readTemperature(); // Celsius

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    // Print out the humidity and temperature values on the screen
    lcd.setCursor(0, 1);
    lcd.print(h);

    lcd.setCursor(8, 1);
    lcd.print(t);

    delay(200);
}
