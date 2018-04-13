EESchema Schematic File Version 4
LIBS:zaphod-controller-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 5
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 1525 2650 1925 1150
U 5AB8A40C
F0 "Power Filtering and Regulation" 50
F1 "power.sch" 50
$EndSheet
$Sheet
S 3925 2650 1925 1150
U 5AB8A40F
F0 "Processing and Control" 50
F1 "microcontroller.sch" 50
$EndSheet
$Sheet
S 1525 4650 1925 1150
U 5AB8A412
F0 "Output Circuits and Connectors" 50
F1 "motor_outputs.sch" 50
$EndSheet
$Sheet
S 3925 4650 1925 1150
U 5AB8A415
F0 "Expansion I/O" 50
F1 "expansion_io.sch" 50
$EndSheet
Text Notes 4400 975  0    157  ~ 31
Delta-Bot Control Electronics
Text Notes 4300 1375 0    79   ~ 0
Handles supervisory and control tasks for the delta-robot.\nDesigned to reduce wiring complexity and improve functionality.
Text Notes 1525 2450 0    50   ~ 0
Input 75VDC from Teknic IPC-3 PSU.\nFilter/protection requirements are minimal.\nProvide regulated 12V for IO Logic Level/Fan/etc.\nProvide regulated 3.3V for microcontroller etc.
Text Notes 3925 2425 0    50   ~ 0
STM32 microcontroller.\nHandles sampling input sensors, aux outputs.\nGenerates 4x sets of step/direction control signals.\nInterfacing etc for PC/Phone UI.
Text Notes 1575 4500 0    50   ~ 0
Output connectors to Clearpath SDSK servos.\nOutput power and data connectors (optocoupled).\nMotor status line input circuit.\nPer-servo current monitoring.
Text Notes 3900 4500 0    50   ~ 0
Output for PWM controlled fan, external temp probes.\nGeneric load drivers for high power end affector accessories.\nOptocoupled outputs for external device control.\nOptocoupled inputs for inbound control.
$Comp
L appli_mount:MNT_M3_PLATED MNT?
U 1 1 5AB8A4A1
P 8175 4500
F 0 "MNT?" H 8328 4500 47  0000 L CNN
F 1 "MNT_M3_PLATED" H 8328 4413 47  0000 L CNN
F 2 "MTGP650H330" H 8175 4675 47  0001 C CNN
F 3 "" H 8175 4500 60  0000 C CNN
	1    8175 4500
	1    0    0    -1  
$EndComp
$Comp
L appli_mount:MNT_M3_PLATED MNT?
U 1 1 5AB8A580
P 9075 4500
F 0 "MNT?" H 9228 4500 47  0000 L CNN
F 1 "MNT_M3_PLATED" H 9228 4413 47  0000 L CNN
F 2 "MTGP650H330" H 9075 4675 47  0001 C CNN
F 3 "" H 9075 4500 60  0000 C CNN
	1    9075 4500
	1    0    0    -1  
$EndComp
$Comp
L appli_mount:MNT_M3_PLATED MNT?
U 1 1 5AB8A632
P 8175 5050
F 0 "MNT?" H 8328 5050 47  0000 L CNN
F 1 "MNT_M3_PLATED" H 8328 4963 47  0000 L CNN
F 2 "MTGP650H330" H 8175 5225 47  0001 C CNN
F 3 "" H 8175 5050 60  0000 C CNN
	1    8175 5050
	1    0    0    -1  
$EndComp
$Comp
L appli_mount:MNT_M3_PLATED MNT?
U 1 1 5AB8A664
P 9075 5050
F 0 "MNT?" H 9228 5050 47  0000 L CNN
F 1 "MNT_M3_PLATED" H 9228 4963 47  0000 L CNN
F 2 "MTGP650H330" H 9075 5225 47  0001 C CNN
F 3 "" H 9075 5050 60  0000 C CNN
	1    9075 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	8175 4700 8175 4800
Wire Wire Line
	8175 4800 9075 4800
Wire Wire Line
	9075 4800 9075 4700
Wire Wire Line
	8175 5350 8175 5250
Wire Wire Line
	8175 5350 9075 5350
Wire Wire Line
	9075 5350 9075 5250
$Comp
L appli_mount:MNT_M3_PLATED MNT?
U 1 1 5AB8A85C
P 8175 5575
F 0 "MNT?" H 8328 5575 47  0000 L CNN
F 1 "MNT_M3_PLATED" H 8328 5488 47  0000 L CNN
F 2 "MTGP650H330" H 8175 5750 47  0001 C CNN
F 3 "" H 8175 5575 60  0000 C CNN
	1    8175 5575
	1    0    0    -1  
$EndComp
$Comp
L appli_mount:MNT_M3_PLATED MNT?
U 1 1 5AB8A863
P 9075 5575
F 0 "MNT?" H 9228 5575 47  0000 L CNN
F 1 "MNT_M3_PLATED" H 9228 5488 47  0000 L CNN
F 2 "MTGP650H330" H 9075 5750 47  0001 C CNN
F 3 "" H 9075 5575 60  0000 C CNN
	1    9075 5575
	1    0    0    -1  
$EndComp
Wire Wire Line
	8175 5775 8175 5900
Wire Wire Line
	8175 5900 9075 5900
Wire Wire Line
	9075 5900 9075 5775
Wire Wire Line
	9075 5350 9225 5350
Connection ~ 9075 5350
Wire Wire Line
	9075 4800 9225 4800
Connection ~ 9075 4800
Wire Wire Line
	9075 5900 9225 5900
Connection ~ 9075 5900
$Comp
L appli_power:GND #PWR0101
U 1 1 5AB8ABC2
P 9225 5900
F 0 "#PWR0101" H 9225 5650 50  0001 C CNN
F 1 "GND" V 9230 5772 50  0000 R CNN
F 2 "" H 9225 5900 60  0000 C CNN
F 3 "" H 9225 5900 60  0000 C CNN
	1    9225 5900
	0    -1   -1   0   
$EndComp
$Comp
L appli_power:GND #PWR0102
U 1 1 5AB8AC0E
P 9225 5350
F 0 "#PWR0102" H 9225 5100 50  0001 C CNN
F 1 "GND" V 9230 5222 50  0000 R CNN
F 2 "" H 9225 5350 60  0000 C CNN
F 3 "" H 9225 5350 60  0000 C CNN
	1    9225 5350
	0    -1   -1   0   
$EndComp
$Comp
L appli_power:GND #PWR0103
U 1 1 5AB8AC25
P 9225 4800
F 0 "#PWR0103" H 9225 4550 50  0001 C CNN
F 1 "GND" V 9230 4672 50  0000 R CNN
F 2 "" H 9225 4800 60  0000 C CNN
F 3 "" H 9225 4800 60  0000 C CNN
	1    9225 4800
	0    -1   -1   0   
$EndComp
Text Notes 8075 4275 0    98   ~ 20
Mechanical Mounts
Text Notes 8425 3175 0    98   ~ 20
Fiducials
Text Notes 8500 2200 0    98   ~ 20
Artwork
Text Notes 7650 2375 0    50   ~ 0
eUI logo
Text Notes 8525 2400 0    50   ~ 0
hh reference
$Comp
L appli_fiducial:FIDUCIAL FID?
U 1 1 5AB8B482
P 8175 3425
F 0 "FID?" H 8328 3469 47  0000 L CNN
F 1 "FIDUCIAL" H 8328 3382 47  0000 L CNN
F 2 "FID*" H 8175 3600 47  0001 C CNN
F 3 "" H 8175 3425 60  0000 C CNN
	1    8175 3425
	1    0    0    -1  
$EndComp
$Comp
L appli_fiducial:FIDUCIAL FID?
U 1 1 5AB8B51B
P 9075 3425
F 0 "FID?" H 9228 3469 47  0000 L CNN
F 1 "FIDUCIAL" H 9228 3382 47  0000 L CNN
F 2 "FID*" H 9075 3600 47  0001 C CNN
F 3 "" H 9075 3425 60  0000 C CNN
	1    9075 3425
	1    0    0    -1  
$EndComp
$Comp
L appli_fiducial:FIDUCIAL FID?
U 1 1 5AB8B644
P 8175 3750
F 0 "FID?" H 8328 3794 47  0000 L CNN
F 1 "FIDUCIAL" H 8328 3707 47  0000 L CNN
F 2 "FID*" H 8175 3925 47  0001 C CNN
F 3 "" H 8175 3750 60  0000 C CNN
	1    8175 3750
	1    0    0    -1  
$EndComp
$Comp
L appli_fiducial:FIDUCIAL FID?
U 1 1 5AB8B67C
P 9075 3750
F 0 "FID?" H 9228 3794 47  0000 L CNN
F 1 "FIDUCIAL" H 9228 3707 47  0000 L CNN
F 2 "FID*" H 9075 3925 47  0001 C CNN
F 3 "" H 9075 3750 60  0000 C CNN
	1    9075 3750
	1    0    0    -1  
$EndComp
Text Notes 9525 2400 0    50   ~ 0
75V Present
$Comp
L Graphic:SYM_Flash_Small #SYM?
U 1 1 5AB8BC9D
P 9725 2700
F 0 "#SYM?" V 9635 2700 50  0001 C CNN
F 1 "SYM_Flash_Small" V 9815 2700 50  0001 C CNN
F 2 "Symbol:Symbol_Highvoltage_Type2_CopperTop_Small" H 9725 2675 50  0001 C CNN
F 3 "~" H 10125 2600 50  0001 C CNN
	1    9725 2700
	1    0    0    -1  
$EndComp
$EndSCHEMATC
