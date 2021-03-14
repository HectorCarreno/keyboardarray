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
#include <TimerOne.h> // add timer one library
#include <string.h> // add string library

#define size_array(x) sizeof(x) / sizeof(x[0]) // define the size of array function
#define leds_Qty 28 // quantity of leds in the board
#define keys_Qty 20 // quantity of keys in the board
#define enconder_Qty 6// quantity of enconders in the board
#define str_buf 30 // buffer for serial data received
#define filter_iter 10 // iteration for adc channel filter

#define DEFAULT_STEP 1 // default encoder step amount
// ***************************************************
#define test_time 100 // time for delay in test led array
#define test_time_l 500 // time for long delay in test led array

uint8_t CATHODES[] = {31, 33, 35, 37, 41, 45, 49}; // cathodes leds pins assignment - columns
uint8_t ANODES[] = {53, 51, 47, 43, 39}; // anodes leds pins assignment - rows

uint8_t COLUMNS[] = {52, 50, 48}; // button columns pins assignment
uint8_t ROWS[] = {34, 36, 38, 40, 42, 44, 46}; // buttons rows pins assignment

uint8_t ENCODER_COLUMNS[] = {24, 22, 23, 25, 27, 29};
uint8_t ENCODER_ROWS[] = {30, 28, 26}; // clk, dt, sw

uint8_t SW_PINS[] = {4, 6, 15, 2, 10, 8, 13, 12, 14, 3, 5, 7, 9, 11}; 
// 6 & 4 -> EFIS SW 1 PILOT
// 2 & 15 -> EFIS SW 2 PILOT
// 13 & 12 -> EFIS SW 1 COPILOT
// 10 & 8 -> EFIS SW 1 COPILOT
// 14 -> FO PILOT
// 3 -> MASTER WARM PILOT
// 5 -> MASTER CAUT PILOT
// 5 -> FO COPILOT
// 5 -> MASTER WARM COPILOT
// 5 -> MASTER CAUT COPILOT

uint8_t ADC_PINS[] = {A1, A0, A2, A3};

uint8_t idx, jdx, tdx, rdx; // counters index

long int time_counter = 0; // define time counter data type and value
long int time_push_pull = 0;
char *char_ptr = NULL;
char char_string[str_buf] = ""; // a char array to hold incoming data

volatile bool stringComplete = false;  // whether the string is complete

const char *LED_BTN_NAME[size_array(ANODES)][size_array(CATHODES)] = {"LOC", "AP1", "A/THR", "AP2", "EXPED", "APPR", NULL,
                                                                      "FD_C", "LS_C", "CSTR_C", "WPT_C", "VOR.D_C", "NDB_C", "ARPT_C",
                                                                      "FD_P", "LS_P", "CSTR_P", "WPT_P", "VOR.D_P", "NDB_P", "ARPT_P", 
                                                                      "F/O_CG", "F/O_CR", "MASTER_WARM_C", "MASTER_CAUT_C", NULL, NULL, NULL,
                                                                      "F/O_PG", "F/O_PR", "MASTER_WARM_P", "MASTER_CAUT_P", NULL, NULL, NULL};

const char *BTN_NAME[size_array(COLUMNS)][size_array(ROWS)] = {"APPR", "EXPED", "AP2", "A/THR", "AP1", "LOC", NULL,
                                                               "FD_P", "LS_P", "ARPT_P", "NDB_P", "VOR.D_P", "WPT_P", "CSTR_P",
                                                               "LS_C", "FD_C", "CSTR_C", "WPT_C", "VOR.D_C", "NDB_C", "ARPT_C"};

const char *ENCODER_NAME[size_array(ENCODER_COLUMNS)] = {"SPD", "HDG", "ALT", "V/S", "EFIS_ALT_C", "EFIS_ALT_P"};

const char *ADC_NAME[size_array(ADC_PINS)] = {"EFIS_MOD_P", "EFIS_RANGE_P", "EFIS_RANGE_C", "EFIS_MOD_C"};
const char *ADC_POS[5] = {"LS", "VOR", "NAV", "ARC", "PLAN"};

const char *SW_NAME[10] = {"EFIS_SW1_P", "EFIS_SW2_P", "EFIS_SW1_C", "EFIS_SW2_C", "SSP_F/O_P",
                           "MST_WARM_P", "MST_CAUT_P", "SSP_F/O_C", "MST_WARM_C", "MST_CAUT_C"};

typedef int8_t atm_err_t; // define type of data to handle different needs

enum {
  off_mode,
  on_mode
}; // enumerating state modes

enum {
  LS,
  VOR,
  NAV,
  ARC,
  PLAN,
  EFI10R,
  EFI20R,
  EFI40R,
  EFI80R,
  EFI160R,
  EFI320R
};

enum {
  OFF_AKC,
  ADF_AKC,
  VOR_AKC,
  HOLD_AKC
};

enum {
  BTN_ERR = -1,
  LOC_PRESS,
  AP1_PRESS,
  A_THR_PRESS,
  AP2_PRESS, 
  EXPED_PRESS,
  APPR_PRESS,
  FD_C_PRESS,
  LS_C_PRESS,
  CSTR_C_PRESS,
  WPT_C_PRESS,
  VOR_D_C_PRESS,
  NDB_C_PRESS,
  ARPT_C_PRESS,
  FD_P_PRESS,
  LS_P_PRESS,
  CSTR_P_PRESS,
  WPT_P_PRESS,
  VOR_D_P_PRESS,
  NDB_P_PRESS,
  ARPT_P_PRESS,
  F_O_CG_PRESS,
  F_O_CR_PRESS,
  MASTER_WARM_C_PRESS,
  MASTER_CAUT_C_PRESS,
  F_O_PG_PRESS,
  F_O_PR_PRESS,
  MASTER_WARM_P_PRESS,
  MASTER_CAUT_P_PRESS,
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

struct encoder_pulses_t {  
  uint8_t column = NULL;
  uint8_t rows[size_array(COLUMNS)];
  uint8_t step = DEFAULT_STEP;
  const char *name = NULL;
};

struct adc_pot_t {
  uint8_t adc_pin = NULL;
  uint16_t adc_val = NULL;
  uint16_t last_adc_val = NULL;
  uint8_t pot_state = NULL;
  uint8_t last_pot_state = NULL;
  const char *pot_name = NULL;
  const char *pos_select = NULL;
};

struct sw_btn_t {
  uint8_t sw_pin[2];
  uint8_t sw_state = NULL;
  uint8_t last_sw_state = NULL;
  const char *sw_name = NULL;
  
};

static void ISR_time(){ // interrupt service rutine of timer one
  time_counter++; // increment counter in time assigned
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
  Serial.print("\n -> \n\n\n\r");
}

static void arrays_initialise(){ // initialise function shows the button array configuration
  Serial.begin(115200); // begin serial communication
  //inputString.reserve(100);
  Timer1.attachInterrupt(ISR_time);
  Timer1.initialize(1000);
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
  delay(random(10, 100));
  for (idx = 0; idx < 4; idx++){
    Serial.println("");
  }
  delay(10);
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
  /*
  Serial.print("\n\r");
  Serial.print(char_ptr);
  Serial.print("\n\r");
  */
  return char_ptr; // return the string received in pointer assigned to function
}

sw_btn_t sw_button[size_array(SW_NAME)];

static void switch_initialise() {
  jdx = NULL; idx = NULL;
  Serial.print("\n\n\rInitialise switch button pins assignment!.");
  for(rdx = 0; rdx < size_array(SW_NAME); ++rdx) {
    if(rdx < 4) {
      for(jdx = 0; jdx < size_array(sw_button[0].sw_pin); ++jdx){
        sw_button[rdx].sw_pin[jdx] = SW_PINS[idx];
        pinMode(sw_button[rdx].sw_pin[jdx], INPUT_PULLUP);
        /*
        Serial.print("Switch pin: ");
        Serial.print(sw_button[rdx].sw_pin[jdx]);
        Serial.print("\t");
        */
        ++idx;
      }
    } else {
        sw_button[rdx].sw_pin[0] = SW_PINS[idx];
        pinMode(sw_button[rdx].sw_pin[0], INPUT_PULLUP);
        /*Serial.print("Switch pin: ");
        Serial.print(sw_button[rdx].sw_pin[0]);
        Serial.print("\t");
        */
        ++idx;
    }
    sw_button[rdx].sw_name = SW_NAME[rdx];
    sw_button[rdx].sw_state = OFF_AKC;
    /*
    Serial.print("Switch name: ");
    Serial.print(sw_button[rdx].sw_name);
    Serial.print("\n\r");
    */
    Serial.print(".");
    delay(random(10,100));
  }
  Serial.print("\n\n\rSwitch button config done!!!\n\n\n\r");
}

static void switch_handle(sw_btn_t sw_status) {
  switch(sw_status.sw_state){
    case ADF_AKC:
        Serial.print(sw_status.sw_name);
        Serial.print(",ADF\n\r");
    break;
    
    case VOR_AKC:
        Serial.print(sw_status.sw_name);
        Serial.print(",VOR\n\r");
    break;
    
    case OFF_AKC:
        Serial.print(sw_status.sw_name);
        Serial.print(",OFF\n\r");
    break;

    case HOLD_AKC:
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
    if (idx < 4) {
      if (!digitalRead(sw_button[idx].sw_pin[0])){
        sw_button[idx].sw_state = ADF_AKC;
      } else if (!digitalRead(sw_button[idx].sw_pin[1])){
        sw_button[idx].sw_state = VOR_AKC;
      } else {
        sw_button[idx].sw_state = OFF_AKC;
      }
    } else {
      if (!digitalRead(sw_button[idx].sw_pin[0])){
        sw_button[idx].sw_state = HOLD_AKC;
      } else {
        sw_button[idx].sw_state = OFF_AKC;
      }
    }
    if (sw_button[idx].last_sw_state != sw_button[idx].sw_state)
    {
      sw_button[idx].last_sw_state = sw_button[idx].sw_state;
      switch_handle(sw_button[idx]);
    }
  }
}



adc_pot_t adc_knobs[size_array(ADC_PINS)]; // array of structures to characterize the analogic inputs

static void adc_initialise() {
  Serial.print("\n\r\n\r # Initialising ADC knobs"); 
  for(idx = 0; idx < size_array(ADC_PINS); idx++){
    adc_knobs[idx].adc_pin = ADC_PINS[idx];
    adc_knobs[idx].pot_name = ADC_NAME[idx];

    for (jdx = 0; jdx < filter_iter; jdx++){
      adc_knobs[idx].adc_val += analogRead(adc_knobs[idx].adc_pin);
      adc_knobs[idx].adc_val /= filter_iter;
    }

    Serial.print("."); 
    adc_knobs[idx].last_adc_val = (map(adc_knobs[idx].adc_val, 0, 113, 10, 320));
    delay(random(10,100));
  }
  Serial.print("\n\r # ADC pins assignement done! \n\n\n\r"); 
}

static void adc_scan() {
  for (idx = 0; idx < size_array(ADC_PINS); ++idx)
  {
    for (jdx = 0; jdx < filter_iter; jdx++){
      adc_knobs[idx].adc_val += analogRead(adc_knobs[idx].adc_pin);
      adc_knobs[idx].adc_val /= filter_iter;
    }

    if((strcmp(adc_knobs[idx].pot_name, "EFIS_MOD_P") == NULL) || (strcmp(adc_knobs[idx].pot_name, "EFIS_MOD_C") == NULL)){
      adc_knobs[idx].pot_state = NULL;
      if(adc_knobs[idx].adc_val >= 0 && adc_knobs[idx].adc_val < 10){
        adc_knobs[idx].pot_state = LS;
        adc_knobs[idx].pos_select = ADC_POS[0];
      } else if(adc_knobs[idx].adc_val >= 10 && adc_knobs[idx].adc_val < 30){
        adc_knobs[idx].pot_state = VOR;
        adc_knobs[idx].pos_select = ADC_POS[1];
      } else if(adc_knobs[idx].adc_val >= 30 && adc_knobs[idx].adc_val < 50){
        adc_knobs[idx].pot_state = NAV;
        adc_knobs[idx].pos_select = ADC_POS[2];
      } else if(adc_knobs[idx].adc_val >= 50 && adc_knobs[idx].adc_val < 70){
        adc_knobs[idx].pot_state = ARC;
        adc_knobs[idx].pos_select = ADC_POS[3];
      } else if(adc_knobs[idx].adc_val >= 70){
        adc_knobs[idx].pot_state = PLAN;
        adc_knobs[idx].pos_select = ADC_POS[4];
      }
      switch(adc_knobs[idx].pot_state){
        case LS:
          if (adc_knobs[idx].last_pot_state == VOR){
            Serial.print(adc_knobs[idx].pot_name);
            Serial.print(",DEC\n\r");
          }
        break;
        case VOR:
          if (adc_knobs[idx].last_pot_state == LS){
            Serial.print(adc_knobs[idx].pot_name);
            Serial.print(",INC\n\r");
          } else if (adc_knobs[idx].last_pot_state == NAV){
            Serial.print(adc_knobs[idx].pot_name);
            Serial.print(",DEC\n\r");
          }
        break;
        case NAV:
          if (adc_knobs[idx].last_pot_state == VOR){
            Serial.print(adc_knobs[idx].pot_name);
            Serial.print(",INC\n\r");
          } else if (adc_knobs[idx].last_pot_state == ARC){
            Serial.print(adc_knobs[idx].pot_name);
            Serial.print(",DEC\n\r");
          }
        break;
        case ARC:
          if (adc_knobs[idx].last_pot_state == NAV){
            Serial.print(adc_knobs[idx].pot_name);
            Serial.print(",INC\n\r");
          } else if (adc_knobs[idx].last_pot_state == PLAN){
            Serial.print(adc_knobs[idx].pot_name);
            Serial.print(",DEC\n\r");
          }
        break;
        case PLAN:
          if (adc_knobs[idx].last_pot_state == ARC){
            Serial.print(adc_knobs[idx].pot_name);
            Serial.print(",INC\n\r");
          }
        break;

        default:
          Serial.print("error in potentiometers - adc converter\n\r");
        break;
      }
      adc_knobs[idx].last_pot_state = adc_knobs[idx].pot_state;
    } else {
      adc_knobs[idx].pot_state = NULL;
      if(adc_knobs[idx].adc_val >= 0 && adc_knobs[idx].adc_val < 10){
        adc_knobs[idx].pot_state = EFI10R;
      } else if(adc_knobs[idx].adc_val >= 10 && adc_knobs[idx].adc_val < 30){
        adc_knobs[idx].pot_state = EFI20R;
      } else if(adc_knobs[idx].adc_val >= 30 && adc_knobs[idx].adc_val < 50){
        adc_knobs[idx].pot_state = EFI40R;
      } else if(adc_knobs[idx].adc_val >= 50 && adc_knobs[idx].adc_val < 70){
        adc_knobs[idx].pot_state = EFI80R;
      } else if(adc_knobs[idx].adc_val >= 70 && adc_knobs[idx].adc_val < 90){
        adc_knobs[idx].pot_state = EFI160R;
      } else if(adc_knobs[idx].adc_val >= 90){
        adc_knobs[idx].pot_state = EFI320R;
      }

      switch(adc_knobs[idx].pot_state){
        case EFI10R:
          if (adc_knobs[idx].last_pot_state == EFI20R){
            Serial.print(adc_knobs[idx].pot_name);
            Serial.print(",DEC\n\r");
          }
        break;
        case EFI20R:
          if (adc_knobs[idx].last_pot_state == EFI10R){
            Serial.print(adc_knobs[idx].pot_name);
            Serial.print(",INC\n\r");
          } else if (adc_knobs[idx].last_pot_state == EFI40R){
            Serial.print(adc_knobs[idx].pot_name);
            Serial.print(",DEC\n\r");
          }
        break;
        case EFI40R:
          if (adc_knobs[idx].last_pot_state == EFI20R){
            Serial.print(adc_knobs[idx].pot_name);
            Serial.print(",INC\n\r");
          } else if (adc_knobs[idx].last_pot_state == EFI80R){
            Serial.print(adc_knobs[idx].pot_name);
            Serial.print(",DEC\n\r");
          }
        break;
        case EFI80R:
          if (adc_knobs[idx].last_pot_state == EFI40R){
            Serial.print(adc_knobs[idx].pot_name);
            Serial.print(",INC\n\r");
          } else if (adc_knobs[idx].last_pot_state == EFI160R){
            Serial.print(adc_knobs[idx].pot_name);
            Serial.print(",DEC\n\r");
          }
        break;
        case EFI160R:
          if (adc_knobs[idx].last_pot_state == EFI80R){
            Serial.print(adc_knobs[idx].pot_name);
            Serial.print(",INC\n\r");
          } else if (adc_knobs[idx].last_pot_state == EFI320R){
            Serial.print(adc_knobs[idx].pot_name);
            Serial.print(",DEC\n\r");
          }
        break;
        case EFI320R:
          if (adc_knobs[idx].last_pot_state == EFI160R){
            Serial.print(adc_knobs[idx].pot_name);
            Serial.print(",INC\n\r");
          } 
        break;

        default:
          Serial.print("error in potentiometers - adc converter\n\r");
        break;
      }
      adc_knobs[idx].last_pot_state = adc_knobs[idx].pot_state;
    }
  }
}
/*
  EFI10R
  EFI20R
  EFI40R
  EFI80R
  EFI160R
  EFI320R
*/


key_btn_t push_key_btn[keys_Qty]; //  array of structures to quantify the key button

static void button_initialise(){ // configure the button pins as inputs ans outputs
 
  Serial.print("# Configuring the push button inputs!");
  //memset(push_key_btn[keys_Qty].key_name, NULL, keys_Qty);
  //Serial.print("\n\n\r # Key button index memory assigned! \n\n\r");
  idx = 0, jdx = 0;
  for (rdx = 0; rdx < size_array(push_key_btn); ++rdx) {
    if (rdx == 6){
      idx = 0;
      jdx = 1;
      //Serial.print("\n\r Right panel configuration \n\r");
    } else if (rdx == 13){
      idx = 0;
      jdx = 2;
      //Serial.print("\n\r Left panel configuration \n\r");
    }
    /*Serial.print("key index -> ");
    push_key_btn[rdx].key_row = ROWS[idx];
    Serial.print(" Set button row -> ");
    Serial.print(push_key_btn[rdx].key_row);
    push_key_btn[rdx].key_column = COLUMNS[jdx];
    Serial.print(" column -> ");
    Serial.print(push_key_btn[rdx].key_column);
    push_key_btn[rdx].key_name = BTN_NAME[jdx][idx];
    Serial.print(" Set name -> ");
    Serial.print(push_key_btn[rdx].key_name);
    Serial.print("\n\r");*/
    idx++;
    Serial.print(".");
    delay(random(10, 50));
  }
  
  for(idx = 0; idx < size_array(ROWS); idx++){
    pinMode(ROWS[idx], INPUT_PULLUP);  
  }

  for(idx = 0; idx < size_array(COLUMNS); idx++){
    pinMode(COLUMNS[idx], OUTPUT);
    digitalWrite(COLUMNS[idx], on_mode); // reset the all outputs
  }

  Serial.print("\n\n\r Push button configuration done! \n\n\n\r ");
}

static void pin_rst(){ // set all button in no press mode
  for (tdx = 0; tdx < size_array(COLUMNS); tdx++){ // sweep the six different columns in the button array.
    digitalWrite(COLUMNS[tdx], on_mode); // reset the all outputs
  }
}

led_btn_t push_led_btn[leds_Qty]; //  array of structures to quantify the button with respective leds

static void led_btn_initilise(){ // initialise the led pin accord each button
  Serial.println("\n\r # Keyboard led array initialising...");
  //delay(2); // pre-delay function
  //memset(push_led_btn[leds_Qty].btn_name, NULL, leds_Qty);
  //Serial.println("\n\r Led's names memory allocated \n\r");
  //Serial.print("\n\n\r Main panel configuration \n\r");
  idx = 0, jdx = 0;
  for(rdx = 0; rdx < size_array(push_led_btn); ++rdx){ // avoid assign pins to button without led
    if (rdx == 6){
      idx = 1;
      jdx = 0;
      //Serial.print("\n\r Right panel configuration \n\r");
    } else if (rdx == 13){
      idx = 2;
      jdx = 0;
      //Serial.print("\n\r Left panel configuration \n\r");
    } else if (rdx == 20){
      idx = 3;
      jdx = 0;
      //Serial.print("\n\r Extrem right panel configuration \n\r");
    } else if (rdx == 24){
      idx = 4;
      jdx = 0;
      //Serial.print("\n\r Extrem left panel configuration \n\n\r");
    }
    push_led_btn[rdx].led_row = ANODES[idx]; // assign pin anode to each row
    push_led_btn[rdx].led_column = CATHODES[jdx]; // assign pin cathode to each column
    push_led_btn[rdx].led_state = off_mode; // define state as low level
    push_led_btn[rdx].btn_name = LED_BTN_NAME[idx][jdx]; // assign name to each button with led
    //Serial.print("\n Set pin "); 
    //Serial.print(push_led_btn[rdx].btn_name);
    //Serial.print(" Anode pin ");
    //Serial.print(push_led_btn[rdx].led_row);
    //Serial.print(" Cathode pin ");
    //Serial.println(push_led_btn[rdx].led_column);
    pinMode(push_led_btn[rdx].led_row, OUTPUT); // define rows as outputs
    pinMode(push_led_btn[rdx].led_column, OUTPUT); // define columns as outputs

    digitalWrite(push_led_btn[rdx].led_row, on_mode); // set row outputs in off mode
    digitalWrite(push_led_btn[rdx].led_column, off_mode); // set column outputs in on mode
    delay(20);
    digitalWrite(push_led_btn[rdx].led_row, off_mode); // set row outputs in off mode
    digitalWrite(push_led_btn[rdx].led_column, on_mode); // set column outputs in on mode
    jdx++; // increment index for each iteration accord with led button array size
  }
  /*for(rdx = 0; rdx < size_array(push_led_btn); ++rdx){
    Serial.println(push_led_btn[rdx].btn_name);
  }*/
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
    } else if(strcmp(led_btn.btn_name, "FD_C,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = FD_C_PRESS;
    } else if(strcmp(led_btn.btn_name, "FD_C,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = FD_C_PRESS;
    } else if(strcmp(led_btn.btn_name, "LS_C,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = LS_C_PRESS;
    } else if(strcmp(led_btn.btn_name, "LS_C,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = LS_C_PRESS;
    } else if(strcmp(led_btn.btn_name, "CSTR_C,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = CSTR_C_PRESS;
    } else if(strcmp(led_btn.btn_name, "CSTR_C,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = CSTR_C_PRESS;
    } else if(strcmp(led_btn.btn_name, "WPT_C,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = WPT_C_PRESS;
    } else if(strcmp(led_btn.btn_name, "WPT_C,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = WPT_C_PRESS;
    } else if(strcmp(led_btn.btn_name, "VOR.D_C,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = VOR_D_C_PRESS;
    } else if(strcmp(led_btn.btn_name, "VOR.D_C,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = VOR_D_C_PRESS;
    } else if(strcmp(led_btn.btn_name, "NDB_C,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = NDB_C_PRESS;
    } else if(strcmp(led_btn.btn_name, "NDB_C,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = NDB_C_PRESS;
    } else if(strcmp(led_btn.btn_name, "ARPT_C,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = ARPT_C_PRESS;
    } else if(strcmp(led_btn.btn_name, "ARPT_C,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = ARPT_C_PRESS;
    } else if(strcmp(led_btn.btn_name, "FD_P,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = FD_P_PRESS;
    } else if(strcmp(led_btn.btn_name, "FD_P,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = FD_P_PRESS;
    } else if(strcmp(led_btn.btn_name, "LS_P,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = LS_P_PRESS;
    } else if(strcmp(led_btn.btn_name, "LS_P,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = LS_P_PRESS;
    } else if(strcmp(led_btn.btn_name, "CSTR_P,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = CSTR_P_PRESS;
    } else if(strcmp(led_btn.btn_name, "CSTR_P,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = CSTR_P_PRESS;
    } else if(strcmp(led_btn.btn_name, "WPT_P,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = WPT_P_PRESS;
    } else if(strcmp(led_btn.btn_name, "WPT_P,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = WPT_P_PRESS;
    } else if(strcmp(led_btn.btn_name, "VOR.D_P,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = VOR_D_P_PRESS;
    } else if(strcmp(led_btn.btn_name, "VOR.D_P,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = VOR_D_P_PRESS;
    } else if(strcmp(led_btn.btn_name, "NDB_P,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = NDB_P_PRESS;
    } else if(strcmp(led_btn.btn_name, "NDB_P,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = NDB_P_PRESS;
    } else if(strcmp(led_btn.btn_name, "ARPT_P,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = ARPT_P_PRESS;
    } else if(strcmp(led_btn.btn_name, "ARPT_P,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = ARPT_P_PRESS;
    } else if(strcmp(led_btn.btn_name, "F/O_PG,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = F_O_PG_PRESS;
    } else if(strcmp(led_btn.btn_name, "F/O_PG,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = F_O_PG_PRESS;
    } else if(strcmp(led_btn.btn_name, "F/O_PR,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = F_O_PR_PRESS;
    } else if(strcmp(led_btn.btn_name, "F/O_PR,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = F_O_PR_PRESS;
    } else if(strcmp(led_btn.btn_name, "MASTER_WARM_P,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = MASTER_WARM_P_PRESS;
    } else if(strcmp(led_btn.btn_name, "MASTER_WARM_P,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = MASTER_WARM_P_PRESS;
    } else if(strcmp(led_btn.btn_name, "MASTER_CAUT_P,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = MASTER_CAUT_P_PRESS;
    } else if(strcmp(led_btn.btn_name, "MASTER_CAUT_P,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = MASTER_CAUT_P_PRESS;
    } else if(strcmp(led_btn.btn_name, "F/O_CG,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = F_O_CG_PRESS;
    } else if(strcmp(led_btn.btn_name, "F/O_CG,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = F_O_CG_PRESS;
    } else if(strcmp(led_btn.btn_name, "F/O_CR,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = F_O_CR_PRESS;
    } else if(strcmp(led_btn.btn_name, "F/O_CR,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = F_O_CR_PRESS;
    } else if(strcmp(led_btn.btn_name, "MASTER_WARM_C,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = MASTER_WARM_C_PRESS;
    } else if(strcmp(led_btn.btn_name, "MASTER_WARM_C,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = MASTER_WARM_C_PRESS;
    } else if(strcmp(led_btn.btn_name, "MASTER_CAUT_C,ON\n") == NULL){
      led_btn.led_state = on_mode;
      led_btn.btn_id = MASTER_CAUT_C_PRESS;
    } else if(strcmp(led_btn.btn_name, "MASTER_CAUT_C,OFF\n") == NULL){
      led_btn.led_state = off_mode;
      led_btn.btn_id = MASTER_CAUT_C_PRESS;
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

encoder_pulses_t encoder_btn[size_array(ENCODER_COLUMNS)]; // array of structures to characterize the encoders

static void encoder_initialise(){
  Serial.print("\n\n\r Initialising encoder configuration!");
  for (idx = 0; idx < size_array(ENCODER_COLUMNS); ++idx){
    encoder_btn[idx].column = ENCODER_COLUMNS[idx];
    encoder_btn[idx].name = ENCODER_NAME[idx];
    for (jdx = 0; jdx < size_array(ENCODER_ROWS); ++jdx) {
      encoder_btn[idx].rows[jdx] = ENCODER_ROWS[jdx]; // clk, dt, sw
    }
    pinMode(encoder_btn[idx].column, OUTPUT);
    digitalWrite(encoder_btn[idx].column , on_mode);
    Serial.print(".");
  }
  for (idx = 0; idx < size_array(ENCODER_ROWS); ++idx){
    pinMode(encoder_btn[0].rows, INPUT);
  }
  Serial.print("\n\n\r Encoder confituration done!\n\n\n\r");
}

static void encoder_columns_rst(){ // set all button in no press mode
  for (tdx = 0; tdx < size_array(ENCODER_COLUMNS); tdx++){ // sweep the six different columns in the button array.
    digitalWrite(encoder_btn[tdx].column, on_mode); // reset the all outputs
  }
}

static void encoder_scan() {
  for (idx = 0; idx < size_array (ENCODER_COLUMNS); ++idx){
    encoder_columns_rst();
    digitalWrite(encoder_btn[idx].column, off_mode);
    
    if(!digitalRead(encoder_btn[idx].rows[0]) < !digitalRead(encoder_btn[idx].rows[1])){
      Serial.print(encoder_btn[idx].name);
      Serial.print(",INC\n\r");
      led_swept_t();
      delay(100);
      led_swept_t();
    } else if(!digitalRead(encoder_btn[idx].rows[0]) > !digitalRead(encoder_btn[idx].rows[1])){
      Serial.print(encoder_btn[idx].name);
      Serial.print(",DEC\n\r");
      led_swept_t();
      delay(100);
      led_swept_t();
    } else if(!digitalRead(encoder_btn[idx].rows[2])){

      time_counter = 0;

      while(!digitalRead(encoder_btn[idx].rows[2])){
        led_swept_t();
        Serial.print("\r");
      }

      if (time_counter <= 1000) {
        Serial.print(encoder_btn[idx].name);
        Serial.print(",PUSH\n\r");  
      } else if (time_counter > 1000){
        Serial.print(encoder_btn[idx].name);
        Serial.print(",PULL\n\r");  
      }
    }
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
  //digitalWrite(43,on_mode);
  //digitalWrite(33,off_mode);


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