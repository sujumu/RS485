/*
  예제 221-3-1 (A아두이노용코드/마스터)
  아두이노에 가변저항을 A0에 연결했다!
  0~1023의 아날로그값을 0~255로 map한 다음에 B아두이노로 값을 RS485로 전송해보자!
  B는 그값을 받아서 모터드라이버의 ENB핀을 제어할것이다!
  그리고 버튼을 D4와 D5에 하나씩 연결해서 D4를 누르면 정방향으로 설정하고 D5를 누르면 방향을 역방향으로 설정하라!
  1602LCD의 SDA핀을 A4에 연결하고 SCL핀을 A5에 연결하자!
 */

#include <SoftwareSerial.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define master 0x99 //153
#define slave1 0x00 //B아두이노
#define slave2 0x01 //C아두이노
#define command1 0x01 //방향
#define command2 0x02 //파워
#define modepin 13
#define readmode LOW
#define sendmode HIGH

#define button1 4 //정방향
#define button2 5 //역방향
int pre_analog = 0;

LiquidCrystal_I2C lcd(0x27,16,2);
SoftwareSerial rs485(2, 3); // RX, TX

byte direct_forward[8]={0,0,0,0,0,0,0,0}; //정방향
byte direct_backward[8]={1,0,0,0,0,0,0,0}; //정방향

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //PC-아두이노간 TTL통신
  rs485.begin(9600); //아두이노-rs485간 TTL통신
  pinMode(modepin,OUTPUT); //송수신제어핀
  digitalWrite(modepin,readmode); //수신모드

  pinMode(button1,INPUT_PULLUP);
  pinMode(button2,INPUT_PULLUP);
  lcd.init();
  lcd.backlight();

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("DC MOTER POWER : ");

  pre_analog = analogRead(A0);
  pre_analog = map(pre_analog,0,1023,0,255);
}

void loop() {
  // put your main code here, to run repeatedly:
  //버튼을 눌렀을때 방향에 관련된부분을 전송해보자!
  if(digitalRead(button1) ==LOW){
   //슬레이브1에게 전송
   rs485_send(slave1,command1,direct_forward);
   byte recv[10];
    if(rs485_receive(recv) == -1){
      Serial.println("슬레이브1 오프라인");
    }else{
      for(int i = 0;i<10;i++){
        if(i==2 || i == 3){
          Serial.print((char)recv[i]);
        }else{
          Serial.print(recv[i],HEX);
        }
        Serial.print(",");
      }
      Serial.println();
    }
    //슬레이브2에게 전송
   rs485_send(slave2,command1,direct_forward);
    if(rs485_receive(recv) == -1){
      Serial.println("슬레이브2 오프라인");
    }else{
      for(int i = 0;i<10;i++){
        if(i==2 || i == 3){
          Serial.print((char)recv[i]);
        }else{
          Serial.print(recv[i],HEX);
        }
        Serial.print(",");
      }
      Serial.println();
    }
   delay(100); 
  }
  if(digitalRead(button2) ==LOW){
    //슬레이브1번
   rs485_send(slave1,command1,direct_backward);
   byte recv[10];
    if(rs485_receive(recv) == -1){
      Serial.println("슬레이브1 오프라인");
    }else{
      for(int i = 0;i<10;i++){
        if(i==2 || i == 3){
          Serial.print((char)recv[i]);
        }else{
          Serial.print(recv[i],HEX);
        }
        Serial.print(",");
      }
      Serial.println();
    }
    //슬레이브2번
   rs485_send(slave2,command1,direct_backward);

    if(rs485_receive(recv) == -1){
      Serial.println("슬레이브2 오프라인");
    }else{
      for(int i = 0;i<10;i++){
        if(i==2 || i == 3){
          Serial.print((char)recv[i]);
        }else{
          Serial.print(recv[i],HEX);
        }
        Serial.print(",");
      }
      Serial.println();
    }
   delay(100); 
  }

  
  //가변저항의 값이 변화될때마다 슬레이브들에게 값을 전송해보자!
  int analog = analogRead(A0); // 0~1023 -> 0~255
  analog = map(analog,0,1023,0,255); //0~255

  //할거다하고~~
  if(pre_analog != analog){
    //슬레이브1에게 보내는 부분(request)
    //아날로그값을 전송하는 메시지패킷을 만들자
    byte msg[8]={analog,0,0,0,0,0,0,0};
    rs485_send(slave1,command2,msg);

    byte recv[10];
    if(rs485_receive(recv) == -1){
      Serial.println("슬레이브1 오프라인");
    }else{
      for(int i = 0;i<10;i++){
        if(i==2 || i == 3){
          Serial.print((char)recv[i]);
        }else{
          Serial.print(recv[i],HEX);
        }
        Serial.print(",");
      }
      Serial.println();
    }
    //슬레이브2에게 전송
    rs485_send(slave2,command2,msg);

    if(rs485_receive(recv) == -1){
      Serial.println("슬레이브2 오프라인");
    }else{
      for(int i = 0;i<10;i++){
        if(i==2 || i == 3){
          Serial.print((char)recv[i]);
        }else{
          Serial.print(recv[i],HEX);
        }
        Serial.print(",");
      }
      Serial.println();
    }
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DC MOTER POWER : ");
    lcd.setCursor(0,1);
    lcd.print(analog);
  }
  
  pre_analog = analog;
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

