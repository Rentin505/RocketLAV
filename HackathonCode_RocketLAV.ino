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
#define G433_MEDIUM
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
  
  //servo1.attach(SERVO_1_PIN);
  
  gpsSerial.begin(GPSBaud);                               
}

void loop() {

  int16_t ax = mpu.getAccelerationX();  // ускорение по оси Х
  int16_t ay = mpu.getAccelerationY();  // ускорение по оси Х
  int16_t az = mpu.getAccelerationZ();  // ускорение по оси Х
  //Сбор пакета данных для передачи
  uint32_t cycleStartTime = millis();
  dataPack.t = cycleStartTime;
   
  ax = mpu.getAccelerationX();
  ax = constrain(ax, -16384, 16384);    // ограничиваем +-1g
  float angle = ax / 16384.0;           // переводим в +-1.0
  // и в угол через арксинус
  if (angle < 0) angle = 90 - degrees(acos(angle));
  else angle = degrees(acos(-angle)) - 90;
  dataPack.ax = angle;
  
  ay = mpu.getAccelerationY();
  ay = constrain(ay, -16384, 16384);    // ограничиваем +-1g
  float angle1 = ay / 16384.0;           // переводим в +-1.0
  // и в угол через арксинус
  if (angle1 < 0) angle1 = 90 - degrees(acos(angle1));
  else angle1 = degrees(acos(-angle1)) - 90;
  dataPack.ay = angle1;
  
  az = mpu.getAccelerationZ();
  az = constrain(az, -16384, 16384);    // ограничиваем +-1g
  float angle2 = az / 16384.0;           // переводим в +-1.0
  // и в угол через арксинус
  if (angle2 < 0) angle2 = 90 - degrees(acos(angle2));
  else angle2 = degrees(acos(-angle2)) - 90;
  dataPack.az = angle2;

  
  dataPack.alti = bme.readAltitude();
  dataPack.temp = bme.readTemperature();
  
  
  tx.sendData(dataPack);
  
  
  delay(100);

  //Запись данных о местоположении на SD накопитель
  while (gpsSerial.available() > 0)
  dataFile.print(gpsSerial.read()); dataFile.print(";");

  //Демонстрация вращения солнечных панелей
  //servo1.write(270);
  //delay(1000);
 // servo1.write(180);
  //delay(1000);
  //servo1.write(90);
  //delay(1000);
 // servo1.write(0);
 // delay(100);  
  
}
