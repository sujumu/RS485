/*
 예제 221-3-3 (C아두이노용 코드/슬레이브2)
 빨간LED가 D4에 연결되어있고 녹색LED가 D5에 연결되어있다!
 A아두이노에서 설정된 방향이 정방향이면 녹색LED를 ON하고 역방향이면 적색LED를 ON하라!
 FND모듈(TM1637)이 CLK가 D6에 DIO가 D7에 연결되었다!
 마스터아두이노에서 DC모터의 설정한 파워값을 FND모듈에 출력하라!
 */

#include <SoftwareSerial.h>
#include <Arduino.h>
#include <TM1637Display.h>
#define CLK 6
#define DIO 7
#define master 0x99 //153
#define slave1 0x00 //0
#define slave2 0x01 //1
#define command1 0x01 //방향
#define command2 0x02 //파워
#define modepin 13
#define readmode LOW
#define sendmode HIGH
#define led1 4 //적색
#define led2 5 //녹색

SoftwareSerial rs485(2, 3); // RX, TX
TM1637Display display(CLK, DIO);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //PC-아두이노간 TTL통신
  rs485.begin(9600); //아두이노-rs485간 TTL통신
  pinMode(modepin,OUTPUT); //송수신제어핀
  digitalWrite(modepin,readmode); //수신모드
  display.setBrightness(0x0f);

  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
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

    if(recv[1] == command1){
      //방향
      if(recv[2] == 0){
        //정방향
        digitalWrite(led1,LOW);
        digitalWrite(led2,HIGH);
      }else if(recv[2] == 1){
        //역방향
        digitalWrite(led1,HIGH);
        digitalWrite(led2,LOW);
      }
    }else if(recv[1] == command2){
      //파워
      display.showNumberDec(recv[2], false);
    }
    
    
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