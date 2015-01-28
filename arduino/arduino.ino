#include <Servo.h>
#include <LiquidCrystal.h>

Servo myservo;

int servoPin = 9;

int prevWickets = -1;

char display2[17];

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() 
{
  Serial.begin(38400);
//  myservo.attach(servoPin);
  
  lcd.begin(16, 2);
  
  lcd.print("Aussie! Aussie!");
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

    if (inputByte == '1' || inputByte == '2')
    {
      int charsRead = Serial.readBytesUntil('\n', display2, 16);
      if (charsRead > 0)
      {
        display2[charsRead] = '\0';
        lcd.setCursor(0, inputByte == '1' ? 0 : 1);
        lcd.print(display2);
      }
    }
  }
}





