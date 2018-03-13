# Connectors and Looms

General overview of components required for power delivery and signalling.

The main PCB will provide distro and sensing (voltage & current sense per motor) as well as IO connections.
This should hopefully keep the wiring neat and professional (looking, at least).

I'd like to sleeve each wire/bundle as well, but we'll see how much time I have.


# Mains Input Connection

Connect output of holder to E-Stop button.

Connect E-Stop to IPC-3 PSU

### Parts

IEC320 Connector 	Fused with DPST switch (no backlight)
Spade Terminals		
Ring Terminal 		
240V Fuse 			M205 Quick Blow Fuse 4A
Emergency Stop		ABB Compact Head Mushroom - 40mm Panel Mount - 1SFA619550R1041 

### Wire

Preferably in black insulated (round):

2.5mm^2		Blue
2.5mm^2		Brown
2.5mm^2		Green/Yellow


# Teknic IPC-3 Open Frame PSU

## Mains Input connector

Connect to input protection circuit(s).

### Parts

Molex MicroFit? 	8-Way - 39-01-2080
Molex MicroFit?		Crimp - 39-00-0039

### Wire

From E-STOP mushroom (2.5mm^2 3-core)

### Pinout

Pin 1 		VCC
Pin 2 		GND

## 75 Output Connector

Connect IPC-3 Output to main PCB.

### Parts

Molex Sabre 		2-Way - 44441-2002 
Molex Sabre 		Crimp - 43375-0001

### Wire

14-16AWG 	Red
14-16AWG 	Black

### Pinout

Pin 1 		VCC
Pin 2 		GND


# Teknic Clearpath Motor

## Power Connector

Connect motor to main PCB

### Parts

Molex MiniFit-Jr 	4-Way - 39-01-3045
Molex MiniFit-Jr 	Crimp - 39-00-0078

### Wire

16AWG 		Red
16AWG 		Black

### Pinout

Pin 1 		GND
Pin 2 		GND
Pin 3 		VCC
Pin 4 		VCC

## Control Connector

Connect to main PCB.

### Parts

Molex MiniFit-Jr 	8-Way - 39-01-3085
Molex MiniFit-Jr 	Crimp - 39-00-0078

### Wire

22AWG 		Blue
22AWG 		White

### Pinout

Pin 1 		HLFB+
Pin 2 		Input B+
Pin 3 		Input A+
Pin 4 		Enable+
Pin 5 		HLFB-
Pin 6 		Input B-
Pin 7 		Input A-
Pin 8 		Enable-

