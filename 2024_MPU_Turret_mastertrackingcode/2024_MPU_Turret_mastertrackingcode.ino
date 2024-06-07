#include <avr/io.h>
#include <Arduino.h>

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
   
    int commaIndex = input.indexOf(',');
    if (commaIndex > 0) {
      float value1 = input.substring(0, commaIndex).toFloat();
      float value2 = input.substring(commaIndex + 1).toFloat();

      int filteredX = value1;
      int filteredY = value2;

      if(filteredX >= 0 && filteredX < 64) {
        if(filteredY >= 0 && filteredY < 50) {
          currentAngle1 += 3;
          currentAngle2 -= 1;
        } else if(filteredY >= 50 && filteredY < 200) {
          currentAngle1 += 3;
          currentAngle2 += 0;
        } else if(filteredY >= 200 && filteredY < 256) {
          currentAngle1 += 3;
          currentAngle2 += 1;
        }
      } else if(filteredX >= 64 && filteredX < 128) {
        if(filteredY >= 0 && filteredY < 50) {
          currentAngle1 += 1.5;
          currentAngle2 -= 1;
        } else if(filteredY >= 50 && filteredY < 200) {
          currentAngle1 += 1.5;
          currentAngle2 += 0;
        } else if(filteredY >= 200 && filteredY < 256) {
          currentAngle1 += 1.5;
          currentAngle2 += 1;
        }
      } else if(filteredX >= 128 && filteredX < 192) {
        if(filteredY >= 0 && filteredY < 50) {
          currentAngle1 -= 0;
          currentAngle2 -= 1;
        } else if(filteredY >= 50 && filteredY < 200) {
          currentAngle1 += 0;
          currentAngle2 -= 0;
        } else if(filteredY >= 200 && filteredY < 256) {
          currentAngle1 += 0;
          currentAngle2 += 1;
        }
      } else if(filteredX >= 192 && filteredX < 256) {
        if(filteredY >= 0 && filteredY < 50) {
          currentAngle1 -= 1.5;
          currentAngle2 -= 1;
        } else if(filteredY >= 50 && filteredY < 200) {
          currentAngle1 -= 1.5;
          currentAngle2 += 0;
        } else if(filteredY >= 200 && filteredY < 256) {
          currentAngle1 -= 1.5;
          currentAngle2 += 1;
        }
      } else if(filteredX >= 256 && filteredX < 320) {
        if(filteredY >= 0 && filteredY < 50) {
          currentAngle1 -= 3;
          currentAngle2 -= 1;
        } else if(filteredY >= 50 && filteredY < 200) {
          currentAngle1 -= 3;
          currentAngle2 += 0;
        } else if(filteredY >= 200 && filteredY < 256) {
          currentAngle1 -= 3;
          currentAngle2 += 1;
        }
      }

      if (currentAngle1 < 0) currentAngle1 = 0;
      if (currentAngle1 > 180) currentAngle1 = 180;
      if (currentAngle2 < 0) currentAngle2 = 0;
      if (currentAngle2 > 180) currentAngle2 = 180;

      servoRun(1, currentAngle1);
      servoRun(2, currentAngle2);

      if(filteredX >= 120 && filteredX < 200 && filteredY >= 90 && filteredY < 150) {
        Serial.println("shooting");

        digitalWrite(PB2, LOW);
        spi_transfer(currentAngle1);
        spi_transfer(currentAngle2);
        digitalWrite(PB2, HIGH);
      }
    } else {
      Serial.println("Error");
    }

    while (Serial.available() > 0) {
      Serial.read();
    }
  } else {
    currentAngle1 += 0;
    currentAngle2 += 0;

    while (Serial.available() > 0) {
      Serial.read();
    }
  }
}
