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
#define leds_Qty 28 // quantity of leds in the board
#define keys_Qty 20 // quantity of keys in the board
#define str_buf 30 // buffer for serial data received
// ***************************************************
#define test_time 40 // time for delay in test led array
#define test_time_l 150 // time for long delay in test led array

uint8_t CATHODES[] = {31, 33, 35, 37, 41, 45, 49}; // cathodes leds pins assignment - columns
uint8_t ANODES[] = {53, 51, 47, 43, 39}; // anodes leds pins assignment - rows

uint8_t ROWS[] = {34, 36, 38, 40, 42, 44, 46}; // buttons rows pins assignment
uint8_t COLUMNS[] = {52, 50, 48}; // button columns pins assignment

uint8_t idx, jdx, tdx, rdx; // counters index

long int time_counter = 0; // define time counter data type and value
char *char_ptr = NULL;
char char_string[str_buf] = ""; // a char array to hold incoming data

volatile bool stringComplete = false;  // whether the string is complete
const char *LED_BTN_NAME[size_array(ANODES)][size_array(CATHODES)] = {"LOC", "AP1", "A/THR", "AP2", "EXPED", "APPR", NULL,
                                                                      "FD_R", "LS_R", "CSTR_R", "WPT_R", "VOR.D_R", "NDB_R", "ARPT_R",
                                                                      "FD_L", "LS_L", "CSTR_L", "WPT_L", "VOR.D_L", "NDB_L", "ARPT_L", 
                                                                      "F/O_RG", "F/O_RR", "MASTER_WARM_R", "MASTER_CAUT_R", NULL, NULL, NULL,
                                                                      "F/O_LG", "F/O_LR", "MASTER_WARM_L", "MASTER_CAUT_L", NULL, NULL, NULL};
// above definition assign rows x columns position with each button respectively 
const char *BTN_NAME[size_array(COLUMNS)][size_array(ROWS)] = {"APPR", "EXPED", "AP2", "A/THR", "AP1", "LOC", NULL,
                                                               "FD_L", "LS_L", "ARPT_L", "NDB_L", "VOR.D_L", "WPT_L", "CSTR_L",
                                                               "LS_R", "FD_R", "CSTR_R", "WPT_R", "VOR.D_R", "NDB_R", "ARPT_R"};


typedef int8_t atm_err_t; // define type of data to handle different needs

enum {
  off_mode,
  on_mode
}; // enumerating state modes

enum {
  BTN_ERR = -1,
  LOC_PRESS,
  AP1_PRESS,
  A_THR_PRESS,
  AP2_PRESS, 
  EXPED_PRESS,
  APPR_PRESS,
  FD_R_PRESS,
  LS_R_PRESS,
  CSTR_R_PRESS,
  WPT_R_PRESS,
  VOR_D_R_PRESS,
  NDB_R_PRESS,
  ARPT_R_PRESS,
  FD_L_PRESS,
  LS_L_PRESS,
  CSTR_L_PRESS,
  WPT_L_PRESS,
  VOR_D_L_PRESS,
  NDB_L_PRESS,
  ARPT_L_PRESS,
  F_O_RG_PRESS,
  F_O_RR_PRESS,
  MASTER_WARM_R_PRESS,
  MASTER_CAUT_R_PRESS,
  F_O_LG_PRESS,
  F_O_LR_PRESS,
  MASTER_WARM_L_PRESS,
  MASTER_CAUT_L_PRESS,
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
  
  Serial.print("\n\r");
  Serial.print(char_ptr);
  Serial.print("\n\r");

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
  Serial.print(size_array(ANODES));  // show rows quantity
  Serial.print(" Size of columns: ");
  Serial.println(size_array(CATHODES)); // show columns quantity
  for(idx = 0; idx < size_array(ANODES);++idx){ // scan all button array
    Serial.println();
    for(jdx = 0; jdx < size_array(CATHODES); ++jdx){
      Serial.print("# Key position: Row ");
      Serial.print(jdx);
      Serial.print(" Column ");
      Serial.print(idx);
      Serial.print(" Button: ");
      Serial.println(LED_BTN_NAME[idx][jdx]); // show the row and column position for button pressed and its name
      delay(10);
    }    
  }
  Serial.print("\n\n\r");
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

  //memset(push_key_btn[keys_Qty].key_name, NULL, keys_Qty);
  Serial.print("\n\n\r # Key button index memory assigned! \n\n\r");
  Serial.print("\n\r Main panel configuration \n\r");
  idx = 0, jdx = 0;
  for (rdx = 0; rdx < size_array(push_key_btn); ++rdx)
  {
    if (rdx == 6){
      idx = 0;
      jdx = 1;
      Serial.print("\n\r Right panel configuration \n\r");
    } else if (rdx == 13){
      idx = 0;
      jdx = 2;
      Serial.print("\n\r Left panel configuration \n\r");
    }
    Serial.print("key index -> ");
    push_key_btn[rdx].key_row = ROWS[idx];
    Serial.print(" Set button row -> ");
    Serial.print(push_key_btn[rdx].key_row);
    push_key_btn[rdx].key_column = COLUMNS[jdx];
    Serial.print(" column -> ");
    Serial.print(push_key_btn[rdx].key_column);
    push_key_btn[rdx].key_name = BTN_NAME[jdx][idx];
    Serial.print(" Set name -> ");
    Serial.print(push_key_btn[rdx].key_name);
    Serial.print("\n\r");

    
    idx++;
    delay(10);
  }
  
  for(idx = 0; idx < size_array(ROWS); idx++){
    pinMode(ROWS[idx], INPUT_PULLUP);  
  }

  for(idx = 0; idx < size_array(COLUMNS); idx++){
    pinMode(COLUMNS[idx], OUTPUT);
    digitalWrite(COLUMNS[idx], on_mode); // reset the all outputs
  }

  Serial.print("\n\r Button configuration done! \n\r ");
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
  Serial.println("\n\r # Set leds pins as outputs...");
  delay(10); // pre-delay function
  //memset(push_led_btn[leds_Qty].btn_name, NULL, leds_Qty);
  Serial.println("\n\r Led's names memory allocated \n\r");
  Serial.print("\n\n\r Main panel configuration \n\r");
  idx = 0, jdx = 0;
  for(rdx = 0; rdx < size_array(push_led_btn); ++rdx){ // avoid assign pins to button without led
    if (rdx == 6){
      idx = 1;
      jdx = 0;
      Serial.print("\n\r Right panel configuration \n\r");
    } else if (rdx == 13){
      idx = 2;
      jdx = 0;
      Serial.print("\n\r Left panel configuration \n\r");
    } else if (rdx == 20){
      idx = 3;
      jdx = 0;
      Serial.print("\n\r Extrem right panel configuration \n\r");
    } else if (rdx == 24){
      idx = 4;
      jdx = 0;
      Serial.print("\n\r Extrem left panel configuration \n\n\r");
    }
    push_led_btn[rdx].led_row = ANODES[idx]; // assign pin anode to each row
    push_led_btn[rdx].led_column = CATHODES[jdx]; // assign pin cathode to each column
    push_led_btn[rdx].led_state = off_mode; // define state as low level
    push_led_btn[rdx].btn_name = LED_BTN_NAME[idx][jdx]; // assign name to each button with led
    Serial.print("\n Set pin "); 
    Serial.print(push_led_btn[rdx].btn_name);
    Serial.print(" Anode pin ");
    Serial.print(push_led_btn[rdx].led_row);
    Serial.print(" Cathode pin ");
    Serial.println(push_led_btn[rdx].led_column);
    pinMode(push_led_btn[rdx].led_row, OUTPUT); // define rows as outputs
    pinMode(push_led_btn[rdx].led_column, OUTPUT); // define columns as outputs

    digitalWrite(push_led_btn[rdx].led_row, on_mode); // set row outputs in off mode
    digitalWrite(push_led_btn[rdx].led_column, off_mode); // set column outputs in on mode
    delay(30);
    digitalWrite(push_led_btn[rdx].led_row, off_mode); // set row outputs in off mode
    digitalWrite(push_led_btn[rdx].led_column, on_mode); // set column outputs in on mode
    jdx++; // increment index for each iteration accord with led button array size
  }
  for(rdx = 0; rdx < size_array(push_led_btn); ++rdx){
    Serial.println(push_led_btn[rdx].btn_name);
  }
}

static void testing_led_array_t(){ // test all led button array
  Serial.print("\n\n Testing leds..... \r\n");
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
  for (idx = 0; idx < 2; ++idx){
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
    Serial.println("ERROR - button name not able");
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

    /*
    Serial.print("\n\r\t # ed_btn.key_name ");
    Serial.print(led_btn.btn_name);
    Serial.print("\n\r");
    */

    if(strcmp(led_btn.btn_name, "LOC,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = LOC_PRESS;
    } else if(strcmp(led_btn.btn_name, "LOC,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = LOC_PRESS;
    } else if(strcmp(led_btn.btn_name, "AP1,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = AP1_PRESS;
    } else if(strcmp(led_btn.btn_name, "AP1,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = AP1_PRESS;
    } else if(strcmp(led_btn.btn_name, "A_THR,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = A_THR_PRESS;
    } else if(strcmp(led_btn.btn_name, "A_THR,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = A_THR_PRESS;
    } else if(strcmp(led_btn.btn_name, "AP2,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = AP2_PRESS;
    } else if(strcmp(led_btn.btn_name, "AP2,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = AP2_PRESS;
    } else if(strcmp(led_btn.btn_name, "EXPED,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = EXPED_PRESS;
    } else if(strcmp(led_btn.btn_name, "EXPED,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = EXPED_PRESS;
    } else if(strcmp(led_btn.btn_name,"APPR,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = APPR_PRESS;
    } else if(strcmp(led_btn.btn_name,"APPR,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = APPR_PRESS;
    } else if(strcmp(led_btn.btn_name, "FD_R,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = FD_R_PRESS;
    } else if(strcmp(led_btn.btn_name, "FD_R,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = FD_R_PRESS;
    } else if(strcmp(led_btn.btn_name, "LS_R,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = LS_R_PRESS;
    } else if(strcmp(led_btn.btn_name, "LS_R,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = LS_R_PRESS;
    } else if(strcmp(led_btn.btn_name, "CSTR_R,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = CSTR_R_PRESS;
    } else if(strcmp(led_btn.btn_name, "CSTR_R,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = CSTR_R_PRESS;
    } else if(strcmp(led_btn.btn_name, "WPT_R,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = WPT_R_PRESS;
    } else if(strcmp(led_btn.btn_name, "WPT_R,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = WPT_R_PRESS;
    } else if(strcmp(led_btn.btn_name, "VOR.D_R,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = VOR_D_R_PRESS;
    } else if(strcmp(led_btn.btn_name, "VOR.D_R,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = VOR_D_R_PRESS;
    } else if(strcmp(led_btn.btn_name, "NDB_R,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = NDB_R_PRESS;
    } else if(strcmp(led_btn.btn_name, "NDB_R,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = NDB_R_PRESS;
    } else if(strcmp(led_btn.btn_name, "ARPT_R,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = ARPT_R_PRESS;
    } else if(strcmp(led_btn.btn_name, "ARPT_R,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = ARPT_R_PRESS;
    } else if(strcmp(led_btn.btn_name, "FD_L,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = FD_L_PRESS;
    } else if(strcmp(led_btn.btn_name, "FD_L,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = FD_L_PRESS;
    } else if(strcmp(led_btn.btn_name, "LS_L,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = LS_L_PRESS;
    } else if(strcmp(led_btn.btn_name, "LS_L,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = LS_L_PRESS;
    } else if(strcmp(led_btn.btn_name, "CSTR_L,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = CSTR_L_PRESS;
    } else if(strcmp(led_btn.btn_name, "CSTR_L,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = CSTR_L_PRESS;
    } else if(strcmp(led_btn.btn_name, "WPT_L,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = WPT_L_PRESS;
    } else if(strcmp(led_btn.btn_name, "WPT_L,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = WPT_L_PRESS;
    } else if(strcmp(led_btn.btn_name, "VOR.D_L,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = VOR_D_L_PRESS;
    } else if(strcmp(led_btn.btn_name, "VOR.D_L,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = VOR_D_L_PRESS;
    } else if(strcmp(led_btn.btn_name, "NDB_L,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = NDB_L_PRESS;
    } else if(strcmp(led_btn.btn_name, "NDB_L,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = NDB_L_PRESS;
    } else if(strcmp(led_btn.btn_name, "ARPT_L,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = ARPT_L_PRESS;
    } else if(strcmp(led_btn.btn_name, "ARPT_L,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = ARPT_L_PRESS;
    } else if(strcmp(led_btn.btn_name, "F/O_LG,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = F_O_LG_PRESS;
    } else if(strcmp(led_btn.btn_name, "F/O_LG,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = F_O_LG_PRESS;
    } else if(strcmp(led_btn.btn_name, "F/O_LR,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = F_O_LR_PRESS;
    } else if(strcmp(led_btn.btn_name, "F/O_LR,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = F_O_LR_PRESS;
    } else if(strcmp(led_btn.btn_name, "MASTER_WARM_L,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = MASTER_WARM_L_PRESS;
    } else if(strcmp(led_btn.btn_name, "MASTER_WARM_L,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = MASTER_WARM_L_PRESS;
    } else if(strcmp(led_btn.btn_name, "MASTER_CAUT_L,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = MASTER_CAUT_L_PRESS;
    } else if(strcmp(led_btn.btn_name, "MASTER_CAUT_L,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = MASTER_CAUT_L_PRESS;
    } else if(strcmp(led_btn.btn_name, "F/O_RG,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = F_O_RG_PRESS;
    } else if(strcmp(led_btn.btn_name, "F/O_RG,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = F_O_RG_PRESS;
    } else if(strcmp(led_btn.btn_name, "F/O_RR,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = F_O_RR_PRESS;
    } else if(strcmp(led_btn.btn_name, "F/O_RR,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = F_O_RR_PRESS;
    } else if(strcmp(led_btn.btn_name, "MASTER_WARM_R,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = MASTER_WARM_R_PRESS;
    } else if(strcmp(led_btn.btn_name, "MASTER_WARM_R,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = MASTER_WARM_R_PRESS;
    } else if(strcmp(led_btn.btn_name, "MASTER_CAUT_R,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = MASTER_CAUT_R_PRESS;
    } else if(strcmp(led_btn.btn_name, "MASTER_CAUT_R,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = MASTER_CAUT_R_PRESS;
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

static void keyboard_scan_t(){
  led_swept_t(); // swept the leds states
  
  for (jdx = 0; jdx < size_array(COLUMNS); jdx++){ //  scan each button to determine if this was pressed
    pin_rst(); // reseting the columns
    digitalWrite(COLUMNS[jdx], off_mode); // able the correct column of button array
    for (idx = 0; idx < size_array(ROWS); idx++){ 
      if (!digitalRead(ROWS[idx])){ // read each button pressed, row by row
        //led_swept_t(); // swept the leds states
        Serial.println(BTN_NAME[jdx][idx]);
        while(!digitalRead(ROWS[idx])){ ; // this function avoid issues at press button
          led_swept_t(); // swept the leds states
        }
        Serial.flush(); // cleaning the serial port
      }
    }
  }
  /*
  for (rdx = 0; rdx < size_array(push_key_btn); ++rdx){
    //pin_rst();
    digitalWrite(push_key_btn[rdx].key_column, off_mode);
    if (!digitalRead(push_key_btn[rdx].key_row));
    {
      led_swept_t();
      Serial.print(push_key_btn[rdx].key_name);
      Serial.print("\n\r");
      delay(1000);
      //while(!digitalRead(push_key_btn[rdx].key_row)){
        //led_swept_t();
      //}
      Serial.flush();
    }
    digitalWrite(push_key_btn[rdx].key_column, on_mode);
  }
  */
}

#endif /* END KEYBOARDCOMPONENTS__H__ */