EESchema Schematic File Version 4
LIBS:zaphod-controller-cache
EELAYER 26 0
EELAYER END
$Descr A3 16535 11693
encoding utf-8
Sheet 5 5
Title "Expansion IO"
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 1550 875  0    79   ~ 16
Internal Expansion IO
Text Notes 2625 7325 0    79   ~ 16
External Isolated Expansion IO
Text Notes 11825 3450 0    79   ~ 16
Fan Control Circuit
Text Notes 11725 1225 0    79   ~ 16
Temperature Sensors
Text Notes 11275 1525 0    50   ~ 0
Expansion temp sensor 0
Text Notes 9375 1525 0    50   ~ 0
On-board temp sensor
Text Notes 7850 7975 0    79   ~ 16
Add-in Card
$Comp
L appli_resistor:NTC_10K_1608M TH?
U 1 1 5ABC263A
P 9850 1900
F 0 "TH?" H 9948 1785 50  0000 L CNN
F 1 "NTC_10K_1608M" H 9948 1705 35  0000 L CNN
F 2 "RESC1608X50*" V 10000 1760 20  0001 C CNN
F 3 "http://au.rs-online.com/webdocs/1423/0900766b81423c04.pdf" V 10025 1760 20  0001 C CNN
F 4 "MURATA" V 10075 1760 20  0001 C CNN "manf"
F 5 "NCP18XH103J03RB" V 10100 1770 20  0001 C CNN "manf#"
F 6 "RS Online" V 10125 1760 20  0001 C CNN "Supplier"
F 7 "619-7740" V 10150 1760 20  0001 C CNN "Supplier Part No"
F 8 "http://au.rs-online.com/web/p/thermistors/6197740/" V 10175 1760 20  0001 C CNN "Supplier URL"
F 9 "0.327" V 10200 1760 20  0001 C CNN "Supplier Price"
F 10 "25" V 10225 1760 20  0001 C CNN "Supplier Price Break"
	1    9850 1900
	1    0    0    -1  
$EndComp
$Comp
L appli_connector_wurth:CONN_01X02 P?
U 1 1 5ABC2851
P 12150 2050
F 0 "P?" H 12228 2091 50  0000 L CNN
F 1 "CONN_01X02" H 12228 2000 50  0000 L CNN
F 2 "Applidyne_Connector_Wurth:61300211121" V 12325 2050 50  0001 C CNN
F 3 "http://www.farnell.com/cad/2124416.pdf" V 13100 2100 50  0001 C CNN
F 4 "WURTH" V 12425 2075 60  0001 C CNN "manf"
F 5 "61300211121" V 12525 2075 60  0001 C CNN "manf#"
F 6 "Element 14" V 12625 2075 60  0001 C CNN "Supplier"
F 7 "2356153" V 12700 2075 60  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/wurth-elektronik/61300211121/header-2-54mm-pin-tht-vertical/dp/2356153" V 12800 2075 60  0001 C CNN "Supplier URL"
F 9 "0.17" V 12900 2075 60  0001 C CNN "Supplier Price"
F 10 "1" V 13000 2075 60  0001 C CNN "Supplier Price Break"
	1    12150 2050
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+3.3V #PWR0185
U 1 1 5ABC2901
P 9850 1800
F 0 "#PWR0185" H 9850 1760 30  0001 C CNN
F 1 "+3.3V" H 9859 1938 30  0000 C CNN
F 2 "" H 9850 1800 60  0000 C CNN
F 3 "" H 9850 1800 60  0000 C CNN
	1    9850 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	9450 2250 9600 2250
Wire Wire Line
	9850 2250 9850 2300
Wire Wire Line
	9850 2200 9850 2250
Connection ~ 9850 2250
Wire Wire Line
	9850 1900 9850 1800
Wire Wire Line
	9850 2600 9850 2650
$Comp
L appli_power:GND #PWR0186
U 1 1 5ABC3416
P 9850 2700
F 0 "#PWR0186" H 9850 2450 50  0001 C CNN
F 1 "GND" H 9855 2527 50  0000 C CNN
F 2 "" H 9850 2700 60  0000 C CNN
F 3 "" H 9850 2700 60  0000 C CNN
	1    9850 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	9600 2350 9600 2250
Connection ~ 9600 2250
Wire Wire Line
	9600 2250 9850 2250
Wire Wire Line
	9600 2550 9600 2650
Wire Wire Line
	9600 2650 9850 2650
Connection ~ 9850 2650
Wire Wire Line
	9850 2650 9850 2700
Wire Wire Line
	9150 2250 9000 2250
$Comp
L appli_power:+3.3V #PWR0187
U 1 1 5ABC37B4
P 11850 1800
F 0 "#PWR0187" H 11850 1760 30  0001 C CNN
F 1 "+3.3V" H 11859 1938 30  0000 C CNN
F 2 "" H 11850 1800 60  0000 C CNN
F 3 "" H 11850 1800 60  0000 C CNN
	1    11850 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	11450 2250 11600 2250
Wire Wire Line
	11850 2250 11850 2300
Connection ~ 11850 2250
Wire Wire Line
	11850 2600 11850 2650
$Comp
L appli_power:GND #PWR0188
U 1 1 5ABC37EA
P 11850 2700
F 0 "#PWR0188" H 11850 2450 50  0001 C CNN
F 1 "GND" H 11855 2527 50  0000 C CNN
F 2 "" H 11850 2700 60  0000 C CNN
F 3 "" H 11850 2700 60  0000 C CNN
	1    11850 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	11600 2350 11600 2250
Connection ~ 11600 2250
Wire Wire Line
	11600 2250 11850 2250
Wire Wire Line
	11600 2550 11600 2650
Wire Wire Line
	11600 2650 11850 2650
Connection ~ 11850 2650
Wire Wire Line
	11850 2650 11850 2700
Wire Wire Line
	11150 2250 11000 2250
Wire Wire Line
	11850 2100 11950 2100
Wire Wire Line
	11850 2100 11850 2250
Wire Wire Line
	11950 2000 11850 2000
Wire Wire Line
	11850 1800 11850 2000
Text GLabel 9000 2250 0    50   Output ~ 0
TEMP_PCB
Text GLabel 11000 2250 0    50   Output ~ 0
TEMP_EXT_0
Text GLabel 10975 5250 0    50   Input ~ 0
FAN_PWM
Text GLabel 10975 4350 0    50   Output ~ 0
FAN_HALL
$Comp
L appli_power:+12V #PWR0189
U 1 1 5AD0DD6F
P 13500 3800
F 0 "#PWR0189" H 13500 3650 50  0001 C CNN
F 1 "+12V" H 13515 3973 50  0000 C CNN
F 2 "" H 13500 3800 60  0000 C CNN
F 3 "" H 13500 3800 60  0000 C CNN
	1    13500 3800
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR0190
U 1 1 5AD0DDC4
P 12600 5550
F 0 "#PWR0190" H 12600 5300 50  0001 C CNN
F 1 "GND" H 12605 5377 50  0000 C CNN
F 2 "" H 12600 5550 60  0000 C CNN
F 3 "" H 12600 5550 60  0000 C CNN
	1    12600 5550
	1    0    0    -1  
$EndComp
$Comp
L appli_transistor:BC847B Q?
U 1 1 5AD0E4D5
P 12500 5250
F 0 "Q?" H 12695 5288 40  0000 L CNN
F 1 "BC847B" H 12695 5212 40  0000 L CNN
F 2 "SOT95P240X110-3L16*" H 12425 5350 29  0001 R CNN
F 3 "http://www.farnell.com/datasheets/1685010.pdf" V 12725 5250 20  0001 C CNN
F 4 "Multicomp" V 12775 5250 20  0001 C CNN "manf"
F 5 "BC847B" V 12800 5250 20  0001 C CNN "manf#"
F 6 "Element14" V 12825 5250 20  0001 C CNN "Supplier"
F 7 "1574343" V 12850 5250 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/bc847b/transistor-npn-45v-100ma-sot-23/dp/1574343" V 12875 5250 20  0001 C CNN "Supplier URL"
F 9 "0.26" V 12900 5250 20  0001 C CNN "Supplier Price"
F 10 "10" V 12925 5250 20  0001 C CNN "Supplier Price Break"
	1    12500 5250
	1    0    0    -1  
$EndComp
$Comp
L appli_resistor:1K00_1608M R?
U 1 1 5AD0E62C
P 11400 5250
F 0 "R?" V 11212 5100 50  0000 C CNN
F 1 "1K00_1608M" V 11292 5100 35  0000 C CNN
F 2 "RESC1608*" V 11510 5100 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 11535 5100 20  0001 C CNN
F 4 "MULTICOMP" V 11585 5100 20  0001 C CNN "manf"
F 5 "MCHP03W8F1001T5E" V 11610 5100 20  0001 C CNN "manf#"
F 6 "Element14" V 11635 5100 20  0001 C CNN "Supplier"
F 7 "1576283" V 11660 5100 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mchp03w8f1001t5e/resistor-thick-film-1k-0-125w/dp/1576283" V 11685 5100 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 11710 5100 20  0001 C CNN "Supplier Price"
F 10 "50+" V 11735 5100 20  0001 C CNN "Supplier Price Break"
	1    11400 5250
	0    1    1    0   
$EndComp
Wire Wire Line
	10975 5250 11100 5250
Wire Wire Line
	12600 5450 12600 5550
$Comp
L appli_power:GND #PWR0191
U 1 1 5AD0F172
P 14050 4575
F 0 "#PWR0191" H 14050 4325 50  0001 C CNN
F 1 "GND" H 14055 4402 50  0000 C CNN
F 2 "" H 14050 4575 60  0000 C CNN
F 3 "" H 14050 4575 60  0000 C CNN
	1    14050 4575
	1    0    0    -1  
$EndComp
$Comp
L appli_connector_wurth:CONN_01X04 P?
U 1 1 5AD0F38F
P 14350 4300
F 0 "P?" H 14428 4341 50  0000 L CNN
F 1 "CONN_01X04" H 14428 4250 50  0000 L CNN
F 2 "Applidyne_Connector_Wurth:61300411121" V 14525 4300 50  0001 C CNN
F 3 "http://www.farnell.com/cad/2124416.pdf" H 14350 3975 50  0001 C CNN
F 4 "WURTH" H 14350 3850 60  0001 C CNN "manf"
F 5 "61300411121" H 14350 3750 60  0001 C CNN "manf#"
F 6 "Element 14" H 14350 3650 60  0001 C CNN "Supplier"
F 7 "2356155" H 14350 3575 60  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/wurth-elektronik/61300411121/header-2-54mm-pin-tht-vertical/dp/2356155" H 14350 3475 60  0001 C CNN "Supplier URL"
F 9 "0.125" H 14350 3375 60  0001 C CNN "Supplier Price"
F 10 "1" H 14350 3275 60  0001 C CNN "Supplier Price Break"
	1    14350 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	14150 4150 14050 4150
Wire Wire Line
	14050 4150 14050 4575
$Comp
L appli_device:BAV99 D?
U 1 1 5AD10A17
P 11600 4350
F 0 "D?" H 11708 4396 50  0000 L CNN
F 1 "BAV99" H 11708 4305 50  0000 L CNN
F 2 "SOT23" H 11800 4200 20  0001 C CNN
F 3 "http://datasheet.octopart.com/BAV99-/T3-NXP-datasheet-5318834.pdf" H 11800 4175 20  0001 C CNN
F 4 "NXP" H 11800 4125 20  0001 C CNN "manf"
F 5 "RS Online" H 11800 4075 20  0001 C CNN "Supplier"
F 6 " 544-4534" H 11800 4050 20  0001 C CNN "Supplier Part No"
F 7 "http://au.rs-online.com/web/p/rectifier-schottky-diodes/5444534/" H 11800 4025 20  0001 C CNN "Supplier URL"
F 8 "0.045" H 11800 4000 20  0001 C CNN "Supplier Price"
F 9 "1" H 11800 3975 20  0001 C CNN "Supplier Price Break"
F 10 "BAV99" H 11800 4100 20  0001 C CNN "manf#"
	1    11600 4350
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR0192
U 1 1 5AD10F65
P 11600 4850
F 0 "#PWR0192" H 11600 4600 50  0001 C CNN
F 1 "GND" H 11605 4677 50  0000 C CNN
F 2 "" H 11600 4850 60  0000 C CNN
F 3 "" H 11600 4850 60  0000 C CNN
	1    11600 4850
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+3.3V #PWR0193
U 1 1 5AD11030
P 11600 4050
F 0 "#PWR0193" H 11600 4010 30  0001 C CNN
F 1 "+3.3V" H 11609 4188 30  0000 C CNN
F 2 "" H 11600 4050 60  0000 C CNN
F 3 "" H 11600 4050 60  0000 C CNN
	1    11600 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	11600 4050 11600 4100
Wire Wire Line
	11600 4600 11600 4800
$Comp
L appli_resistor:1K00_1608M R?
U 1 1 5AD11EAB
P 11400 4350
F 0 "R?" V 11212 4200 50  0000 C CNN
F 1 "1K00_1608M" V 11292 4200 35  0000 C CNN
F 2 "RESC1608*" V 11510 4200 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 11535 4200 20  0001 C CNN
F 4 "MULTICOMP" V 11585 4200 20  0001 C CNN "manf"
F 5 "MCHP03W8F1001T5E" V 11610 4200 20  0001 C CNN "manf#"
F 6 "Element14" V 11635 4200 20  0001 C CNN "Supplier"
F 7 "1576283" V 11660 4200 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mchp03w8f1001t5e/resistor-thick-film-1k-0-125w/dp/1576283" V 11685 4200 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 11710 4200 20  0001 C CNN "Supplier Price"
F 10 "50+" V 11735 4200 20  0001 C CNN "Supplier Price Break"
	1    11400 4350
	0    1    1    0   
$EndComp
Wire Wire Line
	10975 4350 11100 4350
Wire Wire Line
	11400 4350 11600 4350
$Comp
L appli_power:+12V #PWR0194
U 1 1 5AD13E6B
P 13700 5200
F 0 "#PWR0194" H 13700 5050 50  0001 C CNN
F 1 "+12V" H 13715 5373 50  0000 C CNN
F 2 "" H 13700 5200 60  0000 C CNN
F 3 "" H 13700 5200 60  0000 C CNN
	1    13700 5200
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR0195
U 1 1 5AD13E92
P 13925 5650
F 0 "#PWR0195" H 13925 5400 50  0001 C CNN
F 1 "GND" H 13930 5477 50  0000 C CNN
F 2 "" H 13925 5650 60  0000 C CNN
F 3 "" H 13925 5650 60  0000 C CNN
	1    13925 5650
	1    0    0    -1  
$EndComp
Wire Wire Line
	13700 5300 13700 5250
Wire Wire Line
	13700 5500 13700 5550
Wire Wire Line
	13700 5550 13925 5550
Wire Wire Line
	13925 5550 13925 5500
Wire Wire Line
	13925 5300 13925 5250
Wire Wire Line
	13925 5250 13700 5250
Connection ~ 13700 5250
Wire Wire Line
	13700 5250 13700 5200
Wire Wire Line
	13925 5650 13925 5550
Connection ~ 13925 5550
Text Notes 14125 5200 0    50   ~ 0
Bypass caps near \nfan connector
$Comp
L appli_resistor:4K7_1608M R?
U 1 1 5AD1ED64
P 13350 4250
F 0 "R?" H 13277 4065 50  0000 R CNN
F 1 "4K7_1608M" H 13277 4145 35  0000 R CNN
F 2 "RESC1608*" V 13460 4100 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1788326.pdf" V 13485 4100 20  0001 C CNN
F 4 "MULTICOMP" V 13535 4100 20  0001 C CNN "manf"
F 5 "MC0063W060354K7" V 13560 4100 20  0001 C CNN "manf#"
F 6 "Element14" V 13585 4100 20  0001 C CNN "Supplier"
F 7 "9332154" V 13610 4100 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mc0063w060354k7/res-thick-film-4k7-5-0-063w-0603/dp/9332154" V 13635 4100 20  0001 C CNN "Supplier URL"
F 9 "0.024" V 13660 4100 20  0001 C CNN "Supplier Price"
F 10 "1" V 13685 4100 20  0001 C CNN "Supplier Price Break"
	1    13350 4250
	1    0    0    1   
$EndComp
Wire Wire Line
	13350 4250 13350 4350
Wire Wire Line
	13350 4350 14150 4350
Wire Wire Line
	12450 4350 13350 4350
Connection ~ 13350 4350
Wire Wire Line
	12150 4350 12050 4350
Connection ~ 11600 4350
Wire Wire Line
	13500 3800 13500 3900
Wire Wire Line
	13350 3950 13350 3900
Wire Wire Line
	13350 3900 13500 3900
Connection ~ 13500 3900
Wire Wire Line
	13500 3900 13500 4250
Wire Wire Line
	12050 4350 12050 4425
Connection ~ 12050 4350
Wire Wire Line
	12050 4350 11600 4350
$Comp
L appli_resistor:10K0_1608M R?
U 1 1 5AD365D7
P 12050 4425
F 0 "R?" H 12123 4310 50  0000 L CNN
F 1 "10K0_1608M" H 12123 4230 35  0000 L CNN
F 2 "RESC1608*" V 12160 4275 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 12185 4275 20  0001 C CNN
F 4 "TE CONNECTIVITY" V 12235 4275 20  0001 C CNN "manf"
F 5 "CRGH0603F10K" V 12260 4275 20  0001 C CNN "manf#"
F 6 "Element14" V 12285 4275 20  0001 C CNN "Supplier"
F 7 "2332016" V 12310 4275 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/te-connectivity/crgh0603f10k/resistor-power-10k-0-2w-1-0603/dp/2332016" V 12335 4275 20  0001 C CNN "Supplier URL"
F 9 "0.02" V 12360 4275 20  0001 C CNN "Supplier Price"
F 10 "10" V 12385 4275 20  0001 C CNN "Supplier Price Break"
	1    12050 4425
	1    0    0    -1  
$EndComp
Wire Wire Line
	12050 4725 12050 4800
Wire Wire Line
	12050 4800 11600 4800
Connection ~ 11600 4800
Wire Wire Line
	11600 4800 11600 4850
Wire Wire Line
	12600 4450 12850 4450
Wire Wire Line
	12600 4450 12600 5050
Wire Wire Line
	12300 5250 11400 5250
$Comp
L appli_resistor:33K2_1608M R?
U 1 1 5AD500A2
P 12450 4350
F 0 "R?" V 12262 4200 50  0000 C CNN
F 1 "33K2_1608M" V 12342 4200 35  0000 C CNN
F 2 "RESC1608*" V 12560 4200 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2051174.pdf?_ga=1.145166194.774814437.1442284889" V 12585 4200 20  0001 C CNN
F 4 "PANASONIC " V 12635 4200 20  0001 C CNN "manf"
F 5 "ERJPA3F3322V" V 12660 4200 20  0001 C CNN "manf#"
F 6 "Element14" V 12685 4200 20  0001 C CNN "Supplier"
F 7 "2561973" V 12710 4200 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/panasonic-electronic-components/erjpa3f3322v/res-aec-q200-thick-film-33k2-0603/dp/2561973" V 12735 4200 20  0001 C CNN "Supplier URL"
F 9 "0.2" V 12760 4200 20  0001 C CNN "Supplier Price"
F 10 "1" V 12785 4200 20  0001 C CNN "Supplier Price Break"
	1    12450 4350
	0    1    1    0   
$EndComp
Text Notes 10250 4025 0    50   ~ 0
Tachometer uses 12V pullup.\n2.3:1 divider on tach.\nBAV99 prevents input damage.\n
Text Notes 10250 4950 0    50   ~ 0
Drive PWM at 22-25kHz.
Wire Wire Line
	14150 4250 13950 4250
Wire Wire Line
	13650 4250 13500 4250
Text Notes 10375 6075 0    50   ~ 0
https://electronics.stackexchange.com/questions/153846/arduino-how-to-read-and-control-the-speed-of-a-12v-four-wire-fan
Text Notes 14700 4150 0    50   Italic 0
TODO CONNECTOR
$Comp
L appli_connector_wurth:CONN_01X02 P?
U 1 1 5AC12FC5
P 14350 2050
F 0 "P?" H 14428 2091 50  0000 L CNN
F 1 "CONN_01X02" H 14428 2000 50  0000 L CNN
F 2 "Applidyne_Connector_Wurth:61300211121" V 14525 2050 50  0001 C CNN
F 3 "http://www.farnell.com/cad/2124416.pdf" V 15300 2100 50  0001 C CNN
F 4 "WURTH" V 14625 2075 60  0001 C CNN "manf"
F 5 "61300211121" V 14725 2075 60  0001 C CNN "manf#"
F 6 "Element 14" V 14825 2075 60  0001 C CNN "Supplier"
F 7 "2356153" V 14900 2075 60  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/wurth-elektronik/61300211121/header-2-54mm-pin-tht-vertical/dp/2356153" V 15000 2075 60  0001 C CNN "Supplier URL"
F 9 "0.17" V 15100 2075 60  0001 C CNN "Supplier Price"
F 10 "1" V 15200 2075 60  0001 C CNN "Supplier Price Break"
	1    14350 2050
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+3.3V #PWR0196
U 1 1 5AC12FCC
P 14050 1800
F 0 "#PWR0196" H 14050 1760 30  0001 C CNN
F 1 "+3.3V" H 14059 1938 30  0000 C CNN
F 2 "" H 14050 1800 60  0000 C CNN
F 3 "" H 14050 1800 60  0000 C CNN
	1    14050 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	13650 2250 13800 2250
Wire Wire Line
	14050 2250 14050 2300
Connection ~ 14050 2250
Wire Wire Line
	14050 2600 14050 2650
$Comp
L appli_power:GND #PWR0197
U 1 1 5AC13000
P 14050 2700
F 0 "#PWR0197" H 14050 2450 50  0001 C CNN
F 1 "GND" H 14055 2527 50  0000 C CNN
F 2 "" H 14050 2700 60  0000 C CNN
F 3 "" H 14050 2700 60  0000 C CNN
	1    14050 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	13800 2350 13800 2250
Connection ~ 13800 2250
Wire Wire Line
	13800 2250 14050 2250
Wire Wire Line
	13800 2550 13800 2650
Wire Wire Line
	13800 2650 14050 2650
Connection ~ 14050 2650
Wire Wire Line
	14050 2650 14050 2700
Wire Wire Line
	13350 2250 13200 2250
Wire Wire Line
	14050 2100 14150 2100
Wire Wire Line
	14050 2100 14050 2250
Wire Wire Line
	14150 2000 14050 2000
Wire Wire Line
	14050 1800 14050 2000
Text GLabel 13200 2250 0    50   Output ~ 0
TEMP_EXT_1
Text Notes 13550 1525 0    50   ~ 0
Expansion temp sensor 1
$Comp
L appli_resistor:1K00_1608M R?
U 1 1 5AC153F3
P 9450 2250
F 0 "R?" V 9262 2100 50  0000 C CNN
F 1 "1K00_1608M" V 9342 2100 35  0000 C CNN
F 2 "RESC1608*" V 9560 2100 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 9585 2100 20  0001 C CNN
F 4 "MULTICOMP" V 9635 2100 20  0001 C CNN "manf"
F 5 "MCHP03W8F1001T5E" V 9660 2100 20  0001 C CNN "manf#"
F 6 "Element14" V 9685 2100 20  0001 C CNN "Supplier"
F 7 "1576283" V 9710 2100 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mchp03w8f1001t5e/resistor-thick-film-1k-0-125w/dp/1576283" V 9735 2100 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 9760 2100 20  0001 C CNN "Supplier Price"
F 10 "50+" V 9785 2100 20  0001 C CNN "Supplier Price Break"
	1    9450 2250
	0    1    1    0   
$EndComp
$Comp
L appli_resistor:1K00_1608M R?
U 1 1 5AC15558
P 11450 2250
F 0 "R?" V 11262 2100 50  0000 C CNN
F 1 "1K00_1608M" V 11342 2100 35  0000 C CNN
F 2 "RESC1608*" V 11560 2100 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 11585 2100 20  0001 C CNN
F 4 "MULTICOMP" V 11635 2100 20  0001 C CNN "manf"
F 5 "MCHP03W8F1001T5E" V 11660 2100 20  0001 C CNN "manf#"
F 6 "Element14" V 11685 2100 20  0001 C CNN "Supplier"
F 7 "1576283" V 11710 2100 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mchp03w8f1001t5e/resistor-thick-film-1k-0-125w/dp/1576283" V 11735 2100 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 11760 2100 20  0001 C CNN "Supplier Price"
F 10 "50+" V 11785 2100 20  0001 C CNN "Supplier Price Break"
	1    11450 2250
	0    1    1    0   
$EndComp
$Comp
L appli_resistor:1K00_1608M R?
U 1 1 5AC155B6
P 13650 2250
F 0 "R?" V 13462 2100 50  0000 C CNN
F 1 "1K00_1608M" V 13542 2100 35  0000 C CNN
F 2 "RESC1608*" V 13760 2100 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 13785 2100 20  0001 C CNN
F 4 "MULTICOMP" V 13835 2100 20  0001 C CNN "manf"
F 5 "MCHP03W8F1001T5E" V 13860 2100 20  0001 C CNN "manf#"
F 6 "Element14" V 13885 2100 20  0001 C CNN "Supplier"
F 7 "1576283" V 13910 2100 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mchp03w8f1001t5e/resistor-thick-film-1k-0-125w/dp/1576283" V 13935 2100 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 13960 2100 20  0001 C CNN "Supplier Price"
F 10 "50+" V 13985 2100 20  0001 C CNN "Supplier Price Break"
	1    13650 2250
	0    1    1    0   
$EndComp
$Comp
L appli_resistor:10K0_1608M R?
U 1 1 5AC16D64
P 9850 2300
F 0 "R?" H 9923 2185 50  0000 L CNN
F 1 "10K0_1608M" H 9923 2105 35  0000 L CNN
F 2 "RESC1608*" V 9960 2150 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 9985 2150 20  0001 C CNN
F 4 "TE CONNECTIVITY" V 10035 2150 20  0001 C CNN "manf"
F 5 "CRGH0603F10K" V 10060 2150 20  0001 C CNN "manf#"
F 6 "Element14" V 10085 2150 20  0001 C CNN "Supplier"
F 7 "2332016" V 10110 2150 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/te-connectivity/crgh0603f10k/resistor-power-10k-0-2w-1-0603/dp/2332016" V 10135 2150 20  0001 C CNN "Supplier URL"
F 9 "0.02" V 10160 2150 20  0001 C CNN "Supplier Price"
F 10 "10" V 10185 2150 20  0001 C CNN "Supplier Price Break"
	1    9850 2300
	1    0    0    -1  
$EndComp
$Comp
L appli_resistor:10K0_1608M R?
U 1 1 5AC16E9A
P 11850 2300
F 0 "R?" H 11923 2185 50  0000 L CNN
F 1 "10K0_1608M" H 11923 2105 35  0000 L CNN
F 2 "RESC1608*" V 11960 2150 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 11985 2150 20  0001 C CNN
F 4 "TE CONNECTIVITY" V 12035 2150 20  0001 C CNN "manf"
F 5 "CRGH0603F10K" V 12060 2150 20  0001 C CNN "manf#"
F 6 "Element14" V 12085 2150 20  0001 C CNN "Supplier"
F 7 "2332016" V 12110 2150 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/te-connectivity/crgh0603f10k/resistor-power-10k-0-2w-1-0603/dp/2332016" V 12135 2150 20  0001 C CNN "Supplier URL"
F 9 "0.02" V 12160 2150 20  0001 C CNN "Supplier Price"
F 10 "10" V 12185 2150 20  0001 C CNN "Supplier Price Break"
	1    11850 2300
	1    0    0    -1  
$EndComp
$Comp
L appli_resistor:10K0_1608M R?
U 1 1 5AC16EF4
P 14050 2300
F 0 "R?" H 14123 2185 50  0000 L CNN
F 1 "10K0_1608M" H 14123 2105 35  0000 L CNN
F 2 "RESC1608*" V 14160 2150 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 14185 2150 20  0001 C CNN
F 4 "TE CONNECTIVITY" V 14235 2150 20  0001 C CNN "manf"
F 5 "CRGH0603F10K" V 14260 2150 20  0001 C CNN "manf#"
F 6 "Element14" V 14285 2150 20  0001 C CNN "Supplier"
F 7 "2332016" V 14310 2150 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/te-connectivity/crgh0603f10k/resistor-power-10k-0-2w-1-0603/dp/2332016" V 14335 2150 20  0001 C CNN "Supplier URL"
F 9 "0.02" V 14360 2150 20  0001 C CNN "Supplier Price"
F 10 "10" V 14385 2150 20  0001 C CNN "Supplier Price Break"
	1    14050 2300
	1    0    0    -1  
$EndComp
Text GLabel 1400 3150 0    50   Input ~ 0
AUX_PWM_1
Text GLabel 1400 4200 0    50   Input ~ 0
AUX_PWM_2
Text GLabel 1400 2100 0    50   Input ~ 0
AUX_PWM_0
Text GLabel 11650 9050 0    50   Output ~ 0
BUZZER_PWM
Text GLabel 4700 2100 0    50   Output ~ 0
USART_1_TX
Text GLabel 4700 3150 0    50   Input ~ 0
USART_1_RX
Text GLabel 9450 9900 0    50   Output ~ 0
USART_2_TX
Text GLabel 9450 10000 0    50   Input ~ 0
USART_2_RX
Text GLabel 9450 9800 0    50   Output ~ 0
USART_2_RTS
Text GLabel 9450 9700 0    50   Input ~ 0
USART_2_CTS
Text GLabel 7200 9600 0    50   Output ~ 0
USART_2_PWR_EN
Text Notes 12575 7750 0    79   ~ 16
Buzzer
Text Notes 11850 7975 0    50   ~ 0
Used for potential indication of state/warnings
Text GLabel 1575 9950 0    50   Output ~ 0
EXT_INPUT_0
Text GLabel 1675 8800 0    50   Input ~ 0
EXT_OUTPUT_1
Text GLabel 1575 10150 0    50   Output ~ 0
EXT_INPUT_1
Text GLabel 1675 8600 0    50   Input ~ 0
EXT_OUTPUT_0
Text Notes 2675 7575 0    50   ~ 0
Provide 2 optocoupled input/output pairs.\nPair 0 is UART capable, Pair 1 is CAN capable.
$Comp
L appli_power:GND #PWR?
U 1 1 5B002BDB
P 5000 10125
F 0 "#PWR?" H 5000 9875 50  0001 C CNN
F 1 "GND" H 5005 9952 50  0000 C CNN
F 2 "" H 5000 10125 60  0000 C CNN
F 3 "" H 5000 10125 60  0000 C CNN
	1    5000 10125
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5B002C6A
P 4975 9575
F 0 "#PWR?" H 4975 9535 30  0001 C CNN
F 1 "+3.3V" H 4984 9713 30  0000 C CNN
F 2 "" H 4975 9575 60  0000 C CNN
F 3 "" H 4975 9575 60  0000 C CNN
	1    4975 9575
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+12V #PWR?
U 1 1 5B002CC8
P 5300 9600
F 0 "#PWR?" H 5300 9450 50  0001 C CNN
F 1 "+12V" H 5315 9773 50  0000 C CNN
F 2 "" H 5300 9600 60  0000 C CNN
F 3 "" H 5300 9600 60  0000 C CNN
	1    5300 9600
	1    0    0    -1  
$EndComp
$Comp
L appli_resistor:220R_1608M R?
U 1 1 5ACBAE28
P 2125 8600
F 0 "R?" V 1937 8450 50  0000 C CNN
F 1 "220R_1608M" V 2017 8450 35  0000 C CNN
F 2 "RESC1608*" V 2235 8450 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1729044.pdf" V 2260 8450 20  0001 C CNN
F 4 "MULTICOMP" V 2310 8450 20  0001 C CNN "manf"
F 5 "MCRE000124" V 2335 8450 20  0001 C CNN "manf#"
F 6 "Element14" V 2360 8450 20  0001 C CNN "Supplier"
F 7 "1711597" V 2385 8450 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mcre000124/res-thick-film-220r-1-0-05w-0201/dp/1711597" V 2410 8450 20  0001 C CNN "Supplier URL"
F 9 "0.016" V 2435 8450 20  0001 C CNN "Supplier Price"
F 10 "1" V 2460 8450 20  0001 C CNN "Supplier Price Break"
	1    2125 8600
	0    1    1    0   
$EndComp
$Comp
L appli_resistor:220R_1608M R?
U 1 1 5ACBAED5
P 2125 8800
F 0 "R?" V 2225 8650 50  0000 C CNN
F 1 "220R_1608M" V 2300 8650 35  0000 C CNN
F 2 "RESC1608*" V 2235 8650 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1729044.pdf" V 2260 8650 20  0001 C CNN
F 4 "MULTICOMP" V 2310 8650 20  0001 C CNN "manf"
F 5 "MCRE000124" V 2335 8650 20  0001 C CNN "manf#"
F 6 "Element14" V 2360 8650 20  0001 C CNN "Supplier"
F 7 "1711597" V 2385 8650 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mcre000124/res-thick-film-220r-1-0-05w-0201/dp/1711597" V 2410 8650 20  0001 C CNN "Supplier URL"
F 9 "0.016" V 2435 8650 20  0001 C CNN "Supplier Price"
F 10 "1" V 2460 8650 20  0001 C CNN "Supplier Price Break"
	1    2125 8800
	0    1    1    0   
$EndComp
Wire Wire Line
	2125 8600 2375 8600
Wire Wire Line
	2375 8800 2125 8800
Wire Wire Line
	1675 8600 1825 8600
Wire Wire Line
	1825 8800 1675 8800
Wire Wire Line
	3075 8400 3225 8400
Wire Wire Line
	3225 8400 3225 8300
Wire Wire Line
	3075 9000 3225 9000
Wire Wire Line
	3225 9000 3225 9100
$Comp
L appli_power:GND #PWR?
U 1 1 5ACCB935
P 3225 9100
F 0 "#PWR?" H 3225 8850 50  0001 C CNN
F 1 "GND" H 3230 8927 50  0000 C CNN
F 2 "" H 3225 9100 60  0000 C CNN
F 3 "" H 3225 9100 60  0000 C CNN
	1    3225 9100
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5ACCB976
P 3225 8300
F 0 "#PWR?" H 3225 8260 30  0001 C CNN
F 1 "+3.3V" H 3234 8438 30  0000 C CNN
F 2 "" H 3225 8300 60  0000 C CNN
F 3 "" H 3225 8300 60  0000 C CNN
	1    3225 8300
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5ACCB9B7
P 4575 9725
F 0 "#PWR?" H 4575 9685 30  0001 C CNN
F 1 "+3.3V" H 4584 9863 30  0000 C CNN
F 2 "" H 4575 9725 60  0000 C CNN
F 3 "" H 4575 9725 60  0000 C CNN
	1    4575 9725
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5ACCBA34
P 4575 10225
F 0 "#PWR?" H 4575 9975 50  0001 C CNN
F 1 "GND" H 4580 10052 50  0000 C CNN
F 2 "" H 4575 10225 60  0000 C CNN
F 3 "" H 4575 10225 60  0000 C CNN
	1    4575 10225
	1    0    0    -1  
$EndComp
Wire Wire Line
	4575 9725 4575 9875
Wire Wire Line
	4575 10075 4575 10225
Wire Wire Line
	3075 8800 3475 8800
Wire Wire Line
	3075 8600 3475 8600
Text Notes 2675 7850 0    50   ~ 0
220R at 3.3V from micro gives 9mA for strong safe activation.\n5mA min, 8-10mA recommended.
Wire Wire Line
	2375 8400 2275 8400
Wire Wire Line
	2275 8400 2275 9000
Wire Wire Line
	2275 9000 2375 9000
Wire Wire Line
	2275 8400 2275 8300
Connection ~ 2275 8400
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5ACEDD92
P 2275 8300
F 0 "#PWR?" H 2275 8260 30  0001 C CNN
F 1 "+3.3V" H 2284 8438 30  0000 C CNN
F 2 "" H 2275 8300 60  0000 C CNN
F 3 "" H 2275 8300 60  0000 C CNN
	1    2275 8300
	1    0    0    -1  
$EndComp
$Comp
L appli_resistor:220R_1608M R?
U 1 1 5ACB9C38
P 3600 9800
F 0 "R?" V 3412 9650 50  0000 C CNN
F 1 "220R_1608M" V 3492 9650 35  0000 C CNN
F 2 "RESC1608*" V 3710 9650 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1729044.pdf" V 3735 9650 20  0001 C CNN
F 4 "MULTICOMP" V 3785 9650 20  0001 C CNN "manf"
F 5 "MCRE000124" V 3810 9650 20  0001 C CNN "manf#"
F 6 "Element14" V 3835 9650 20  0001 C CNN "Supplier"
F 7 "1711597" V 3860 9650 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mcre000124/res-thick-film-220r-1-0-05w-0201/dp/1711597" V 3885 9650 20  0001 C CNN "Supplier URL"
F 9 "0.016" V 3910 9650 20  0001 C CNN "Supplier Price"
F 10 "1" V 3935 9650 20  0001 C CNN "Supplier Price Break"
	1    3600 9800
	0    1    1    0   
$EndComp
$Comp
L appli_resistor:220R_1608M R?
U 1 1 5ACB9CD8
P 3625 10150
F 0 "R?" V 3725 10000 50  0000 C CNN
F 1 "220R_1608M" V 3800 10000 35  0000 C CNN
F 2 "RESC1608*" V 3735 10000 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1729044.pdf" V 3760 10000 20  0001 C CNN
F 4 "MULTICOMP" V 3810 10000 20  0001 C CNN "manf"
F 5 "MCRE000124" V 3835 10000 20  0001 C CNN "manf#"
F 6 "Element14" V 3860 10000 20  0001 C CNN "Supplier"
F 7 "1711597" V 3885 10000 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mcre000124/res-thick-film-220r-1-0-05w-0201/dp/1711597" V 3910 10000 20  0001 C CNN "Supplier URL"
F 9 "0.016" V 3935 10000 20  0001 C CNN "Supplier Price"
F 10 "1" V 3960 10000 20  0001 C CNN "Supplier Price Break"
	1    3625 10150
	0    1    1    0   
$EndComp
$Comp
L appli_power:+12V #PWR?
U 1 1 5AD29E22
P 7150 6150
F 0 "#PWR?" H 7150 6000 50  0001 C CNN
F 1 "+12V" V 7150 6400 50  0000 C CNN
F 2 "" H 7150 6150 60  0000 C CNN
F 3 "" H 7150 6150 60  0000 C CNN
	1    7150 6150
	0    -1   -1   0   
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5AD29E9F
P 3350 4600
F 0 "#PWR?" H 3350 4350 50  0001 C CNN
F 1 "GND" H 3355 4427 50  0000 C CNN
F 2 "" H 3350 4600 60  0000 C CNN
F 3 "" H 3350 4600 60  0000 C CNN
	1    3350 4600
	1    0    0    -1  
$EndComp
Text GLabel 1400 5250 0    50   BiDi ~ 0
AUX_ANALOG_0
Text GLabel 1400 6300 0    50   BiDi ~ 0
AUX_ANALOG_1
Text Notes 1225 1450 0    50   ~ 0
To be used with LED drivers, servos or motors/pumps.\n\nAnalog AUX channels are ADC and DAC capable.\nPWM channels are assigned to non-conflicting timers.\nAll AUX IO can be used as simple input/output pins.
$Comp
L appli_device:BAV99 D?
U 1 1 5AD4CE46
P 1650 3150
F 0 "D?" H 1758 3196 50  0000 L CNN
F 1 "BAV99" H 1758 3105 50  0000 L CNN
F 2 "SOT23" H 1850 3000 20  0001 C CNN
F 3 "http://datasheet.octopart.com/BAV99-/T3-NXP-datasheet-5318834.pdf" H 1850 2975 20  0001 C CNN
F 4 "NXP" H 1850 2925 20  0001 C CNN "manf"
F 5 "RS Online" H 1850 2875 20  0001 C CNN "Supplier"
F 6 " 544-4534" H 1850 2850 20  0001 C CNN "Supplier Part No"
F 7 "http://au.rs-online.com/web/p/rectifier-schottky-diodes/5444534/" H 1850 2825 20  0001 C CNN "Supplier URL"
F 8 "0.045" H 1850 2800 20  0001 C CNN "Supplier Price"
F 9 "1" H 1850 2775 20  0001 C CNN "Supplier Price Break"
F 10 "BAV99" H 1850 2900 20  0001 C CNN "manf#"
	1    1650 3150
	1    0    0    -1  
$EndComp
$Comp
L appli_resistor:220R_1608M R?
U 1 1 5AD4CFC2
P 2400 3150
F 0 "R?" V 2212 3000 50  0000 C CNN
F 1 "220R_1608M" V 2292 3000 35  0000 C CNN
F 2 "RESC1608*" V 2510 3000 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1729044.pdf" V 2535 3000 20  0001 C CNN
F 4 "MULTICOMP" V 2585 3000 20  0001 C CNN "manf"
F 5 "MCRE000124" V 2610 3000 20  0001 C CNN "manf#"
F 6 "Element14" V 2635 3000 20  0001 C CNN "Supplier"
F 7 "1711597" V 2660 3000 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mcre000124/res-thick-film-220r-1-0-05w-0201/dp/1711597" V 2685 3000 20  0001 C CNN "Supplier URL"
F 9 "0.016" V 2710 3000 20  0001 C CNN "Supplier Price"
F 10 "1" V 2735 3000 20  0001 C CNN "Supplier Price Break"
	1    2400 3150
	0    1    1    0   
$EndComp
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5AD4D1B6
P 1650 2850
F 0 "#PWR?" H 1650 2810 30  0001 C CNN
F 1 "+3.3V" H 1659 2988 30  0000 C CNN
F 2 "" H 1650 2850 60  0000 C CNN
F 3 "" H 1650 2850 60  0000 C CNN
	1    1650 2850
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5AD4D21B
P 1650 3450
F 0 "#PWR?" H 1650 3200 50  0001 C CNN
F 1 "GND" H 1655 3277 50  0000 C CNN
F 2 "" H 1650 3450 60  0000 C CNN
F 3 "" H 1650 3450 60  0000 C CNN
	1    1650 3450
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5AD4D4DF
P 2550 3450
F 0 "#PWR?" H 2550 3200 50  0001 C CNN
F 1 "GND" H 2555 3277 50  0000 C CNN
F 2 "" H 2550 3450 60  0000 C CNN
F 3 "" H 2550 3450 60  0000 C CNN
	1    2550 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	1650 3150 2100 3150
Wire Wire Line
	2400 3150 2550 3150
Wire Wire Line
	2550 3150 2550 3200
Wire Wire Line
	2550 3400 2550 3450
Wire Wire Line
	1650 3450 1650 3400
Wire Wire Line
	1650 2900 1650 2850
Connection ~ 2550 3150
Wire Wire Line
	1650 3150 1400 3150
Connection ~ 1650 3150
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5AD69BC0
P 6650 3750
F 0 "#PWR?" H 6650 3710 30  0001 C CNN
F 1 "+3.3V" H 6659 3888 30  0000 C CNN
F 2 "" H 6650 3750 60  0000 C CNN
F 3 "" H 6650 3750 60  0000 C CNN
	1    6650 3750
	1    0    0    -1  
$EndComp
$Comp
L appli_device:BAV99 D?
U 1 1 5AD69E9C
P 1650 4200
F 0 "D?" H 1758 4246 50  0000 L CNN
F 1 "BAV99" H 1758 4155 50  0000 L CNN
F 2 "SOT23" H 1850 4050 20  0001 C CNN
F 3 "http://datasheet.octopart.com/BAV99-/T3-NXP-datasheet-5318834.pdf" H 1850 4025 20  0001 C CNN
F 4 "NXP" H 1850 3975 20  0001 C CNN "manf"
F 5 "RS Online" H 1850 3925 20  0001 C CNN "Supplier"
F 6 " 544-4534" H 1850 3900 20  0001 C CNN "Supplier Part No"
F 7 "http://au.rs-online.com/web/p/rectifier-schottky-diodes/5444534/" H 1850 3875 20  0001 C CNN "Supplier URL"
F 8 "0.045" H 1850 3850 20  0001 C CNN "Supplier Price"
F 9 "1" H 1850 3825 20  0001 C CNN "Supplier Price Break"
F 10 "BAV99" H 1850 3950 20  0001 C CNN "manf#"
	1    1650 4200
	1    0    0    -1  
$EndComp
$Comp
L appli_resistor:220R_1608M R?
U 1 1 5AD69EAA
P 2400 4200
F 0 "R?" V 2212 4050 50  0000 C CNN
F 1 "220R_1608M" V 2292 4050 35  0000 C CNN
F 2 "RESC1608*" V 2510 4050 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1729044.pdf" V 2535 4050 20  0001 C CNN
F 4 "MULTICOMP" V 2585 4050 20  0001 C CNN "manf"
F 5 "MCRE000124" V 2610 4050 20  0001 C CNN "manf#"
F 6 "Element14" V 2635 4050 20  0001 C CNN "Supplier"
F 7 "1711597" V 2660 4050 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mcre000124/res-thick-film-220r-1-0-05w-0201/dp/1711597" V 2685 4050 20  0001 C CNN "Supplier URL"
F 9 "0.016" V 2710 4050 20  0001 C CNN "Supplier Price"
F 10 "1" V 2735 4050 20  0001 C CNN "Supplier Price Break"
	1    2400 4200
	0    1    1    0   
$EndComp
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5AD69EB1
P 1650 3900
F 0 "#PWR?" H 1650 3860 30  0001 C CNN
F 1 "+3.3V" H 1659 4038 30  0000 C CNN
F 2 "" H 1650 3900 60  0000 C CNN
F 3 "" H 1650 3900 60  0000 C CNN
	1    1650 3900
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5AD69EB7
P 1650 4500
F 0 "#PWR?" H 1650 4250 50  0001 C CNN
F 1 "GND" H 1655 4327 50  0000 C CNN
F 2 "" H 1650 4500 60  0000 C CNN
F 3 "" H 1650 4500 60  0000 C CNN
	1    1650 4500
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5AD69ECB
P 2550 4500
F 0 "#PWR?" H 2550 4250 50  0001 C CNN
F 1 "GND" H 2555 4327 50  0000 C CNN
F 2 "" H 2550 4500 60  0000 C CNN
F 3 "" H 2550 4500 60  0000 C CNN
	1    2550 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	1650 4200 2100 4200
Wire Wire Line
	2400 4200 2550 4200
Wire Wire Line
	2550 4200 2550 4250
Wire Wire Line
	2550 4450 2550 4500
Wire Wire Line
	1650 4500 1650 4450
Wire Wire Line
	1650 3950 1650 3900
Connection ~ 2550 4200
Wire Wire Line
	1650 4200 1400 4200
Connection ~ 1650 4200
$Comp
L appli_device:BAV99 D?
U 1 1 5AD6D427
P 1650 5250
F 0 "D?" H 1758 5296 50  0000 L CNN
F 1 "BAV99" H 1758 5205 50  0000 L CNN
F 2 "SOT23" H 1850 5100 20  0001 C CNN
F 3 "http://datasheet.octopart.com/BAV99-/T3-NXP-datasheet-5318834.pdf" H 1850 5075 20  0001 C CNN
F 4 "NXP" H 1850 5025 20  0001 C CNN "manf"
F 5 "RS Online" H 1850 4975 20  0001 C CNN "Supplier"
F 6 " 544-4534" H 1850 4950 20  0001 C CNN "Supplier Part No"
F 7 "http://au.rs-online.com/web/p/rectifier-schottky-diodes/5444534/" H 1850 4925 20  0001 C CNN "Supplier URL"
F 8 "0.045" H 1850 4900 20  0001 C CNN "Supplier Price"
F 9 "1" H 1850 4875 20  0001 C CNN "Supplier Price Break"
F 10 "BAV99" H 1850 5000 20  0001 C CNN "manf#"
	1    1650 5250
	1    0    0    -1  
$EndComp
$Comp
L appli_resistor:220R_1608M R?
U 1 1 5AD6D435
P 2400 5250
F 0 "R?" V 2212 5100 50  0000 C CNN
F 1 "220R_1608M" V 2292 5100 35  0000 C CNN
F 2 "RESC1608*" V 2510 5100 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1729044.pdf" V 2535 5100 20  0001 C CNN
F 4 "MULTICOMP" V 2585 5100 20  0001 C CNN "manf"
F 5 "MCRE000124" V 2610 5100 20  0001 C CNN "manf#"
F 6 "Element14" V 2635 5100 20  0001 C CNN "Supplier"
F 7 "1711597" V 2660 5100 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mcre000124/res-thick-film-220r-1-0-05w-0201/dp/1711597" V 2685 5100 20  0001 C CNN "Supplier URL"
F 9 "0.016" V 2710 5100 20  0001 C CNN "Supplier Price"
F 10 "1" V 2735 5100 20  0001 C CNN "Supplier Price Break"
	1    2400 5250
	0    1    1    0   
$EndComp
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5AD6D43C
P 1650 4950
F 0 "#PWR?" H 1650 4910 30  0001 C CNN
F 1 "+3.3V" H 1659 5088 30  0000 C CNN
F 2 "" H 1650 4950 60  0000 C CNN
F 3 "" H 1650 4950 60  0000 C CNN
	1    1650 4950
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5AD6D442
P 1650 5550
F 0 "#PWR?" H 1650 5300 50  0001 C CNN
F 1 "GND" H 1655 5377 50  0000 C CNN
F 2 "" H 1650 5550 60  0000 C CNN
F 3 "" H 1650 5550 60  0000 C CNN
	1    1650 5550
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5AD6D456
P 2550 5550
F 0 "#PWR?" H 2550 5300 50  0001 C CNN
F 1 "GND" H 2555 5377 50  0000 C CNN
F 2 "" H 2550 5550 60  0000 C CNN
F 3 "" H 2550 5550 60  0000 C CNN
	1    2550 5550
	1    0    0    -1  
$EndComp
Wire Wire Line
	1650 5250 2100 5250
Wire Wire Line
	2400 5250 2550 5250
Wire Wire Line
	2550 5250 2550 5300
Wire Wire Line
	2550 5500 2550 5550
Wire Wire Line
	1650 5550 1650 5500
Wire Wire Line
	1650 5000 1650 4950
Connection ~ 2550 5250
Wire Wire Line
	1650 5250 1400 5250
Connection ~ 1650 5250
$Comp
L appli_device:BAV99 D?
U 1 1 5AD6D46D
P 1650 6300
F 0 "D?" H 1758 6346 50  0000 L CNN
F 1 "BAV99" H 1758 6255 50  0000 L CNN
F 2 "SOT23" H 1850 6150 20  0001 C CNN
F 3 "http://datasheet.octopart.com/BAV99-/T3-NXP-datasheet-5318834.pdf" H 1850 6125 20  0001 C CNN
F 4 "NXP" H 1850 6075 20  0001 C CNN "manf"
F 5 "RS Online" H 1850 6025 20  0001 C CNN "Supplier"
F 6 " 544-4534" H 1850 6000 20  0001 C CNN "Supplier Part No"
F 7 "http://au.rs-online.com/web/p/rectifier-schottky-diodes/5444534/" H 1850 5975 20  0001 C CNN "Supplier URL"
F 8 "0.045" H 1850 5950 20  0001 C CNN "Supplier Price"
F 9 "1" H 1850 5925 20  0001 C CNN "Supplier Price Break"
F 10 "BAV99" H 1850 6050 20  0001 C CNN "manf#"
	1    1650 6300
	1    0    0    -1  
$EndComp
$Comp
L appli_resistor:220R_1608M R?
U 1 1 5AD6D47B
P 2400 6300
F 0 "R?" V 2212 6150 50  0000 C CNN
F 1 "220R_1608M" V 2292 6150 35  0000 C CNN
F 2 "RESC1608*" V 2510 6150 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1729044.pdf" V 2535 6150 20  0001 C CNN
F 4 "MULTICOMP" V 2585 6150 20  0001 C CNN "manf"
F 5 "MCRE000124" V 2610 6150 20  0001 C CNN "manf#"
F 6 "Element14" V 2635 6150 20  0001 C CNN "Supplier"
F 7 "1711597" V 2660 6150 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mcre000124/res-thick-film-220r-1-0-05w-0201/dp/1711597" V 2685 6150 20  0001 C CNN "Supplier URL"
F 9 "0.016" V 2710 6150 20  0001 C CNN "Supplier Price"
F 10 "1" V 2735 6150 20  0001 C CNN "Supplier Price Break"
	1    2400 6300
	0    1    1    0   
$EndComp
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5AD6D482
P 1650 6000
F 0 "#PWR?" H 1650 5960 30  0001 C CNN
F 1 "+3.3V" H 1659 6138 30  0000 C CNN
F 2 "" H 1650 6000 60  0000 C CNN
F 3 "" H 1650 6000 60  0000 C CNN
	1    1650 6000
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5AD6D488
P 1650 6600
F 0 "#PWR?" H 1650 6350 50  0001 C CNN
F 1 "GND" H 1655 6427 50  0000 C CNN
F 2 "" H 1650 6600 60  0000 C CNN
F 3 "" H 1650 6600 60  0000 C CNN
	1    1650 6600
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5AD6D49C
P 2550 6600
F 0 "#PWR?" H 2550 6350 50  0001 C CNN
F 1 "GND" H 2555 6427 50  0000 C CNN
F 2 "" H 2550 6600 60  0000 C CNN
F 3 "" H 2550 6600 60  0000 C CNN
	1    2550 6600
	1    0    0    -1  
$EndComp
Wire Wire Line
	1650 6300 2100 6300
Wire Wire Line
	2400 6300 2550 6300
Wire Wire Line
	2550 6300 2550 6350
Wire Wire Line
	2550 6550 2550 6600
Wire Wire Line
	1650 6600 1650 6550
Wire Wire Line
	1650 6050 1650 6000
Connection ~ 2550 6300
Wire Wire Line
	1650 6300 1400 6300
Connection ~ 1650 6300
$Comp
L appli_connector_jst:JST_GH_6_RA P?
U 1 1 5AD83EAF
P 3600 4250
F 0 "P?" H 3678 4291 50  0000 L CNN
F 1 "JST_GH_6_RA" H 3678 4200 50  0000 L CNN
F 2 "Applidyne_Connector_JST:SM06B-GHS-TB" H 3600 3050 50  0001 C CNN
F 3 "http://docs-asia.electrocomponents.com/webdocs/10ce/0900766b810cef18.pdf" H 3600 3320 50  0001 C CNN
F 4 "JST" H 3610 3840 60  0001 C CNN "manf"
F 5 "SM06B-GHS-TB(LF)(SN)" H 3600 3750 60  0001 C CNN "manf#"
F 6 "RS Online" H 3610 3640 60  0001 C CNN "Supplier"
F 7 "752-1800" H 3610 3540 60  0001 C CNN "Supplier Part No"
F 8 "http://au.rs-online.com/web/p/pcb-headers/7521800/" H 3600 3440 60  0001 C CNN "Supplier URL"
F 9 "0.802" H 3600 3230 60  0001 C CNN "Supplier Price"
F 10 "5" H 3600 3140 60  0001 C CNN "Supplier Price Break"
	1    3600 4250
	1    0    0    -1  
$EndComp
Text GLabel 4700 5250 0    50   BiDi ~ 0
I2C_SDA
Text GLabel 4700 4200 0    50   BiDi ~ 0
I2C_SCK
$Comp
L appli_device:BAV99 D?
U 1 1 5ADBA82D
P 1650 2100
F 0 "D?" H 1758 2146 50  0000 L CNN
F 1 "BAV99" H 1758 2055 50  0000 L CNN
F 2 "SOT23" H 1850 1950 20  0001 C CNN
F 3 "http://datasheet.octopart.com/BAV99-/T3-NXP-datasheet-5318834.pdf" H 1850 1925 20  0001 C CNN
F 4 "NXP" H 1850 1875 20  0001 C CNN "manf"
F 5 "RS Online" H 1850 1825 20  0001 C CNN "Supplier"
F 6 " 544-4534" H 1850 1800 20  0001 C CNN "Supplier Part No"
F 7 "http://au.rs-online.com/web/p/rectifier-schottky-diodes/5444534/" H 1850 1775 20  0001 C CNN "Supplier URL"
F 8 "0.045" H 1850 1750 20  0001 C CNN "Supplier Price"
F 9 "1" H 1850 1725 20  0001 C CNN "Supplier Price Break"
F 10 "BAV99" H 1850 1850 20  0001 C CNN "manf#"
	1    1650 2100
	1    0    0    -1  
$EndComp
$Comp
L appli_resistor:220R_1608M R?
U 1 1 5ADBA83B
P 2400 2100
F 0 "R?" V 2212 1950 50  0000 C CNN
F 1 "220R_1608M" V 2292 1950 35  0000 C CNN
F 2 "RESC1608*" V 2510 1950 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1729044.pdf" V 2535 1950 20  0001 C CNN
F 4 "MULTICOMP" V 2585 1950 20  0001 C CNN "manf"
F 5 "MCRE000124" V 2610 1950 20  0001 C CNN "manf#"
F 6 "Element14" V 2635 1950 20  0001 C CNN "Supplier"
F 7 "1711597" V 2660 1950 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mcre000124/res-thick-film-220r-1-0-05w-0201/dp/1711597" V 2685 1950 20  0001 C CNN "Supplier URL"
F 9 "0.016" V 2710 1950 20  0001 C CNN "Supplier Price"
F 10 "1" V 2735 1950 20  0001 C CNN "Supplier Price Break"
	1    2400 2100
	0    1    1    0   
$EndComp
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5ADBA842
P 1650 1800
F 0 "#PWR?" H 1650 1760 30  0001 C CNN
F 1 "+3.3V" H 1659 1938 30  0000 C CNN
F 2 "" H 1650 1800 60  0000 C CNN
F 3 "" H 1650 1800 60  0000 C CNN
	1    1650 1800
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5ADBA848
P 1650 2400
F 0 "#PWR?" H 1650 2150 50  0001 C CNN
F 1 "GND" H 1655 2227 50  0000 C CNN
F 2 "" H 1650 2400 60  0000 C CNN
F 3 "" H 1650 2400 60  0000 C CNN
	1    1650 2400
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5ADBA85C
P 2550 2400
F 0 "#PWR?" H 2550 2150 50  0001 C CNN
F 1 "GND" H 2555 2227 50  0000 C CNN
F 2 "" H 2550 2400 60  0000 C CNN
F 3 "" H 2550 2400 60  0000 C CNN
	1    2550 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	1650 2100 2100 2100
Wire Wire Line
	2400 2100 2550 2100
Wire Wire Line
	2550 2100 2550 2150
Wire Wire Line
	2550 2350 2550 2400
Wire Wire Line
	1650 2400 1650 2350
Wire Wire Line
	1650 1850 1650 1800
Connection ~ 2550 2100
Wire Wire Line
	1650 2100 1400 2100
Connection ~ 1650 2100
Wire Wire Line
	3400 4100 3000 4100
Wire Wire Line
	3000 4100 3000 3150
Wire Wire Line
	2550 3150 3000 3150
Wire Wire Line
	3400 4000 3100 4000
Wire Wire Line
	3100 4000 3100 2100
Wire Wire Line
	2550 2100 3100 2100
Wire Wire Line
	2550 4200 3400 4200
Wire Wire Line
	3400 4300 3000 4300
Wire Wire Line
	3000 4300 3000 5250
Wire Wire Line
	2550 5250 3000 5250
Wire Wire Line
	3400 4400 3100 4400
Wire Wire Line
	3100 4400 3100 6300
Wire Wire Line
	2550 6300 3100 6300
Wire Wire Line
	3350 4600 3350 4500
Wire Wire Line
	3350 4500 3400 4500
Text Notes 4725 875  0    79   ~ 16
Internal Expansion Serial
Text Notes 4600 1400 0    50   ~ 0
I2C and Serial Expansion for internal or end effector use.\nAllows for sensors, or daughterboards etc.\nI2C pullup are provided on this board.
Wire Wire Line
	6700 4400 6650 4400
Wire Wire Line
	6650 4400 6650 4550
$Comp
L appli_power:GND #PWR?
U 1 1 5AE0D3F7
P 6650 4550
F 0 "#PWR?" H 6650 4300 50  0001 C CNN
F 1 "GND" H 6655 4377 50  0000 C CNN
F 2 "" H 6650 4550 60  0000 C CNN
F 3 "" H 6650 4550 60  0000 C CNN
	1    6650 4550
	1    0    0    -1  
$EndComp
$Comp
L appli_device:BAV99 D?
U 1 1 5AE3D819
P 4950 2100
F 0 "D?" H 5058 2146 50  0000 L CNN
F 1 "BAV99" H 5058 2055 50  0000 L CNN
F 2 "SOT23" H 5150 1950 20  0001 C CNN
F 3 "http://datasheet.octopart.com/BAV99-/T3-NXP-datasheet-5318834.pdf" H 5150 1925 20  0001 C CNN
F 4 "NXP" H 5150 1875 20  0001 C CNN "manf"
F 5 "RS Online" H 5150 1825 20  0001 C CNN "Supplier"
F 6 " 544-4534" H 5150 1800 20  0001 C CNN "Supplier Part No"
F 7 "http://au.rs-online.com/web/p/rectifier-schottky-diodes/5444534/" H 5150 1775 20  0001 C CNN "Supplier URL"
F 8 "0.045" H 5150 1750 20  0001 C CNN "Supplier Price"
F 9 "1" H 5150 1725 20  0001 C CNN "Supplier Price Break"
F 10 "BAV99" H 5150 1850 20  0001 C CNN "manf#"
	1    4950 2100
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5AE3D82E
P 4950 1800
F 0 "#PWR?" H 4950 1760 30  0001 C CNN
F 1 "+3.3V" H 4959 1938 30  0000 C CNN
F 2 "" H 4950 1800 60  0000 C CNN
F 3 "" H 4950 1800 60  0000 C CNN
	1    4950 1800
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5AE3D834
P 4950 2400
F 0 "#PWR?" H 4950 2150 50  0001 C CNN
F 1 "GND" H 4955 2227 50  0000 C CNN
F 2 "" H 4950 2400 60  0000 C CNN
F 3 "" H 4950 2400 60  0000 C CNN
	1    4950 2400
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5AE3D848
P 5850 2400
F 0 "#PWR?" H 5850 2150 50  0001 C CNN
F 1 "GND" H 5855 2227 50  0000 C CNN
F 2 "" H 5850 2400 60  0000 C CNN
F 3 "" H 5850 2400 60  0000 C CNN
	1    5850 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 2100 5400 2100
Wire Wire Line
	5700 2100 5850 2100
Wire Wire Line
	5850 2350 5850 2400
Wire Wire Line
	4950 2400 4950 2350
Wire Wire Line
	4950 1850 4950 1800
Wire Wire Line
	4950 2100 4700 2100
Connection ~ 4950 2100
$Comp
L appli_device:BAV99 D?
U 1 1 5AE3D85E
P 4950 3150
F 0 "D?" H 5058 3196 50  0000 L CNN
F 1 "BAV99" H 5058 3105 50  0000 L CNN
F 2 "SOT23" H 5150 3000 20  0001 C CNN
F 3 "http://datasheet.octopart.com/BAV99-/T3-NXP-datasheet-5318834.pdf" H 5150 2975 20  0001 C CNN
F 4 "NXP" H 5150 2925 20  0001 C CNN "manf"
F 5 "RS Online" H 5150 2875 20  0001 C CNN "Supplier"
F 6 " 544-4534" H 5150 2850 20  0001 C CNN "Supplier Part No"
F 7 "http://au.rs-online.com/web/p/rectifier-schottky-diodes/5444534/" H 5150 2825 20  0001 C CNN "Supplier URL"
F 8 "0.045" H 5150 2800 20  0001 C CNN "Supplier Price"
F 9 "1" H 5150 2775 20  0001 C CNN "Supplier Price Break"
F 10 "BAV99" H 5150 2900 20  0001 C CNN "manf#"
	1    4950 3150
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5AE3D873
P 4950 2850
F 0 "#PWR?" H 4950 2810 30  0001 C CNN
F 1 "+3.3V" H 4959 2988 30  0000 C CNN
F 2 "" H 4950 2850 60  0000 C CNN
F 3 "" H 4950 2850 60  0000 C CNN
	1    4950 2850
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5AE3D879
P 4950 3450
F 0 "#PWR?" H 4950 3200 50  0001 C CNN
F 1 "GND" H 4955 3277 50  0000 C CNN
F 2 "" H 4950 3450 60  0000 C CNN
F 3 "" H 4950 3450 60  0000 C CNN
	1    4950 3450
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5AE3D88D
P 5850 3450
F 0 "#PWR?" H 5850 3200 50  0001 C CNN
F 1 "GND" H 5855 3277 50  0000 C CNN
F 2 "" H 5850 3450 60  0000 C CNN
F 3 "" H 5850 3450 60  0000 C CNN
	1    5850 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 3150 5400 3150
Wire Wire Line
	5850 3400 5850 3450
Wire Wire Line
	4950 3450 4950 3400
Wire Wire Line
	4950 2900 4950 2850
Wire Wire Line
	4950 3150 4700 3150
Connection ~ 4950 3150
$Comp
L appli_device:BAV99 D?
U 1 1 5AE3D8A3
P 4950 4200
F 0 "D?" H 5058 4246 50  0000 L CNN
F 1 "BAV99" H 5058 4155 50  0000 L CNN
F 2 "SOT23" H 5150 4050 20  0001 C CNN
F 3 "http://datasheet.octopart.com/BAV99-/T3-NXP-datasheet-5318834.pdf" H 5150 4025 20  0001 C CNN
F 4 "NXP" H 5150 3975 20  0001 C CNN "manf"
F 5 "RS Online" H 5150 3925 20  0001 C CNN "Supplier"
F 6 " 544-4534" H 5150 3900 20  0001 C CNN "Supplier Part No"
F 7 "http://au.rs-online.com/web/p/rectifier-schottky-diodes/5444534/" H 5150 3875 20  0001 C CNN "Supplier URL"
F 8 "0.045" H 5150 3850 20  0001 C CNN "Supplier Price"
F 9 "1" H 5150 3825 20  0001 C CNN "Supplier Price Break"
F 10 "BAV99" H 5150 3950 20  0001 C CNN "manf#"
	1    4950 4200
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5AE3D8B8
P 4950 3900
F 0 "#PWR?" H 4950 3860 30  0001 C CNN
F 1 "+3.3V" H 4959 4038 30  0000 C CNN
F 2 "" H 4950 3900 60  0000 C CNN
F 3 "" H 4950 3900 60  0000 C CNN
	1    4950 3900
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5AE3D8BE
P 4950 4500
F 0 "#PWR?" H 4950 4250 50  0001 C CNN
F 1 "GND" H 4955 4327 50  0000 C CNN
F 2 "" H 4950 4500 60  0000 C CNN
F 3 "" H 4950 4500 60  0000 C CNN
	1    4950 4500
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5AE3D8D2
P 5850 4500
F 0 "#PWR?" H 5850 4250 50  0001 C CNN
F 1 "GND" H 5855 4327 50  0000 C CNN
F 2 "" H 5850 4500 60  0000 C CNN
F 3 "" H 5850 4500 60  0000 C CNN
	1    5850 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 4200 5400 4200
Wire Wire Line
	5850 4450 5850 4500
Wire Wire Line
	4950 4500 4950 4450
Wire Wire Line
	4950 3950 4950 3900
Wire Wire Line
	4950 4200 4700 4200
Connection ~ 4950 4200
$Comp
L appli_device:BAV99 D?
U 1 1 5AE3D8E8
P 4950 5250
F 0 "D?" H 5058 5296 50  0000 L CNN
F 1 "BAV99" H 5058 5205 50  0000 L CNN
F 2 "SOT23" H 5150 5100 20  0001 C CNN
F 3 "http://datasheet.octopart.com/BAV99-/T3-NXP-datasheet-5318834.pdf" H 5150 5075 20  0001 C CNN
F 4 "NXP" H 5150 5025 20  0001 C CNN "manf"
F 5 "RS Online" H 5150 4975 20  0001 C CNN "Supplier"
F 6 " 544-4534" H 5150 4950 20  0001 C CNN "Supplier Part No"
F 7 "http://au.rs-online.com/web/p/rectifier-schottky-diodes/5444534/" H 5150 4925 20  0001 C CNN "Supplier URL"
F 8 "0.045" H 5150 4900 20  0001 C CNN "Supplier Price"
F 9 "1" H 5150 4875 20  0001 C CNN "Supplier Price Break"
F 10 "BAV99" H 5150 5000 20  0001 C CNN "manf#"
	1    4950 5250
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5AE3D8FD
P 4950 4950
F 0 "#PWR?" H 4950 4910 30  0001 C CNN
F 1 "+3.3V" H 4959 5088 30  0000 C CNN
F 2 "" H 4950 4950 60  0000 C CNN
F 3 "" H 4950 4950 60  0000 C CNN
	1    4950 4950
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5AE3D903
P 4950 5550
F 0 "#PWR?" H 4950 5300 50  0001 C CNN
F 1 "GND" H 4955 5377 50  0000 C CNN
F 2 "" H 4950 5550 60  0000 C CNN
F 3 "" H 4950 5550 60  0000 C CNN
	1    4950 5550
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5AE3D917
P 5850 5550
F 0 "#PWR?" H 5850 5300 50  0001 C CNN
F 1 "GND" H 5855 5377 50  0000 C CNN
F 2 "" H 5850 5550 60  0000 C CNN
F 3 "" H 5850 5550 60  0000 C CNN
	1    5850 5550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 5250 5400 5250
Wire Wire Line
	5700 5250 5850 5250
Wire Wire Line
	5850 5500 5850 5550
Wire Wire Line
	4950 5550 4950 5500
Wire Wire Line
	4950 5000 4950 4950
Wire Wire Line
	5700 4200 5850 4200
Wire Wire Line
	5700 3150 5850 3150
Wire Wire Line
	5850 2100 5850 2150
Wire Wire Line
	5850 3150 5850 3200
Wire Wire Line
	5850 4200 5850 4250
Wire Wire Line
	5850 5250 5850 5300
Wire Wire Line
	6400 4000 6400 2100
Wire Wire Line
	5850 2100 6400 2100
Wire Wire Line
	5850 4200 6700 4200
Wire Wire Line
	6300 4300 6300 5250
Wire Wire Line
	5850 5250 6300 5250
Wire Wire Line
	4950 5250 4700 5250
Connection ~ 4950 5250
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5AF06D31
P 7150 6550
F 0 "#PWR?" H 7150 6510 30  0001 C CNN
F 1 "+3.3V" V 7150 6725 30  0000 C CNN
F 2 "" H 7150 6550 60  0000 C CNN
F 3 "" H 7150 6550 60  0000 C CNN
	1    7150 6550
	0    -1   -1   0   
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5AF06DAE
P 7450 6750
F 0 "#PWR?" H 7450 6500 50  0001 C CNN
F 1 "GND" H 7455 6577 50  0000 C CNN
F 2 "" H 7450 6750 60  0000 C CNN
F 3 "" H 7450 6750 60  0000 C CNN
	1    7450 6750
	1    0    0    -1  
$EndComp
Text Notes 7425 8275 0    50   ~ 0
Expansion Header for ESP8266/32 or BLE module.\nProvide USART with external power control.
$Comp
L appli_transistor:2N7002P Q?
U 1 1 5AF3D9B7
P 12550 9000
F 0 "Q?" H 12741 9046 50  0000 L CNN
F 1 "2N7002P" H 12741 8955 50  0000 L CNN
F 2 "SOT23" H 12750 8900 50  0001 L CIN
F 3 "http://docs-asia.electrocomponents.com/webdocs/1385/0900766b813856ab.pdf" H 10920 8730 50  0001 L CNN
F 4 "Fairchild" H 12550 8500 60  0001 C CNN "manf"
F 5 " 2N7002,215" H 12550 8380 60  0001 C CNN "manf#"
F 6 "RS Online" H 12550 8280 60  0001 C CNN "Supplier"
F 7 "http://au.rs-online.com/web/p/mosfet-transistors/4367379/" H 12550 8160 60  0001 C CNN "Supplier URL"
F 8 " 436-7379" H 12550 8030 60  0001 C CNN "Supplier Part No"
F 9 "0.232" H 12550 7910 60  0001 C CNN "Supplier Price"
F 10 "25" H 12550 7780 60  0001 C CNN "Supplier Price Break"
	1    12550 9000
	1    0    0    -1  
$EndComp
$Comp
L appli_resistor:10R_1608M R?
U 1 1 5AF3DC0C
P 12050 9050
F 0 "R?" V 11862 8900 50  0000 C CNN
F 1 "10R_1608M" V 11942 8900 35  0000 C CNN
F 2 "RESC1608*" V 12160 8900 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 12185 8900 20  0001 C CNN
F 4 "MULTICOMP" V 12235 8900 20  0001 C CNN "manf"
F 5 "MCHP03W8F100JT5E" V 12260 8900 20  0001 C CNN "manf#"
F 6 "Element14" V 12285 8900 20  0001 C CNN "Supplier"
F 7 "1576250" V 12310 8900 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mchp03w8f100jt5e/res-thick-film-10r-1-0-125w-0603/dp/1576250" V 12335 8900 20  0001 C CNN "Supplier URL"
F 9 "0.025" V 12360 8900 20  0001 C CNN "Supplier Price"
F 10 "1" V 12385 8900 20  0001 C CNN "Supplier Price Break"
	1    12050 9050
	0    1    1    0   
$EndComp
$Comp
L appli_resistor:10K0_1608M R?
U 1 1 5AF3E125
P 12150 9150
F 0 "R?" H 12223 9035 50  0000 L CNN
F 1 "10K0_1608M" H 12223 8955 35  0000 L CNN
F 2 "RESC1608*" V 12260 9000 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 12285 9000 20  0001 C CNN
F 4 "TE CONNECTIVITY" V 12335 9000 20  0001 C CNN "manf"
F 5 "CRGH0603F10K" V 12360 9000 20  0001 C CNN "manf#"
F 6 "Element14" V 12385 9000 20  0001 C CNN "Supplier"
F 7 "2332016" V 12410 9000 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/te-connectivity/crgh0603f10k/resistor-power-10k-0-2w-1-0603/dp/2332016" V 12435 9000 20  0001 C CNN "Supplier URL"
F 9 "0.02" V 12460 9000 20  0001 C CNN "Supplier Price"
F 10 "10" V 12485 9000 20  0001 C CNN "Supplier Price Break"
	1    12150 9150
	1    0    0    -1  
$EndComp
Wire Wire Line
	12150 9450 12150 9550
Wire Wire Line
	12150 9550 12650 9550
Wire Wire Line
	12650 9550 12650 9200
Wire Wire Line
	12350 9050 12150 9050
Wire Wire Line
	11750 9050 11650 9050
Wire Wire Line
	12150 9150 12150 9050
Connection ~ 12150 9050
Wire Wire Line
	12150 9050 12050 9050
Wire Wire Line
	12650 9550 12650 9600
Connection ~ 12650 9550
Wire Wire Line
	12650 8800 12650 8650
$Comp
L appli_power:GND #PWR?
U 1 1 5AF786FF
P 12650 9600
F 0 "#PWR?" H 12650 9350 50  0001 C CNN
F 1 "GND" H 12655 9427 50  0000 C CNN
F 2 "" H 12650 9600 60  0000 C CNN
F 3 "" H 12650 9600 60  0000 C CNN
	1    12650 9600
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+12V #PWR?
U 1 1 5AF8C7A8
P 12650 8375
F 0 "#PWR?" H 12650 8225 50  0001 C CNN
F 1 "+12V" H 12665 8548 50  0000 C CNN
F 2 "" H 12650 8375 60  0000 C CNN
F 3 "" H 12650 8375 60  0000 C CNN
	1    12650 8375
	1    0    0    -1  
$EndComp
$Comp
L appli_special:PKM22EPPH4001 LS?
U 1 1 5AF8C982
P 13000 8600
F 0 "LS?" H 13248 8663 60  0000 L CNN
F 1 "PKM22EPPH4001" H 13248 8557 60  0000 L CNN
F 2 "Applidyne_Special:PKM22EPPH4001" H 13000 8250 60  0001 C CNN
F 3 "http://docs-asia.electrocomponents.com/webdocs/0c1e/0900766b80c1ed4a.pdf" H 13000 8360 60  0001 C CNN
F 4 "MURATA" H 13010 8130 60  0001 C CNN "manf"
F 5 "PKM22EPPH4001-B0" H 13000 8020 60  0001 C CNN "manf#"
F 6 "RS Online" H 13000 7920 60  0001 C CNN "Supplier"
F 7 "498-6582" H 13000 7830 60  0001 C CNN "Supplier Part No"
F 8 "http://au.rs-online.com/web/p/piezo-buzzer-components/4986582/" H 13000 7740 60  0001 C CNN "Supplier URL"
F 9 "1.4" H 13000 7630 60  0001 C CNN "Supplier Price"
F 10 "1" H 13000 7530 60  0001 C CNN "Supplier Price Break"
	1    13000 8600
	1    0    0    -1  
$EndComp
Wire Wire Line
	12650 8550 12650 8375
Wire Wire Line
	12650 8550 12750 8550
Wire Wire Line
	12750 8650 12650 8650
$Comp
L appli_transistor:2N7002P Q?
U 1 1 5B0B1DE7
P 8250 9550
F 0 "Q?" H 8441 9596 50  0000 L CNN
F 1 "2N7002P" H 8441 9505 50  0000 L CNN
F 2 "SOT23" H 8450 9450 50  0001 L CIN
F 3 "http://docs-asia.electrocomponents.com/webdocs/1385/0900766b813856ab.pdf" H 6620 9280 50  0001 L CNN
F 4 "Fairchild" H 8250 9050 60  0001 C CNN "manf"
F 5 " 2N7002,215" H 8250 8930 60  0001 C CNN "manf#"
F 6 "RS Online" H 8250 8830 60  0001 C CNN "Supplier"
F 7 "http://au.rs-online.com/web/p/mosfet-transistors/4367379/" H 8250 8710 60  0001 C CNN "Supplier URL"
F 8 " 436-7379" H 8250 8580 60  0001 C CNN "Supplier Part No"
F 9 "0.232" H 8250 8460 60  0001 C CNN "Supplier Price"
F 10 "25" H 8250 8330 60  0001 C CNN "Supplier Price Break"
	1    8250 9550
	1    0    0    -1  
$EndComp
$Comp
L appli_resistor:10R_1608M R?
U 1 1 5B0BD8B8
P 7650 9600
F 0 "R?" V 7462 9450 50  0000 C CNN
F 1 "10R_1608M" V 7542 9450 35  0000 C CNN
F 2 "RESC1608*" V 7760 9450 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 7785 9450 20  0001 C CNN
F 4 "MULTICOMP" V 7835 9450 20  0001 C CNN "manf"
F 5 "MCHP03W8F100JT5E" V 7860 9450 20  0001 C CNN "manf#"
F 6 "Element14" V 7885 9450 20  0001 C CNN "Supplier"
F 7 "1576250" V 7910 9450 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mchp03w8f100jt5e/res-thick-film-10r-1-0-125w-0603/dp/1576250" V 7935 9450 20  0001 C CNN "Supplier URL"
F 9 "0.025" V 7960 9450 20  0001 C CNN "Supplier Price"
F 10 "1" V 7985 9450 20  0001 C CNN "Supplier Price Break"
	1    7650 9600
	0    1    1    0   
$EndComp
$Comp
L appli_resistor:10K0_1608M R?
U 1 1 5B0BD968
P 7850 9700
F 0 "R?" H 7923 9585 50  0000 L CNN
F 1 "10K0_1608M" H 7923 9505 35  0000 L CNN
F 2 "RESC1608*" V 7960 9550 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 7985 9550 20  0001 C CNN
F 4 "TE CONNECTIVITY" V 8035 9550 20  0001 C CNN "manf"
F 5 "CRGH0603F10K" V 8060 9550 20  0001 C CNN "manf#"
F 6 "Element14" V 8085 9550 20  0001 C CNN "Supplier"
F 7 "2332016" V 8110 9550 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/te-connectivity/crgh0603f10k/resistor-power-10k-0-2w-1-0603/dp/2332016" V 8135 9550 20  0001 C CNN "Supplier URL"
F 9 "0.02" V 8160 9550 20  0001 C CNN "Supplier Price"
F 10 "10" V 8185 9550 20  0001 C CNN "Supplier Price Break"
	1    7850 9700
	1    0    0    -1  
$EndComp
$Comp
L appli_transistor:NTR1P02T1G Q?
U 1 1 5B0C8ADE
P 8900 9200
F 0 "Q?" H 9091 9246 50  0000 L CNN
F 1 "NTR1P02T1G" H 9091 9155 50  0000 L CNN
F 2 "" H 9148 9100 50  0001 L CIN
F 3 "http://www.farnell.com/datasheets/1708311.pdf" H 7270 8930 50  0001 L CNN
F 4 "On Semi" H 8900 8700 60  0001 C CNN "manf"
F 5 "NTR1P02T1G" H 8900 8580 60  0001 C CNN "manf#"
F 6 "Element14" H 8900 8480 60  0001 C CNN "Supplier"
F 7 "http://au.element14.com/on-semiconductor/ntr1p02t1g/mosfet-p-ch-20v-1a-sot-23-3/dp/2317617" H 8900 8360 60  0001 C CNN "Supplier URL"
F 8 "2317617" H 8900 8230 60  0001 C CNN "Supplier Part No"
F 9 "0.53" H 8900 8110 60  0001 C CNN "Supplier Price"
F 10 "1" H 8900 7980 60  0001 C CNN "Supplier Price Break"
	1    8900 9200
	1    0    0    -1  
$EndComp
$Comp
L appli_resistor:10K0_1608M R?
U 1 1 5B0C8C44
P 8350 8850
F 0 "R?" H 8423 8735 50  0000 L CNN
F 1 "10K0_1608M" H 8423 8655 35  0000 L CNN
F 2 "RESC1608*" V 8460 8700 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 8485 8700 20  0001 C CNN
F 4 "TE CONNECTIVITY" V 8535 8700 20  0001 C CNN "manf"
F 5 "CRGH0603F10K" V 8560 8700 20  0001 C CNN "manf#"
F 6 "Element14" V 8585 8700 20  0001 C CNN "Supplier"
F 7 "2332016" V 8610 8700 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/te-connectivity/crgh0603f10k/resistor-power-10k-0-2w-1-0603/dp/2332016" V 8635 8700 20  0001 C CNN "Supplier URL"
F 9 "0.02" V 8660 8700 20  0001 C CNN "Supplier Price"
F 10 "10" V 8685 8700 20  0001 C CNN "Supplier Price Break"
	1    8350 8850
	1    0    0    -1  
$EndComp
Wire Wire Line
	8350 9350 8350 9250
Wire Wire Line
	8700 9250 8350 9250
Connection ~ 8350 9250
Wire Wire Line
	8350 9250 8350 9150
Wire Wire Line
	8050 9600 7850 9600
Wire Wire Line
	7850 9700 7850 9600
Connection ~ 7850 9600
Wire Wire Line
	7850 9600 7650 9600
Wire Wire Line
	7850 10000 7850 10100
Wire Wire Line
	7850 10100 8350 10100
Wire Wire Line
	8350 10100 8350 9750
Wire Wire Line
	8350 10100 8350 10200
Connection ~ 8350 10100
Wire Wire Line
	7350 9600 7200 9600
Wire Wire Line
	8350 8850 8350 8750
Wire Wire Line
	8350 8750 9000 8750
Wire Wire Line
	9000 8750 9000 9000
Wire Wire Line
	9000 9400 9000 9600
$Comp
L appli_power:GND #PWR?
U 1 1 5B1370F8
P 8350 10200
F 0 "#PWR?" H 8350 9950 50  0001 C CNN
F 1 "GND" H 8355 10027 50  0000 C CNN
F 2 "" H 8350 10200 60  0000 C CNN
F 3 "" H 8350 10200 60  0000 C CNN
	1    8350 10200
	1    0    0    -1  
$EndComp
Wire Wire Line
	9000 9600 9600 9600
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5B15AAFC
P 9000 8700
F 0 "#PWR?" H 9000 8660 30  0001 C CNN
F 1 "+3.3V" H 9009 8838 30  0000 C CNN
F 2 "" H 9000 8700 60  0000 C CNN
F 3 "" H 9000 8700 60  0000 C CNN
	1    9000 8700
	1    0    0    -1  
$EndComp
Wire Wire Line
	9000 8700 9000 8750
Connection ~ 9000 8750
$Comp
L appli_power:GND #PWR?
U 1 1 5B17E8CF
P 9450 10200
F 0 "#PWR?" H 9450 9950 50  0001 C CNN
F 1 "GND" H 9455 10027 50  0000 C CNN
F 2 "" H 9450 10200 60  0000 C CNN
F 3 "" H 9450 10200 60  0000 C CNN
	1    9450 10200
	1    0    0    -1  
$EndComp
Wire Wire Line
	9450 10200 9450 10100
Wire Wire Line
	9450 10100 9600 10100
Wire Wire Line
	9450 10000 9600 10000
Wire Wire Line
	9450 9900 9600 9900
Wire Wire Line
	9450 9800 9600 9800
Wire Wire Line
	9450 9700 9600 9700
Text Notes 13500 9150 0    118  ~ 24
TODO find correct buzzer\n
Text Notes 9150 8925 0    50   ~ 0
Rated to 1A
$Comp
L appli_resistor:4K7_1608M R?
U 1 1 5B32017C
P 12850 4250
F 0 "R?" H 12777 4065 50  0000 R CNN
F 1 "4K7_1608M" H 12777 4145 35  0000 R CNN
F 2 "RESC1608*" V 12960 4100 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1788326.pdf" V 12985 4100 20  0001 C CNN
F 4 "MULTICOMP" V 13035 4100 20  0001 C CNN "manf"
F 5 "MC0063W060354K7" V 13060 4100 20  0001 C CNN "manf#"
F 6 "Element14" V 13085 4100 20  0001 C CNN "Supplier"
F 7 "9332154" V 13110 4100 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mc0063w060354k7/res-thick-film-4k7-5-0-063w-0603/dp/9332154" V 13135 4100 20  0001 C CNN "Supplier URL"
F 9 "0.024" V 13160 4100 20  0001 C CNN "Supplier Price"
F 10 "1" V 13185 4100 20  0001 C CNN "Supplier Price Break"
	1    12850 4250
	1    0    0    1   
$EndComp
Wire Wire Line
	12850 3800 12850 3950
Wire Wire Line
	12850 4250 12850 4450
Connection ~ 12850 4450
Wire Wire Line
	12850 4450 14150 4450
$Comp
L appli_capacitor:1n_X7R_1608M C?
U 1 1 5ADB3DC4
P 2550 2150
F 0 "C?" H 2628 2085 50  0000 L CNN
F 1 "1n_X7R_1608M" H 2628 2005 35  0000 L CNN
F 2 "CAPC1608*" V 2665 2050 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1679506.pdf" V 2690 2050 20  0001 C CNN
F 4 "AVX" V 2740 2050 20  0001 C CNN "manf"
F 5 "06031C102KAT2A" V 2765 2050 20  0001 C CNN "manf#"
F 6 "Element14" V 2790 2050 20  0001 C CNN "Supplier"
F 7 "1833874" V 2815 2050 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/avx/06031c102kat2a/cap-mlcc-x7r-1nf-100v-0603/dp/1833874?ost=1833874" V 2840 2050 20  0001 C CNN "Supplier URL"
F 9 "0.004" V 2865 2050 20  0001 C CNN "Supplier Price"
F 10 "10" V 2890 2050 20  0001 C CNN "Supplier Price Break"
	1    2550 2150
	1    0    0    -1  
$EndComp
$Comp
L appli_capacitor:1n_X7R_1608M C?
U 1 1 5ADB433B
P 2550 3200
F 0 "C?" H 2628 3135 50  0000 L CNN
F 1 "1n_X7R_1608M" H 2628 3055 35  0000 L CNN
F 2 "CAPC1608*" V 2665 3100 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1679506.pdf" V 2690 3100 20  0001 C CNN
F 4 "AVX" V 2740 3100 20  0001 C CNN "manf"
F 5 "06031C102KAT2A" V 2765 3100 20  0001 C CNN "manf#"
F 6 "Element14" V 2790 3100 20  0001 C CNN "Supplier"
F 7 "1833874" V 2815 3100 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/avx/06031c102kat2a/cap-mlcc-x7r-1nf-100v-0603/dp/1833874?ost=1833874" V 2840 3100 20  0001 C CNN "Supplier URL"
F 9 "0.004" V 2865 3100 20  0001 C CNN "Supplier Price"
F 10 "10" V 2890 3100 20  0001 C CNN "Supplier Price Break"
	1    2550 3200
	1    0    0    -1  
$EndComp
$Comp
L appli_capacitor:1n_X7R_1608M C?
U 1 1 5ADB43F1
P 2550 4250
F 0 "C?" H 2628 4185 50  0000 L CNN
F 1 "1n_X7R_1608M" H 2628 4105 35  0000 L CNN
F 2 "CAPC1608*" V 2665 4150 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1679506.pdf" V 2690 4150 20  0001 C CNN
F 4 "AVX" V 2740 4150 20  0001 C CNN "manf"
F 5 "06031C102KAT2A" V 2765 4150 20  0001 C CNN "manf#"
F 6 "Element14" V 2790 4150 20  0001 C CNN "Supplier"
F 7 "1833874" V 2815 4150 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/avx/06031c102kat2a/cap-mlcc-x7r-1nf-100v-0603/dp/1833874?ost=1833874" V 2840 4150 20  0001 C CNN "Supplier URL"
F 9 "0.004" V 2865 4150 20  0001 C CNN "Supplier Price"
F 10 "10" V 2890 4150 20  0001 C CNN "Supplier Price Break"
	1    2550 4250
	1    0    0    -1  
$EndComp
$Comp
L appli_capacitor:1n_X7R_1608M C?
U 1 1 5ADB44B5
P 2550 5300
F 0 "C?" H 2628 5235 50  0000 L CNN
F 1 "1n_X7R_1608M" H 2628 5155 35  0000 L CNN
F 2 "CAPC1608*" V 2665 5200 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1679506.pdf" V 2690 5200 20  0001 C CNN
F 4 "AVX" V 2740 5200 20  0001 C CNN "manf"
F 5 "06031C102KAT2A" V 2765 5200 20  0001 C CNN "manf#"
F 6 "Element14" V 2790 5200 20  0001 C CNN "Supplier"
F 7 "1833874" V 2815 5200 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/avx/06031c102kat2a/cap-mlcc-x7r-1nf-100v-0603/dp/1833874?ost=1833874" V 2840 5200 20  0001 C CNN "Supplier URL"
F 9 "0.004" V 2865 5200 20  0001 C CNN "Supplier Price"
F 10 "10" V 2890 5200 20  0001 C CNN "Supplier Price Break"
	1    2550 5300
	1    0    0    -1  
$EndComp
$Comp
L appli_capacitor:1n_X7R_1608M C?
U 1 1 5ADB4748
P 2550 6350
F 0 "C?" H 2628 6285 50  0000 L CNN
F 1 "1n_X7R_1608M" H 2628 6205 35  0000 L CNN
F 2 "CAPC1608*" V 2665 6250 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1679506.pdf" V 2690 6250 20  0001 C CNN
F 4 "AVX" V 2740 6250 20  0001 C CNN "manf"
F 5 "06031C102KAT2A" V 2765 6250 20  0001 C CNN "manf#"
F 6 "Element14" V 2790 6250 20  0001 C CNN "Supplier"
F 7 "1833874" V 2815 6250 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/avx/06031c102kat2a/cap-mlcc-x7r-1nf-100v-0603/dp/1833874?ost=1833874" V 2840 6250 20  0001 C CNN "Supplier URL"
F 9 "0.004" V 2865 6250 20  0001 C CNN "Supplier Price"
F 10 "10" V 2890 6250 20  0001 C CNN "Supplier Price Break"
	1    2550 6350
	1    0    0    -1  
$EndComp
$Comp
L appli_capacitor:1n_X7R_1608M C?
U 1 1 5ADB499A
P 5850 5300
F 0 "C?" H 5928 5235 50  0000 L CNN
F 1 "1n_X7R_1608M" H 5928 5155 35  0000 L CNN
F 2 "CAPC1608*" V 5965 5200 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1679506.pdf" V 5990 5200 20  0001 C CNN
F 4 "AVX" V 6040 5200 20  0001 C CNN "manf"
F 5 "06031C102KAT2A" V 6065 5200 20  0001 C CNN "manf#"
F 6 "Element14" V 6090 5200 20  0001 C CNN "Supplier"
F 7 "1833874" V 6115 5200 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/avx/06031c102kat2a/cap-mlcc-x7r-1nf-100v-0603/dp/1833874?ost=1833874" V 6140 5200 20  0001 C CNN "Supplier URL"
F 9 "0.004" V 6165 5200 20  0001 C CNN "Supplier Price"
F 10 "10" V 6190 5200 20  0001 C CNN "Supplier Price Break"
	1    5850 5300
	1    0    0    -1  
$EndComp
$Comp
L appli_capacitor:1n_X7R_1608M C?
U 1 1 5ADB4FDB
P 5850 4250
F 0 "C?" H 5928 4185 50  0000 L CNN
F 1 "1n_X7R_1608M" H 5928 4105 35  0000 L CNN
F 2 "CAPC1608*" V 5965 4150 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1679506.pdf" V 5990 4150 20  0001 C CNN
F 4 "AVX" V 6040 4150 20  0001 C CNN "manf"
F 5 "06031C102KAT2A" V 6065 4150 20  0001 C CNN "manf#"
F 6 "Element14" V 6090 4150 20  0001 C CNN "Supplier"
F 7 "1833874" V 6115 4150 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/avx/06031c102kat2a/cap-mlcc-x7r-1nf-100v-0603/dp/1833874?ost=1833874" V 6140 4150 20  0001 C CNN "Supplier URL"
F 9 "0.004" V 6165 4150 20  0001 C CNN "Supplier Price"
F 10 "10" V 6190 4150 20  0001 C CNN "Supplier Price Break"
	1    5850 4250
	1    0    0    -1  
$EndComp
$Comp
L appli_capacitor:1n_X7R_1608M C?
U 1 1 5ADB508F
P 5850 3200
F 0 "C?" H 5928 3135 50  0000 L CNN
F 1 "1n_X7R_1608M" H 5928 3055 35  0000 L CNN
F 2 "CAPC1608*" V 5965 3100 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1679506.pdf" V 5990 3100 20  0001 C CNN
F 4 "AVX" V 6040 3100 20  0001 C CNN "manf"
F 5 "06031C102KAT2A" V 6065 3100 20  0001 C CNN "manf#"
F 6 "Element14" V 6090 3100 20  0001 C CNN "Supplier"
F 7 "1833874" V 6115 3100 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/avx/06031c102kat2a/cap-mlcc-x7r-1nf-100v-0603/dp/1833874?ost=1833874" V 6140 3100 20  0001 C CNN "Supplier URL"
F 9 "0.004" V 6165 3100 20  0001 C CNN "Supplier Price"
F 10 "10" V 6190 3100 20  0001 C CNN "Supplier Price Break"
	1    5850 3200
	1    0    0    -1  
$EndComp
$Comp
L appli_capacitor:1n_X7R_1608M C?
U 1 1 5ADB531E
P 5850 2150
F 0 "C?" H 5928 2085 50  0000 L CNN
F 1 "1n_X7R_1608M" H 5928 2005 35  0000 L CNN
F 2 "CAPC1608*" V 5965 2050 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1679506.pdf" V 5990 2050 20  0001 C CNN
F 4 "AVX" V 6040 2050 20  0001 C CNN "manf"
F 5 "06031C102KAT2A" V 6065 2050 20  0001 C CNN "manf#"
F 6 "Element14" V 6090 2050 20  0001 C CNN "Supplier"
F 7 "1833874" V 6115 2050 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/avx/06031c102kat2a/cap-mlcc-x7r-1nf-100v-0603/dp/1833874?ost=1833874" V 6140 2050 20  0001 C CNN "Supplier URL"
F 9 "0.004" V 6165 2050 20  0001 C CNN "Supplier Price"
F 10 "10" V 6190 2050 20  0001 C CNN "Supplier Price Break"
	1    5850 2150
	1    0    0    -1  
$EndComp
$Comp
L appli_resistor:10R_1608M R?
U 1 1 5ADB697D
P 5700 4200
F 0 "R?" V 5512 4050 50  0000 C CNN
F 1 "10R_1608M" V 5592 4050 35  0000 C CNN
F 2 "RESC1608*" V 5810 4050 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 5835 4050 20  0001 C CNN
F 4 "MULTICOMP" V 5885 4050 20  0001 C CNN "manf"
F 5 "MCHP03W8F100JT5E" V 5910 4050 20  0001 C CNN "manf#"
F 6 "Element14" V 5935 4050 20  0001 C CNN "Supplier"
F 7 "1576250" V 5960 4050 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mchp03w8f100jt5e/res-thick-film-10r-1-0-125w-0603/dp/1576250" V 5985 4050 20  0001 C CNN "Supplier URL"
F 9 "0.025" V 6010 4050 20  0001 C CNN "Supplier Price"
F 10 "1" V 6035 4050 20  0001 C CNN "Supplier Price Break"
	1    5700 4200
	0    1    1    0   
$EndComp
$Comp
L appli_resistor:10R_1608M R?
U 1 1 5ADB6C2D
P 5700 5250
F 0 "R?" V 5512 5100 50  0000 C CNN
F 1 "10R_1608M" V 5592 5100 35  0000 C CNN
F 2 "RESC1608*" V 5810 5100 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 5835 5100 20  0001 C CNN
F 4 "MULTICOMP" V 5885 5100 20  0001 C CNN "manf"
F 5 "MCHP03W8F100JT5E" V 5910 5100 20  0001 C CNN "manf#"
F 6 "Element14" V 5935 5100 20  0001 C CNN "Supplier"
F 7 "1576250" V 5960 5100 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mchp03w8f100jt5e/res-thick-film-10r-1-0-125w-0603/dp/1576250" V 5985 5100 20  0001 C CNN "Supplier URL"
F 9 "0.025" V 6010 5100 20  0001 C CNN "Supplier Price"
F 10 "1" V 6035 5100 20  0001 C CNN "Supplier Price Break"
	1    5700 5250
	0    1    1    0   
$EndComp
$Comp
L appli_resistor:100R_1608M R?
U 1 1 5ADB7F7E
P 5700 3150
F 0 "R?" V 5512 3000 50  0000 C CNN
F 1 "100R_1608M" V 5592 3000 35  0000 C CNN
F 2 "RESC1608*" V 5810 3000 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1788326.pdf" V 5835 3000 20  0001 C CNN
F 4 "MULTICOMP" V 5885 3000 20  0001 C CNN "manf"
F 5 "MC0063W06035100R" V 5910 3000 20  0001 C CNN "manf#"
F 6 "Element14" V 5935 3000 20  0001 C CNN "Supplier"
F 7 "9331689" V 5960 3000 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mc0063w06035100r/product-range-mc-series/dp/9331689" V 5985 3000 20  0001 C CNN "Supplier URL"
F 9 "0.022" V 6010 3000 20  0001 C CNN "Supplier Price"
F 10 "50" V 6035 3000 20  0001 C CNN "Supplier Price Break"
	1    5700 3150
	0    1    1    0   
$EndComp
$Comp
L appli_resistor:100R_1608M R?
U 1 1 5ADB81E7
P 5700 2100
F 0 "R?" V 5512 1950 50  0000 C CNN
F 1 "100R_1608M" V 5592 1950 35  0000 C CNN
F 2 "RESC1608*" V 5810 1950 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1788326.pdf" V 5835 1950 20  0001 C CNN
F 4 "MULTICOMP" V 5885 1950 20  0001 C CNN "manf"
F 5 "MC0063W06035100R" V 5910 1950 20  0001 C CNN "manf#"
F 6 "Element14" V 5935 1950 20  0001 C CNN "Supplier"
F 7 "9331689" V 5960 1950 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mc0063w06035100r/product-range-mc-series/dp/9331689" V 5985 1950 20  0001 C CNN "Supplier URL"
F 9 "0.022" V 6010 1950 20  0001 C CNN "Supplier Price"
F 10 "50" V 6035 1950 20  0001 C CNN "Supplier Price Break"
	1    5700 2100
	0    1    1    0   
$EndComp
$Comp
L appli_capacitor:100n_X7R_1608M C?
U 1 1 5ADB8912
P 9600 2550
F 0 "C?" H 9522 2415 50  0000 R CNN
F 1 "100n_X7R_1608M" H 9522 2495 35  0000 R CNN
F 2 "CAPC1608*" V 9715 2450 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1732728.pdf" V 9740 2450 20  0001 C CNN
F 4 "KEMET" V 9790 2450 20  0001 C CNN "manf"
F 5 "C0603C104K5RACAUTO" V 9815 2450 20  0001 C CNN "manf#"
F 6 "Element14" V 9840 2450 20  0001 C CNN "Supplier"
F 7 "2070398" V 9865 2450 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c0603c104k5racauto/cap-mlcc-x7r-100nf-50v-0603/dp/2070398" V 9890 2450 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 9915 2450 20  0001 C CNN "Supplier Price"
F 10 "1" V 9940 2450 20  0001 C CNN "Supplier Price Break"
	1    9600 2550
	1    0    0    1   
$EndComp
$Comp
L appli_capacitor:100n_X7R_1608M C?
U 1 1 5ADB9376
P 11600 2550
F 0 "C?" H 11522 2415 50  0000 R CNN
F 1 "100n_X7R_1608M" H 11522 2495 35  0000 R CNN
F 2 "CAPC1608*" V 11715 2450 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1732728.pdf" V 11740 2450 20  0001 C CNN
F 4 "KEMET" V 11790 2450 20  0001 C CNN "manf"
F 5 "C0603C104K5RACAUTO" V 11815 2450 20  0001 C CNN "manf#"
F 6 "Element14" V 11840 2450 20  0001 C CNN "Supplier"
F 7 "2070398" V 11865 2450 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c0603c104k5racauto/cap-mlcc-x7r-100nf-50v-0603/dp/2070398" V 11890 2450 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 11915 2450 20  0001 C CNN "Supplier Price"
F 10 "1" V 11940 2450 20  0001 C CNN "Supplier Price Break"
	1    11600 2550
	1    0    0    1   
$EndComp
$Comp
L appli_capacitor:100n_X7R_1608M C?
U 1 1 5ADB9430
P 13800 2550
F 0 "C?" H 13722 2415 50  0000 R CNN
F 1 "100n_X7R_1608M" H 13722 2495 35  0000 R CNN
F 2 "CAPC1608*" V 13915 2450 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1732728.pdf" V 13940 2450 20  0001 C CNN
F 4 "KEMET" V 13990 2450 20  0001 C CNN "manf"
F 5 "C0603C104K5RACAUTO" V 14015 2450 20  0001 C CNN "manf#"
F 6 "Element14" V 14040 2450 20  0001 C CNN "Supplier"
F 7 "2070398" V 14065 2450 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c0603c104k5racauto/cap-mlcc-x7r-100nf-50v-0603/dp/2070398" V 14090 2450 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 14115 2450 20  0001 C CNN "Supplier Price"
F 10 "1" V 14140 2450 20  0001 C CNN "Supplier Price Break"
	1    13800 2550
	1    0    0    1   
$EndComp
$Comp
L appli_capacitor:100n_X7R_1608M C?
U 1 1 5ADB98F0
P 13925 5500
F 0 "C?" H 13847 5365 50  0000 R CNN
F 1 "100n_X7R_1608M" H 13847 5445 35  0000 R CNN
F 2 "CAPC1608*" V 14040 5400 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1732728.pdf" V 14065 5400 20  0001 C CNN
F 4 "KEMET" V 14115 5400 20  0001 C CNN "manf"
F 5 "C0603C104K5RACAUTO" V 14140 5400 20  0001 C CNN "manf#"
F 6 "Element14" V 14165 5400 20  0001 C CNN "Supplier"
F 7 "2070398" V 14190 5400 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c0603c104k5racauto/cap-mlcc-x7r-100nf-50v-0603/dp/2070398" V 14215 5400 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 14240 5400 20  0001 C CNN "Supplier Price"
F 10 "1" V 14265 5400 20  0001 C CNN "Supplier Price Break"
	1    13925 5500
	-1   0    0    1   
$EndComp
$Comp
L appli_capacitor:22u_X7R_3225M C?
U 1 1 5ADB9CC8
P 13700 5500
F 0 "C?" H 13623 5365 50  0000 R CNN
F 1 "22u_X7R_3225M" H 13623 5445 35  0000 R CNN
F 2 "CAPC3225*" V 13815 5400 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2111999.pdf?_ga=1.157172477.774814437.1442284889" V 13840 5400 20  0001 C CNN
F 4 "muRata" V 13890 5400 20  0001 C CNN "manf"
F 5 "C1210C226K3RACTU" V 13915 5400 20  0001 C CNN "manf#"
F 6 "Element14" V 13940 5400 20  0001 C CNN "Supplier"
F 7 "2473550" V 13965 5400 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c1210c226k3ractu/cap-mlcc-x7r-22uf-25v-1210/dp/2473550" V 13990 5400 20  0001 C CNN "Supplier URL"
F 9 "1.02" V 14015 5400 20  0001 C CNN "Supplier Price"
F 10 "50" V 14040 5400 20  0001 C CNN "Supplier Price Break"
	1    13700 5500
	1    0    0    1   
$EndComp
$Comp
L appli_capacitor:100n_X7R_1608M C?
U 1 1 5ADBB9EC
P 4575 10075
F 0 "C?" H 4497 9940 50  0000 R CNN
F 1 "100n_X7R_1608M" H 4497 10020 35  0000 R CNN
F 2 "CAPC1608*" V 4690 9975 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1732728.pdf" V 4715 9975 20  0001 C CNN
F 4 "KEMET" V 4765 9975 20  0001 C CNN "manf"
F 5 "C0603C104K5RACAUTO" V 4790 9975 20  0001 C CNN "manf#"
F 6 "Element14" V 4815 9975 20  0001 C CNN "Supplier"
F 7 "2070398" V 4840 9975 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c0603c104k5racauto/cap-mlcc-x7r-100nf-50v-0603/dp/2070398" V 4865 9975 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 4890 9975 20  0001 C CNN "Supplier Price"
F 10 "1" V 4915 9975 20  0001 C CNN "Supplier Price Break"
	1    4575 10075
	-1   0    0    1   
$EndComp
$Comp
L appli_fuse:0.5A_1210L_PTC F?
U 1 1 5ADD9F16
P 13950 4250
F 0 "F?" V 13762 4100 50  0000 C CNN
F 1 "0.5A_1210L_PTC" V 13842 4100 35  0000 C CNN
F 2 "Applidyne_Fuse:FUSC3225X85L50N" V 14060 4100 20  0001 C CNN
F 3 "http://docs-asia.electrocomponents.com/webdocs/1420/0900766b8142082b.pdf" V 14085 4100 20  0001 C CNN
F 4 "LittelFuse" V 14135 4100 20  0001 C CNN "manf"
F 5 " 1210L050YR" V 14160 4100 20  0001 C CNN "manf#"
F 6 "RS Online" V 14185 4100 20  0001 C CNN "Supplier"
F 7 "787-4202" V 14210 4100 20  0001 C CNN "Supplier Part No"
F 8 "http://au.rs-online.com/web/p/resettable-surface-mount-fuses/7874202/" V 14235 4100 20  0001 C CNN "Supplier URL"
F 9 "0.486" V 14260 4100 20  0001 C CNN "Supplier Price"
F 10 "10" V 14285 4100 20  0001 C CNN "Supplier Price Break"
	1    13950 4250
	0    1    1    0   
$EndComp
$Comp
L appli_power:+5V #PWR?
U 1 1 5B4D5FB5
P 7150 6350
F 0 "#PWR?" H 7150 6200 50  0001 C CNN
F 1 "+5V" V 7165 6478 50  0000 L CNN
F 2 "" H 7150 6350 60  0000 C CNN
F 3 "" H 7150 6350 60  0000 C CNN
	1    7150 6350
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6700 4300 6300 4300
Wire Wire Line
	5850 3150 6300 3150
Wire Wire Line
	6300 4100 6300 3150
Wire Wire Line
	6650 3750 6650 3900
Wire Wire Line
	6700 4100 6300 4100
Wire Wire Line
	6700 4000 6400 4000
Wire Wire Line
	6650 3900 6700 3900
$Comp
L appli_connector_jst:JST_GH_6_RA P?
U 1 1 5AD83FAD
P 6900 4150
F 0 "P?" H 6978 4191 50  0000 L CNN
F 1 "JST_GH_6_RA" H 6978 4100 50  0000 L CNN
F 2 "Applidyne_Connector_JST:SM06B-GHS-TB" H 6900 2950 50  0001 C CNN
F 3 "http://docs-asia.electrocomponents.com/webdocs/10ce/0900766b810cef18.pdf" H 6900 3220 50  0001 C CNN
F 4 "JST" H 6910 3740 60  0001 C CNN "manf"
F 5 "SM06B-GHS-TB(LF)(SN)" H 6900 3650 60  0001 C CNN "manf#"
F 6 "RS Online" H 6910 3540 60  0001 C CNN "Supplier"
F 7 "752-1800" H 6910 3440 60  0001 C CNN "Supplier Part No"
F 8 "http://au.rs-online.com/web/p/pcb-headers/7521800/" H 6900 3340 60  0001 C CNN "Supplier URL"
F 9 "0.802" H 6900 3130 60  0001 C CNN "Supplier Price"
F 10 "5" H 6900 3040 60  0001 C CNN "Supplier Price Break"
	1    6900 4150
	1    0    0    -1  
$EndComp
$Comp
L appli_connector_wurth:CONN_01X06 P?
U 1 1 5B5370D4
P 9800 9850
F 0 "P?" H 9878 9891 50  0000 L CNN
F 1 "CONN_01X06" H 9878 9800 50  0000 L CNN
F 2 "Applidyne_Connector_Wurth:61300611121" V 9975 9850 50  0001 C CNN
F 3 "http://www.farnell.com/cad/2124416.pdf" H 9800 9500 50  0001 C CNN
F 4 "WURTH" H 9800 9400 60  0001 C CNN "manf"
F 5 "61300611121" H 9800 9300 60  0001 C CNN "manf#"
F 6 "Element 14" H 9800 9200 60  0001 C CNN "Supplier"
F 7 "2356158" H 9800 9125 60  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/wurth-elektronik/61300611121/header-2-54mm-pin-tht-vertical/dp/2356158" H 9800 9025 60  0001 C CNN "Supplier URL"
F 9 "0.17" H 9800 8925 60  0001 C CNN "Supplier Price"
F 10 "1" H 9800 8825 60  0001 C CNN "Supplier Price Break"
	1    9800 9850
	1    0    0    -1  
$EndComp
$Comp
L appli_connector_wurth:CONN_01X06 P?
U 1 1 5B58B350
P 7875 6400
F 0 "P?" H 7953 6441 50  0000 L CNN
F 1 "CONN_01X06" H 7953 6350 50  0000 L CNN
F 2 "Applidyne_Connector_Wurth:61300611121" V 8050 6400 50  0001 C CNN
F 3 "http://www.farnell.com/cad/2124416.pdf" H 7875 6050 50  0001 C CNN
F 4 "WURTH" H 7875 5950 60  0001 C CNN "manf"
F 5 "61300611121" H 7875 5850 60  0001 C CNN "manf#"
F 6 "Element 14" H 7875 5750 60  0001 C CNN "Supplier"
F 7 "2356158" H 7875 5675 60  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/wurth-elektronik/61300611121/header-2-54mm-pin-tht-vertical/dp/2356158" H 7875 5575 60  0001 C CNN "Supplier URL"
F 9 "0.17" H 7875 5475 60  0001 C CNN "Supplier Price"
F 10 "1" H 7875 5375 60  0001 C CNN "Supplier Price Break"
	1    7875 6400
	1    0    0    -1  
$EndComp
Wire Wire Line
	7675 6150 7150 6150
Wire Wire Line
	7675 6250 7450 6250
Wire Wire Line
	7450 6250 7450 6450
Wire Wire Line
	7675 6450 7450 6450
Connection ~ 7450 6450
Wire Wire Line
	7450 6450 7450 6650
Wire Wire Line
	7675 6650 7450 6650
Connection ~ 7450 6650
Wire Wire Line
	7450 6650 7450 6750
Wire Wire Line
	7675 6350 7150 6350
Wire Wire Line
	7675 6550 7150 6550
Text Notes 7150 5825 0    79   ~ 16
Power Outputs
$Comp
L appli_power:+5V #PWR?
U 1 1 5B665B8C
P 12850 3800
F 0 "#PWR?" H 12850 3650 50  0001 C CNN
F 1 "+5V" H 12865 3973 50  0000 C CNN
F 2 "" H 12850 3800 60  0000 C CNN
F 3 "" H 12850 3800 60  0000 C CNN
	1    12850 3800
	1    0    0    -1  
$EndComp
$EndSCHEMATC
