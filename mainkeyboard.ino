#include <string.h>
#include <arduino.h>

#define size_array(x)   sizeof(x) / sizeof(x[0])
#define period_t    1000

uint8_t COLUMNS[] = {36, 34, 32, 30, 28, 26, 24, 22, 29, 31};
uint8_t ROWS[] = {53, 52, 50, 48, 46, 44, 42, 40, 38, 23, 37, 25, 35, 27, 33};
uint8_t idx, jdx, tdx;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  initializing_pins();
  Serial.print("Starting MainKeyboard");
  for (idx = 0; idx < random(10, 30); idx++) {
    Serial.print(".");
    delay(250);
  }
  Serial.print("Done!");
  Serial.println(".");
  delay(1000);
  Serial.print("Press button!");
  for (idx = 0; idx < random(4, 10); idx++) {
    Serial.print(".");
    delay(50);
  }
  Serial.println(".");
}

static void initializing_pins() {
  for (idx = 0; idx < 14 ; idx++) {
    Serial.print("#");
    delay(10);
  }
  Serial.print(" MAINKEYBOARD ");
  for (idx = 0; idx < 9 ; idx++) {
    Serial.print("#");
    delay(10);
  }
  Serial.println("#");
  for (idx = 0; idx < 8 ; idx++) {
    Serial.print("#");
    delay(10);
  }
  Serial.print(" MCDU AEROSOFT A320 SIMULATOR ");
  for (idx = 0; idx < 8 ; idx++) {
    Serial.print("#");
    delay(10);
  }
  Serial.println("#");
  delay(random(500, 1000));
  for (idx = 0 ; idx < 4 ; idx++) {
    Serial.println("");
  }
  delay(10);
  Serial.print("Setting inputs");
  for (idx = 0; idx < 10; idx++) {
    Serial.print(".");
    delay(250);
  }
  Serial.println(".");
  delay(random(250, 1000));
  for (idx = 0; idx < size_array(ROWS); idx++) {
    Serial.print("Set pin ");
    Serial.println(ROWS[idx]);
    pinMode(ROWS[idx], INPUT_PULLUP);
    delay(20);
  }
  for (idx = 0 ; idx < 2 ; idx++) {
    Serial.println("");
  }
  Serial.print("Setting outputs");
  for (idx = 0; idx < random(4, 10); idx++) {
    Serial.print(".");
    delay(250);
  }
  Serial.println(".");
  delay(1000);
  for (idx = 0; idx < size_array(COLUMNS); idx++) {
    Serial.print("Set pin ");
    Serial.println(COLUMNS[idx]);
    pinMode(COLUMNS[idx], OUTPUT);
    delay(20);
    digitalWrite(COLUMNS[idx], LOW);
  }
  for (idx = 0 ; idx < 2 ; idx++) {
    Serial.println("");
  }
}
static void pin_rst() {
  for (tdx = 0; tdx < size_array(COLUMNS); tdx++) {
    digitalWrite(COLUMNS[tdx], HIGH);
  }
}
void loop() {
  // put your main code here, to run repeatedly:
  for (jdx = 0; jdx < size_array(COLUMNS); jdx++) {
    pin_rst();
    digitalWrite(COLUMNS[jdx], LOW);
    for (idx = 0; idx < size_array(ROWS); idx++) {
      if (!digitalRead(ROWS[idx])) {
        Serial.print("Button pressed Column: ");
        Serial.print(COLUMNS[jdx]);
        Serial.print(" Row: ");
        Serial.println(ROWS[idx]);
        delay(500);
        Serial.flush();
      }
    }
  }
}
