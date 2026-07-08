/*
 예제 223-1-2 (B아두이노용코드/슬레이브1)
 A아두이노에서 날아오는 RFID 태그 UID 값을 LCD에 출력
*/

#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define slave1 0x00
#define modepin 13
#define readmode LOW
#define sendmode HIGH

SoftwareSerial rs485(2, 3); // RX, TX

void setup() {
  Serial.begin(9600);
  rs485.begin(9600);

  rs485.setTimeout(100);

  pinMode(modepin, OUTPUT);
  digitalWrite(modepin, readmode);

  lcd.init();
  lcd.backlight();
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("LCD Ready");
}

void loop() {
  if(rs485.available() >= 10) {
    byte recv[10];
    rs485.readBytes(recv, 10);

    if(recv[0] != slave1) {
      return;
    }

    Serial.print("(슬레이브1)수신데이터=");
    for(int i = 0; i < 10; i++) {
      Serial.print(recv[i], HEX);
      Serial.print(",");
    }
    Serial.println();

    String tag_id = "";

    for(int i = 2; i <= 5; i++) {
      if(recv[i] < 0x10) {
        tag_id += "0";
      }

      tag_id += String(recv[i], HEX);
      tag_id += " ";
    }

    tag_id.toUpperCase();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("RFID TAG ID:");

    lcd.setCursor(0, 1);
    lcd.print(tag_id);

    byte data[10];
    data[0] = recv[0];
    data[1] = recv[1];
    data[2] = 'O';
    data[3] = 'K';

    for(int i = 4; i < 10; i++) {
      data[i] = 0x00;
    }

    Serial.print("(슬레이브1)응답데이터=");
    for(int i = 0; i < 10; i++) {
      Serial.print(data[i], HEX);
      Serial.print(",");
    }
    Serial.println();

    digitalWrite(modepin, sendmode);
    rs485.write(data, 10);
    rs485.flush();
    digitalWrite(modepin, readmode);
  }
}