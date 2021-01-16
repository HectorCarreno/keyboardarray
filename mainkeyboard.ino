#include "keyboardComponents.h"

void setup(){ // configuration of MCU
  // put your setup code here, to run once:
  Serial.begin(115200); // begin serial communication
  //inputString.reserve(100);
  Timer1.attachInterrupt(ISR_time);
  Timer1.initialize(1000);
  array_initialise(); // initialise the array features
  Serial.println(); 
  button_initialise(); // initialising the pins operation mode configuration
  led_btn_initilise();
  testing_led_array_t();
  lets_started();
}

static void ISR_time(){ // interrupt service rutine of timer one
  time_counter++; // increment counter in time assigned
}

void loop(){
  // put your main code here, to run repeatedly:
  if (serialEvent_handler()){
    toggle_btn_t(serialReceive()); // call led button toggle function if it's available  
  }
  keyboard_scan_t();
}
