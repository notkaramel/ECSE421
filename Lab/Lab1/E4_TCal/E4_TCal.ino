/*
You Are a Pilot: Calibration script

Calibration script example: https://learn.adafruit.com/adxl345-digital-accelerometer/programming
*/


#include <FastIMU.h>
#include <Wire.h>

#define IMU_ADDRESS 0x68    // Change to the address of the IMU
MPU6500 IMU;               // Change to the name of any supported IMU! 

calData calib = { 0 };  // Calibration data
AccelData accelData;    // Sensor data

float AccelMinX = 0;
float AccelMaxX = 0;
float AccelMinY = 0;
float AccelMaxY = 0;
float AccelMinZ = 0;
float AccelMaxZ = 0;

void setup() {
  Wire.begin();
  Wire.setClock(400000); //400khz clock
  Serial.begin(115200);
  while (!Serial) {
    ;
  }

  int err = IMU.init(calib, IMU_ADDRESS);
    if (err != 0) {
        Serial.print("Error initializing IMU: ");
        Serial.println(err);
        while (true) {
            ;
        }
    }
  
}

void loop() {
    Serial.println("Type key when ready..."); 
    while (!Serial.available()){}  // wait for a character
    IMU.update();
    IMU.getAccel(&accelData);

    if (accelData.accelX < AccelMinX) AccelMinX = accelData.accelX;
    if (accelData.accelX > AccelMaxX) AccelMaxX = accelData.accelX;

    if (accelData.accelY < AccelMinY) AccelMinY = accelData.accelY;
    if (accelData.accelY > AccelMaxY) AccelMaxY = accelData.accelY;

    if (accelData.accelZ < AccelMinZ) AccelMinZ = accelData.accelZ;
    if (accelData.accelZ > AccelMaxZ) AccelMaxZ = accelData.accelZ;

    Serial.print("Accel Minimums: "); Serial.print(AccelMinX); Serial.print("  ");Serial.print(AccelMinY); Serial.print("  "); Serial.print(AccelMinZ); Serial.println();
    Serial.print("Accel Maximums: "); Serial.print(AccelMaxX); Serial.print("  ");Serial.print(AccelMaxY); Serial.print("  "); Serial.print(AccelMaxZ); Serial.println();

      while (Serial.available())
    {
      Serial.read();  // clear the input buffer
    }
}