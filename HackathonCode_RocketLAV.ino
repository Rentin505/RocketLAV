//Объявление библиотек
#include <BME280.h>
#include "MPU6050.h"
#include <Wire.h> 
#include <Gyver433.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <SD.h>
#include <SPI.h>

// Переменные препроцессора
#define G433_SLOW
#define UPDATE_TIME 264
#define SERVO_1_PIN 5
#define CS_PIN      10

// Глобальные объекты
BME280 bme;
MPU6050 mpu;
Gyver433_TX<2>tx;
Servo servo1;
File dataFile;

//Задание структуры передаваемого файла
struct dataPackStruct{
  uint32_t t;
  
  int16_t ax;
  int16_t ay;
  int16_t az;

  int alti;
  int temp;
};

dataPackStruct dataPack;

//Подключение GPS модуля
int RXPin = 6;                  
int TXPin = 7;                  
int GPSBaud = 9600;             
SoftwareSerial gpsSerial(RXPin, TXPin); 


void setup() {
  Wire.begin();
  bme.begin(0x76, 0x58);
  bme.setSampling(BME280::MODE_NORMAL,
                  BME280::SAMPLING_X1,
                  BME280::SAMPLING_X4,
                  BME280::SAMPLING_NONE,
                  BME280::FILTER_X16,
                  BME280::STANDBY_MS_1);
                  
  mpu.initialize();
  
  servo1.attach(SERVO_1_PIN);
  
  gpsSerial.begin(GPSBaud);                               
}

void loop() {
  //Сбор пакета данных для передачи
  uint32_t cycleStartTime = millis();
  dataPack.t = cycleStartTime; 
  dataPack.ax = mpu.getAccelerationX();
  dataPack.ay = mpu.getAccelerationY();
  dataPack.az = mpu.getAccelerationZ();
  dataPack.alti = bme.readAltitude();
  dataPack.temp = bme.readTemperature();
  tx.sendData(dataPack);
  delay(1000);

  //Запись данных о местоположении на SD накопитель
  while (gpsSerial.available() > 0)
  dataFile.print(gpsSerial.read()); dataFile.print(";");

  //Демонстрация вращения солнечных панелей
  servo1.write(270);
  delay(1000);
  servo1.write(180);
  delay(1000);
  servo1.write(90);
  delay(1000);
  servo1.write(0);
  delay(1000);  
  
}
