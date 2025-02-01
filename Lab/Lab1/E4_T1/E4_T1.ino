/*
Exercise 4: You Are a Pilot
Example script from the lab instruction
*/

#include <FastIMU.h>
#include <Wire.h>

#define IMU_ADDRESS 0x68  //Change to the address of the IMU
MPU6500 IMU;              //Change to the name of any supported IMU!

calData calib = { 0 };  //Calibration data
AccelData accelData;    //Sensor data
GyroData gyroData;

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
}

void loop() {
  IMU.update();
  IMU.getAccel(&accelData);
  Serial.print(accelData.accelX);
  Serial.print("\t");
  Serial.print(accelData.accelY);
  Serial.print("\t");
  Serial.print(accelData.accelZ);
  Serial.print("\t");
  IMU.getGyro(&gyroData);
  Serial.print(gyroData.gyroX);
  Serial.print("\t");
  Serial.print(gyroData.gyroY);
  Serial.print("\t");
  Serial.print(gyroData.gyroZ);
  Serial.println();
  delay(50);
}