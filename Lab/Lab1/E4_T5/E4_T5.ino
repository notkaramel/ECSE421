/*
Task: With your accelerometer and gyroscope calibrated, connect the LCD to your board. Design an airplane instrument panel. Display the yaw, pitch, and roll of your plane, as well as the elevation (estimated through the accelerometer readings). 
*/

#include <FastIMU.h>
#include <Wire.h>

#define IMU_ADDRESS 0x68
MPU6500 IMU; 

calData calib = { 0 };
AccelData accelData;
GyroData gyroData;


float rawXLow = - 0.97, rawXHigh = 1.03;
float rawYLow = - 1.01, rawYHigh = 0.99;
float rawZLow = - 0.99, rawZHigh = 1.03;
float refRange = 2.0;

float correctX = 0.0;
float correctY = 0.0;
float correctZ = 0.0;

#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

#define BUZZ 3

void setup() {
    pinMode(BUZZ, OUTPUT);
    Wire.begin();
    Wire.setClock(400000); //400khz clock
    Serial.begin(115200);
    while (!Serial) {;}

    int err = IMU.init(calib, IMU_ADDRESS);
    if (err != 0) {
        Serial.print("Error initializing IMU: ");
        Serial.println(err);
        while (true) {;}
    }

    lcd.begin(18, 2);

    // Printing the labels of pitch, roll, yaw, and elevation on the LCD
    lcd.setCursor(0,0);
    lcd.print("P   R   Y   E cm"); // Elevation in centimeters
}

float velocityZ = 0.0, elevation = 0.0;

void loop() {
    IMU.update();
    IMU.getAccel(&accelData);

    correctX = (accelData.accelX - rawXLow) * refRange / (rawXHigh - rawXLow) - 1;
    correctY = (accelData.accelY - rawYLow) * refRange / (rawYHigh - rawYLow) - 1;
    correctZ = (accelData.accelZ - rawZLow) * refRange / (rawZHigh - rawZLow) - 1;


    // Getting the gyro data and printing the pitch, roll, and yaw on the LCD
    IMU.getGyro(&gyroData);
    int pitch = gyroData.gyroX, roll = gyroData.gyroY, yaw = gyroData.gyroZ;
    lcd.setCursor(0, 1);
    lcd.print(int(pitch));
    
    lcd.setCursor(4, 1);
    lcd.print(int(roll));

    lcd.setCursor(8, 1);
    lcd.print(int(yaw));

    // Calculate and print the elevation on the LCD
    lcd.setCursor(12, 1);
    /*
    dt = 0.01s = 100ms
    v += (a-1) * g * dt (* unit conversion from m/s to cm/s)
    s += v * dt
    NOTE: There is a mistake/noise error that causes the elevation to drift over time, making it inaccurate (decaying uncontrollably) 
    */
    velocityZ += (correctZ - 1) * 9.8; // cm/s
    elevation += velocityZ * 0.01; // cm
    lcd.print(elevation); // cm

    // Buzz if pitch is too high or too low
    if (pitch > 50 || pitch < -50) {
        tone(BUZZ, 450);
    } else {
        noTone(BUZZ);
    }

    Serial.print("Pitch: ");
    Serial.print(pitch);
    Serial.print(", Roll: ");
    Serial.print(roll);
    Serial.print(", Yaw: ");
    Serial.println(yaw);

    delay(100);
}