# RS485 Multi-Node Communication and Control

Arduino와 MAX485 모듈을 활용하여 다중 노드 간 RS485 통신 및 원격 장치 제어 시스템을 구현한 프로젝트입니다.

마스터·슬레이브 구조를 기반으로 여러 장치 간 데이터 송수신을 수행하고, 자체 정의한 10바이트 통신 프레임을 이용하여 장치 주소 구분, 센서 및 제어 데이터 전송, RFID 인증, 모터 제어 등을 구현하였습니다.

또한 USB-RS485 변환기와 C# WinForms를 이용한 PC 기반 장치 제어 기능을 실습하였습니다.

## 1. Development Environment

- **MCU:** Arduino
- **RS485 Transceiver:** MAX485
- **IDE:** Arduino IDE, Visual Studio
- **Language:** C/C++ (Arduino), C#
- **Communication:** UART, RS485, SPI, I2C
- **PC Interface:** USB-RS485, C# WinForms

## 2. Hardware Components

| Component | Description |
|---|---|
| Arduino | 마스터 및 슬레이브 제어 |
| MAX485 | UART와 RS485 차동 신호 변환 |
| RC522 RFID | RFID 태그 UID 인식 |
| 1602 I2C LCD | 수신 데이터 및 상태 표시 |
| Push Buttons | 제어 명령 입력 |
| Potentiometer | 아날로그 제어값 입력 |
| DC Motor / Motor Driver | 방향 및 속도 제어 |
| TM1637 FND | 숫자 데이터 표시 |
| LEDs | 제어 및 인증 상태 표시 |
| USB-RS485 Converter | PC와 RS485 네트워크 연결 |

## 3. Communication Interfaces

| Interface | Application |
|---|---|
| UART | MCU와 MAX485 간 직렬 데이터 송수신 |
| RS485 | 다중 노드 간 차동 신호 통신 |
| SPI | RC522 RFID 모듈 제어 |
| I2C | LCD 제어 |
| GPIO | 버튼 및 LED 제어 |
| PWM | DC모터 속도 제어 |
| USB-RS485 | PC 기반 장치 제어 및 모니터링 |

## 4. Main Features

### RS485 Communication
- MAX485 기반 다중 노드 통신 구현
- 마스터 1대 및 슬레이브 2대 구성 실습
- 송수신 방향 제어를 이용한 반이중 통신
- 슬레이브 주소를 이용한 장치 구분
- 요청·응답 방식의 데이터 송수신

### Custom Communication Protocol
- 10바이트 고정 길이 통신 프레임 구성
- 슬레이브 ID 및 명령 코드에 따른 메시지 처리
- 제어값과 센서 데이터 송수신
- 슬레이브 응답 메시지 확인
- 타임아웃을 통한 응답 상태 판단

### RFID-Based Control
- RC522 모듈을 이용한 RFID UID 인식
- RFID UID 데이터의 RS485 전송
- 슬레이브 LCD에 UID 표시
- 등록된 UID 비교 및 LED 인증 결과 표시

### DC Motor Control
- 버튼을 이용한 모터 회전 방향 설정
- 가변저항을 통한 PWM 제어값 생성
- RS485를 이용한 방향 및 속도 명령 전송
- 슬레이브에서 수신 명령에 따라 DC모터 제어
- LED 및 FND를 이용한 제어 상태 표시

### PC-Based Control
- USB-RS485 변환기를 이용한 PC 통신
- C# WinForms 기반 장치 제어 GUI
- 슬레이브 주소를 이용한 장치별 명령 전송
- 수신 응답 및 장치 상태 확인
- FND 표시값 원격 제어

## 5. Source Code

| Directory | Description |
|---|---|
| `RS485#1/` | RS485 통신 기초 실습 |
| `RS485#2/` | 다중 노드 통신 실습 |
| `RS485#3/` | RS485 기반 장치 제어 실습 |
| `RS485#4 아두이노` | PC 연동 슬레이브 제어 코드 |
| `RS485#4 윈폼` | C# WinForms 제어 프로그램 |

본 저장소는 RS485 통신을 활용한 임베디드 다중 노드 제어 실습 소스코드를 정리한 것입니다.

## 6. Project Documentation

프로젝트의 상세한 통신 프레임 구성, 하드웨어 연결, 구현 과정 및 동작 검증 결과는 별도의 포트폴리오 자료에서 확인할 수 있습니다.

- **Project Report & Demonstration:** https://drive.google.com/drive/u/1/folders/1Xw5DMm6TW0g0qNxZ1qj8qqkJahJMWlgc

## 7. Development Notes

본 프로젝트에서는 UART와 MAX485를 이용하여 RS485 다중 노드 통신 환경을 구성하고, 자체 정의한 통신 프레임을 통해 여러 장치를 제어하였습니다.

특히 송수신 방향 제어, 슬레이브 주소 구분, 명령 코드 처리, 응답 확인 및 타임아웃 기능을 구현하여 RS485 멀티드롭 통신의 동작 원리를 학습하였습니다.

또한 C# WinForms와 USB-RS485 변환기를 이용하여 PC에서 개별 장치를 제어하는 환경을 구성하였습니다.

본 실습의 자체 프로토콜은 Modbus RTU와 구분됩니다.
