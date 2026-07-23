/*
 예제 232-1-3 (FND모듈이 연결된 슬레이브2번코드)
 아두이노에 FND모듈(TM1637)을 3개를 아래와 같이 연결한다!
 첫번째모듈 : CLK=D4, DIO=D5
 두번째모듈 : CLK=D6, DIO=D7
 세번째모듈 : CLK=D8, DIO=D9
 마스터쪽에서 날라오는 자이로센서의 각도값을 X,Y,Z축 순서로 각 FND모듈에 출력하라!
 */

#include <SoftwareSerial.h>
#include <Arduino.h>
#include <TM1637Display.h>
#define CLK1 4
#define DIO1 5
#define CLK2 6
#define DIO2 7
#define CLK3 8
#define DIO3 9
TM1637Display display1(CLK1, DIO1);
TM1637Display display2(CLK2, DIO2);
TM1637Display display3(CLK3, DIO3);
#define master 0x99 //153
#define slave1 0x00 //0
#define slave2 0x01 //1
#define modepin 13
#define readmode LOW
#define sendmode HIGH
SoftwareSerial rs485(2, 3); // RX, TX

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //PC-아두이노간 TTL통신
  rs485.begin(9600); //아두이노-rs485간 TTL통신
  pinMode(modepin,OUTPUT); //송수신제어핀
  digitalWrite(modepin,readmode); //수신모드

  display1.setBrightness(0x0f);
  display2.setBrightness(0x0f);
  display3.setBrightness(0x0f);
  display1.showNumberDec(0, false);
  display2.showNumberDec(0, false);
  display3.showNumberDec(0, false);
}
void loop() {
  // put your main code here, to run repeatedly:
  if(rs485.available()){
    //485모듈로 들어오는 값이 존재한다면
    byte recv[10];
    rs485.readBytes(recv,10);
    if(recv[0] != slave2) return; //쌩~
    
    for(int i =0;i<10;i++){
      Serial.print(recv[i],HEX);
      Serial.print(",");
    }
    Serial.println(); 

    //recv[2] X
    //recv[3] Y
    //recv[4] Z
    display1.showNumberDec(recv[2], false);
    display2.showNumberDec(recv[3], false);
    display3.showNumberDec(recv[4], false);
    
    byte data[10];
    data[0] = recv[0];
    data[1] = recv[1];
    //응답데이터
    data[2] = 'O';
    data[3] = 'K';
    for(int i =4;i<10;i++){
      data[i] = 0x00;
    }
    for(int i =0;i<10;i++){
      Serial.print(data[i],HEX);
      Serial.print(",");
    }
    Serial.println(); 
    digitalWrite(modepin,sendmode);
    rs485.write(data,10);
    digitalWrite(modepin,readmode);
  }
}