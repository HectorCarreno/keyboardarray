/******************************************************
* Developed by Hector Carreno
* Date: 12 january 2021
* Name: keyboardComponents.h
* Components: leds handler
*             button handler
*             toggle handler
*             serial receive handler
*             declaration initialise functions
* This is a open source developed by the author, any issues 
* can shows dependig of each application. This components was 
* tested and work accord with the need. Please follow the 
* comments across whole document to know how you must use the
* components. Any doubt you can send it to hfcbteacher@gmail.com 
*******************************************************/
#ifndef __KEYBOARDCOMPONENTS_H__
#define __KEYBOARDCOMPONENTS_H__ 
#include <arduino.h> // add arduino library
#include <math.h> // add math library
#include <string.h> // add string library

#define size_array(x) sizeof(x) / sizeof(x[0]) // define the size of array function
#define leds_Qty 16 // quantity of leds in the board
#define switch_qty 7 // quantity of switches in the board
#define str_buf 32 // buffer for serial data received

//#define DEFAULT_STEP 1 // default encoder step amount
// ***************************************************
#define test_time 50 // time for delay in test led array
#define test_time_l 30 // time for long delay in test led array

uint8_t RELAY = 13; // Relay module output
uint8_t CATHODES[] = {8, 7, 6, 5}; // cathodes leds pins assignment - columns
uint8_t ANODES[] = {9, 10, 11, 12}; // anodes leds pins assignment - rows
uint8_t SW_PINS[] = {4, 3, 2, A0, A1, A2, A3}; 
// 4 -> BRK FAN
// 3 -> TERR ON ND
// 2 -> MAX DECEL AUTO BRK
// A0 -> MED DECEL AUTO BRK
// A1 -> LO DECEL AUTO BRK
// A2 -> TERR ON ND EXT
// A3 -> LANDING TRAIN LEVER

uint8_t idx, jdx, tdx, rdx; // counters index

long int time_counter = 0; // define time counter data type and value
// long int time_push_pull = 0;
char *char_ptr = NULL;
char char_string[str_buf] = ""; // a char array to hold incoming data

volatile bool stringComplete = false;  // whether the string is complete

const char *LED_BTN_NAME[size_array(ANODES)][size_array(CATHODES)] = {"BRK_FAN_HOT",    "UNLK3",              "UNLK2",                  "UNLK1",
                                                                      "BRK_FAN_ON",     "UNLK3_ON",           "UNLK2_ON",               "UNLK1_ON",
                                                                      "TERR_ON_ND_EX",  "MAX_DEC_AUTO_BRK",   "MED_DEC_AUTO_BRK",       "LO_DEC_AUTO_BRK",
                                                                      "TERR_ON_ND",     "MAX_DEC_ON_AUTO_BRK", "MED_DEC_ON_AUTO_BRK",   "LO_DEC_ON_AUTO_BRK"};

const char *SW_NAME[] = {"BRK_FAN", "TERR_ON_ND", "MAX_DEC_AUTO_BRK", "MED_DEC_AUTO_BRK", "LO_DEC_AUTO_BRK", "TERR_ON_ND_EXT", "LAND_TRAIN_LEVER"};

typedef int8_t atm_err_t; // define type of data to handle different needs

enum {
  off_mode,
  on_mode
}; // enumerating state modes

enum {
  OFF,
  HOLD
};

enum {
  BTN_ERR = -1,
  BRK_FAN_HOT_PRESS,
  UNLK3_PRESS,
  UNLK2_PRESS,
  UNLK1_PRESS, 
  BRK_FAN_ON_PRESS,
  UNLK3_ON_PRESS,
  UNLK2_ON_PRESS,
  UNLK1_ON_PRESS,
  TERR_ON_ND_EX_PRESS,
  MAX_DEC_AUTO_BRK_PRESS,
  MED_DEC_AUTO_BRK_PRESS,
  LO_DEC_AUTO_BRK_PRESS,
  TERR_ON_ND_PRESS,
  MAX_DEC_ON_AUTO_BRK_PRESS,
  MED_DEC_ON_AUTO_BRK_PRESS,
  LO_DEC_ON_AUTO_BRK_PRESS,
  //.........
  FULLY_PRESS
}; // enumerating leds identities F/O_RG", "F/O_RR", "MASTER_WARM_R", "MASTER_CAUT_R"

struct led_btn_t {// structure of one led button
  uint8_t led_row = NULL;  // row pin assigned
  uint8_t led_column = NULL;  // column pin assigned
  volatile bool led_state = off_mode; // initial state
  const char *btn_name = NULL;  // button name
  uint8_t btn_id = NULL; // pin identification 
};

struct sw_btn_t {
  uint8_t sw_pin = NULL;
  uint8_t sw_state = NULL;
  uint8_t last_sw_state = NULL;
  const char *sw_name = NULL;
};


// static void ISR_time(){ // interrupt service rutine of timer one
//   time_counter++; // increment counter in time assigned
// }
static void arrays_initialise(){ // initialise function shows the button array configuration
  Serial.begin(115200); // begin serial communication
  //inputString.reserve(100);
  // Timer1.attachInterrupt(ISR_time);
  // Timer1.initialize(1000);
}


volatile bool serialEvent_handler() {
  idx = 0; // index for make the swept in the serial port
  if (Serial.available() > 0) // ask if serial available have any data
  {
    memset(char_string, NULL, str_buf); // clear the string
    while (Serial.available()) { 
    delayMicroseconds(200);
    char_string[idx] = (char)Serial.read(); // add it to the inputString:
    // if the incoming character is a newline, set a flag so the main loop can, do something about it:
    if (char_string[idx] == '\n') { // wait for new line character to activate the flag
      stringComplete = true;
    }
      idx++; // increase the index
    }
  }
  return stringComplete; // return flag to function value
}

static char* serialReceive(){
  // print the string when a newline arrives:
  char_ptr = NULL; // clear char pointer
  char_ptr = char_string; // store string received from serial port in char pointer
  stringComplete = false; // clean flag

  // Serial.print("\n\r");
  // Serial.print(char_ptr);
  // Serial.print("\n\r");
  return char_ptr; // return the string received in pointer assigned to function
}

static void relay_initialization() {
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, on_mode);
}

sw_btn_t sw_button[size_array(SW_NAME)];

static void switch_initialise() {
  jdx = NULL; idx = NULL;
  for(rdx = 0; rdx < size_array(SW_NAME); ++rdx) {
    
    sw_button[rdx].sw_pin = SW_PINS[rdx];
    pinMode(sw_button[rdx].sw_pin, INPUT);

    sw_button[rdx].sw_name = SW_NAME[rdx];
    sw_button[rdx].sw_state = OFF;
    sw_button[rdx].last_sw_state = OFF;
    delay(50);
  }
}

static void switch_handle(sw_btn_t sw_status) {
  switch(sw_status.sw_state){
    case OFF:
        Serial.print(sw_status.sw_name);
        Serial.print(",OFF\n\r");
    break;

    case HOLD:
        Serial.print(sw_status.sw_name);
        Serial.print(",HOLD\n\r");        
    break;

    default:
        Serial.print("error - swithes \n\r"); 
    break;
  }
}

static void switch_scan() {
  for (idx = 0; idx < size_array(SW_NAME); ++idx){
    
    if (!digitalRead(sw_button[idx].sw_pin)){
      sw_button[idx].sw_state = HOLD;
      if(idx == 6){
        delay(50);
      }
    } else {
      sw_button[idx].sw_state = OFF;
    }

    if (sw_button[idx].last_sw_state != sw_button[idx].sw_state)
    {
      sw_button[idx].last_sw_state = sw_button[idx].sw_state;
      switch_handle(sw_button[idx]);
    }
  }
}

led_btn_t push_led_btn[leds_Qty]; //  array of structures to quantify the button with respective leds

static void led_btn_initilise(){ // initialise the led pin accord each button
  idx = 0, jdx = 0;
  for(rdx = 0; rdx < size_array(push_led_btn); ++rdx){ // avoid assign pins to button without led
    if (rdx == 4){
      idx = 1;
      jdx = 0;
    } else if (rdx == 8){
      idx = 2;
      jdx = 0;
    } else if (rdx == 12){
      idx = 3;
      jdx = 0;
    }

    push_led_btn[rdx].led_row = ANODES[idx]; // assign pin anode to each row
    push_led_btn[rdx].led_column = CATHODES[jdx]; // assign pin cathode to each column
    push_led_btn[rdx].led_state = off_mode; // define state as low level
    push_led_btn[rdx].btn_name = LED_BTN_NAME[idx][jdx]; // assign name to each button with led

    pinMode(push_led_btn[rdx].led_row, OUTPUT); // define rows as outputs
    pinMode(push_led_btn[rdx].led_column, OUTPUT); // define columns as outputs

    digitalWrite(push_led_btn[rdx].led_row, on_mode); // set row outputs in off mode
    digitalWrite(push_led_btn[rdx].led_column, off_mode); // set column outputs in on mode
    delay(100);
    digitalWrite(push_led_btn[rdx].led_row, off_mode); // set row outputs in off mode
    digitalWrite(push_led_btn[rdx].led_column, on_mode); // set column outputs in on mode
    jdx++; // increment index for each iteration accord with led button array size
  }
}

static void testing_led_array_t(){ // test all led button array
  for (idx = 0; idx < 1; ++idx){  
    for (rdx = 0; rdx < size_array(push_led_btn); ++rdx){
      digitalWrite(push_led_btn[rdx].led_row, on_mode);
      digitalWrite(push_led_btn[rdx].led_column, off_mode);
      delay(test_time);
      digitalWrite(push_led_btn[rdx].led_row, off_mode);
      digitalWrite(push_led_btn[rdx].led_column, on_mode);
    }
    delay(test_time);
    for (rdx = size_array(push_led_btn); rdx > 0*size_array(push_led_btn); --rdx){
      digitalWrite(push_led_btn[rdx].led_row, on_mode);
      digitalWrite(push_led_btn[rdx].led_column, off_mode);
      delay(test_time);
      digitalWrite(push_led_btn[rdx].led_row, off_mode);
      digitalWrite(push_led_btn[rdx].led_column, on_mode);
    }
  }
  for (idx = 0; idx < 1; ++idx){
    delay(test_time_l);
    for (rdx = 0; rdx < size_array(push_led_btn); ++rdx){
      digitalWrite(push_led_btn[rdx].led_row, on_mode);
      digitalWrite(push_led_btn[rdx].led_column, off_mode);
    }
    delay(test_time_l);
    for (rdx = 0; rdx < size_array(push_led_btn); ++rdx){
        digitalWrite(push_led_btn[rdx].led_row, off_mode);
        digitalWrite(push_led_btn[rdx].led_column, on_mode);
    }
  }
}

static void toggle_led(led_btn_t led_t){ // Toggle led pins states
  atm_err_t err = led_t.btn_id;
  if (err == BTN_ERR){
    Serial.println("ERROR - button name not found");
  } else if(err == FULLY_PRESS){
    for (rdx = 0; rdx < size_array(push_led_btn); ++rdx){
      push_led_btn[rdx].led_state = led_t.led_state; // set all leds state
    }
  } else { // avoid error reading led state 
    push_led_btn[led_t.btn_id].led_state = led_t.led_state; // toggle state for each led button
  } 
}

static void led_swept_t(){ // swept each led button state
  for (rdx = 0; rdx < size_array(push_led_btn); ++rdx){
      if (push_led_btn[rdx].led_state){ // just when the state is high, set the led button in on mode
          digitalWrite(push_led_btn[rdx].led_row, push_led_btn[rdx].led_state);
          digitalWrite(push_led_btn[rdx].led_column, !push_led_btn[rdx].led_state);
          delayMicroseconds(750); // return to off mode for avoid error with two or more started leds
          digitalWrite(push_led_btn[rdx].led_row, !push_led_btn[rdx].led_state);
          digitalWrite(push_led_btn[rdx].led_column, push_led_btn[rdx].led_state);  
        } else {
          digitalWrite(push_led_btn[rdx].led_column, !push_led_btn[rdx].led_state);  
        }
    }
}

static void led_pos_handler(led_btn_t led_btn){ // this function handle the button ident depending each button pressed
  if (led_btn.btn_name != NULL){
    if(strcmp(led_btn.btn_name, "BRK_FAN_HOT_PRESS,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = BRK_FAN_HOT_PRESS;
    } else if(strcmp(led_btn.btn_name, "BRK_FAN_HOT_PRESS,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = BRK_FAN_HOT_PRESS;
    } else if(strcmp(led_btn.btn_name, "UNLK3_PRESS,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = UNLK3_PRESS;
    } else if(strcmp(led_btn.btn_name, "UNLK3_PRESS,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = UNLK3_PRESS;
    } else if(strcmp(led_btn.btn_name, "UNLK2_PRESS,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = UNLK2_PRESS;
    } else if(strcmp(led_btn.btn_name, "UNLK2_PRESS,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = UNLK2_PRESS;
    } else if(strcmp(led_btn.btn_name, "UNLK1_PRESS,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = UNLK1_PRESS;
    } else if(strcmp(led_btn.btn_name, "UNLK1_PRESS,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = UNLK1_PRESS;
    } else if(strcmp(led_btn.btn_name, "BRK_FAN_ON_PRESS,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = BRK_FAN_ON_PRESS;
    } else if(strcmp(led_btn.btn_name, "BRK_FAN_ON_PRESS,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = BRK_FAN_ON_PRESS;
    } else if(strcmp(led_btn.btn_name,"UNLK3_ON_PRESS,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = UNLK3_ON_PRESS;
    } else if(strcmp(led_btn.btn_name,"UNLK3_ON_PRESS,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = UNLK3_ON_PRESS;
    } else if(strcmp(led_btn.btn_name, "UNLK2_ON_PRESS,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = UNLK2_ON_PRESS;
    } else if(strcmp(led_btn.btn_name, "UNLK2_ON_PRESS,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = UNLK2_ON_PRESS;
    } else if(strcmp(led_btn.btn_name, "UNLK1_ON_PRESS,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = UNLK1_ON_PRESS;
    } else if(strcmp(led_btn.btn_name, "UNLK1_ON_PRESS,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = UNLK1_ON_PRESS;
    } else if(strcmp(led_btn.btn_name, "TERR_ON_ND_EX_PRESS,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = TERR_ON_ND_EX_PRESS;
    } else if(strcmp(led_btn.btn_name, "TERR_ON_ND_EX_PRESS,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = TERR_ON_ND_EX_PRESS;
    } else if(strcmp(led_btn.btn_name, "MAX_DEC_AUTO_BRK_PRESS,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = MAX_DEC_AUTO_BRK_PRESS;
    } else if(strcmp(led_btn.btn_name, "MAX_DEC_AUTO_BRK_PRESS,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = MAX_DEC_AUTO_BRK_PRESS;
    } else if(strcmp(led_btn.btn_name, "MED_DEC_AUTO_BRK_PRESS,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = MED_DEC_AUTO_BRK_PRESS;
    } else if(strcmp(led_btn.btn_name, "MED_DEC_AUTO_BRK_PRESS,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = MED_DEC_AUTO_BRK_PRESS;
    } else if(strcmp(led_btn.btn_name, "LO_DEC_AUTO_BRK_PRESS,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = LO_DEC_AUTO_BRK_PRESS;
    } else if(strcmp(led_btn.btn_name, "LO_DEC_AUTO_BRK_PRESS,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = LO_DEC_AUTO_BRK_PRESS;
    } else if(strcmp(led_btn.btn_name, "TERR_ON_ND_PRESS,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = TERR_ON_ND_PRESS;
    } else if(strcmp(led_btn.btn_name, "TERR_ON_ND_PRESS,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = TERR_ON_ND_PRESS;
    } else if(strcmp(led_btn.btn_name, "MAX_DEC_ON_AUTO_BRK_PRESS,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = MAX_DEC_ON_AUTO_BRK_PRESS;
    } else if(strcmp(led_btn.btn_name, "MAX_DEC_ON_AUTO_BRK_PRESS,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = MAX_DEC_ON_AUTO_BRK_PRESS;
    } else if(strcmp(led_btn.btn_name, "MED_DEC_ON_AUTO_BRK_PRESS,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = MED_DEC_ON_AUTO_BRK_PRESS;
    } else if(strcmp(led_btn.btn_name, "MED_DEC_ON_AUTO_BRK_PRESS,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = MED_DEC_ON_AUTO_BRK_PRESS;
    } else if(strcmp(led_btn.btn_name, "LO_DEC_ON_AUTO_BRK_PRESS,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = LO_DEC_ON_AUTO_BRK_PRESS;
    } else if(strcmp(led_btn.btn_name, "LO_DEC_ON_AUTO_BRK_PRESS,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = LO_DEC_ON_AUTO_BRK_PRESS;
    } else if(strcmp(led_btn.btn_name, "ELECTRIC_BUS,ON\n") == NULL){
      digitalWrite(RELAY, on_mode);
    } else if(strcmp(led_btn.btn_name, "ELECTRIC_BUS,OFF\n") == NULL){
      digitalWrite(RELAY, off_mode);
    } else if(strcmp(led_btn.btn_name, "FULLY,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = FULLY_PRESS;
    } else if(strcmp(led_btn.btn_name, "FULLY,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = FULLY_PRESS;
    } else {
      led_btn.btn_id = BTN_ERR;
      led_btn.btn_name = NULL;
    }
    toggle_led(led_btn); // call toggle_lede function to change the state depending on button pressed
    led_btn.btn_name = NULL; // clear the button name pointer
  } else {
    Serial.println("error - call to button led does't work");
  }
}

static void toggle_btn_t(char *btn_name_id){ // main function to call the handler, assign the ident from main function of the program
  struct led_btn_t button; //  set the variable for the led structure
  button.btn_name = btn_name_id; //  set the button name to the structure
  led_pos_handler(button); // call handler function
  btn_name_id = NULL; // clear the button id pointer
  button.btn_name = NULL; // clear the button name pointer
}
#endif /* END KEYBOARDCOMPONENTS__H__ */