#include <Wire.h>
#include <MPU6050_tockn.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "printf.h"
#include <RF24_config.h>
int CE_PIN = 9, CSN_PIN = 10, initanga, initangb;
RF24 object(CE_PIN, CSN_PIN);
uint64_t address = 0x4321ABCDE1LL;
MPU6050 mpu6050(Wire);
long timer = 0;
struct angs
{
  int x, z, c;
} data;
void setup()
{
  Serial.begin(9600);
  pinMode(6, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  mpu6050.update();
  initanga = mpu6050.getGyroAngleZ();
  initangb = mpu6050.getGyroAngleX();

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
  if (millis() - timer > 200)
  {
    Serial.print("gyroAngleX : "); Serial.print(mpu6050.getGyroAngleX());
    //Serial.print("\tgyroAngleY : "); Serial.print(mpu6050.getGyroAngleY());
    Serial.print("\tgyroAngleZ : "); Serial.println(mpu6050.getGyroAngleZ());
    /*Serial.print("angleX : "); Serial.print(mpu6050.getAngleX());
      Serial.print("\tangleY : "); Serial.print(mpu6050.getAngleY());
      Serial.print("\tangleZ : "); Serial.println(mpu6050.getAngleZ());*/
    data.x = map(mpu6050.getGyroAngleZ(), initanga - 90, initanga + 90, 1, 179);
    if (data.x < 1)
      data.x = 1;
    if (data.x > 179)
      data.x = 179;
    data.z = map(mpu6050.getGyroAngleX(), initangb - 90, initangb + 90, 1, 179);
    if (data.z < 1)
      data.z = 1;
    if (data.z > 179)
      data.z = 179;
    if (digitalRead(3) == LOW)
      data.c = 1;
    else if (digitalRead(4) == LOW)
      data.c = 2;
    else if (digitalRead(5) == LOW)
      data.c = 3;
    else if (digitalRead(6) == LOW)
      data.c = 4;
    else
      data.c = 5;
    object.write(&data, sizeof(data));
    //object.write(&servoang, sizeof(servoang));
    Serial.println(data.x);
    Serial.println(data.z);
    Serial.println(data.c);
    Serial.println("=======================================================\n");
    timer = millis();
  }
}
