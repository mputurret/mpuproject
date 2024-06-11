#include <Servo.h>

Servo myServo;  
const int servoPin = 9;  

const int commPin1 = 10;
const int commPin2 = 11;
const int commPin3 = 12;
const int commPin4 = 13;

const int defAng = 90;  
const int pressAng = 45; 

void setup() {
  myServo.attach(servoPin);  
  myServo.write(defAng);

  // 통신 핀
  pinMode(commPin1, INPUT);
  pinMode(commPin2, INPUT);
  pinMode(commPin3, INPUT);
  pinMode(commPin4, INPUT);

  Serial.begin(9600);
}

void loop() {
  // 통신 핀의 상태 read
  int commState1 = digitalRead(commPin1);
  int commState2 = digitalRead(commPin2);
  int commState3 = digitalRead(commPin3);
  int commState4 = digitalRead(commPin4);

  if (commState1 == HIGH && commState2 == HIGH && commState3 == HIGH && commState4 == HIGH) {
    myServo.write(pressAng);
  } else {
    myServo.write(defAng);  // 버튼이 안 눌렸을 때 서보 각도를 set
  }

  delay(15);  // 모터가 위치로 이동할 시간 줌
}
