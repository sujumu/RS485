/*
 예제 221-3-2 (B아두이노용 코드/슬레이브1)
 B아두이노에는 모터드라이버가 연결되고 DC모터 1개를 제어하도록 해보자!
 IN3, IN4 , ENB핀을 이용해서 B쪽 모터를 제어해보자!
 IN3을 D4에 연결하고 In4를 D5에 연결하고 ENB는 D6에 연결했다!
 A아두이노에서 날라오는 방향값에 따라서 모터드라이버의 방향을 결정하고
 파워값을 이용해서 모터를 PWM제어해보라!
 */

#include <SoftwareSerial.h>
#define master 0x99 //153
#define slave1 0x00 //0
#define slave2 0x01 //1
#define command1 0x01 //방향
#define command2 0x02 //파워
#define modepin 13
#define readmode LOW
#define sendmode HIGH
 
#define IN3 4 //디지털제어
#define IN4 5 //디지털제어
#define ENB 6 //아날로그제어

SoftwareSerial rs485(2, 3); // RX, TX

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //PC-아두이노간 TTL통신
  rs485.begin(9600); //아두이노-rs485간 TTL통신
  pinMode(modepin,OUTPUT); //송수신제어핀
  digitalWrite(modepin,readmode); //수신모드

  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
}
void loop() {
  // put your main code here, to run repeatedly:
  if(rs485.available()){
    //485모듈로 들어오는 값이 존재한다면
    byte recv[10];
    rs485.readBytes(recv,10);
    if(recv[0] != slave1) return; //쌩~

    
    for(int i =0;i<10;i++){
      Serial.print(recv[i],HEX);
      Serial.print(",");
    }
    Serial.println(); 

    if(recv[1] == command1){
      //방향과 관련된 오다~
      //recv[2] == 0 정방향
      //recv[2] == 1 역방향
      if(recv[2] == 0){
        digitalWrite(IN3,HIGH);
        digitalWrite(IN4,LOW);
      }else if(recv[2] == 1){
        digitalWrite(IN3,LOW);
        digitalWrite(IN4,HIGH);
      }
    }else if(recv[1] == command2){
      //파워와 관련된 오다
      //recv[2] : 파워값
      analogWrite(ENB,recv[2]);
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