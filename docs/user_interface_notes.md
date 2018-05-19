# Delta Robot User Interface

General musings and tracking of user interface development.

## Overall System

| Group        	| Component          	| Task                                      	| Implemented 	|
|--------------	|--------------------	|-------------------------------------------	|:-----------:	|
| Fan Control  	| Speed %            	| Show current duty cycle                   	|             	|
|              	| Speed RPM          	| Show current RPM                          	|             	|
|              	| Fan Curve          	| Allow edits of temp/speed curve           	|             	|
| Temperatures 	| PCB Ambient        	| In open-air part of PCB                   	|             	|
|              	| PCB 12V Reg        	| Underneath 12V regulator                  	|             	|
|              	| External Probe     	| Flying probe (near 75V PSU)               	|             	|
| Motion       	| Status             	| Idle, homing, transit, line, spline, etc  	|             	|
|              	| Cartesian Pos      	| (X, Y, Z) in 20.05mm format               	|             	|
|              	| Angular Pos        	| Shaft angles in 34.5° format              	|             	|
| Status       	| System State       	| Overall hardware status                   	|             	|
|              	| LED 0              	| Indication matching onboard leds          	|             	|
|              	| LED 1              	|                                           	|             	|
|              	| LED 2              	|                                           	|             	|
|              	| Button 0           	| State of button presses (accept UI press) 	|             	|
|              	| Button 1           	|                                           	|             	|
|              	| CPU Load %         	| Utilisation Stat                          	|             	|
|              	| Micro Core Temp    	| Internal temperature sensor               	|             	|
|              	| Add-in card state  	| If add-in card is powered or not          	|             	|
|              	| Buzzer state       	| If buzzer is currently indicating or not  	| ?           	|
| Motor X      	| State              	| Passive, Homing, Idle, Motion, Error      	|             	|
|              	| Feedback Signal    	| -100% to +100% value for velocity/torque  	|             	|
|              	| Current Position   	| Internal tracked current position         	|             	|
|              	| Target Position    	| Servo driver target                       	|             	|
|              	| Power              	| Consumption in Watts                      	|             	|
|              	| Current            	| Draw in 1.25A format                      	|             	|
|              	| Current Fault Flag 	| Indication from sense IC of OC event      	|             	|


# UI Components required

The UI will require the following generic data driven components:

- Graphs (continuous display of live values).
- Statistics/Labels of data.
- Indicator 'lights' for button and status led indication.
- Fan curve viewer/editor.
- State Indication (text mapper from int -> strings).
- Notifications/alerts
- Mode Selection Module.
- Emergency stop button.

- Visualiser of end effector position/trail in cartesian space.
- Visualiser of delta mechanism with movements (either do IK or FK at UI side).

These elements will require grouping to improve readability, and dropdowns, tabs or navigation required to expose all information.

For example:

- Dropdown/tab in isolated box to select between motor overviews, and Servo 1-3 detail status.
- Grouping of status values, temperatures and state information.
- System stats like CPU load and microcontroller core temperature on a separate page or footer element.

Some states will flag critical issues or confirmations, and these should require either an OS level notification, in-app banner, or challenge dialog.

## Mode Selection Module

Allow selection between operation modes: Manual control, demo sequences, run program, accept serial

### Manual Mode 

Direct user input of motion controls.  
Suggest ability to toggle which input method is used:

- UI entry (sliders/buttons that fire a fixed amount of movement per press).
- Mouse/touchpad control (xy control in a box).
- Joystick control (bluetooth or connected gamepads).
- Spacemouse Control (attached to the UI/host computer).
- Manual entry of position as x,y,z fields, in mm.

### Demo Mode

Run through preset motion demos to show off the control accuracy and velocities of the delta.

Concepts include:

- Tracing out simplistic 2D geometry.
- Trace out 3D cube edges.
- Spiral and cone style patterns.

### Program Mode

UI presents interface for file selection.  
Either microcontroller stores the file, then runs, or the UI feeds motion commands piecemeal.  
Shows progress indication bar.  

### Serial Mode

Accept control inputs from external connection and other devices.  
This allows the UI to be used as a supervisor.

