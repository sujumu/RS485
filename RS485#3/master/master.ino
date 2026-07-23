/*
 예제 232-1-1 (마스터 아두이노 코드)
 아두이노에 1602LCD모듈을 SDA를 A4에, SCL을 A5에 연결해서 결과를 출력한다!
 슬레이브1번에 연결된 6축자이로센서(MPU6050)에서 측정되는 3축의 각도값을 전송받아서
 LCD화면에 결과를 출력하고, 그값을 슬레이브2번으로 토스하라!
 그러면 슬레이브2번에 연결된 FND모듈(TM1637) 3개에 각축에 대한 결과를 디스플레이 할 것이다!
 */

#include <SoftwareSerial.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
#define master 0x99 //153
#define slave1 0x00 //0
#define slave2 0x01 //1
#define command1 0x01
#define command2 0x02
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
  lcd.init();
  lcd.backlight();
}

void loop() {
  // put your main code here, to run repeatedly:

  //슬레이브1에게 보내는 부분(request)
  byte message[8]={0};
  rs485_send(slave1,command1,message);

  byte recv[10];
  if(rs485_receive(recv) == -1){
    Serial.println("슬레이브1 오프라인");
  }else{
    
    Serial.print("X=");
    Serial.print(recv[2]);
    Serial.print(",Y=");
    Serial.print(recv[3]);
    Serial.print(",Z=");
    Serial.print(recv[4]);
    Serial.println();

    //16
    //X(0)    Y(5)     Z(10)
    //100  120   90
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("X=");
    lcd.setCursor(5,0);
    lcd.print("Y=");
    lcd.setCursor(10,0);
    lcd.print("Z=");
    lcd.setCursor(0,1);
    lcd.print(recv[2]);
    lcd.setCursor(5,1);
    lcd.print(recv[3]);
    lcd.setCursor(10,1);
    lcd.print(recv[4]);
    /*
    Serial.print("슬레이브1=");
    for(int i = 0;i<10;i++){
      if(i==2||i==3){
        Serial.print((char)recv[i]);
      }else{
        Serial.print(recv[i],HEX);
      }
      
      Serial.print(",");
    }
    
    Serial.println();
    */
  }
  
  for(int i = 0;i<8;i++){
    message[i]=recv[i+2];
  }
  
  rs485_send(slave2,command1,message);

  recv[10];
  if(rs485_receive(recv) == -1){
    Serial.println("슬레이브2 오프라인");
  }else{
    
    Serial.print("슬레이브2=");
    for(int i = 0;i<10;i++){
      if(i==2||i==3){
        Serial.print((char)recv[i]);
      }else{
        Serial.print(recv[i],HEX);
      }
      Serial.print(",");
    }
    Serial.println();
    
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
  /*
  Serial.print("(마스터)송신데이터=");
  for(int i = 0;i<10;i++){
    Serial.print(data[i]);
    Serial.print(",");
  }
  Serial.println();
  */
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
