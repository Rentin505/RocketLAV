//Обьявление библиотек
#include <Gyver433.h> 
#include <Wire.h> 
#include <LiquidCrystal_I2C.h> 

LiquidCrystal_I2C LCD(0x27,16,2);
Gyver433_RX<2, 20> rx; 

//Задание структуры принятого файла
typedef struct{
  uint32_t t;
  
  int16_t ax;
  int16_t ay;
  int16_t az;

  int alti;
  int temp;
}dataPackStruct;

void setup() {

  // Иницилизация serial порта
  Serial.begin(9600); 

  //Работа с дисплеем
  LCD.init(); 
  LCD.backlight();  
  LCD.setCursor(1, 0);     
  LCD.print("RocketLAV");     

  //Режимы работы кнопки и зумера
  pinMode(4, INPUT);
  pinMode(3, OUTPUT);

}


void loop() {

  //Дополнительный функционал в виде кнопки и зумера
  if (digitalRead(4)==LOW) {
    digitalWrite(3, HIGH); 
  }
  else {
    digitalWrite(3, LOW);
      }

  //Работа с принятием данных
  if (rx.tick()) {
    dataPackStruct DataPack ;            
    if (rx.readData(DataPack)) {  
      
      Serial.println("Time: " + DataPack.t);
      Serial.println("AccelX: " + DataPack.ax);
      Serial.println("AccelY: " + DataPack.ay);
      Serial.println("AccelZ: " + DataPack.az);
      Serial.println ("HIGHT: " + DataPack.alti);
      Serial.println ("TEMP: " + DataPack.temp);
      Serial.println();
    } else {
      Serial.println("Wrong data");
    }
  }
}
