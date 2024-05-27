#include <avr/io.h>

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
        OCR1A = degValue;  // Motor 1 (핀 9)
    } else if (motor == 2) {
        OCR1B = degValue;  // Motor 2 (핀 10)
    }
}

int currentAngle1 = 90;
int currentAngle2 = 90;

void setup() {
  Serial.begin(9600);
  DDRB |= (1 << PB1) | (1 << PB2);

  TCCR1A = 0;
  TCCR1B = 0;

  TCCR1B |= (1 << WGM13) | (1 << WGM12);
  TCCR1A |= (1 << WGM11);

  TCCR1A |= (1 << COM1A1) | (1 << COM1B1);

  TCCR1B |= (1 << CS11);

  ICR1 = 39999;

  servoRun(1, currentAngle1);
  servoRun(2, currentAngle2);

  Serial.println("Enter angle changes (-180 to 180) for motor 1 and motor 2 separated by a comma (e.g., 10,-20):");
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    int commaIndex = input.indexOf(',');
    if (commaIndex > 0) {
      int angleChange1 = input.substring(0, commaIndex).toInt();
      int angleChange2 = input.substring(commaIndex + 1).toInt();

      if (angleChange1 >= -180 && angleChange1 <= 180) {
        currentAngle1 += angleChange1;
        if (currentAngle1 < 0) currentAngle1 = 0;
        if (currentAngle1 > 180) currentAngle1 = 180;

        servoRun(1, currentAngle1);
        Serial.print("Motor 1 moved to: ");
        Serial.print(currentAngle1);
        Serial.println(" degrees");
      } else {
        Serial.println("Invalid angle change for motor 1. Enter a value between -180 and 180.");
      }

      if (angleChange2 >= -180 && angleChange2 <= 180) {
        currentAngle2 += angleChange2;
        if (currentAngle2 < 0) currentAngle2 = 0;
        if (currentAngle2 > 180) currentAngle2 = 180;

        servoRun(2, currentAngle2);
        Serial.print("Motor 2 moved to: ");
        Serial.print(currentAngle2);
        Serial.println(" degrees");
      } else {
        Serial.println("Invalid angle change for motor 2. Enter a value between -180 and 180.");
      }

    } else {
      Serial.println("Invalid input format. Please enter two angle changes separated by a comma.");
    }

    Serial.println("Enter angle changes (-180 to 180) for motor 1 and motor 2 separated by a comma (e.g., 10,-20):");

    while (Serial.available() > 0) {
      Serial.read();
    }
  }
}
