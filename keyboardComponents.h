#ifndef __KEYBOARDCOMPONENTS_H__
#define __KEYBOARDCOMPONENTS_H__ 
#include <arduino.h> // add arduino library
#include <TimerOne.h>
#include <string.h> // add string library
#define size_array(x) sizeof(x) / sizeof(x[0]) // define the size of array function

uint8_t CATHODES[] = {41, 39, 37, 35, 33, 31}; // cathodes pins assignment
uint8_t ANODES[] = {23, 25, 27}; // anodes pins assignment
uint8_t COLUMNS[] = {28, 30, 32, 34, 36, 38}; // columns pins assignment
uint8_t ROWS[] = {26, 24, 22}; // rows pins assignment
uint8_t idx, jdx, tdx, rdx; // counters index
long int time_counter = 0;
const char *BTN[size_array(ROWS)][size_array(COLUMNS)] = {"ENG", "BLEED", "PRESS", "ELEC", "HYD", "FUEL",
                                                          "APU", "COND", "DOOR", "WHELL", "F/CTL", "ALL",
                                                        "CLR_L", "TO CONFIG", "SYS", "RCL", "EMER/CANC", "CLR_R"};
// above definition assign rows x columns position with each button respectively 

typedef int8_t atm_err_t;

enum {
  off_mode,
  on_mode
};

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
  CLR_R_PRESS
};

struct led_btn_t
{
  uint8_t led_row = NULL;
  uint8_t led_column = NULL;
  volatile bool led_state = off_mode;
  const char *btn_name = NULL;
  uint8_t btn_id = NULL;
};

// Leds pins assignment with the array conections
//led_btn_t push_led_btn[14] = {{0,0,0,BTN[0][0]}, {0,1,0,BTN[0][1]}, {0,2,0,BTN[0][2]}, {0,3,0,BTN[0][3]}, {0,4,0,BTN[0][4]}, {0,5,0,BTN[0][5]},
//                              {1,0,0,BTN[1][0]}, {1,1,0,BTN[1][1]}, {1,2,0,BTN[1][2]}, {1,3,0,BTN[1][3]}, {1,4,0,BTN[1][4]}, 
//                              {2,0,0,BTN[2][0]},                    {2,2,0,BTN[2][2]},                                       {2,5,0,BTN[2][5]}};

led_btn_t push_led_btn[14]; //  array of structures to quantify the button with respective leds

static void led_btn_initilise(){
  Serial.println("\n\r # Set leds pins as outputs...");
    idx = 0, jdx = 0;
  for(rdx = 0; rdx < size_array(push_led_btn); ++rdx){
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
    push_led_btn[rdx].led_row = ANODES[idx];
    push_led_btn[rdx].led_column = CATHODES[jdx];
    push_led_btn[rdx].led_state = off_mode;
    push_led_btn[rdx].btn_name = BTN[idx][jdx];
    Serial.print("\n Set pin ");
    Serial.print(push_led_btn[rdx].btn_name);
    Serial.print(" Anode pin ");
    Serial.print(push_led_btn[rdx].led_row);
    Serial.print(" Cathode pin ");
    Serial.println(push_led_btn[rdx].led_column);
    pinMode(push_led_btn[rdx].led_row, OUTPUT);
    pinMode(push_led_btn[rdx].led_column, OUTPUT);

    digitalWrite(push_led_btn[rdx].led_row, off_mode);
    digitalWrite(push_led_btn[rdx].led_column, on_mode);
    jdx++;
  }
  Serial.println("\n\n\n Testing leds.....");
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

static void toogle_led(led_btn_t led_t){
  atm_err_t err = led_t.btn_id;
  if (err != -1){
    //Serial.println("ERROR - none key available");
    push_led_btn[led_t.btn_id].led_state = !push_led_btn[led_t.btn_id].led_state;
    //Serial.print("\n # STATE ");
    //Serial.println(push_led_btn[led_t.btn_id].led_state);
    if (push_led_btn[led_t.btn_id].led_state){
      //Serial.print("\n Activate led \n");
      digitalWrite(push_led_btn[led_t.btn_id].led_column, !push_led_btn[led_t.btn_id].led_state);
      digitalWrite(push_led_btn[led_t.btn_id].led_row, push_led_btn[led_t.btn_id].led_state);
    } else if (!push_led_btn[led_t.btn_id].led_state){
      //Serial.print("\n deactivate led \n");
      digitalWrite(push_led_btn[led_t.btn_id].led_column, !push_led_btn[led_t.btn_id].led_state);
      digitalWrite(push_led_btn[led_t.btn_id].led_row, push_led_btn[led_t.btn_id].led_state);
    }
  }
  for (rdx = 0; rdx < size_array(push_led_btn); ++rdx){
      digitalWrite(push_led_btn[rdx].led_row, off_mode);
      digitalWrite(push_led_btn[rdx].led_column, on_mode);
  } 
}

static void led_swept_t(){

  for (rdx = 0; rdx < size_array(push_led_btn); ++rdx){
      if (push_led_btn[rdx].led_state){
          digitalWrite(push_led_btn[rdx].led_row, push_led_btn[rdx].led_state);
          digitalWrite(push_led_btn[rdx].led_column, !push_led_btn[rdx].led_state);
          delayMicroseconds(400);
          digitalWrite(push_led_btn[rdx].led_row, !push_led_btn[rdx].led_state);
          digitalWrite(push_led_btn[rdx].led_column, push_led_btn[rdx].led_state);  
        }
    }
}

static void led_pos_handler(led_btn_t led_btn){
  uint8_t event_id;
  Serial.println(led_btn.btn_name);
  if(led_btn.btn_name == "ENG"){
    led_btn.btn_id = ENG_PRESS;
  }else if(led_btn.btn_name == "BLEED"){
    led_btn.btn_id = BLEED_PRESS;
  }else if(led_btn.btn_name == "PRESS"){
    led_btn.btn_id = PRESS_PRESS;
  }else if(led_btn.btn_name == "ELEC"){
    led_btn.btn_id = ELEC_PRESS;
  }else if(led_btn.btn_name == "HYD"){
    led_btn.btn_id = HYD_PRESS;
  }else if(led_btn.btn_name == "FUEL"){
    led_btn.btn_id = FUEL_PRESS;
  }else if(led_btn.btn_name == "APU"){
    led_btn.btn_id = APU_PRESS;
  }else if(led_btn.btn_name == "COND"){
    led_btn.btn_id = COND_PRESS;
  }else if(led_btn.btn_name == "DOOR"){
    led_btn.btn_id = DOOR_PRESS;
  }else if(led_btn.btn_name == "WHELL"){
    led_btn.btn_id = WHELL_PRESS;
  }else if(led_btn.btn_name == "F/CTL"){
    led_btn.btn_id = F_CTL_PRESS;
  }else if(led_btn.btn_name == "CLR_L"){
    led_btn.btn_id = CLR_L_PRESS;
  }else if(led_btn.btn_name == "SYS"){
    led_btn.btn_id = SYS_PRESS;
  }else if(led_btn.btn_name == "CLR_R"){
    led_btn.btn_id = CLR_R_PRESS;
  }else{
    led_btn.btn_id = -1;
  }
  toogle_led(led_btn);
}

void toogle_btn_t(char *btn_name_id){
  struct led_btn_t button;
  button.btn_name = btn_name_id;
  led_pos_handler(button);
}


#endif /* END KEYBOARDCOMPONENTS__H__ */