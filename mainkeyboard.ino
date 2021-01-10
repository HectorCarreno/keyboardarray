#include <string.h>
#include <arduino.h>

#define size_array(x) sizeof(x) / sizeof(x[0])
#define period_t 1000
//#define BTN {"ENG", "BLEED", "PRESS", "ELEC", "HYD", "FUEL", "APU", "COND", "DOOR", "WHELL", "F/CTL", "ALL", "CLR", "TO CONFIG", "SYS", "RCL", "EMER/CANC", "CLR"}

uint8_t COLUMNS[] = {28, 30, 32, 34, 36, 38};
uint8_t ROWS[] = {26, 24, 22};
uint8_t idx, jdx, tdx;
const char *BTN[size_array(ROWS)][size_array(COLUMNS)] = {"ENG", "BLEED", "PRESS", "ELEC", "HYD", "FUEL", "APU", "COND", "DOOR", "WHELL", "F/CTL", "ALL", "CLR", "TO CONFIG", "SYS", "RCL", "EMER/CANC", "CLR"};

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  Serial.println("-> Keyboard array was configurated thus: ");
  
  Serial.print("!Size of rows: ");
  Serial.print(size_array(ROWS));
  Serial.print(" Size of columns: ");
  Serial.println(size_array(COLUMNS));

  for(idx = 0; idx < size_array(COLUMNS);++idx)
  {
    Serial.println();
    for(jdx = 0; jdx < size_array(ROWS); ++jdx)
    {
      Serial.print("# Key position: Row ");
      Serial.print(jdx);
      Serial.print(" Column ");
      Serial.print(idx);
      Serial.print(" Button: ");
      Serial.println(BTN[jdx][idx]);
      delay(20);
    }    
  }
  Serial.println();
  initializing_pins();
  Serial.print("Starting MainKeyboard") ;
  for (idx = 0; idx < random(10, 30); idx++)
  {
    Serial.print(".");
    delay(250);
  }
  Serial.print("Done!");
  Serial.println(".");
  delay(1000);
  Serial.print("Ready for press button!");
  for (idx = 0; idx < random(4, 10); idx++)
  {
    Serial.print(".");
    delay(50);
  }
  for (idx = 0; idx < 3; idx++)
  {
    Serial.println();
  }
  Serial.print("-> ");
}

static void initializing_pins()
{
  for (idx = 0; idx < 14; idx++)
  {
    Serial.print("#");
    delay(10);
  }
  Serial.print(" MAINKEYBOARD ");
  for (idx = 0; idx < 9; idx++)
  {
    Serial.print("#");
    delay(10);
  }
  Serial.println("#");
  for (idx = 0; idx < 8; idx++)
  {
    Serial.print("#");
    delay(10);
  }
  Serial.print(" MCDU AEROSOFT A320 SIMULATOR ");
  for (idx = 0; idx < 8; idx++)
  {
    Serial.print("#");
    delay(10);
  }
  Serial.println("#");
  delay(random(500, 1000));
  for (idx = 0; idx < 4; idx++)
  {
    Serial.println("");
  }
  delay(10);
  Serial.print("Setting inputs");
  for (idx = 0; idx < 10; idx++)
  {
    Serial.print(".");
    delay(10);
  }
  Serial.println(".");
  delay(random(250, 1000));
  for (idx = 0; idx < size_array(ROWS); idx++)
  {
    Serial.print("Set pin ");
    Serial.println(ROWS[idx]);
    pinMode(ROWS[idx], INPUT_PULLUP);
    delay(100);
  }
  for (idx = 0; idx < 2; idx++)
  {
    Serial.println();
  }
  Serial.print("Setting outputs");
  for (idx = 0; idx < random(4, 10); idx++)
  {
    Serial.print(".");
    delay(100);
  }
  Serial.println(".");
  delay(1000);
  for (idx = 0; idx < size_array(COLUMNS); idx++)
  {
    Serial.print("Set pin ");
    Serial.println(COLUMNS[idx]);
    pinMode(COLUMNS[idx], OUTPUT);
    delay(100);
    digitalWrite(COLUMNS[idx], LOW);
  }
  for (idx = 0; idx < 2; idx++)
  {
    Serial.println("");
  }
}

static void pin_rst()
{
  for (tdx = 0; tdx < size_array(COLUMNS); tdx++)
  {
    digitalWrite(COLUMNS[tdx], HIGH);
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
  for (jdx = 0; jdx < size_array(COLUMNS); jdx++)
  {
    pin_rst();
    digitalWrite(COLUMNS[jdx], LOW);
    for (idx = 0; idx < size_array(ROWS); idx++)
    {
      if (!digitalRead(ROWS[idx]))
      {
        Serial.println(BTN[idx][jdx]);
        delay(500);
        Serial.flush();
      }
    }
  }
}
