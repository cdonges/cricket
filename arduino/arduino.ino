#include <Servo.h>
Servo myservo;

void setup() 
{
  Serial.begin(38400);
  myservo.attach(11);
}

void loop() 
{
  if (Serial.available() > 0) 
  {
    //Read buffer
    char inputByte = Serial.read();
    
    if (inputByte == 'w')
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
     }
  }
}
