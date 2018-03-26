EESchema Schematic File Version 4
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 5
Title "Servo Interfaces"
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 5175 1025 0    79   ~ 16
Servo Power Connectors
Text Notes 2025 900  0    79   ~ 16
Servo Current Monitors
Text Notes 4925 5000 0    79   ~ 16
Servo Data Connectors
Text Notes 1650 5000 0    79   ~ 16
Servo IO Circuits
$Comp
L appli_capacitor:100NF_X7R_1005M C?
U 1 1 5AB91AFB
P 2400 2050
F 0 "C?" H 2323 1915 50  0000 R CNN
F 1 "100NF_X7R_1005M" H 2323 1995 35  0000 R CNN
F 2 "Applidyne_Capacitor:CAPC1005X60N" V 2515 1950 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1936565.pdf" V 2540 1950 20  0001 C CNN
F 4 "TDK" V 2590 1950 20  0001 C CNN "manf"
F 5 "CGA2B3X7R1H104K050BB" V 2615 1950 20  0001 C CNN "manf#"
F 6 "Element14" V 2640 1950 20  0001 C CNN "Supplier"
F 7 "2210822" V 2665 1950 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/tdk/cga2b3x7r1h104k050bb/cap-mlcc-x7r-100nf-50v-0402/dp/2210822" V 2690 1950 20  0001 C CNN "Supplier URL"
F 9 "0.087" V 2715 1950 20  0001 C CNN "Supplier Price"
F 10 "100" V 2740 1950 20  0001 C CNN "Supplier Price Break"
	1    2400 2050
	1    0    0    1   
$EndComp
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5AB91CBE
P 2400 1350
F 0 "#PWR?" H 2400 1310 30  0001 C CNN
F 1 "+3.3V" H 2409 1488 30  0000 C CNN
F 2 "" H 2400 1350 60  0000 C CNN
F 3 "" H 2400 1350 60  0000 C CNN
	1    2400 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2400 1850 2400 1500
Wire Wire Line
	2400 1500 2550 1500
Connection ~ 2400 1500
Wire Wire Line
	2400 1500 2400 1350
Wire Wire Line
	3050 2200 3050 2250
Wire Wire Line
	2400 2050 2400 2300
$Comp
L appli_power:GND #PWR?
U 1 1 5AB91D62
P 3050 2300
F 0 "#PWR?" H 3050 2050 50  0001 C CNN
F 1 "GND" H 3055 2127 50  0000 C CNN
F 2 "" H 3050 2300 60  0000 C CNN
F 3 "" H 3050 2300 60  0000 C CNN
	1    3050 2300
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5AB91D78
P 2400 2300
F 0 "#PWR?" H 2400 2050 50  0001 C CNN
F 1 "GND" H 2405 2127 50  0000 C CNN
F 2 "" H 2400 2300 60  0000 C CNN
F 3 "" H 2400 2300 60  0000 C CNN
	1    2400 2300
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+POWER #PWR?
U 1 1 5AB91E53
P 3700 1350
F 0 "#PWR?" H 3700 1300 20  0001 C CNN
F 1 "+POWER" H 3703 1488 30  0000 C CNN
F 2 "" H 3700 1350 60  0000 C CNN
F 3 "" H 3700 1350 60  0000 C CNN
	1    3700 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 1350 3700 1500
Wire Wire Line
	3700 1500 3550 1500
Wire Wire Line
	3550 1700 4475 1700
$Comp
L appli_special:ACS711KEXLT_15AB U?
U 1 1 5AB955A4
P 3050 1700
F 0 "U?" H 3050 2165 50  0000 C CNN
F 1 "ACS711KEXLT_15AB" H 3050 2074 50  0000 C CNN
F 2 "Applidyne_QFN:ACS-EX-12" H 3050 900 50  0001 C CNN
F 3 "http://www.allegromicro.com/~/media/Files/Datasheets/ACS711-Datasheet.ashx" H 3050 600 50  0001 C CNN
F 4 "Allegro MicroSystems" H 3050 800 50  0001 C CNN "manf"
F 5 "ACS711KEXLT-15AB-T" H 3050 700 50  0001 C CNN "manf#"
F 6 "DigiKey" H 3050 500 50  0001 C CNN "Supplier"
F 7 "620-1483-1-ND" H 3050 400 50  0001 C CNN "Suppler Part No"
F 8 "https://www.digikey.com.au/product-detail/en/allegro-microsystems-llc/ACS711KEXLT-15AB-T/620-1483-1-ND/3868192" H 3050 300 50  0001 C CNN "Supplier URL"
F 9 "1.90" H 3050 200 50  0001 C CNN "Supplier Price"
F 10 "1" H 3050 100 50  0001 C CNN "Supplier Price Break"
	1    3050 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2850 2200 2850 2250
Wire Wire Line
	2850 2250 2950 2250
Connection ~ 3050 2250
Wire Wire Line
	3050 2250 3050 2300
Wire Wire Line
	3250 2200 3250 2250
Wire Wire Line
	3250 2250 3150 2250
Wire Wire Line
	3150 2200 3150 2250
Connection ~ 3150 2250
Wire Wire Line
	3150 2250 3050 2250
Wire Wire Line
	2950 2200 2950 2250
Connection ~ 2950 2250
Wire Wire Line
	2950 2250 3050 2250
Text Notes 3375 2225 0    50   ~ 0
Datasheet suggests \nNC->GND for better EMI
Text Notes 900  1425 0    50   ~ 0
90mA/A sensitivity.\n3.3V/2 = 0A
Wire Wire Line
	2550 1600 1700 1600
Wire Wire Line
	2550 1700 2150 1700
$Comp
L appli_resistor:10K_1005M R?
U 1 1 5AB96600
P 2150 1575
F 0 "R?" H 2077 1390 50  0000 R CNN
F 1 "10K_1005M" H 2077 1470 35  0000 R CNN
F 2 "Applidyne_Resistor:RESC1005X40N" V 2260 1425 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2111203.pdf" V 2285 1425 20  0001 C CNN
F 4 "VISHAY" V 2335 1425 20  0001 C CNN "manf"
F 5 "CRCW040210K0FKED" V 2360 1425 20  0001 C CNN "manf#"
F 6 "Element14" V 2385 1425 20  0001 C CNN "Supplier"
F 7 "1469669" V 2410 1425 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/vishay/crcw040210k0fked/res-thick-film-10k-1-0-063w-0402/dp/1469669" V 2435 1425 20  0001 C CNN "Supplier URL"
F 9 "0.009" V 2460 1425 20  0001 C CNN "Supplier Price"
F 10 "10" V 2485 1425 20  0001 C CNN "Supplier Price Break"
	1    2150 1575
	1    0    0    1   
$EndComp
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5AB96FA3
P 2150 1250
F 0 "#PWR?" H 2150 1210 30  0001 C CNN
F 1 "+3.3V" H 2159 1388 30  0000 C CNN
F 2 "" H 2150 1250 60  0000 C CNN
F 3 "" H 2150 1250 60  0000 C CNN
	1    2150 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 1250 2150 1275
Wire Wire Line
	2150 1575 2150 1700
Connection ~ 2150 1700
Wire Wire Line
	2150 1700 1600 1700
Text GLabel 1600 1550 0    50   Output ~ 0
MOTOR_1_CURRENT
Text GLabel 1600 1700 0    50   Output ~ 0
~MOTOR_1_OC
Text Notes 825  1975 0    50   ~ 0
FAULT latches low when\ncurrent > 15A rating
Wire Wire Line
	1700 1600 1700 1550
Wire Wire Line
	1700 1550 1600 1550
$Comp
L appli_power:GND #PWR?
U 1 1 5AB9A0AB
P 4475 1900
F 0 "#PWR?" H 4475 1650 50  0001 C CNN
F 1 "GND" H 4480 1727 50  0000 C CNN
F 2 "" H 4475 1900 60  0000 C CNN
F 3 "" H 4475 1900 60  0000 C CNN
	1    4475 1900
	1    0    0    -1  
$EndComp
$EndSCHEMATC
