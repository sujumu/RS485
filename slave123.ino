/*
 C#예제 74-3-1(1번 아두이노용 코드)
 FND모듈(TM1637)이 CLK를 D4에 연결하고 DIO를 D5에 연결했다!
 윈폼에서 날라오는 명령에 따라 숫자를 FND모듈에 출력하시오!
 */

#include <SoftwareSerial.h>
#include <Arduino.h>
#include <TM1637Display.h>
#define CLK 4
#define DIO 5
#define master 0x99 //153
#define slave1 0x00 //0
#define slave2 0x01 //1
#define slave3 0x02 //1
#define modepin 13
#define readmode LOW
#define sendmode HIGH

SoftwareSerial rs485(2, 3); // RX, TX
TM1637Display display(CLK, DIO);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //PC-아두이노간 TTL통신
  rs485.begin(9600); //아두이노-rs485간 TTL통신
  pinMode(modepin,OUTPUT); //송수신제어핀
  digitalWrite(modepin,readmode); //수신모드
  display.setBrightness(0x0f);
  display.showNumberDec(0, false);
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

    //여기서 TM1637을 제어하면 되겠다!
    //recv[2] ~ recv[5]
    String num = String((char)recv[2]) + String((char)recv[3])+String((char)recv[4])+String((char)recv[5]);
    display.showNumberDec(num.toInt(), false);
    
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