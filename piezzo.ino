#include <avr/interrupt.h>

#define BUZZER_PIN 3
#define BUZZER_PIN_MASK (1 << PD3)
#define BUZZER_FREQ 440.00

volatile bool trigger_interrupt = false;

void setup() {
  Serial.begin(9600);
  cli();
  DDRD |= BUZZER_PIN_MASK;

  TCCR2A |= (1 << WGM21) | (1 << WGM20);
  TCCR2A |= (1 << COM2B1);
  TCCR2B |= (1 << CS22) | (1 << CS21);
  OCR2A = (F_CPU / 256 / BUZZER_FREQ) - 1;

  sei();
}

void loop() {
  if (Serial.available() > 0) {
    while (Serial.available() > 0) {
      Serial.read();
    }
    trigger_interrupt = true;
    DDRD |= BUZZER_PIN_MASK;
    TIMSK2 |= (1 << OCIE2A);

    if (trigger_interrupt) {
      trigger_interrupt = false;
      delay(1000);
      TIMSK2 &= ~(1 << OCIE2A);
      PORTD &= ~BUZZER_PIN_MASK;
    }
  } else {
    DDRD &= ~BUZZER_PIN_MASK;
  }
}

ISR(TIMER2_COMPA_vect) {
  PORTD ^= BUZZER_PIN_MASK;
}
