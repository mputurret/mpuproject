#include <Servo.h>

Servo servoX; 
Servo servoY;  

// pin 설정을 해봅시다
const int buttonPin = 2;
const int commPin1 = 4;
const int commPin2 = 11;
const int commPin3 = 12;
const int commPin4 = 13;

const int joyX = A0; 
const int joyY = A1; 

const int centerValue = 512; 
const int delayTime = 15; 

int currentAngleX = 90; // X축 모터 현재각, 초기값 = 90도
int currentAngleY = 90;

unsigned long previsec = 0; // 이전 시간
const unsigned long interval = 10; // 회전

void setup() {
  servoX.attach(9); // X축 
  servoY.attach(10); // Y축

//2
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(commPin1, OUTPUT);
  pinMode(commPin2, OUTPUT);
  pinMode(commPin3, OUTPUT);
  pinMode(commPin4, OUTPUT);
//

  Serial.begin(9600);
}

void loop() {

  unsigned long currmillis = millis();

  if (currmillis - previsec >= interval) {
    int xValue = analogRead(joyX);
    int yValue = analogRead(joyY);

    // X축 모터 회전 제어
    // deadzone은 150 정도가 적당한 듯
    if ((xValue < centerValue - 150) || (xValue > centerValue + 150)) {
      int targetAngleX = map(xValue, 0, 1023, 0, 180); // 조이스틱 값 -> 각도 변환

      // 부드러운 이동 구현
      if (targetAngleX > currentAngleX) {
        currentAngleX = min(currentAngleX + 1, targetAngleX);
      } else if (targetAngleX < currentAngleX) {
        currentAngleX = max(currentAngleX - 1, targetAngleX);
      }
      
      servoX.write(currentAngleX);
    }

    // Y축 모터 회전 제어
    if ((yValue < centerValue - 150) || (yValue > centerValue + 150)) {
      int targetAngleY = map(yValue, 0, 1023, 0, 180); 

      if (targetAngleY > currentAngleY) {
        currentAngleY = min(currentAngleY + 1, targetAngleY);
      } else if (targetAngleY < currentAngleY) {
        currentAngleY = max(currentAngleY - 1, targetAngleY);
      }
      
      servoY.write(currentAngleY);
    }

    previsec = currmillis;
  }

//3 야매통신
  int buttonState = digitalRead(buttonPin);

  if (buttonState == LOW) {
    digitalWrite(commPin1, HIGH);
    digitalWrite(commPin2, HIGH);
    digitalWrite(commPin3, HIGH);
    digitalWrite(commPin4, HIGH);
  } else {
    digitalWrite(commPin1, LOW);
    digitalWrite(commPin2, LOW);
    digitalWrite(commPin3, LOW);
    digitalWrite(commPin4, LOW);
  }
//

  delay(delayTime);
}
