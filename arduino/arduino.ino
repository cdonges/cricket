#include <Servo.h>
Servo myservo;

int dataPin = 2;
int latchPin = 3;
int clockPin = 4;
int runs9 = 5;
int runs10 = 10;
int servoPin = 11;

int wicketsPins[] = { 
  6, 7, 8, 9 };

int prevWickets = -1;

void setup() 
{
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(runs9, OUTPUT);
  pinMode(runs10, OUTPUT);

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

    if (inputByte == 'r')
    {
      int runs = Serial.parseInt();

      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, runs);
      digitalWrite(latchPin, HIGH);

      digitalWrite(runs9, (runs >> 8) & 1 ? HIGH : LOW);
      digitalWrite(runs10, (runs >> 9) & 1 ? HIGH : LOW);
    }
    
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
  }
}







