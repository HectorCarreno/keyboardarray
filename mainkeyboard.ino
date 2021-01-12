#include "keyboardComponents.h"

void setup(){ // configuration of MCU
  // put your setup code here, to run once:
  Serial.begin(115200); // begin serial communication
  Timer1.attachInterrupt(ISR_time);
  Timer1.initialize(100);
  array_initialise(); // initialise the array features
  Serial.println(); 
  button_initialise(); // initialising the pins operation mode configuration
  led_btn_initilise();
  testing_led_array_t();
  Serial.print("\n\n Starting MainKeyboard") ; // it works for beautify the code, there are so much of this on whole code
  for (idx = 0; idx < random(2, 6); idx++){
    Serial.print(".");
    delay(20);
  }
  Serial.print("Done!");
  Serial.println(".");
  delay(20);
  Serial.print("\n\n Ready for press button!");
  for (idx = 0; idx < random(2, 6); idx++){
    Serial.print(".");
    delay(20);
  }
  for (idx = 0; idx < 2; idx++){
    Serial.println();
  }
  Serial.print("\n -> \n\r");
}

static void ISR_time(){ // interrupt service rutine of timer one
  time_counter++; // increment counter in time assigned
}

void loop(){
  // put your main code here, to run repeatedly:
  led_swept_t(); // swept the leds states
  for (jdx = 0; jdx < size_array(COLUMNS); jdx++){ //  scan each button to determine if this was pressed
    pin_rst(); // reseting the columns
    digitalWrite(COLUMNS[jdx], LOW); // able the correct column of button array
    for (idx = 0; idx < size_array(ROWS); idx++){ 
      if (!digitalRead(ROWS[idx])){ // read each button pressed, row by row
        toggle_btn_t(BTN[idx][jdx]); // call led button toggle function if it's available
        led_swept_t(); // swept the leds states
        while(!digitalRead(ROWS[idx])){ ; // this function avoid issues at press button
          led_swept_t(); // swept the leds states
        }
        Serial.flush(); // cleaning the serial port
      }
    }
  }
}
