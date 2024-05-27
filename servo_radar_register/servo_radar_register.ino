#define OC2A 0x08

const int servoPin = 11; // 서보 모터가 연결된 핀
int currentAngle = 0;
int angle;

void setup() {
  Serial.begin(9600);    // 시리얼 통신 시작
  DDRB |= OC2A;

  // 타이머 2 설정 (서보 모터 제어를 위해 20ms 주기로 설정)
  TCCR2A = 0;           // 타이머 2의 레지스터 A 클리어
  TCCR2B = 0;           // 타이머 2의 레지스터 B 클리어
  TCNT2 = 0;            // 타이머 2 카운터 초기화

  // Fast PWM 모드 설정, 비교 출력 모드: 비반전, 프리스케일러 64
  TCCR2A |= (1 << WGM21) | (1 << WGM20) | (1 << COM2A1);
  TCCR2B |= (1 << WGM22) | (1 << CS22);

  // TOP 값 설정 (20ms 주기)
  OCR2A = 249; // 주기: (16MHz / (64 * 256)) ≈ 1kHz, 목표 주기: 20ms (50Hz)
  
  // 초기 각도로 설정하여 서보 모터를 테스트
  int pwmValue = map(0, 0, 180, 1000, 5000);
  OCR2A = pwmValue;
  delay(1000); // 초기화 후 서보 모터가 안정될 시간을 줍니다.
}

void loop() {
  for (int angle = 0; angle<= 180; angle += 10){
    int pwmValue = map(angle, 0, 180, 1000, 5000);
    OCR2A = pwmValue;
    delayMicroseconds(500);
  }
}

void setServoAngle(int angle) {
  int pulseWidth = map(angle, 0, 180, 31, 62); // 1ms (0도) ~ 2ms (180도)
  OCR2B = pulseWidth; // 비교 값 설정
}
