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
Input 75VDC from Teknic IPC PSU.\nFilter/protection requirements are minimal.\nProvide regulated 12V for IO Logic Level/Fan/etc.\nProvide regulated 3.3V for microcontroller etc.
Text Notes 3925 2425 0    50   ~ 0
STM32 microcontroller.\nHandles sampling input sensors, aux outputs.\nGenerates 4x sets of step/direction control signals.\nInterfacing etc for PC/Phone UI.
Text Notes 1575 4500 0    50   ~ 0
Output connectors to Clearpath SDSK servos.\nOutput power and data connectors (optocoupled).\nMotor status line input circuit.\nPer-servo current monitoring.
Text Notes 3900 4500 0    50   ~ 0
Output for PWM controlled fan, external temp probes.\nGeneric outputs for internal or end affector accessories.\nOptocoupled outputs for external device control.\nOptocoupled inputs for inbound control.
$Comp
L appli_mount:MNT_M3_PLATED MNT1
U 1 1 5AB8A4A1
P 7150 4550
F 0 "MNT1" H 7303 4550 47  0000 L CNN
F 1 "MNT_M3_PLATED" H 7303 4463 47  0000 L CNN
F 2 "Applidyne_Mount:MTGP650H330" H 7150 4725 47  0001 C CNN
F 3 "" H 7150 4550 60  0000 C CNN
	1    7150 4550
	1    0    0    -1  
$EndComp
$Comp
L appli_mount:MNT_M3_PLATED MNT2
U 1 1 5AB8A632
P 7150 5100
F 0 "MNT2" H 7303 5100 47  0000 L CNN
F 1 "MNT_M3_PLATED" H 7303 5013 47  0000 L CNN
F 2 "Applidyne_Mount:MTGP650H330" H 7150 5275 47  0001 C CNN
F 3 "" H 7150 5100 60  0000 C CNN
	1    7150 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	7150 4750 7150 4850
Wire Wire Line
	7150 5400 7150 5300
$Comp
L appli_mount:MNT_M3_PLATED MNT3
U 1 1 5AB8A85C
P 7150 5625
F 0 "MNT3" H 7303 5625 47  0000 L CNN
F 1 "MNT_M3_PLATED" H 7303 5538 47  0000 L CNN
F 2 "Applidyne_Mount:MTGP650H330" H 7150 5800 47  0001 C CNN
F 3 "" H 7150 5625 60  0000 C CNN
	1    7150 5625
	1    0    0    -1  
$EndComp
Wire Wire Line
	7150 5825 7150 5950
$Comp
L appli_power:GND #PWR0101
U 1 1 5AB8ABC2
P 8200 5950
F 0 "#PWR0101" H 8200 5700 50  0001 C CNN
F 1 "GND" V 8205 5822 50  0000 R CNN
F 2 "" H 8200 5950 60  0000 C CNN
F 3 "" H 8200 5950 60  0000 C CNN
	1    8200 5950
	0    -1   -1   0   
$EndComp
$Comp
L appli_power:GND #PWR0102
U 1 1 5AB8AC0E
P 8200 5400
F 0 "#PWR0102" H 8200 5150 50  0001 C CNN
F 1 "GND" V 8205 5272 50  0000 R CNN
F 2 "" H 8200 5400 60  0000 C CNN
F 3 "" H 8200 5400 60  0000 C CNN
	1    8200 5400
	0    -1   -1   0   
$EndComp
$Comp
L appli_power:GND #PWR0103
U 1 1 5AB8AC25
P 8200 4850
F 0 "#PWR0103" H 8200 4600 50  0001 C CNN
F 1 "GND" V 8205 4722 50  0000 R CNN
F 2 "" H 8200 4850 60  0000 C CNN
F 3 "" H 8200 4850 60  0000 C CNN
	1    8200 4850
	0    -1   -1   0   
$EndComp
Text Notes 7050 4325 0    98   ~ 20
Mechanical Mounts
Text Notes 9625 4325 0    98   ~ 20
Fiducials
Text Notes 8475 1800 0    98   ~ 20
Artwork
Text Notes 7350 1975 0    50   ~ 0
DONT PANIC
$Comp
L appli_fiducial:FIDUCIAL FID1
U 1 1 5AB8B482
P 9950 4650
F 0 "FID1" H 10103 4694 47  0000 L CNN
F 1 "FIDUCIAL" H 10103 4607 47  0000 L CNN
F 2 "Applidyne_Fiducial:FID100X200" H 9950 4825 47  0001 C CNN
F 3 "" H 9950 4650 60  0000 C CNN
	1    9950 4650
	1    0    0    -1  
$EndComp
$Comp
L appli_fiducial:FIDUCIAL FID3
U 1 1 5AB8B51B
P 9950 5325
F 0 "FID3" H 10103 5369 47  0000 L CNN
F 1 "FIDUCIAL" H 10103 5282 47  0000 L CNN
F 2 "Applidyne_Fiducial:FID100X200" H 9950 5500 47  0001 C CNN
F 3 "" H 9950 5325 60  0000 C CNN
	1    9950 5325
	1    0    0    -1  
$EndComp
$Comp
L appli_fiducial:FIDUCIAL FID2
U 1 1 5AB8B644
P 9950 4975
F 0 "FID2" H 10103 5019 47  0000 L CNN
F 1 "FIDUCIAL" H 10103 4932 47  0000 L CNN
F 2 "Applidyne_Fiducial:FID100X200" H 9950 5150 47  0001 C CNN
F 3 "" H 9950 4975 60  0000 C CNN
	1    9950 4975
	1    0    0    -1  
$EndComp
$Comp
L appli_fiducial:FIDUCIAL FID4
U 1 1 5AB8B67C
P 9950 5650
F 0 "FID4" H 10103 5694 47  0000 L CNN
F 1 "FIDUCIAL" H 10103 5607 47  0000 L CNN
F 2 "Applidyne_Fiducial:FID100X200" H 9950 5825 47  0001 C CNN
F 3 "" H 9950 5650 60  0000 C CNN
	1    9950 5650
	1    0    0    -1  
$EndComp
Text Notes 9300 3100 0    50   ~ 0
Lightning Bolt
$Comp
L appli_logo:LOGO LOGO1
U 1 1 5AE808C1
P 7550 2200
F 0 "LOGO1" H 7728 2200 47  0000 L CNN
F 1 "LOGO" H 7550 1925 47  0001 C CNN
F 2 "Applidyne_Logo:PANIC_LOGO890X810" H 7550 2000 47  0001 C CNN
F 3 "" H 7550 2200 60  0000 C CNN
	1    7550 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	7150 5400 8200 5400
Wire Wire Line
	7150 4850 8200 4850
Wire Wire Line
	7150 5950 8200 5950
$Comp
L appli_logo:LOGO LOGO11
U 1 1 5AE0BB40
P 10375 3350
F 0 "LOGO11" H 10553 3350 47  0000 L CNN
F 1 "LOGO" H 10375 3075 47  0001 C CNN
F 2 "Applidyne_Logo:FIRE_LOGO_320X420" H 10375 3150 47  0001 C CNN
F 3 "" H 10375 3350 60  0000 C CNN
	1    10375 3350
	1    0    0    -1  
$EndComp
Text Notes 10225 3100 0    50   ~ 0
Hot Fire
$Comp
L appli_logo:LOGO LOGO8
U 1 1 5AE0BEB4
P 9725 2350
F 0 "LOGO8" H 9903 2350 47  0000 L CNN
F 1 "LOGO" H 9725 2075 47  0001 C CNN
F 2 "Applidyne_Logo:WARN_TRIANGLE_LOGO440X400" H 9725 2150 47  0001 C CNN
F 3 "" H 9725 2350 60  0000 C CNN
	1    9725 2350
	1    0    0    -1  
$EndComp
Text Notes 9625 2075 0    50   ~ 0
WARN Triangle
$Comp
L appli_logo:LOGO LOGO9
U 1 1 5AE0BFFF
P 9725 2725
F 0 "LOGO9" H 9903 2725 47  0000 L CNN
F 1 "LOGO" H 9725 2450 47  0001 C CNN
F 2 "Applidyne_Logo:WARN_TRIANGLE_LOGO440X400" H 9725 2525 47  0001 C CNN
F 3 "" H 9725 2725 60  0000 C CNN
	1    9725 2725
	1    0    0    -1  
$EndComp
$Comp
L appli_logo:LOGO LOGO6
U 1 1 5AE0C033
P 9350 2350
F 0 "LOGO6" H 9528 2350 47  0000 L CNN
F 1 "LOGO" H 9350 2075 47  0001 C CNN
F 2 "Applidyne_Logo:WARN_TRIANGLE_LOGO440X400" H 9350 2150 47  0001 C CNN
F 3 "" H 9350 2350 60  0000 C CNN
	1    9350 2350
	-1   0    0    -1  
$EndComp
$Comp
L appli_logo:LOGO LOGO7
U 1 1 5AE0C07F
P 9350 2725
F 0 "LOGO7" H 9528 2725 47  0000 L CNN
F 1 "LOGO" H 9350 2450 47  0001 C CNN
F 2 "Applidyne_Logo:WARN_TRIANGLE_LOGO440X400" H 9350 2525 47  0001 C CNN
F 3 "" H 9350 2725 60  0000 C CNN
	1    9350 2725
	-1   0    0    -1  
$EndComp
$Comp
L appli_logo:LOGO LOGO10
U 1 1 5AE0C51A
P 10375 2550
F 0 "LOGO10" H 10553 2550 47  0000 L CNN
F 1 "LOGO" H 10375 2275 47  0001 C CNN
F 2 "Applidyne_Logo:WARN_TRIANGLE_LOGO440X400_MASK" H 10375 2350 47  0001 C CNN
F 3 "" H 10375 2550 60  0000 C CNN
	1    10375 2550
	1    0    0    -1  
$EndComp
$Comp
L appli_logo:LOGO LOGO4
U 1 1 5AE0D33C
P 8225 2775
F 0 "LOGO4" H 8403 2775 47  0000 L CNN
F 1 "LOGO" H 8225 2500 47  0001 C CNN
F 2 "Applidyne_Logo:THERMO_LOGO_170X350" H 8225 2575 47  0001 C CNN
F 3 "" H 8225 2775 60  0000 C CNN
	1    8225 2775
	1    0    0    -1  
$EndComp
Text Notes 8025 2525 0    50   ~ 0
Thermometre
$Comp
L appli_logo:LOGO LOGO5
U 1 1 5AE0D3D7
P 8225 3350
F 0 "LOGO5" H 8403 3350 47  0000 L CNN
F 1 "LOGO" H 8225 3075 47  0001 C CNN
F 2 "Applidyne_Logo:USB_LOGO410X250" H 8225 3150 47  0001 C CNN
F 3 "" H 8225 3350 60  0000 C CNN
	1    8225 3350
	1    0    0    -1  
$EndComp
Text Notes 8025 3100 0    50   ~ 0
USB Symbol
$Comp
L appli_logo:LOGO LOGO2
U 1 1 5AE0DAF8
P 7550 2775
F 0 "LOGO2" H 7728 2775 47  0000 L CNN
F 1 "LOGO" H 7550 2500 47  0001 C CNN
F 2 "Applidyne_Logo:BUG_LOGO250X250" H 7550 2575 47  0001 C CNN
F 3 "" H 7550 2775 60  0000 C CNN
	1    7550 2775
	1    0    0    -1  
$EndComp
Text Notes 7350 2525 0    50   ~ 0
Bug Symbol
$Comp
L appli_logo:LOGO LOGO3
U 1 1 5AE0E4A8
P 7550 3350
F 0 "LOGO3" H 7728 3350 47  0000 L CNN
F 1 "LOGO" H 7550 3075 47  0001 C CNN
F 2 "Applidyne_Logo:BULB_LOGO230X330" H 7550 3150 47  0001 C CNN
F 3 "" H 7550 3350 60  0000 C CNN
	1    7550 3350
	1    0    0    -1  
$EndComp
Text Notes 7350 3100 0    50   ~ 0
Lamp Symbol
$Comp
L appli_logo:LOGO LOGO12
U 1 1 5AE14C78
P 9525 3350
F 0 "LOGO12" H 9703 3350 47  0000 L CNN
F 1 "LOGO" H 9525 3075 47  0001 C CNN
F 2 "Applidyne_Logo:BOLT_LOGO_310X490" H 9525 3150 47  0001 C CNN
F 3 "" H 9525 3350 60  0000 C CNN
	1    9525 3350
	1    0    0    -1  
$EndComp
$EndSCHEMATC
