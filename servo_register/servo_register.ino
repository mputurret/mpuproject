int currentAngle = 90;  // 서보모터의 현재 각도 (초기값은 90도로 설정)
int angleChange;        // 입력된 각도 변화를 저장할 변수

void setup() {
  Serial.begin(9600);    // 시리얼 통신 시작
  pinMode(9, OUTPUT);    // 핀 9을 출력으로 설정

  // Timer1 설정
  TCCR1A = 0;            // 타이머/카운터 컨트롤 레지스터 A 초기화
  TCCR1B = 0;            // 타이머/카운터 컨트롤 레지스터 B 초기화

  // 고속 PWM 모드 14 설정: WGM13 = 1, WGM12 = 1, WGM11 = 1, WGM10 = 0
  TCCR1B |= (1 << WGM13) | (1 << WGM12);
  TCCR1A |= (1 << WGM11);

  // 비교 출력 모드 설정: 비반전 모드 (OC1A 핀, 핀 9 사용)
  TCCR1A |= (1 << COM1A1);

  // 분주비 설정: 8분주 (타이머 클럭 소스 선택)
  TCCR1B |= (1 << CS11);

  // TOP 값 설정 (20ms 주기, 서보 모터 제어에 적합)
  ICR1 = 39999;

  // 초기 각도로 서보모터 설정
  int pwmValue = map(currentAngle, 0, 180, 1000, 5000);
  OCR1A = pwmValue;

  Serial.println("Enter angle change (-180 to 180):");  // 안내 메시지 출력
}

void loop() {
  if (Serial.available()) {  // 시리얼 입력이 있는 경우
    angleChange = Serial.parseInt();  // 입력된 각도 변화를 정수로 변환
    if (angleChange >= -180 && angleChange <= 180) {  // 각도 변화가 -180~180 범위 내인지 확인
      currentAngle += angleChange;  // 현재 각도에 각도 변화를 더함
      if (currentAngle < 0) currentAngle = 0;  // 최소 각도 제한
      if (currentAngle > 180) currentAngle = 180;  // 최대 각도 제한
      
      // 각도를 PWM 듀티 사이클로 변환
      int pwmValue = map(currentAngle, 0, 180, 1000, 5000);
      OCR1A = pwmValue;  // 서보모터를 입력된 각도로 회전
      Serial.print("Moved to: ");
      Serial.print(currentAngle);
      Serial.println(" degrees");
    } else {
      Serial.println("Invalid angle change. Enter a value between -180 and 180.");
    }
    Serial.println("Enter angle change (-180 to 180):");  // 다음 입력 안내 메시지 출력

    // 시리얼 버퍼 비우기
    while (Serial.available() > 0) {
      Serial.read();
    }
  }
}