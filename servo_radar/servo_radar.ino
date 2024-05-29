#include <Servo.h>

const int trigPin = 9;   // 초음파 센서의 Trig 핀
const int echoPin = 10;  // 초음파 센서의 Echo 핀
const int servoPin = 11;        // 서보 모터 객체 생성

Servo radarServo;

void setup() {
  Serial.begin(9600);    // 시리얼 통신 시작
  radarServo.attach(servoPin);  // 서보 모터를 Digital Pin 11에 연결
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  for (int angle = 0; angle < 180; angle += 10) {
    // 서보 모터를 현재 각도로 회전
    radarServo.write(angle);
    delay(500);

    // 초음파 발사
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // 초음파 수신 및 거리 계산
    long duration = pulseIn(echoPin, HIGH);
    int distance = duration * 0.034 / 2;

    // 거리와 각도를 시리얼 모니터에 출력
    Serial.print("Angle: ");
    Serial.print(angle);
    Serial.print("  Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }

    for (int angle = 180; angle > 0; angle -= 10) {
    // 서보 모터를 현재 각도로 회전
    radarServo.write(angle);
    delay(500);

    // 초음파 발사
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // 초음파 수신 및 거리 계산
    long duration = pulseIn(echoPin, HIGH);
    int distance = duration * 0.034 / 2;

    // 거리와 각도를 시리얼 모니터에 출력
    Serial.print("Angle: ");
    Serial.print(angle);
    Serial.print("  Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }


  delay(500);  // 1초 대기
}