/*
Exercise 3: The Weather
We all know that one of the easy conversations to have when you can't think of a good conversation topic is to talk about the weather. To impress your crush, you want to add to this conversation, so you decide to create a device to monitor the weather to the best of your abilities.

For the next part of the lab, connect the Temperature and Humidity sensor to your Arduino board. To obtain data from this sensor, you will have to install the DHT library. This can be accomplished through the library manager in the Arduino IDE, accessible from the Sketch -> Include Library -> Manage Libraries menu. Note that this sensor has a maximum sampling period of 2 seconds.

Task: Write a program that displays the temperature and humidity in the serial monitor.

You will now connect the LCD to your Arduino. Below is a wiring diagram to help you. Note the resistor connecting to the V0 pin. This adjusts the contrast ratio. You can choose a resistor that leads to the contrast ratio of your choice. You may also use a potentiometer if you have one.
*/

// DHT instructions & code: https://randomnerdtutorials.com/complete-guide-for-dht11dht22-humidity-and-temperature-sensor-with-arduino/

#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(9600);
    dht.begin();
}
void loop() {
    float h = dht.readHumidity();
    // Read temperature as Celsius
    float t = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" degC");
    delay(500);
}
