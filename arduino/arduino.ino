#include <LiquidCrystal.h>

int piezo = 10;

int prevWickets = -1;

char d1a[30] = " ";
char d1b[8] = " ";

char d2a[30] = " ";
char d2b[8] = " ";

int pos = 0;
int longest = 0;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup()
{
  Serial.begin(38400);

  pinMode(piezo, OUTPUT);

  lcd.begin(16, 2);
  lcd.clear();
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

  for (int i = 0; i < 9; i++)
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

  if (Serial.available() > 0)
  {
    //Read buffer
    char inputByte = Serial.read();

    if (inputByte == 'w')
    {
      int wickets = Serial.parseInt();
    }

    if (inputByte == 'r')
    {
      int runs = Serial.parseInt();
    }

    if (inputByte == '1' || inputByte == '2')
    {
      char teamOrScore = Serial.read();

      char* buff;

      if (teamOrScore == 't')
      {
        if (inputByte == '1')
        {
          buff = d1a;
        }
        else if (inputByte == '2')
        {
          buff = d2a;
        }
        
          longest = max(strlen(d1a), strlen(d2a));
      }

      if (teamOrScore == 's')
      {
        if (inputByte == '1')
        {
          buff = d1b;
        }
        else if (inputByte == '2')
        {
          buff = d2b;
        }
      }

      int charsRead = Serial.readBytesUntil('\n', buff, teamOrScore == 't' ? 29 : 7);
      if (charsRead > 0)
      {
        buff[charsRead] = '\0';
      }
    }
  }
}






