# Connectors and Looms

General overview of components required for power delivery and signalling.

The main PCB will provide distro and sensing (voltage & current sense per motor) as well as IO connections.
This should hopefully keep the wiring neat and professional (looking, at least).

I'd like to sleeve each wire/bundle as well, but we'll see how much time I have.

# Mains Input Connection

Connect output of holder to IPC-3/5 PSU.

### Parts

- IEC320 Connector 	- Fused with DPST switch (no backlight). 
- Spade Terminals	- Female, fully shielded terminals. 
- Ring Terminal 	- for grounding connection(s). 
- 240IEC320V Fuse 	- M205 Quick Blow Fuse 4A. 

https://www.jaycar.com.au/iec-fuse-chassis-male-power-plug-with-switch/p/PP4003
https://www.aliexpress.com/item/AC250V-10A-2pin-ON-OFF-Rocker-Switch-IEC320-C14-Inlet-Socket-Single-Fuse-red-Switch-black/32861211516.html

Spade crimps - Potentially dubious quality, but better than the unshielded ones I have on hand...  
https://www.aliexpress.com/item/Lowest-Price-30PCS-Yellow-red-blue-Splice-Wire-Connectors-Insulated-Male-Female-Crimp-Spade-Terminals-18/32764325209.html

### Wire

Preferably in black insulated (round):

- 2.5mm^2		Blue
- 2.5mm^2		Brown
- 2.5mm^2		Green/Yellow

### Pinout

- Active is connected directly to the fuse holder on the IEC connector. 
- Connect the fused active to the switch. 
- Connect the switched active to the IPC active connection. 
- Neutral directly to IPC neutral pin. 
- Earth directly to the IPC neutral pin. 


# Teknic IPC-3/5 Open Frame PSU

## Mains Input connector

Connect to input protection circuit(s).

### Parts

- Molex MiniFit-Jr 	8-Way - 39-01-2080
- Molex MiniFit-Jr	Crimp - 39-00-0039

### Wire

From IEC switch unit (2.5mm^2 3-core).  
This was cut from an older black 240-IEC cable.

### Pinout

- Pin 1-4 	No Connect for 230V, read manual for 110V operation
- Pin 5 		AC Active
- Pin 2 		Earth
- Pin 2 		Earth
- Pin 8 		AC Neutral

## 75 Output Connector

- Connect IPC-3/5 Output to main PCB.  
- The PSU and the PCB use common connector and pinout.
- This is also listed in the Teknic IPC User manual PDF.

### Parts

- Molex Sabre 		2-Way - 44441-2002 
- Molex Sabre 		Crimp - 43375-0001

### Wire

- 14-16AWG 	Red
- 14-16AWG 	Black

### Pinout

- Pin 1 		VCC
- Pin 2 		GND


# Teknic Clearpath Motor(s)

I bought ModDIY pre-crimped mini-fit cables to reduce the labour of individually sleeving these.  
In hindsight, I should have provisioned a panel mount connector for the top plate, and had the full looms made up (including connectors) as the ModDIY cables had thick heatshrink that interferes with insertion into the housing, taking as long/longer than full DIY cables...

## Power Connector

- Connect motor power to the main PCB.
- Motor and PCB share common pinouts, connectors etc.

### Parts

- Molex MiniFit-Jr 	4-Way - 39-01-3045
- Molex MiniFit-Jr 	Crimp - 39-00-0078

https://au.rs-online.com/web/p/pcb-connector-housings/6704575/  
https://au.rs-online.com/web/p/pcb-connector-contacts/2155887  

### Wire

- 16AWG 		Red
- 16AWG 		Black

### Pinout

- Pin 1 		VCC
- Pin 2 		GND
- Pin 3 		VCC
- Pin 4 		GND

## Control Connector

- Connect clearpath data connector to main PCB.  
- Motor and PCB share common pinouts, connectors etc.  

### Parts

- Molex MiniFit-Jr 	8-Way - 39-01-3085
- Molex MiniFit-Jr 	Crimp - 39-00-0078

https://au.rs-online.com/web/p/pcb-connector-housings/8428197/
https://au.rs-online.com/web/p/pcb-connector-contacts/2155887

### Wire

- 22AWG 		Blue
- 22AWG 		White

### Pinout

- Pin 1 		HLFB+
- Pin 2 		Input B+
- Pin 3 		Input A+
- Pin 4 		Enable+
- Pin 5 		HLFB-
- Pin 6 		Input B-
- Pin 7 		Input A-
- Pin 8 		Enable-

The PCB connects the + signals to the common 12V rail, as signalling is done on the low side.

# Expansion LEMO Connectors

## Data Connector

Allows for user-domain expansion of USB and some generic IO (UART, CAN, 2x input, 2x output).

### Parts

LEMO 0B 9-pin

Socket: EGG-0B-309-CLL
https://au.mouser.com/ProductDetail/LEMO/EGG0B309CLL

Plug: FGG-0B-309-CLAD-52Z
https://au.mouser.com/ProductDetail/LEMO/FGG0B309CLAD52Z

As a pair (off-brand equiv): 
https://www.aliexpress.com/item/LEMO-connector-EGG-0B-309-CLL-FGG-0B-309-CLAD-Z-31Z-42Z-52Z-LEMO-connector/32318767655.html

### Wire

USB3		Black Sheath

Usb cable provides 9 usable conductors (not counting the shield), with good shielding, and compatible diameter with lemo sheath.

Black glue-lined 6mm heatshrink was used to protect the joints on the panel mount side.

### Pinout

When looking at the back side of the LEMO panel mount connector (wire side), with the alignment notch facing up.
The pin pattern is defined where pin 1 is the pin directly underneath the alignment notch, counting upwards in a counter clockwise direction, and pin 9 is the centre pin.

```
     1      
  2     8   
 3   9   7  
  4     6   
     5     
```

'Legacy' USB FS Connection:

- Pin 1 		GND 	BLACK
- Pin 2 		D+		GREEN
- Pin 3 		D-		WHITE
- Pin 4 		5V		RED

IO Group 1 (CAN Capable):

- Pin 5 		In/RX	BLUE
- Pin 6 		Out/TX 	YELLOW
- Pin 9 		IOGND	BARE (in pair shield)

IO Group 0 (UART Capable):

- Pin 7 		In/RX	PURPLE
- Pin 8 		Out/TX 	ORANGE
- Pin 9 		IOGND	BARE (in pair shield)

IO and USB don't share ground. The two IO groups share ground cores as the PCB joins them anyway.

### Notes

- The USB3 insulation diameter just fits inside the LEMO strain relief jacket. This required a bit of force, and some slight lubrication of the wire.
- The USB Shield connection (outer copper core, and woven+foil shield) was not electrically connected at the lemo plug/socket side.
- Hot air gun was used to slightly soften the panel-mount -> PCB loom to allow bending. These bends are reasonably well retained after cooling.


## Motor Connector

Allows for addition of an external clearpath motor (or something that uses the same IO style)

### Parts

LEMO 0B 7-pin

Socket: EGG-0B-307-CLL
https://au.mouser.com/ProductDetail/LEMO/EGG0B307CLL

Plug: FGG-0B-307-CLAD-52Z
https://au.mouser.com/ProductDetail/LEMO/FGG0B307CLAD52Z

As a pair (off-brand equiv): 
https://www.aliexpress.com/item/LEMO-connector-EGG-0B-307-CLL-FGG-0B-307-CLAD-Z-31Z-42Z-52Z-LEMO-connector/32318771454.html

### Wire

N/A

### Pinout

Not completed yet.

### Notes

- The 7-pin connector was selected for unique pin count, higher supported wire gauge (and higher per-pin current).
- Not intended for continuous high power delivery to motors, mostly for use with rotation or belt surfaces, or possibly rail drive.


# Expansion Temperature Probe

Used for temperature sensing off the main control board. 

Recommended location is inside the warmer area of the IPC-3/5 PSU to allow for automatically moderated airflow.

### Parts

10K NTC Thermistor  
https://www.aliexpress.com/item/10K-Precision-Epoxy-Thermistor-3435-NTC-Pack-of-2/32819250168.html

2-pin 2.54mm pitch 'DuPont' connector

### Wire

Thermistor already comes pre-wired.

### Pinout

It behaves as a resistor, pinout doesn't matter. 

Crimp both terminals into the 2-pin socket, connect to temp header on PCB.

