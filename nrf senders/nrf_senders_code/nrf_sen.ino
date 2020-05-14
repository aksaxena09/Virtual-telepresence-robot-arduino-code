#include <Wire.h>
#include <MPU6050_tockn.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "printf.h"
#include <RF24_config.h>
int CE_PIN = 9, CSN_PIN = 10, servoang = 0, initang;
RF24 object(CE_PIN, CSN_PIN);
uint64_t address = 0x4321ABCDE1LL;
MPU6050 mpu6050(Wire);
long timer = 0;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  mpu6050.update();
  initang = mpu6050.getGyroAngleX();

  object.begin();
  printf_begin();
  object.setPALevel(RF24_PA_MAX);
  object.setChannel(121);
  object.setDataRate(RF24_250KBPS);
  object.openWritingPipe(address);
  object.printDetails();
}

void loop()
{
  mpu6050.update();
  if (millis() - timer > 100)
  {
    Serial.print("gyroAngleX : "); Serial.print(mpu6050.getGyroAngleX());
    Serial.print("\tgyroAngleY : "); Serial.print(mpu6050.getGyroAngleY());
    Serial.print("\tgyroAngleZ : "); Serial.println(mpu6050.getGyroAngleZ());
    Serial.print("angleX : "); Serial.print(mpu6050.getAngleX());
    Serial.print("\tangleY : "); Serial.print(mpu6050.getAngleY());
    Serial.print("\tangleZ : "); Serial.println(mpu6050.getAngleZ());
    servoang = map(mpu6050.getGyroAngleX(), initang - 95, initang + 95, 1, 179);
    if (servoang < 1)
      servoang = 1;
    if (servoang > 179)
      servoang = 179;
    object.write(&servoang, sizeof(servoang));
    Serial.println(servoang);
    Serial.println("=======================================================\n");
    timer = millis();
  }
}
