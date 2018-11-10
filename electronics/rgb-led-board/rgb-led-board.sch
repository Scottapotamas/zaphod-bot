EESchema Schematic File Version 4
LIBS:power
LIBS:device
LIBS:74xx
LIBS:audio
LIBS:interface
LIBS:rgb-led-board-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "3-Channel LED driver"
Date "2018-11-10"
Rev "1.0"
Comp "Electric UI"
Comment1 "Scott Rapson"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L appli_power:GND #PWR06
U 1 1 5B8CD752
P 3800 4700
F 0 "#PWR06" H 3800 4450 50  0001 C CNN
F 1 "GND" H 3805 4527 50  0000 C CNN
F 2 "" H 3800 4700 60  0000 C CNN
F 3 "" H 3800 4700 60  0000 C CNN
	1    3800 4700
	1    0    0    -1  
$EndComp
Text Notes 3150 1650 0    79   ~ 16
Input Connection
Text Notes 7300 1650 0    79   ~ 16
LED Output Connection
Text Notes 5200 1650 0    79   ~ 16
Drive Electronics
Wire Wire Line
	8050 2650 7650 2650
Text Label 7550 3000 0    50   ~ 0
LED_1
Text Label 7550 3350 0    50   ~ 0
LED_2
Text Label 7550 3700 0    50   ~ 0
LED_3
$Comp
L appli_special:AL1793AFE-13 U1
U 1 1 5B8D236C
P 5750 3300
F 0 "U1" H 5750 3750 60  0000 C CNN
F 1 "AL1793AFE-13" H 5750 2650 60  0000 C CNN
F 2 "Applidyne_QFN:QFN14P50_400X300X65L40X25N" H 5750 2050 60  0001 C CNN
F 3 "https://www.diodes.com/assets/Datasheets/AL1791_1792_1793_1794.pdf" H 5750 2175 60  0001 C CNN
F 4 "DIODES INCORPORATED" H 5750 1935 60  0001 C CNN "manf"
F 5 "AL1793AFE-13" H 5760 1825 60  0001 C CNN "manf#"
F 6 "Mouser" H 5750 1715 60  0001 C CNN "Supplier"
F 7 "621-AL1793AFE-13" H 5750 1605 60  0001 C CNN "Supplier Part No"
F 8 "https://au.mouser.com/ProductDetail/Diodes-Incorporated/AL1793AFE-13" H 5750 1505 60  0001 C CNN "Supplier URL"
F 9 "2.25" H 5750 1385 60  0001 C CNN "Supplier Price"
F 10 "10" H 5760 1275 60  0001 C CNN "Supplier Price Break"
	1    5750 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 3750 6600 3750
Wire Wire Line
	6600 3750 6600 4100
Wire Wire Line
	6400 3650 6600 3650
Wire Wire Line
	6600 3650 6600 3750
Connection ~ 6600 3750
Wire Wire Line
	6400 3550 6600 3550
Wire Wire Line
	6600 3550 6600 3650
Connection ~ 6600 3650
$Comp
L appli_power:GND #PWR011
U 1 1 5B8D2D95
P 6600 4100
F 0 "#PWR011" H 6600 3850 50  0001 C CNN
F 1 "GND" H 6605 3927 50  0000 C CNN
F 2 "" H 6600 4100 60  0000 C CNN
F 3 "" H 6600 4100 60  0000 C CNN
	1    6600 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 3750 4900 3750
Wire Wire Line
	4900 3750 4900 4100
$Comp
L appli_power:GND #PWR07
U 1 1 5B8D31CD
P 4900 4100
F 0 "#PWR07" H 4900 3850 50  0001 C CNN
F 1 "GND" H 4905 3927 50  0000 C CNN
F 2 "" H 4900 4100 60  0000 C CNN
F 3 "" H 4900 4100 60  0000 C CNN
	1    4900 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 3450 7400 3450
Wire Wire Line
	7400 3450 7400 3700
Wire Wire Line
	7400 3700 8050 3700
Wire Wire Line
	6400 3350 8050 3350
Wire Wire Line
	6400 3250 7400 3250
Wire Wire Line
	7400 3000 8050 3000
Wire Wire Line
	6400 3050 6600 3050
Wire Wire Line
	6600 3050 6600 2800
Text Notes 6450 2150 0    50   ~ 0
Iled = 2000 * ( 1.5V / Rset )\n6k gives 500mA\n12k gives 250mA
$Comp
L appli_resistor:12K7_1608M R2
U 1 1 5B8D8138
P 5600 2600
F 0 "R2" V 5412 2450 50  0000 C CNN
F 1 "12K7_1608M" V 5492 2450 35  0000 C CNN
F 2 "Applidyne_Resistor:RESC1608X50N" V 5710 2450 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1911175.pdf" V 5735 2450 20  0001 C CNN
F 4 "PANASONIC" V 5785 2450 20  0001 C CNN "manf"
F 5 "ERJ3EKF1272V" V 5810 2450 20  0001 C CNN "manf#"
F 6 "Element14" V 5835 2450 20  0001 C CNN "Supplier"
F 7 "2059415" V 5860 2450 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/panasonic-electronic-components/erj3ekf1272v/res-thick-film-12k7-1-0-1w-0603/dp/2059415" V 5885 2450 20  0001 C CNN "Supplier URL"
F 9 "0.014" V 5910 2450 20  0001 C CNN "Supplier Price"
F 10 "50" V 5935 2450 20  0001 C CNN "Supplier Price Break"
	1    5600 2600
	0    -1   1    0   
$EndComp
Wire Wire Line
	3650 2500 3550 2500
Wire Wire Line
	3550 2200 3650 2200
Wire Wire Line
	5100 3150 4900 3150
Wire Wire Line
	7400 3250 7400 3000
Wire Wire Line
	4900 2600 4900 3150
$Comp
L appli_power:+POWER #PWR04
U 1 1 5B8E9DA0
P 3725 2200
F 0 "#PWR04" H 3725 2150 20  0001 C CNN
F 1 "+POWER" V 3728 2308 30  0000 L CNN
F 2 "" H 3725 2200 60  0000 C CNN
F 3 "" H 3725 2200 60  0000 C CNN
	1    3725 2200
	0    1    1    0   
$EndComp
$Comp
L appli_power:+POWER #PWR012
U 1 1 5B8E9E03
P 7650 2650
F 0 "#PWR012" H 7650 2600 20  0001 C CNN
F 1 "+POWER" V 7654 2758 30  0000 L CNN
F 2 "" H 7650 2650 60  0000 C CNN
F 3 "" H 7650 2650 60  0000 C CNN
	1    7650 2650
	0    -1   -1   0   
$EndComp
$Comp
L appli_power:+POWER #PWR010
U 1 1 5B8E9E22
P 6600 2800
F 0 "#PWR010" H 6600 2750 20  0001 C CNN
F 1 "+POWER" H 6603 2938 30  0000 C CNN
F 2 "" H 6600 2800 60  0000 C CNN
F 3 "" H 6600 2800 60  0000 C CNN
	1    6600 2800
	1    0    0    -1  
$EndComp
Text Label 3650 3450 0    50   ~ 0
PWM3
Text Label 3650 3550 0    50   ~ 0
PWM2
Text Label 3650 3650 0    50   ~ 0
PWM1
Text Label 3650 3250 0    50   ~ 0
POWER_GOOD
Text Label 3650 3350 0    50   ~ 0
~FAULT
Text Label 3650 3150 0    50   ~ 0
ENABLE
Wire Wire Line
	4900 2100 4900 2000
Wire Wire Line
	5900 2600 6000 2600
Wire Wire Line
	5900 2000 6000 2000
$Comp
L appli_resistor:12K7_1608M R1
U 1 1 5B8ED7A8
P 5600 2000
F 0 "R1" V 5788 1850 50  0000 C CNN
F 1 "12K7_1608M" V 5708 1850 35  0000 C CNN
F 2 "Applidyne_Resistor:RESC1608X50N" V 5710 1850 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1911175.pdf" V 5735 1850 20  0001 C CNN
F 4 "PANASONIC" V 5785 1850 20  0001 C CNN "manf"
F 5 "ERJ3EKF1272V" V 5810 1850 20  0001 C CNN "manf#"
F 6 "Element14" V 5835 1850 20  0001 C CNN "Supplier"
F 7 "2059415" V 5860 1850 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/panasonic-electronic-components/erj3ekf1272v/res-thick-film-12k7-1-0-1w-0603/dp/2059415" V 5885 1850 20  0001 C CNN "Supplier URL"
F 9 "0.014" V 5910 1850 20  0001 C CNN "Supplier Price"
F 10 "50" V 5935 1850 20  0001 C CNN "Supplier Price Break"
	1    5600 2000
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4900 2000 5600 2000
Text Notes 2200 3200 0    50   ~ 0
High to enable, internal PD
Text Notes 2200 3350 0    50   ~ 0
Open Drain, ASSERT LOW
Text Notes 2500 3625 0    50   ~ 0
PWM inputs have\ninternal pulldown
Wire Bus Line
	3200 3450 3200 3650
Wire Bus Line
	3200 3350 3200 3250
$Comp
L appli_power:GND #PWR09
U 1 1 5B8F30E6
P 6000 2600
F 0 "#PWR09" H 6000 2350 50  0001 C CNN
F 1 "GND" V 6005 2472 50  0000 R CNN
F 2 "" H 6000 2600 60  0000 C CNN
F 3 "" H 6000 2600 60  0000 C CNN
	1    6000 2600
	0    -1   -1   0   
$EndComp
$Comp
L appli_power:GND #PWR08
U 1 1 5B8F38DE
P 6000 2000
F 0 "#PWR08" H 6000 1750 50  0001 C CNN
F 1 "GND" V 6005 1872 50  0000 R CNN
F 2 "" H 6000 2000 60  0000 C CNN
F 3 "" H 6000 2000 60  0000 C CNN
	1    6000 2000
	0    -1   -1   0   
$EndComp
Text Notes 6425 2400 0    50   ~ 0
Bridge for full output current.
$Comp
L appli_power:+POWER #PWR05
U 1 1 5B8F50FE
P 3800 4250
F 0 "#PWR05" H 3800 4200 20  0001 C CNN
F 1 "+POWER" H 3803 4388 30  0000 C CNN
F 2 "" H 3800 4250 60  0000 C CNN
F 3 "" H 3800 4250 60  0000 C CNN
	1    3800 4250
	1    0    0    -1  
$EndComp
$Comp
L appli_capacitor:22u_X7R_3225M C1
U 1 1 5B8F550E
P 3800 4350
F 0 "C1" H 3878 4285 50  0000 L CNN
F 1 "22u_X7R_3225M" H 3878 4205 35  0000 L CNN
F 2 "Applidyne_Capacitor:CAPC3225X170N" V 3915 4250 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2111999.pdf?_ga=1.157172477.774814437.1442284889" V 3940 4250 20  0001 C CNN
F 4 "muRata" V 3990 4250 20  0001 C CNN "manf"
F 5 "C1210C226K3RACTU" V 4015 4250 20  0001 C CNN "manf#"
F 6 "Element14" V 4040 4250 20  0001 C CNN "Supplier"
F 7 "2473550" V 4065 4250 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c1210c226k3ractu/cap-mlcc-x7r-22uf-25v-1210/dp/2473550" V 4090 4250 20  0001 C CNN "Supplier URL"
F 9 "1.02" V 4115 4250 20  0001 C CNN "Supplier Price"
F 10 "50" V 4140 4250 20  0001 C CNN "Supplier Price Break"
	1    3800 4350
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR03
U 1 1 5B8F560B
P 3650 2500
F 0 "#PWR03" H 3650 2250 50  0001 C CNN
F 1 "GND" H 3655 2327 50  0000 C CNN
F 2 "" H 3650 2500 60  0000 C CNN
F 3 "" H 3650 2500 60  0000 C CNN
	1    3650 2500
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3800 4700 3800 4550
Wire Wire Line
	3800 4350 3800 4250
$Comp
L appli_power:PWR_FLAG #FLG0101
U 1 1 5B8FDE10
P 3650 2125
F 0 "#FLG0101" H 3650 2175 30  0001 C CNN
F 1 "PWR_FLAG" H 3650 2258 30  0000 C CNN
F 2 "" H 3650 2125 60  0000 C CNN
F 3 "" H 3650 2125 60  0000 C CNN
	1    3650 2125
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 2125 3650 2200
Connection ~ 3650 2200
Wire Wire Line
	3650 2200 3725 2200
$Comp
L appli_link:SOLDER-LINK-2 SL1
U 1 1 5BE6CF23
P 4900 2250
F 0 "SL1" V 4862 2192 40  0000 R CNN
F 1 "SOLDER-LINK-2" V 4938 2192 40  0000 R CNN
F 2 "Applidyne_Link:SJ1608-2N" V 5015 2150 20  0001 C CNN
F 3 "DatasheetURL" V 5040 2150 20  0001 C CNN
F 4 "Manufacturer" V 5090 2150 20  0001 C CNN "manf"
F 5 "ManufacturerPartNo" V 5115 2150 20  0001 C CNN "manf#"
F 6 "Supplier" V 5140 2150 20  0001 C CNN "Supplier"
F 7 "SupplierPartNo" V 5165 2150 20  0001 C CNN "Supplier Part No"
F 8 "SupplierURL" V 5190 2150 20  0001 C CNN "Supplier URL"
F 9 "0" V 5215 2150 20  0001 C CNN "Supplier Price"
F 10 "1" V 5240 2150 20  0001 C CNN "Supplier Price Break"
	1    4900 2250
	0    -1   1    0   
$EndComp
Wire Wire Line
	4900 2400 4900 2600
Connection ~ 4900 2600
Wire Wire Line
	4900 2600 5600 2600
$Comp
L appli_connector:PAD_SOLDER_2mm P4
U 1 1 5BE64212
P 8250 2650
F 0 "P4" H 8327 2691 50  0000 L CNN
F 1 "PAD_SOLDER_2mm" H 8327 2600 50  0000 L CNN
F 2 "Applidyne_Connector:PAD_SOLDER_2mm" H 8250 2435 50  0001 C CNN
F 3 "" H 8250 2350 50  0001 C CNN
F 4 "DIY" H 8250 2300 60  0001 C CNN "manf"
F 5 "No part" H 8250 2250 60  0001 C CNN "Supplier"
F 6 "0.0" H 8250 2150 60  0001 C CNN "Supplier Price"
F 7 "1" H 8250 2050 60  0001 C CNN "Supplier Price Break"
	1    8250 2650
	1    0    0    -1  
$EndComp
$Comp
L appli_connector:PAD_SOLDER_2mm P5
U 1 1 5BE64867
P 8250 3000
F 0 "P5" H 8327 3041 50  0000 L CNN
F 1 "PAD_SOLDER_2mm" H 8327 2950 50  0000 L CNN
F 2 "Applidyne_Connector:PAD_SOLDER_2mm" H 8250 2785 50  0001 C CNN
F 3 "" H 8250 2700 50  0001 C CNN
F 4 "DIY" H 8250 2650 60  0001 C CNN "manf"
F 5 "No part" H 8250 2600 60  0001 C CNN "Supplier"
F 6 "0.0" H 8250 2500 60  0001 C CNN "Supplier Price"
F 7 "1" H 8250 2400 60  0001 C CNN "Supplier Price Break"
	1    8250 3000
	1    0    0    -1  
$EndComp
$Comp
L appli_connector:PAD_SOLDER_2mm P6
U 1 1 5BE648E5
P 8250 3350
F 0 "P6" H 8327 3391 50  0000 L CNN
F 1 "PAD_SOLDER_2mm" H 8327 3300 50  0000 L CNN
F 2 "Applidyne_Connector:PAD_SOLDER_2mm" H 8250 3135 50  0001 C CNN
F 3 "" H 8250 3050 50  0001 C CNN
F 4 "DIY" H 8250 3000 60  0001 C CNN "manf"
F 5 "No part" H 8250 2950 60  0001 C CNN "Supplier"
F 6 "0.0" H 8250 2850 60  0001 C CNN "Supplier Price"
F 7 "1" H 8250 2750 60  0001 C CNN "Supplier Price Break"
	1    8250 3350
	1    0    0    -1  
$EndComp
$Comp
L appli_connector:PAD_SOLDER_2mm P7
U 1 1 5BE64992
P 8250 3700
F 0 "P7" H 8327 3741 50  0000 L CNN
F 1 "PAD_SOLDER_2mm" H 8327 3650 50  0000 L CNN
F 2 "Applidyne_Connector:PAD_SOLDER_2mm" H 8250 3485 50  0001 C CNN
F 3 "" H 8250 3400 50  0001 C CNN
F 4 "DIY" H 8250 3350 60  0001 C CNN "manf"
F 5 "No part" H 8250 3300 60  0001 C CNN "Supplier"
F 6 "0.0" H 8250 3200 60  0001 C CNN "Supplier Price"
F 7 "1" H 8250 3100 60  0001 C CNN "Supplier Price Break"
	1    8250 3700
	1    0    0    -1  
$EndComp
$Comp
L appli_connector:PAD_SOLDER_2mm P2
U 1 1 5BE651DF
P 3350 2500
F 0 "P2" H 3500 2550 50  0000 C CNN
F 1 "PAD_SOLDER_2mm" H 3800 2450 50  0000 C CNN
F 2 "Applidyne_Connector:PAD_SOLDER_2mm" H 3350 2285 50  0001 C CNN
F 3 "" H 3350 2200 50  0001 C CNN
F 4 "DIY" H 3350 2150 60  0001 C CNN "manf"
F 5 "No part" H 3350 2100 60  0001 C CNN "Supplier"
F 6 "0.0" H 3350 2000 60  0001 C CNN "Supplier Price"
F 7 "1" H 3350 1900 60  0001 C CNN "Supplier Price Break"
	1    3350 2500
	-1   0    0    -1  
$EndComp
$Comp
L appli_connector:PAD_SOLDER_2mm P1
U 1 1 5BE656B8
P 3350 2200
F 0 "P1" H 3500 2250 50  0000 C CNN
F 1 "PAD_SOLDER_2mm" H 3800 2150 50  0000 C CNN
F 2 "Applidyne_Connector:PAD_SOLDER_2mm" H 3350 1985 50  0001 C CNN
F 3 "" H 3350 1900 50  0001 C CNN
F 4 "DIY" H 3350 1850 60  0001 C CNN "manf"
F 5 "No part" H 3350 1800 60  0001 C CNN "Supplier"
F 6 "0.0" H 3350 1700 60  0001 C CNN "Supplier Price"
F 7 "1" H 3350 1600 60  0001 C CNN "Supplier Price Break"
	1    3350 2200
	-1   0    0    -1  
$EndComp
$Comp
L appli_connector_wurth:CONN_01X08 P3
U 1 1 5BE6660F
P 3350 3300
F 0 "P3" H 3269 3865 50  0000 C CNN
F 1 "CONN_01X08" H 3269 3774 50  0000 C CNN
F 2 "Applidyne_Connector_Wurth:61300811121" V 3525 3300 50  0001 C CNN
F 3 "http://www.farnell.com/cad/2124416.pdf" V 3600 3300 50  0001 C CNN
F 4 "WURTH" H 3350 2850 60  0001 C CNN "manf"
F 5 "61300811121" H 3350 2750 60  0001 C CNN "manf#"
F 6 "Element 14" H 3350 2650 60  0001 C CNN "Supplier"
F 7 "2356160" H 3350 2575 60  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/wurth-elektronik/61300811121/header-2-54mm-pin-tht-vertical/dp/2356160" H 3350 2475 60  0001 C CNN "Supplier URL"
F 9 "0.358" H 3350 2375 60  0001 C CNN "Supplier Price"
F 10 "1" H 3350 2275 60  0001 C CNN "Supplier Price Break"
	1    3350 3300
	-1   0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR0101
U 1 1 5BE66CC2
P 4000 3050
F 0 "#PWR0101" H 4000 2800 50  0001 C CNN
F 1 "GND" H 4005 2877 50  0000 C CNN
F 2 "" H 4000 3050 60  0000 C CNN
F 3 "" H 4000 3050 60  0000 C CNN
	1    4000 3050
	0    -1   -1   0   
$EndComp
$Comp
L appli_power:+POWER #PWR0102
U 1 1 5BE66F57
P 3800 2950
F 0 "#PWR0102" H 3800 2900 20  0001 C CNN
F 1 "+POWER" V 3803 3058 30  0000 L CNN
F 2 "" H 3800 2950 60  0000 C CNN
F 3 "" H 3800 2950 60  0000 C CNN
	1    3800 2950
	0    1    1    0   
$EndComp
Wire Wire Line
	4000 3050 3550 3050
Wire Wire Line
	3550 2950 3800 2950
Wire Wire Line
	5100 3050 4550 3050
Wire Wire Line
	5100 3250 4550 3250
Wire Wire Line
	5100 3350 4550 3350
Wire Wire Line
	5100 3450 4550 3450
Wire Wire Line
	5100 3550 4550 3550
Wire Wire Line
	5100 3650 4550 3650
Wire Wire Line
	3550 3650 3650 3650
Wire Wire Line
	3650 3550 3550 3550
Wire Wire Line
	3550 3450 3650 3450
Wire Wire Line
	3650 3250 3550 3250
Wire Wire Line
	3650 3350 3550 3350
Wire Wire Line
	3650 3150 3550 3150
Text Label 4550 3450 0    50   ~ 0
PWM1
Text Label 4550 3550 0    50   ~ 0
PWM2
Text Label 4550 3650 0    50   ~ 0
PWM3
Text Label 4550 3250 0    50   ~ 0
~FAULT
Text Label 4550 3350 0    50   ~ 0
POWER_GOOD
Text Label 4550 3050 0    50   ~ 0
ENABLE
$EndSCHEMATC
