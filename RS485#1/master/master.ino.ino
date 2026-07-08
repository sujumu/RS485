/*
 예제 223-1-1 (A아두이노용코드/마스터)
 아두이노에 RFRD리더(RC522)를 연결했다!
 SPI통신에 맞게 연결하고 리셋핀을 D9에 CS핀을 D10에 연결했다!
 RFID리더로 태그를 리딩했을때 iD값을 RS485로 B와 C아두이노로 각각 전송하라!
 */

#include <SoftwareSerial.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
 
MFRC522 rfid(SS_PIN, RST_PIN);
#define master 0x99 //153
#define slave1 0x00 //0
#define slave2 0x01 //1
#define command1 0x01
#define command2 0x02
#define modepin 8
#define readmode LOW
#define sendmode HIGH
SoftwareSerial rs485(2, 3); // RX, TX

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //PC-아두이노간 TTL통신
  rs485.begin(9600); //아두이노-rs485간 TTL통신
  pinMode(modepin,OUTPUT); //송수신제어핀
  digitalWrite(modepin,readmode); //수신모드
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
}

void loop() {
  // put your main code here, to run repeatedly:

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  
   
    Serial.println(F("The NUID tag is:"));
    Serial.print(F("In hex: "));
    printHex(rfid.uid.uidByte, rfid.uid.size);
    //rfid.uid.uidByte[0] ~ rfid.uid.uidByte[3]
    //슬레이브1에게 전송(B아두이노)
    //슬레이브1에게 보내는 부분(request)
    byte message[8]={0};
    for(int i = 0;i<4;i++){
      message[i] = rfid.uid.uidByte[i];
    }
    rs485_send(slave1,command1,message);

    byte recv[10];
    if(rs485_receive(recv) == -1){
      Serial.println("슬레이브1 오프라인");
    }else{
      for(int i = 0;i<10;i++){
        Serial.print(recv[i],HEX);
        Serial.print(",");
      }
      Serial.println();
    }
    
    //슬레이브2에게 전송(C아두이노)
    rs485_send(slave2,command1,message);

    if(rs485_receive(recv) == -1){
      Serial.println("슬레이브2 오프라인");
    }else{
      for(int i = 0;i<10;i++){
        Serial.print(recv[i],HEX);
        Serial.print(",");
      }
      Serial.println();
    }

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();

  
}
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

void rs485_send(byte id, byte command, byte message[]){
  byte data[10];
  data[0] = id;
  data[1] = command;
  for(int i =2;i<10;i++){
    data[i] = message[i-2];
  }
  //단순 결과확인 출력문
  Serial.print("(마스터)송신데이터=");
  for(int i = 0;i<10;i++){
    Serial.print(data[i]);
    Serial.print(",");
  }
  Serial.println();

  //송신부분
  digitalWrite(modepin,sendmode);
  rs485.write(data,10);
  digitalWrite(modepin,readmode);
}

int rs485_receive(byte recv[]){
  //슬레이브1에서 날린 데이터가 존재할때까지 무한루프
  unsigned long t = millis(); //루프진입직전의 시간
  while(1){
    if(millis() - t > 2000){
      //루프안에서 누른 스톱워치하고 t하고의 시간차이가 2000ms 이상이면~
      //Serial.println("오프라인");
      return -1;
      break;
    }
    if(rs485.available()){
      //485모듈에서 아두이노쪽으로 수신한값이 존재한다면~
      //데이터 읽기
      rs485.readBytes(recv,10);
      /*
      Serial.print("(마스터)수신데이터=");
      for(int i =0;i<10;i++){
        Serial.print(recv[i],HEX);
        Serial.print(",");
      }
      Serial.println();
      */
      return 0;
      break;
    }
  }
}
