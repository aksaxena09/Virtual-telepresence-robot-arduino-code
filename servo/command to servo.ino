#include <Wire.h>
#include <MPU6050_tockn.h>
#include <Servo.h>
Servo servo;
int servoang, initang;
MPU6050 mpu6050(Wire);
long timer = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(6, OUTPUT); //connect servo wire 2 to ~6
  servo.attach(3);//connect servo sig to ~3
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  mpu6050.update();
  initang = mpu6050.getGyroAngleX();
  Serial.println(initang);
  Serial.println("=======================================================\n");
}

void loop()
{
  mpu6050.update();
  if (millis() - timer > 500)
  {
    Serial.print("gyroAngleX : "); Serial.println(mpu6050.getGyroAngleX());
    servoang = map(mpu6050.getGyroAngleX(), initang - 95, initang + 95, 1, 179);
    if (servoang < 1)
      servoang = 1;
    if (servoang > 179)
      servoang = 179;
    servo.write(servoang);
    Serial.println(servoang);
    Serial.println("=======================================================\n");
    timer = millis();
  }
}
