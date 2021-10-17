#include "keyboardComponents.h"

void setup(){ // configuration of MCU
  // put your setup code here, to run once:
  arrays_initialise();
  switch_initialise(); // initialising all components
  relay_initialization(); // initialising relay module
  led_btn_initilise(); testing_led_array_t(); // initialising & testing led array in the keyboard
  Serial.println("READY!");
}

void loop(){
  // put your main code here, to run repeatedly
  if (serialEvent_handler()){
    toggle_btn_t(serialReceive()); // call led button toggle function if it's available  
  }
  switch_scan();
  led_swept_t();
}
