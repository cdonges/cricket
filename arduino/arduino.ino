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
        int angle=0;
        while (angle <=90)
        {
            myservo.write(angle);
            delay(10);
            angle++;
        }
        while (angle > 0)
        {
            myservo.write(angle);
            delay(10);
            angle--;
        }
     }
  }
}
