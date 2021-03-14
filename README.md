# keyboard_array

This project have as objetive make an keyboard array and thus minimize the hardware to use in the MCU and the code.

# Requiriments
- Atmega2560 development module or Arduino MEGA
- Keyboard array hardware ( in this case 10 rows x 15 columns)

# How it works?
Each button belong to one row and one column. In this project the rows is declared like a inputs and columns like a outputs. 
The rows pins works using the pull up resistor configuration in the MCU. 

You can see how is pull up resistor setup
https://www.circuitbasics.com/wp-content/uploads/2020/05/Untitled-design.png (this link isn't my property! but we can use like an example schematic)

The Atmega2560 can reduce the hardware implemented due into its hardware this configuration is possible, it's very simple to make if you check the datasheet or you know something about Arduino in the case that you use it. Always the columns outputs are seted in high level to this maner when any key button is pressed, the rows inputs don'r read the change (due to pull up resistor configuration). But when one column output is seted in low level and any key button is pressed, then the row input to which belong goes to low level, representing the change of the state on the digital pin and thus the MCU reach to know the location of the column and row and to this the key button pressed.

## ECAM v2.0 and v2.1

These versions have also new features like switched on of leds array when the button is pressed and data serial receive as handler of this leds array.

	ECAM v2.0 ---> Once the button is pressed then led switched on, if button is pressed again then led switched off.
	ECAM v2.1 ---> The leds array is handler by SERIAL COM port from an external device, like a PC, PLC, MCU and so on.

## Autopilot v1.1

	AUTOPILOT v1.1 ---> It has full components. Push button array, Led array, Encoder knobs array, Potentiomenter knobs array, switch selector array. Airbus A320 full autopilot panel. 

# AUTOPILOT v1.1 COMMANDS
The commands to access to each led in the keyboard array you can find [here!](led_command_list.md)

## TAGS
	- V1.0
	- ECAM v1.0
	- ECAM v2.0 (Stable)
	- ECAM v2.1 (Stable)
	- AUTOPILOT v1.0 (Stable)
	- AUTOPILOT v1.1 (Stable -  Full functions)



These versions have gotten a keyboard components on a external file called KeyboardComponents.h. This header file have all handlers for each components of this project, we can to say that it is a library using C++ and C. You can to check each components of that library and identify the purpose for each one of them following the comments. Any question you can ask me via email or github repository.