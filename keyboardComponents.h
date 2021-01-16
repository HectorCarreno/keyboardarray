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
#include <TimerOne.h> // add timer one library
#include <string.h> // add string library

#define size_array(x) sizeof(x) / sizeof(x[0]) // define the size of array function
#define leds_Qty 14 // quantity of leds in the board
#define keys_Qty 18 // quantity of keys in the board
#define str_buf 16

uint8_t CATHODES[] = {41, 39, 37, 35, 33, 31}; // cathodes pins assignment
uint8_t ANODES[] = {23, 25, 27}; // anodes pins assignment
uint8_t COLUMNS[] = {28, 30, 32, 34, 36, 38}; // columns pins assignment
uint8_t ROWS[] = {26, 24, 22}; // rows pins assignment
uint8_t idx, jdx, tdx, rdx; // counters index
long int time_counter = 0; // define time counter data type and value
char *char_ptr = NULL;
char char_string[str_buf] = ""; // a char array to hold incoming data
volatile bool stringComplete = false;  // whether the string is complete
const char *BTN[size_array(ROWS)][size_array(COLUMNS)] = {"ENG", "BLEED", "PRESS", "ELEC", "HYD", "FUEL",
                                                          "APU", "COND", "DOOR", "WHELL", "F/CTL", "ALL",
                                                        "CLR_L", "TO CONFIG", "SYS", "RCL", "EMER/CANC", "CLR_R"};
// above definition assign rows x columns position with each button respectively 

typedef int8_t atm_err_t; // define type of data to handle different needs
typedef volatile bool FAIL_STATE; 

enum {
  off_mode,
  on_mode
}; // enumerating state modes

enum {
  ENG_PRESS,
  BLEED_PRESS,
  PRESS_PRESS,
  ELEC_PRESS, 
  HYD_PRESS,
  FUEL_PRESS,
  APU_PRESS,
  COND_PRESS,
  DOOR_PRESS,
  WHELL_PRESS,
  F_CTL_PRESS,
  CLR_L_PRESS,
  SYS_PRESS,
  CLR_R_PRESS,
  FULLY_PRESS
}; // enumerating leds identities

struct led_btn_t {// structure of one led button
  uint8_t led_row = NULL;  // row pin assigned
  uint8_t led_column = NULL;  // column pin assigned
  volatile bool led_state = off_mode; // initial state
  const char *btn_name = NULL;  // button name
  uint8_t btn_id = NULL; // pin identification 
};

struct key_btn_t { // structure of one key button
  uint8_t key_row = NULL; //  key row button assigned
  uint8_t key_column = NULL; // key column button assigned
  const char *key_name = NULL; // key name
};

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
  return char_ptr; // return the string received in pointer assigned to function
}

// Leds pins assignment with the array conections
//led_btn_t push_led_btn[14] = {{0,0,0,BTN[0][0]}, {0,1,0,BTN[0][1]}, {0,2,0,BTN[0][2]}, {0,3,0,BTN[0][3]}, {0,4,0,BTN[0][4]}, {0,5,0,BTN[0][5]},
//                              {1,0,0,BTN[1][0]}, {1,1,0,BTN[1][1]}, {1,2,0,BTN[1][2]}, {1,3,0,BTN[1][3]}, {1,4,0,BTN[1][4]}, 
//                              {2,0,0,BTN[2][0]},                    {2,2,0,BTN[2][2]},                                       {2,5,0,BTN[2][5]}};

led_btn_t push_led_btn[leds_Qty]; //  array of structures to quantify the button with respective leds
key_btn_t push_key_btn[keys_Qty]; //  array of structures to quantify the key button

static void array_initialise(){ // initialise function shows the button array configuration
  Serial.println("-> Keyboard array was configurated thus: "); 
  Serial.print("!Size of rows: "); 
  Serial.print(size_array(ROWS));  // show rows quantity
  Serial.print(" Size of columns: ");
  Serial.println(size_array(COLUMNS)); // show columns quantity
  for(idx = 0; idx < size_array(COLUMNS);++idx){ // scan all button array
    Serial.println();
    for(jdx = 0; jdx < size_array(ROWS); ++jdx){
      Serial.print("# Key position: Row ");
      Serial.print(jdx);
      Serial.print(" Column ");
      Serial.print(idx);
      Serial.print(" Button: ");
      Serial.println(BTN[jdx][idx]); // show the row and column position for button pressed and its name
      delay(10);
    }    
  }
}

static void button_initialise(){ // configure the button pins as inputs ans outputs
  for (idx = 0; idx < 14; idx++){
    Serial.print("#");
    delay(10);
  }
  Serial.print(" MAINKEYBOARD ");
  for (idx = 0; idx < 9; idx++){
    Serial.print("#");
    delay(10);
  }
  Serial.println("#");
  for (idx = 0; idx < 8; idx++){
    Serial.print("#");
    delay(10);
  }
  Serial.print(" MCDU AEROSOFT A320 SIMULATOR ");
  for (idx = 0; idx < 8; idx++){
    Serial.print("#");
    delay(10);
  }
  Serial.println("#");
  delay(random(50, 200));
  for (idx = 0; idx < 4; idx++){
    Serial.println("");
  }
  delay(10);
  Serial.print("# Configuring the push button inputs");
  for (idx = 0; idx < 10; idx++){
    Serial.print(".");
    delay(10);
  }
  Serial.println(".");
  delay(random(50, 200));

  rdx = NULL;
  memset(push_key_btn[keys_Qty].key_name, NULL, keys_Qty);
  for (idx = 0; idx < size_array(ROWS); idx++){
    for (jdx = 0; jdx < size_array(COLUMNS); jdx++) { 
      Serial.print("\n\n\r # key index -> ");
      Serial.print(rdx + 1);
      push_key_btn[rdx].key_row = ROWS[idx];
      Serial.print(" Set row -> ");
      Serial.print(push_key_btn[rdx].key_row);
      push_key_btn[rdx].key_column = COLUMNS[jdx];
      Serial.print(" Set column -> ");
      Serial.print(push_key_btn[rdx].key_column);
      push_key_btn[rdx].key_name = BTN[idx][jdx];
      Serial.print(" Set name -> ");
      Serial.println(push_key_btn[rdx].key_name);
      rdx++;
    }
  }

  for (rdx = 0; rdx < keys_Qty; rdx++){
    pinMode(push_key_btn[rdx].key_row, INPUT_PULLUP);
    pinMode(push_key_btn[rdx].key_column, OUTPUT);
    digitalWrite(push_key_btn[tdx].key_column, on_mode); // reset the all outputs
  }    
}

static void pin_rst(){ // set all button in no press mode
  for (tdx = 0; tdx < size_array(COLUMNS); tdx++){ // sweep the six different columns in the button array.
    //digitalWrite(push_key_btn[tdx].key_column, on_mode); // reset the all outputs
    digitalWrite(COLUMNS[tdx], on_mode); // reset the all outputs
  }
}

static void lets_started(){
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



static void led_btn_initilise(){ // initialise the led pin accord each button
  memset(push_led_btn[leds_Qty].btn_name, NULL, leds_Qty);
  Serial.println("\n\r # Set leds pins as outputs...");
    idx = 0, jdx = 0;
  for(rdx = 0; rdx < size_array(push_led_btn); ++rdx){ // avoid assign pins to button without led
    if (rdx == 6){
      idx = 1;
      jdx = 0;
    } else if (rdx == 11){
      idx = 2;
      jdx = 0;
    } else if (rdx == 12){
      jdx = 2;
    } else if (rdx == 13){
      jdx = 5;
    }
    push_led_btn[rdx].led_row = ANODES[idx]; // assign pin anode to each row
    push_led_btn[rdx].led_column = CATHODES[jdx]; // assign pin cathode to each column
    push_led_btn[rdx].led_state = off_mode; // define state as low level
    push_led_btn[rdx].btn_name = BTN[idx][jdx]; // assign name to each button with led
    Serial.print("\n Set pin "); 
    Serial.print(push_led_btn[rdx].btn_name);
    Serial.print(" Anode pin ");
    Serial.print(push_led_btn[rdx].led_row);
    Serial.print(" Cathode pin ");
    Serial.println(push_led_btn[rdx].led_column);
    pinMode(push_led_btn[rdx].led_row, OUTPUT); // define rows as outputs
    pinMode(push_led_btn[rdx].led_column, OUTPUT); // define columns as outputs

    digitalWrite(push_led_btn[rdx].led_row, off_mode); // set row outputs in off mode
    digitalWrite(push_led_btn[rdx].led_column, on_mode); // set column outputs in on mode
    jdx++; // increment index for each iteration accord with led button array size
  }
}

static void testing_led_array_t(){ // test all led button array
  Serial.print("\n\n Testing leds..... \r\n");
  for (idx = 0; idx < 2; ++idx){  
    for (rdx = 0; rdx < size_array(push_led_btn); ++rdx){
      digitalWrite(push_led_btn[rdx].led_row, on_mode);
      digitalWrite(push_led_btn[rdx].led_column, off_mode);
      delay(30);
      digitalWrite(push_led_btn[rdx].led_row, off_mode);
      digitalWrite(push_led_btn[rdx].led_column, on_mode);
    }
    delay(30);
    for (rdx = size_array(push_led_btn); rdx > 0*size_array(push_led_btn); --rdx){
      digitalWrite(push_led_btn[rdx].led_row, on_mode);
      digitalWrite(push_led_btn[rdx].led_column, off_mode);
      delay(30);
      digitalWrite(push_led_btn[rdx].led_row, off_mode);
      digitalWrite(push_led_btn[rdx].led_column, on_mode);
    }
  }
  for (idx = 0; idx < 2; ++idx){
    delay(250);
    for (rdx = 0; rdx < size_array(push_led_btn); ++rdx){
      digitalWrite(push_led_btn[rdx].led_row, on_mode);
      digitalWrite(push_led_btn[rdx].led_column, off_mode);
    }
    delay(250);
    for (rdx = 0; rdx < size_array(push_led_btn); ++rdx){
        digitalWrite(push_led_btn[rdx].led_row, off_mode);
        digitalWrite(push_led_btn[rdx].led_column, on_mode);
    }
  }
}

static void toggle_led(led_btn_t led_t){ // Toggle led pins states
  atm_err_t err = led_t.btn_id;
  if (err == -1){
    Serial.println("ERROR - button name not able");
  } if(err == FULLY_PRESS){
    for (rdx = 0; rdx < size_array(push_led_btn); ++rdx){
      push_led_btn[rdx].led_state = led_t.led_state;
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
          delayMicroseconds(200); // return to off mode for avoid error with two or more started leds
          digitalWrite(push_led_btn[rdx].led_row, !push_led_btn[rdx].led_state);
          digitalWrite(push_led_btn[rdx].led_column, push_led_btn[rdx].led_state);  
        } else {
          digitalWrite(push_led_btn[rdx].led_column, !push_led_btn[rdx].led_state);  
        }
    }
}

static void led_pos_handler(led_btn_t led_btn){ // this function handle the button ident depending each button pressed
  if (led_btn.btn_name != NULL){
    if(strcmp(led_btn.btn_name, "ENG,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = ENG_PRESS;
    } else if(strcmp(led_btn.btn_name, "ENG,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = ENG_PRESS;
    } else if(strcmp(led_btn.btn_name, "BLEED,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = BLEED_PRESS;
    } else if(strcmp(led_btn.btn_name, "BLEED,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = BLEED_PRESS;
    } else if(strcmp(led_btn.btn_name, "PRESS,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = PRESS_PRESS;
    } else if(strcmp(led_btn.btn_name, "PRESS,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = PRESS_PRESS;
    } else if(strcmp(led_btn.btn_name, "ELEC,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = ELEC_PRESS;
    } else if(strcmp(led_btn.btn_name, "ELEC,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = ELEC_PRESS;
    } else if(strcmp(led_btn.btn_name, "HYD,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = HYD_PRESS;
    } else if(strcmp(led_btn.btn_name, "HYD,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = HYD_PRESS;
    } else if(strcmp(led_btn.btn_name,"FUEL,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = FUEL_PRESS;
    } else if(strcmp(led_btn.btn_name,"FUEL,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = FUEL_PRESS;
    } else if(strcmp(led_btn.btn_name, "APU,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = APU_PRESS;
    } else if(strcmp(led_btn.btn_name, "APU,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = APU_PRESS;
    } else if(strcmp(led_btn.btn_name, "COND,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = COND_PRESS;
    } else if(strcmp(led_btn.btn_name, "COND,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = COND_PRESS;
    } else if(strcmp(led_btn.btn_name, "DOOR,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = DOOR_PRESS;
    } else if(strcmp(led_btn.btn_name, "DOOR,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = DOOR_PRESS;
    } else if(strcmp(led_btn.btn_name, "WHELL,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = WHELL_PRESS;
    } else if(strcmp(led_btn.btn_name, "WHELL,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = WHELL_PRESS;
    } else if(strcmp(led_btn.btn_name, "F/CTL,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = F_CTL_PRESS;
    } else if(strcmp(led_btn.btn_name, "F/CTL,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = F_CTL_PRESS;
    } else if(strcmp(led_btn.btn_name, "CLR_L,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = CLR_L_PRESS;
    } else if(strcmp(led_btn.btn_name, "CLR_L,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = CLR_L_PRESS;
    } else if(strcmp(led_btn.btn_name, "SYS,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = SYS_PRESS;
    } else if(strcmp(led_btn.btn_name, "SYS,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = SYS_PRESS;
    } else if(strcmp(led_btn.btn_name, "CLR_R,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = CLR_R_PRESS;
    } else if(strcmp(led_btn.btn_name, "CLR_R,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = CLR_R_PRESS;
    } else if(strcmp(led_btn.btn_name, "FULLY,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = FULLY_PRESS;
    } else if(strcmp(led_btn.btn_name, "FULLY,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = FULLY_PRESS;
    } else {
      led_btn.btn_id = -1;
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

static void keyboard_scan_t(){
  led_swept_t(); // swept the leds states
  for (jdx = 0; jdx < size_array(COLUMNS); jdx++){ //  scan each button to determine if this was pressed
    pin_rst(); // reseting the columns
    digitalWrite(COLUMNS[jdx], LOW); // able the correct column of button array
    for (idx = 0; idx < size_array(ROWS); idx++){ 
      if (!digitalRead(ROWS[idx])){ // read each button pressed, row by row
        led_swept_t(); // swept the leds states
        while(!digitalRead(ROWS[idx])){ ; // this function avoid issues at press button
          led_swept_t(); // swept the leds states
        }
        Serial.println(BTN[idx][jdx]);
        Serial.flush(); // cleaning the serial port
      }
    }
  }
}

#endif /* END KEYBOARDCOMPONENTS__H__ */