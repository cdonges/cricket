#include <Servo.h>
Servo myservo;

int dataPin = 2;
int latchPin = 3;
int clockPin = 4;
int servoPin = 11;

int prevWickets = -1;

void setup() 
{
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

      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, wickets);
      digitalWrite(latchPin, HIGH);

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
    }
  }
}





