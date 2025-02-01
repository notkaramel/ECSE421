/*
You Are a Pilot: Output calibration data
*/

#include <FastIMU.h>
#include <Wire.h>

#define IMU_ADDRESS 0x68  //Change to the address of the IMU
MPU6500 IMU;              //Change to the name of any supported IMU!

calData calib = { 0 };  //Calibration data
AccelData accelData;    //Sensor data

void setup() {
  Wire.begin();
  Wire.setClock(400000);  //400khz clock
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

  Serial.println("Time, X, Y, Z");
}

float rawXLow = - 0.97, rawXHigh = 1.03;
float rawYLow = - 1.01, rawYHigh = 0.99;
float rawZLow = - 0.99, rawZHigh = 1.03;
float refRange = 2.0;

float correctX = 0.0;
float correctY = 0.0;
float correctZ = 0.0;

void loop() {
  IMU.update();
  IMU.getAccel(&accelData);

  correctX = (accelData.accelX - rawXLow) * refRange / (rawXHigh - rawXLow) - 1;
  correctY = (accelData.accelY - rawYLow) * refRange / (rawYHigh - rawYLow) - 1;
  correctZ = (accelData.accelZ - rawZLow) * refRange / (rawZHigh - rawZLow) - 1;
  Serial.print(millis());
  Serial.print(", ");
  Serial.print(correctX);
  Serial.print(", ");
  Serial.print(correctY);
  Serial.print(", ");
  Serial.print(correctZ);
  Serial.println();
  delay(100);
}