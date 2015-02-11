#include <Servo.h>
#include <LiquidCrystal.h>

Servo myservo;

int piezo = 10;
int duration = 250;
int notes[] = {261, 293, 329, 349, 392, 440, 493, 523, 587, 659, 698, 783, 880};

int servoPin = 9;

int prevWickets = -1;

char d1a[30] = "Australia Koala Bears";
char d1b[8] = "  2/222";

char d2a[30] = "India Elephants";
char d2b[8] = "   9/72";

int pos = 0;
int longest = 0;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() 
{
  //Serial.begin(38400);
  //myservo.attach(servoPin);
  
  pinMode(piezo, OUTPUT);
    for (int i = 0; i < 13; i++)
    {
    tone(piezo, notes[i], duration);
    delay(duration);
    }
  
  lcd.begin(16, 2);
  lcd.clear();

  longest = max(strlen(d1a), strlen(d2a));
}

void loop() 
{

      lcd.setCursor(9, 0);
      lcd.print(d1b);

      lcd.setCursor(9, 1);
      lcd.print(d2b);

      if (pos > longest - 9)
      {
        pos = 0;
        delay(1000);
      }

      //lcd.noDisplay();
      for(int i = 0; i < 9; i++)
      {
        if (pos < strlen(d1a) - 8)
        {
          lcd.setCursor(i, 0);
          lcd.print(d1a[i + pos]);
        }
        
        if (pos < strlen(d2a) - 8)
        {
          lcd.setCursor(i, 2);
          lcd.print(d2a[i + pos]);
        }
      }
      pos++;
      
      delay(500);
      
      //lcd.display();

      /*  if (Serial.available() > 0) 
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
  }*/
}





