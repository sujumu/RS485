/*
 예제 223-1-3 (C용 아두이노코드/슬레이브2)
 아두이노에 적색LED가 D4에 연결되고 녹색LED가 D5에 연결되었다!
 A아두이노에서 전송받은 RFID태그의 ID값이 등록된 ID면 녹색LED, 그렇지 않으면 적색 LED를 켜시오!
 권한이 있는카드 예시)
 D4 6D A2 2A
 4D 94 A0 3D
 1C 75 84 3D
 */

#include <SoftwareSerial.h>
#define master 0x99 //153
#define slave1 0x00 //0
#define slave2 0x01 //1
#define modepin 13
#define readmode LOW
#define sendmode HIGH

#define led1 4
#define led2 5

 
 
byte auth[3][4] = {
  {0xD9, 0x31, 0x33, 0x07},
  {0x4D, 0x94, 0xA0, 0x3D},
  {0x1C, 0x75, 0x84, 0x3D}
};

SoftwareSerial rs485(2, 3); // RX, TX

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //PC-아두이노간 TTL통신
  rs485.begin(9600); //아두이노-rs485간 TTL통신
  pinMode(modepin,OUTPUT); //송수신제어핀
  digitalWrite(modepin,readmode); //수신모드

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

    //수신값을 비교해보자!
    //대상 데이터 recv[2]~recv[5]
    //권한이 있는 카드가 3개니까 루프를 3회 돌려보자!
    bool is_auth = false;
    for(int i = 0;i<3;i++){
      if(auth[i][0] == recv[2] &&auth[i][1] == recv[3] && auth[i][2] == recv[4] && auth[i][3] == recv[5]){
        is_auth = true;
        break;
      }
    }

    if(is_auth){
      //권한이 있는 카드
      digitalWrite(led1,LOW);
      digitalWrite(led2,HIGH);
    }else{
      //권한이 없는 카드
      digitalWrite(led1,HIGH);
      digitalWrite(led2,LOW);
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