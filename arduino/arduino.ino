#include <Servo.h>
Servo myservo;

int dataPin = 2;
int latchPin = 3;
int clockPin = 4;
int overflowPin = 5;
int servoPin = 11;

int wicketsPins[] = { 
  6, 7, 8, 9 };

int prevWickets = -1;

void setup() 
{
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(overflowPin, OUTPUT);

  for (int i = 0; i < 4; i++)
  {
    pinMode(wicketsPins[i], OUTPUT);
  }

  Serial.begin(38400);
  myservo.attach(servoPin);
}

void loop() 
{
  if (Serial.available() > 0) 
  {
    //Read buffer
    char inputByte = Serial.read();

    if (inputByte == 'w')
    {
      int wickets = Serial.parseInt();

      for (int i = 0; i < 4; i++)
      {
        digitalWrite(wicketsPins[i], (wickets >> i) & 1 ? HIGH : LOW);
      }

      if (prevWickets != wickets)
      {
        for (int i = 0; i < 10; i++)
        {
          int angle=0;
          while (angle <=90)
          {
            myservo.write(angle);
            delay(5);
            angle++;
          }
          while (angle > 10)
          {
            myservo.write(angle);
            delay(5);
            angle--;
          }
        }
        prevWickets = wickets;
      }
    }

    if (inputByte == 'r')
    {
      int runs = Serial.parseInt();

      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, runs);
      digitalWrite(latchPin, HIGH);

      digitalWrite(overflowPin, (runs >> 8) & 1 ? HIGH : LOW);
    }
  }
}







