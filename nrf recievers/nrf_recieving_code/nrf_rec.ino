#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "printf.h"
#include <RF24_config.h>
int CE_PIN = 9, CSN_PIN = 10, servoang;
RF24 object(CE_PIN, CSN_PIN);
uint64_t address = 0x4321ABCDE1LL;
Servo servo;
long timer = 0;

void setup() {
  Serial.begin(9600); object.begin();
  servo.attach(3);//connect servo sig to ~3
  printf_begin();
  object.setPALevel(RF24_PA_MAX);
  object.setChannel(121);
  object.setDataRate(RF24_250KBPS);
  object.openReadingPipe(0, address);
  object.startListening();
  object.printDetails();
}

void loop()
{
  if (millis() - timer > 100)
  {
    if (object.available())
    {
      object.read(&servoang, sizeof(servoang));
    }
    servo.write(servoang);
    Serial.println(servoang);
    Serial.println("=======\n");
    timer = millis();
  }
}
