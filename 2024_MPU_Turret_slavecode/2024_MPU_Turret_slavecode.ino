#include <avr/io.h>
#include <avr/interrupt.h>
#include <Arduino.h>

volatile bool servo_return_flag = false;
volatile bool delay_flag = false;

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

int initialAngle = 90; 
int currentAngle = 90;

void setup() {
  Serial.begin(9600);
  cli();
  DDRB |= (1 << PB1);
  TCCR1A = 0;
  TCCR1B = 0;

  TCCR1B |= (1 << WGM13) | (1 << WGM12);
  TCCR1A |= (1 << WGM11);
  TCCR1A |= (1 << COM1A1);
  TCCR1B |= (1 << CS11);

  ICR1 = 39999;

  servoRun(initialAngle);

  DDRB &= ~((1 << PB3) | (1 << PB5));
  DDRB |= (1 << PB4); 
  SPCR |= (1 << SPE); 

  TCCR0A = 0;
  TCCR0B = 0;
  TCCR0A |= (1 << WGM01); 
  OCR0A = 156; 
  TIMSK0 |= (1 << OCIE0A); 
  TCCR0B |= (1 << CS02) | (1 << CS00);
  sei();
}

void loop() {
  // SPI 통신
  if (SPSR & (1 << SPIF)) {
    uint8_t receivedAngle1 = SPDR;
    while (!(SPSR & (1 << SPIF)));
    uint8_t receivedAngle2 = SPDR;

    Serial.print("Received angles: ");
    Serial.print(receivedAngle1);
    Serial.print(", ");
    Serial.println(receivedAngle2);

    // 0.8초 지연
    delay_flag = true;
  }
}

ISR(TIMER0_COMPA_vect) {
  static uint16_t delay_count = 0;
  static uint16_t return_count = 0;

  if (delay_flag) {
    delay_count++;
    if (delay_count >= 60) {
      currentAngle = initialAngle - 45;
      if (currentAngle < 0) currentAngle = 0;

      servoRun(currentAngle);
      Serial.print("Motor moved to: ");
      Serial.print(currentAngle);
      Serial.println(" degrees");
      servo_return_flag = true;
      delay_flag = false;
      delay_count = 0;
    }
  }

  if (servo_return_flag) {
    return_count++;
    if (return_count >= 60) {
      servoRun(initialAngle);
      Serial.print("Returned to: ");
      Serial.print(initialAngle);
      Serial.println(" degrees");
      servo_return_flag = false;
      return_count = 0;
    }
  }
}
