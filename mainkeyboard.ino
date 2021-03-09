#include "keyboardComponents.h"

void setup(){ // configuration of MCU
  // put your setup code here, to run once:
  Serial.begin(115200); // begin serial communication
  //inputString.reserve(100);
  Timer1.attachInterrupt(ISR_time);
  Timer1.initialize(1000);
  arrays_initialise(); // initialise the array features
  button_initialise(); adc_initialise(); switch_initialise(); encoder_initialise(); // initialising all components
  led_btn_initilise(); testing_led_array_t(); // initialising & testing led array in the keyboard
  lets_started(); // left ready to work
}

static void ISR_time(){ // interrupt service rutine of timer one
  time_counter++; // increment counter in time assigned
}

void loop(){
  // put your main code here, to run repeatedly
  if (serialEvent_handler()){
    toggle_btn_t(serialReceive()); // call led button toggle function if it's available  
  }
  adc_scan();
  switch_scan();
  encoder_scan();
  keyboard_scan_t();
}
