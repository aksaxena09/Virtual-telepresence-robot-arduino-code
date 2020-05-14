#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "printf.h"
#include <RF24_config.h>
int CE_PIN = 9, CSN_PIN = 10, servoang;
RF24 object(CE_PIN, CSN_PIN);
uint64_t address = 0x4321ABCDE1LL;
Servo servoA, servoB;
long timer = 0;
struct angs
{
  int x, z, c;
} data;
void setup() {
  Serial.begin(9600);
  object.begin();
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  servoA.attach(6);//connect servo sig to ~3
  servoB.attach(7);//connect servo sig to ~4
  printf_begin();

  object.setPALevel(RF24_PA_MAX);
  object.setChannel(121);
  object.setDataRate(RF24_250KBPS);
  object.openReadingPipe(0, address);
  object.startListening();
  object.printDetails();
  data.x = 90;
  data.z = 90;
  servoA.write(data.x);
  servoB.write(data.z);
  stopp();
}

void loop()
{
  if (millis() - timer > 200)
  {
    if (object.available())
    {
      object.read(&data, sizeof(data));
    }
    servoA.write(data.x);
    servoB.write(data.z);
    if (data.c == 1)
      straight();
    else if (data.c == 2)
      left();
    else if (data.c == 3)
      right();
    else if (data.c == 4)
      back();
    else
      stopp();
    Serial.println(data.x);
    Serial.println(data.z);
    Serial.println(data.c);
    Serial.println("=======\n");
    timer = millis();
  }
}
void back()
{
  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
  //delay(10000);
}
void right()
{
  digitalWrite(2, LOW);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
}
void left()
{
  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
}
void straight()
{
  digitalWrite(2, LOW);
  digitalWrite(3, HIGH);
  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
}
void stopp()
{
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
}
