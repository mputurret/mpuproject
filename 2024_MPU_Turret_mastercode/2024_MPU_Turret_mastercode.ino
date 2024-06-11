#include <avr/io.h>
#include <Arduino.h>

#define BUZZER_PIN 3
#define BUZZER_PIN_MASK (1 << PD3)
#define BUZZER_FREQ 440.00

volatile bool buzzer_active = false;
volatile uint16_t buzzer_count = 0;

void servoRun(uint8_t motor, uint8_t degree)
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

    if (motor == 1) {
        OCR1A = degValue;
    } else if (motor == 2) {
        OCR1B = degValue;
    }
}

int currentAngle1 = 90;
int currentAngle2 = 90;

void setup() {
  Serial.begin(9600);
  DDRB |= (1 << PB1) | (1 << PB2);
  DDRB |= (1 << PB3) | (1 << PB5) | (1 << PB2);
  SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0);
  DDRD |= BUZZER_PIN_MASK;
  TCCR2A = 0;
  TCCR2B = 0;
  TCCR2A |= (1 << WGM21) | (1 << WGM20);
  TCCR2A |= (1 << COM2B0);
  TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);
  OCR2A = (F_CPU / 1024 / BUZZER_FREQ) - 1;

  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= (1 << WGM13) | (1 << WGM12);
  TCCR1A |= (1 << WGM11);
  TCCR1A |= (1 << COM1A1) | (1 << COM1B1);
  TCCR1B |= (1 << CS11);
  ICR1 = 39999;

  servoRun(1, currentAngle1);
  servoRun(2, currentAngle2);
}

uint8_t spi_transfer(uint8_t data) {
  SPDR = data;
  while (!(SPSR & (1 << SPIF)));
  return SPDR;
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    Serial.println(input);
    
    int commaIndex = input.indexOf(',');
    if (commaIndex > 0) {
      float value1 = input.substring(0, commaIndex).toFloat();
      float value2 = input.substring(commaIndex + 1).toFloat();

      int angleChange1 = (value1-160) / 5.8;
      int angleChange2 = (value2-128) / 5.8;

      currentAngle1 = angleChange1;
      currentAngle2 = angleChange2;

      if (currentAngle1 < 0) currentAngle1 = 0;
      if (currentAngle1 > 180) currentAngle1 = 180;
      if (currentAngle2 < 0) currentAngle2 = 0;
      if (currentAngle2 > 180) currentAngle2 = 180;

      buzzer_active = true;
      buzzer_count = 0;
      TIMSK2 |= (1 << OCIE2A);

      while (buzzer_active);

      servoRun(1, currentAngle1);
      servoRun(2, currentAngle2);
      DDRB &= ~(1 << 0x04);  
      Serial.println("Sending to slave...");
      spi_transfer(currentAngle1);
      spi_transfer(currentAngle2);
      DDRB |= (1 << 0x04);
    } else {
      Serial.println("Error");
    }

    while (Serial.available() > 0) {
      Serial.read();
    }
  }
}

ISR(TIMER2_COMPA_vect) {
  if (buzzer_active) {
    buzzer_count++;
    if (buzzer_count >= 80) {
      TIMSK2 &= ~(1 << OCIE2A);
      PORTD &= ~BUZZER_PIN_MASK;
      buzzer_active = false;
    } else {
      PORTD ^= BUZZER_PIN_MASK;
    }
  }
}
