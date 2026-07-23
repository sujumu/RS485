/*
 예제 232-1-2 (자이로센서가 있는 슬레이브1 코드)
 아두이노에 자이로센서(MPU6050)을 SDA를 A4에 연결하고 SCL을 A5에 연결하라!
 10ms에 한번씩 자이로센서의 값을 측정을 해서 내부적으로 보관하고 있다가
 마스터가 요청하는 시점에 데이터를 반환하라!
 */

#include <SoftwareSerial.h>
#include "Wire.h"
#include <MPU6050_light.h>
MPU6050 mpu(Wire);
#define master 0x99 //153
#define slave1 0x00 //0
#define slave2 0x01 //1
#define modepin 13
#define readmode LOW
#define sendmode HIGH
SoftwareSerial rs485(2, 3); // RX, TX

unsigned long timer = 0;
int x=0;
int y=0;
int z=0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //PC-아두이노간 TTL통신
  rs485.begin(9600); //아두이노-rs485간 TTL통신
  pinMode(modepin,OUTPUT); //송수신제어핀
  digitalWrite(modepin,readmode); //수신모드

  Wire.begin();
  mpu.begin();
  Serial.println(F("Calculating gyro offset, do not move MPU6050"));
  delay(1000);
  mpu.calcGyroOffsets();
  Serial.println("Done!\n");
}
void loop() {
  // put your main code here, to run repeatedly:
  mpu.update();
  
  if((millis()-timer)>10){ // print data every 10ms
    x = mpu.getAngleX()+90;
    y = mpu.getAngleY()+90;
    z = mpu.getAngleZ()+90;
    if(x < 0) x=0;
    if(x>180) x=180;
    if(y<0)y=0;
    if(y>180)y=180;
    if(z<0)z=0;
    if(z>180)z=180;
  timer = millis();  
  }
  
  if(rs485.available()){
    //485모듈로 들어오는 값이 존재한다면
    byte recv[10];
    rs485.readBytes(recv,10);
    if(recv[0] != slave1) return; //쌩~

    /*
    for(int i =0;i<10;i++){
      Serial.print(recv[i],HEX);
      Serial.print(",");
    }
    Serial.println(); 
    */
    

    byte data[10];
    data[0] = recv[0];
    data[1] = recv[1];
    //응답데이터
    data[2] = x;
    data[3] = y;
    data[4] = z;
    for(int i =5;i<10;i++){
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