#include <avr/io.h>
#include <avr/interrupt.h>
#include <Arduino.h>

volatile bool servo_return_flag = false;
volatile bool delay_flag = false;

// 서보 모터 제어 함수
void servoRun(int degree)
{
    uint16_t degValue;

    if (degree < 0)
    {
        degree = 0;
    }
    else if (degree > 180)
    {
        degree = 180;
    }

    degValue = (uint16_t)((degree / 180.0) * (5000 - 1000) + 1000);
    OCR1A = degValue;
}

int initialAngle = 90;  // 초기 각도
int currentAngle = 90;

void setup() {
  Serial.begin(9600);
  cli();

  // 서보 모터 설정
  DDRB |= (1 << PB1);  // PB1 핀을 출력으로 설정
  TCCR1A = 0;
  TCCR1B = 0;

  // PWM 모드 14
  TCCR1B |= (1 << WGM13) | (1 << WGM12);
  TCCR1A |= (1 << WGM11);
  TCCR1A |= (1 << COM1A1);
  TCCR1B |= (1 << CS11);

  // TOP 값
  ICR1 = 39999;

  // 초기 서보 모터 위치 설정
  servoRun(initialAngle);

  // SPI 설정
  DDRB &= ~((1 << PB3) | (1 << PB5)); // MOSI, SCK를 입력으로 설정
  DDRB |= (1 << PB4); // MISO를 출력으로 설정
  SPCR |= (1 << SPE); // SPI 활성화, Slave 모드 설정

  // 타이머 설정 (타이머 0 사용)
  TCCR0A = 0;
  TCCR0B = 0;
  TCCR0A |= (1 << WGM01); // CTC 모드
  OCR0A = 156; // 0.01초마다 인터럽트 발생 (8MHz / 1024 / 100)
  TIMSK0 |= (1 << OCIE0A); // 타이머 인터럽트 활성화
  TCCR0B |= (1 << CS02) | (1 << CS00); // 프리스케일러 1024 설정

  sei();
}

void loop() {
  // SPI 통신을 통해 데이터 수신
  if (SPSR & (1 << SPIF)) {
    uint8_t receivedAngle1 = SPDR;
    while (!(SPSR & (1 << SPIF)));
    uint8_t receivedAngle2 = SPDR;

    Serial.print("Received angles: ");
    Serial.print(receivedAngle1);
    Serial.print(", ");
    Serial.println(receivedAngle2);

    // 0.8초 지연을 위한 타이머 플래그 설정
    delay_flag = true;
  }
}

// 타이머 0 비교 일치 인터럽트 서비스 루틴
ISR(TIMER0_COMPA_vect) {
  static uint16_t delay_count = 0;
  static uint16_t return_count = 0;

  if (delay_flag) {
    delay_count++;
    if (delay_count >= 80) { // 0.01초 * 80 = 0.8초
      // 서보 모터 -45도 회전
      currentAngle = initialAngle - 45;
      if (currentAngle < 0) currentAngle = 0;

      servoRun(currentAngle);
      Serial.print("Motor moved to: ");
      Serial.print(currentAngle);
      Serial.println(" degrees");

      // 0.8초 후 원위치로 돌아오도록 타이머 플래그 설정
      servo_return_flag = true;
      delay_flag = false;
      delay_count = 0;
    }
  }

  if (servo_return_flag) {
    return_count++;
    if (return_count >= 80) { // 0.01초 * 80 = 0.8초
      servoRun(initialAngle);
      Serial.print("Returned to: ");
      Serial.print(initialAngle);
      Serial.println(" degrees");
      servo_return_flag = false;
      return_count = 0;
    }
  }
}