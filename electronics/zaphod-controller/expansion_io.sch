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
Text Notes 3450 7075 0    79   ~ 16
External Isolated Expansion IO
Text Notes 11825 3450 0    79   ~ 16
Fan Control Circuit
Text Notes 11725 1225 0    79   ~ 16
Temperature Sensors
Text Notes 11275 1525 0    50   ~ 0
Onboard PSU Temp
Text Notes 9375 1525 0    50   ~ 0
On-board temp sensor
Text Notes 9850 7175 0    79   ~ 16
Add-in Card
$Comp
L appli_resistor:NTC_10K_1608M TH1
U 1 1 5ABC263A
P 9850 1900
F 0 "TH1" H 9948 1785 50  0000 L CNN
F 1 "NTC_10K_1608M" H 9948 1705 35  0000 L CNN
F 2 "Applidyne_Resistor:RESC1608X50N" V 10000 1760 20  0001 C CNN
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
	11850 2200 11850 2250
Wire Wire Line
	11850 1800 11850 1900
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
L appli_transistor:BC847B Q5
U 1 1 5AD0E4D5
P 12500 5250
F 0 "Q5" H 12695 5288 40  0000 L CNN
F 1 "BC847B" H 12695 5212 40  0000 L CNN
F 2 "Applidyne_SOT:SOT95P240X110-3L16N" H 12425 5350 29  0001 R CNN
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
L appli_resistor:1K00_1608M R60
U 1 1 5AD0E62C
P 11400 5250
F 0 "R60" V 11212 5100 50  0000 C CNN
F 1 "1K00_1608M" V 11292 5100 35  0000 C CNN
F 2 "Applidyne_Resistor:RESC1608X50N" V 11510 5100 20  0001 C CNN
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
Wire Wire Line
	14150 4150 14050 4150
Wire Wire Line
	14050 4150 14050 4575
$Comp
L appli_device:BAV99 D23
U 1 1 5AD10A17
P 11600 4350
F 0 "D23" H 11708 4396 50  0000 L CNN
F 1 "BAV99" H 11708 4305 50  0000 L CNN
F 2 "Applidyne_SOT:SOT95P240X110-3L16N" H 11800 4200 20  0001 C CNN
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
L appli_resistor:4K7_1608M R66
U 1 1 5AD1ED64
P 13350 4250
F 0 "R66" H 13277 4065 50  0000 R CNN
F 1 "4K7_1608M" H 13277 4145 35  0000 R CNN
F 2 "Applidyne_Resistor:RESC1608X50N" V 13460 4100 20  0001 C CNN
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
L appli_resistor:10K0_1608M R61
U 1 1 5AD365D7
P 12050 4425
F 0 "R61" H 12123 4310 50  0000 L CNN
F 1 "10K0_1608M" H 12123 4230 35  0000 L CNN
F 2 "Applidyne_Resistor:RESC1608X50N" V 12160 4275 20  0001 C CNN
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
L appli_resistor:33K2_1608M R64
U 1 1 5AD500A2
P 12450 4350
F 0 "R64" V 12262 4200 50  0000 C CNN
F 1 "33K2_1608M" V 12342 4200 35  0000 C CNN
F 2 "Applidyne_Resistor:RESC1608X50N" V 12560 4200 20  0001 C CNN
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
$Comp
L appli_connector_wurth:CONN_01X02 P18
U 1 1 5AC12FC5
P 14350 2050
F 0 "P18" H 14428 2091 50  0000 L CNN
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
Text GLabel 1400 3150 0    50   Input ~ 0
AUX_PWM_1
Text GLabel 1400 4200 0    50   Input ~ 0
AUX_PWM_2
Text GLabel 1400 2100 0    50   Input ~ 0
AUX_PWM_0
Text GLabel 13375 8725 0    50   Output ~ 0
BUZZER_PWM
Text GLabel 4700 2100 0    50   Output ~ 0
USART_1_TX
Text GLabel 4700 3150 0    50   Input ~ 0
USART_1_RX
Text GLabel 11450 9100 0    50   Output ~ 0
USART_2_TX
Text GLabel 11450 9200 0    50   Input ~ 0
USART_2_RX
Text GLabel 11450 9000 0    50   Output ~ 0
USART_2_RTS
Text GLabel 11450 8900 0    50   Input ~ 0
USART_2_CTS
Text GLabel 9200 8800 0    50   Output ~ 0
USART_2_PWR_EN
Text Notes 14300 7425 0    79   ~ 16
Buzzer
Text Notes 13575 7650 0    50   ~ 0
Used for potential indication of state/warnings
Text GLabel 1425 10300 0    50   Output ~ 0
EXT_INPUT_0
Text GLabel 1450 10100 0    50   Input ~ 0
EXT_OUTPUT_1
Text GLabel 1425 10500 0    50   Output ~ 0
EXT_INPUT_1
Text GLabel 1450 9900 0    50   Input ~ 0
EXT_OUTPUT_0
Text Notes 4650 7350 0    50   ~ 0
Provide 2 optocoupled input/output pairs.\nPair 0 is UART capable, Pair 1 is CAN capable.
Text Notes 4650 7600 0    50   ~ 0
220R at 3.3V from micro gives 9mA for strong safe activation.\n5mA min, 8-10mA recommended.
$Comp
L appli_power:+12V #PWR091
U 1 1 5AD29E22
P 8300 3800
F 0 "#PWR091" H 8300 3650 50  0001 C CNN
F 1 "+12V" V 8300 4050 50  0000 C CNN
F 2 "" H 8300 3800 60  0000 C CNN
F 3 "" H 8300 3800 60  0000 C CNN
	1    8300 3800
	0    -1   -1   0   
$EndComp
Text GLabel 1400 5250 0    50   BiDi ~ 0
AUX_ANALOG_0
Text GLabel 1400 6300 0    50   BiDi ~ 0
AUX_ANALOG_1
Text Notes 1225 1450 0    50   ~ 0
To be used with LED drivers, servos or motors/pumps.\n\nAnalog AUX channels are ADC and DAC capable.\nPWM channels are assigned to non-conflicting timers.\nAll AUX IO can be used as simple input/output pins.
$Comp
L appli_device:BAV99 D15
U 1 1 5AD4CE46
P 1650 3150
F 0 "D15" H 1758 3196 50  0000 L CNN
F 1 "BAV99" H 1758 3105 50  0000 L CNN
F 2 "Applidyne_SOT:SOT95P240X110-3L16N" H 1850 3000 20  0001 C CNN
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
L appli_power:+3.3V #PWR055
U 1 1 5AD4D1B6
P 1650 2850
F 0 "#PWR055" H 1650 2810 30  0001 C CNN
F 1 "+3.3V" H 1659 2988 30  0000 C CNN
F 2 "" H 1650 2850 60  0000 C CNN
F 3 "" H 1650 2850 60  0000 C CNN
	1    1650 2850
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR056
U 1 1 5AD4D21B
P 1650 3450
F 0 "#PWR056" H 1650 3200 50  0001 C CNN
F 1 "GND" H 1655 3277 50  0000 C CNN
F 2 "" H 1650 3450 60  0000 C CNN
F 3 "" H 1650 3450 60  0000 C CNN
	1    1650 3450
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR065
U 1 1 5AD4D4DF
P 2550 3450
F 0 "#PWR065" H 2550 3200 50  0001 C CNN
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
L appli_power:+3.3V #PWR089
U 1 1 5AD69BC0
P 6650 3750
F 0 "#PWR089" H 6650 3710 30  0001 C CNN
F 1 "+3.3V" H 6659 3888 30  0000 C CNN
F 2 "" H 6650 3750 60  0000 C CNN
F 3 "" H 6650 3750 60  0000 C CNN
	1    6650 3750
	1    0    0    -1  
$EndComp
$Comp
L appli_device:BAV99 D16
U 1 1 5AD69E9C
P 1650 4200
F 0 "D16" H 1758 4246 50  0000 L CNN
F 1 "BAV99" H 1758 4155 50  0000 L CNN
F 2 "Applidyne_SOT:SOT95P240X110-3L16N" H 1850 4050 20  0001 C CNN
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
L appli_power:+3.3V #PWR057
U 1 1 5AD69EB1
P 1650 3900
F 0 "#PWR057" H 1650 3860 30  0001 C CNN
F 1 "+3.3V" H 1659 4038 30  0000 C CNN
F 2 "" H 1650 3900 60  0000 C CNN
F 3 "" H 1650 3900 60  0000 C CNN
	1    1650 3900
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR058
U 1 1 5AD69EB7
P 1650 4500
F 0 "#PWR058" H 1650 4250 50  0001 C CNN
F 1 "GND" H 1655 4327 50  0000 C CNN
F 2 "" H 1650 4500 60  0000 C CNN
F 3 "" H 1650 4500 60  0000 C CNN
	1    1650 4500
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR066
U 1 1 5AD69ECB
P 2550 4500
F 0 "#PWR066" H 2550 4250 50  0001 C CNN
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
L appli_device:BAV99 D17
U 1 1 5AD6D427
P 1650 5250
F 0 "D17" H 1758 5296 50  0000 L CNN
F 1 "BAV99" H 1758 5205 50  0000 L CNN
F 2 "Applidyne_SOT:SOT95P240X110-3L16N" H 1850 5100 20  0001 C CNN
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
L appli_power:+3.3V #PWR059
U 1 1 5AD6D43C
P 1650 4950
F 0 "#PWR059" H 1650 4910 30  0001 C CNN
F 1 "+3.3V" H 1659 5088 30  0000 C CNN
F 2 "" H 1650 4950 60  0000 C CNN
F 3 "" H 1650 4950 60  0000 C CNN
	1    1650 4950
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR060
U 1 1 5AD6D442
P 1650 5550
F 0 "#PWR060" H 1650 5300 50  0001 C CNN
F 1 "GND" H 1655 5377 50  0000 C CNN
F 2 "" H 1650 5550 60  0000 C CNN
F 3 "" H 1650 5550 60  0000 C CNN
	1    1650 5550
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR067
U 1 1 5AD6D456
P 2550 5550
F 0 "#PWR067" H 2550 5300 50  0001 C CNN
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
L appli_device:BAV99 D18
U 1 1 5AD6D46D
P 1650 6300
F 0 "D18" H 1758 6346 50  0000 L CNN
F 1 "BAV99" H 1758 6255 50  0000 L CNN
F 2 "Applidyne_SOT:SOT95P240X110-3L16N" H 1850 6150 20  0001 C CNN
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
L appli_power:+3.3V #PWR061
U 1 1 5AD6D482
P 1650 6000
F 0 "#PWR061" H 1650 5960 30  0001 C CNN
F 1 "+3.3V" H 1659 6138 30  0000 C CNN
F 2 "" H 1650 6000 60  0000 C CNN
F 3 "" H 1650 6000 60  0000 C CNN
	1    1650 6000
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR062
U 1 1 5AD6D488
P 1650 6600
F 0 "#PWR062" H 1650 6350 50  0001 C CNN
F 1 "GND" H 1655 6427 50  0000 C CNN
F 2 "" H 1650 6600 60  0000 C CNN
F 3 "" H 1650 6600 60  0000 C CNN
	1    1650 6600
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR068
U 1 1 5AD6D49C
P 2550 6600
F 0 "#PWR068" H 2550 6350 50  0001 C CNN
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
Text GLabel 4700 5250 0    50   BiDi ~ 0
I2C_SDA
Text GLabel 4700 4200 0    50   BiDi ~ 0
I2C_SCK
$Comp
L appli_device:BAV99 D14
U 1 1 5ADBA82D
P 1650 2100
F 0 "D14" H 1758 2146 50  0000 L CNN
F 1 "BAV99" H 1758 2055 50  0000 L CNN
F 2 "Applidyne_SOT:SOT95P240X110-3L16N" H 1850 1950 20  0001 C CNN
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
L appli_power:+3.3V #PWR053
U 1 1 5ADBA842
P 1650 1800
F 0 "#PWR053" H 1650 1760 30  0001 C CNN
F 1 "+3.3V" H 1659 1938 30  0000 C CNN
F 2 "" H 1650 1800 60  0000 C CNN
F 3 "" H 1650 1800 60  0000 C CNN
	1    1650 1800
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR054
U 1 1 5ADBA848
P 1650 2400
F 0 "#PWR054" H 1650 2150 50  0001 C CNN
F 1 "GND" H 1655 2227 50  0000 C CNN
F 2 "" H 1650 2400 60  0000 C CNN
F 3 "" H 1650 2400 60  0000 C CNN
	1    1650 2400
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR064
U 1 1 5ADBA85C
P 2550 2400
F 0 "#PWR064" H 2550 2150 50  0001 C CNN
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
	3100 4100 3100 3150
Wire Wire Line
	2550 3150 3100 3150
Wire Wire Line
	3200 4000 3200 2100
Wire Wire Line
	2550 2100 3200 2100
Wire Wire Line
	2550 4200 3500 4200
Wire Wire Line
	3100 4500 3100 5250
Wire Wire Line
	2550 5250 3100 5250
Wire Wire Line
	3200 4600 3200 6300
Wire Wire Line
	2550 6300 3200 6300
Text Notes 4725 875  0    79   ~ 16
Internal Expansion Serial
Text Notes 4600 1400 0    50   ~ 0
I2C and Serial Expansion for internal or end effector use.\nAllows for sensors, or daughterboards etc.\nI2C pullup are provided on this board.
Wire Wire Line
	6700 4400 6650 4400
Wire Wire Line
	6650 4400 6650 4550
$Comp
L appli_power:GND #PWR090
U 1 1 5AE0D3F7
P 6650 4550
F 0 "#PWR090" H 6650 4300 50  0001 C CNN
F 1 "GND" H 6655 4377 50  0000 C CNN
F 2 "" H 6650 4550 60  0000 C CNN
F 3 "" H 6650 4550 60  0000 C CNN
	1    6650 4550
	1    0    0    -1  
$EndComp
$Comp
L appli_device:BAV99 D19
U 1 1 5AE3D819
P 4950 2100
F 0 "D19" H 5058 2146 50  0000 L CNN
F 1 "BAV99" H 5058 2055 50  0000 L CNN
F 2 "Applidyne_SOT:SOT95P240X110-3L16N" H 5150 1950 20  0001 C CNN
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
L appli_power:+3.3V #PWR074
U 1 1 5AE3D82E
P 4950 1800
F 0 "#PWR074" H 4950 1760 30  0001 C CNN
F 1 "+3.3V" H 4959 1938 30  0000 C CNN
F 2 "" H 4950 1800 60  0000 C CNN
F 3 "" H 4950 1800 60  0000 C CNN
	1    4950 1800
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR075
U 1 1 5AE3D834
P 4950 2400
F 0 "#PWR075" H 4950 2150 50  0001 C CNN
F 1 "GND" H 4955 2227 50  0000 C CNN
F 2 "" H 4950 2400 60  0000 C CNN
F 3 "" H 4950 2400 60  0000 C CNN
	1    4950 2400
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR085
U 1 1 5AE3D848
P 5850 2400
F 0 "#PWR085" H 5850 2150 50  0001 C CNN
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
L appli_device:BAV99 D20
U 1 1 5AE3D85E
P 4950 3150
F 0 "D20" H 5058 3196 50  0000 L CNN
F 1 "BAV99" H 5058 3105 50  0000 L CNN
F 2 "Applidyne_SOT:SOT95P240X110-3L16N" H 5150 3000 20  0001 C CNN
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
L appli_power:+3.3V #PWR076
U 1 1 5AE3D873
P 4950 2850
F 0 "#PWR076" H 4950 2810 30  0001 C CNN
F 1 "+3.3V" H 4959 2988 30  0000 C CNN
F 2 "" H 4950 2850 60  0000 C CNN
F 3 "" H 4950 2850 60  0000 C CNN
	1    4950 2850
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR077
U 1 1 5AE3D879
P 4950 3450
F 0 "#PWR077" H 4950 3200 50  0001 C CNN
F 1 "GND" H 4955 3277 50  0000 C CNN
F 2 "" H 4950 3450 60  0000 C CNN
F 3 "" H 4950 3450 60  0000 C CNN
	1    4950 3450
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR086
U 1 1 5AE3D88D
P 5850 3450
F 0 "#PWR086" H 5850 3200 50  0001 C CNN
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
L appli_device:BAV99 D21
U 1 1 5AE3D8A3
P 4950 4200
F 0 "D21" H 5058 4246 50  0000 L CNN
F 1 "BAV99" H 5058 4155 50  0000 L CNN
F 2 "Applidyne_SOT:SOT95P240X110-3L16N" H 5150 4050 20  0001 C CNN
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
L appli_power:+3.3V #PWR078
U 1 1 5AE3D8B8
P 4950 3900
F 0 "#PWR078" H 4950 3860 30  0001 C CNN
F 1 "+3.3V" H 4959 4038 30  0000 C CNN
F 2 "" H 4950 3900 60  0000 C CNN
F 3 "" H 4950 3900 60  0000 C CNN
	1    4950 3900
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR079
U 1 1 5AE3D8BE
P 4950 4500
F 0 "#PWR079" H 4950 4250 50  0001 C CNN
F 1 "GND" H 4955 4327 50  0000 C CNN
F 2 "" H 4950 4500 60  0000 C CNN
F 3 "" H 4950 4500 60  0000 C CNN
	1    4950 4500
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR087
U 1 1 5AE3D8D2
P 5850 4500
F 0 "#PWR087" H 5850 4250 50  0001 C CNN
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
L appli_device:BAV99 D22
U 1 1 5AE3D8E8
P 4950 5250
F 0 "D22" H 5058 5296 50  0000 L CNN
F 1 "BAV99" H 5058 5205 50  0000 L CNN
F 2 "Applidyne_SOT:SOT95P240X110-3L16N" H 5150 5100 20  0001 C CNN
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
L appli_power:+3.3V #PWR080
U 1 1 5AE3D8FD
P 4950 4950
F 0 "#PWR080" H 4950 4910 30  0001 C CNN
F 1 "+3.3V" H 4959 5088 30  0000 C CNN
F 2 "" H 4950 4950 60  0000 C CNN
F 3 "" H 4950 4950 60  0000 C CNN
	1    4950 4950
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR081
U 1 1 5AE3D903
P 4950 5550
F 0 "#PWR081" H 4950 5300 50  0001 C CNN
F 1 "GND" H 4955 5377 50  0000 C CNN
F 2 "" H 4950 5550 60  0000 C CNN
F 3 "" H 4950 5550 60  0000 C CNN
	1    4950 5550
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR088
U 1 1 5AE3D917
P 5850 5550
F 0 "#PWR088" H 5850 5300 50  0001 C CNN
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
	6500 4000 6500 2100
Wire Wire Line
	5850 2100 6500 2100
Wire Wire Line
	5850 4200 6700 4200
Wire Wire Line
	6400 4300 6400 5250
Wire Wire Line
	5850 5250 6400 5250
Wire Wire Line
	4950 5250 4700 5250
Connection ~ 4950 5250
$Comp
L appli_power:+3.3V #PWR093
U 1 1 5AF06D31
P 8300 4000
F 0 "#PWR093" H 8300 3960 30  0001 C CNN
F 1 "+3.3V" V 8300 4225 30  0000 C CNN
F 2 "" H 8300 4000 60  0000 C CNN
F 3 "" H 8300 4000 60  0000 C CNN
	1    8300 4000
	0    -1   -1   0   
$EndComp
Text Notes 9425 7475 0    50   ~ 0
Expansion Header for ESP8266/32 or BLE module.\nProvide USART with external power control.
$Comp
L appli_transistor:2N7002P Q6
U 1 1 5AF3D9B7
P 14275 8675
F 0 "Q6" H 14466 8721 50  0000 L CNN
F 1 "2N7002P" H 14466 8630 50  0000 L CNN
F 2 "Applidyne_SOT:SOT95P240X110-3L16N" H 14475 8575 50  0001 L CIN
F 3 "http://docs-asia.electrocomponents.com/webdocs/1385/0900766b813856ab.pdf" H 12645 8405 50  0001 L CNN
F 4 "Fairchild" H 14275 8175 60  0001 C CNN "manf"
F 5 " 2N7002,215" H 14275 8055 60  0001 C CNN "manf#"
F 6 "RS Online" H 14275 7955 60  0001 C CNN "Supplier"
F 7 "http://au.rs-online.com/web/p/mosfet-transistors/4367379/" H 14275 7835 60  0001 C CNN "Supplier URL"
F 8 " 436-7379" H 14275 7705 60  0001 C CNN "Supplier Part No"
F 9 "0.232" H 14275 7585 60  0001 C CNN "Supplier Price"
F 10 "25" H 14275 7455 60  0001 C CNN "Supplier Price Break"
	1    14275 8675
	1    0    0    -1  
$EndComp
$Comp
L appli_resistor:10R_1608M R62
U 1 1 5AF3DC0C
P 13775 8725
F 0 "R62" V 13587 8575 50  0000 C CNN
F 1 "10R_1608M" V 13667 8575 35  0000 C CNN
F 2 "Applidyne_Resistor:RESC1608X50N" V 13885 8575 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 13910 8575 20  0001 C CNN
F 4 "MULTICOMP" V 13960 8575 20  0001 C CNN "manf"
F 5 "MCHP03W8F100JT5E" V 13985 8575 20  0001 C CNN "manf#"
F 6 "Element14" V 14010 8575 20  0001 C CNN "Supplier"
F 7 "1576250" V 14035 8575 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mchp03w8f100jt5e/res-thick-film-10r-1-0-125w-0603/dp/1576250" V 14060 8575 20  0001 C CNN "Supplier URL"
F 9 "0.025" V 14085 8575 20  0001 C CNN "Supplier Price"
F 10 "1" V 14110 8575 20  0001 C CNN "Supplier Price Break"
	1    13775 8725
	0    1    1    0   
$EndComp
$Comp
L appli_resistor:10K0_1608M R63
U 1 1 5AF3E125
P 13875 8825
F 0 "R63" H 13948 8710 50  0000 L CNN
F 1 "10K0_1608M" H 13948 8630 35  0000 L CNN
F 2 "Applidyne_Resistor:RESC1608X50N" V 13985 8675 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 14010 8675 20  0001 C CNN
F 4 "TE CONNECTIVITY" V 14060 8675 20  0001 C CNN "manf"
F 5 "CRGH0603F10K" V 14085 8675 20  0001 C CNN "manf#"
F 6 "Element14" V 14110 8675 20  0001 C CNN "Supplier"
F 7 "2332016" V 14135 8675 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/te-connectivity/crgh0603f10k/resistor-power-10k-0-2w-1-0603/dp/2332016" V 14160 8675 20  0001 C CNN "Supplier URL"
F 9 "0.02" V 14185 8675 20  0001 C CNN "Supplier Price"
F 10 "10" V 14210 8675 20  0001 C CNN "Supplier Price Break"
	1    13875 8825
	1    0    0    -1  
$EndComp
Wire Wire Line
	13875 9125 13875 9225
Wire Wire Line
	13875 9225 14375 9225
Wire Wire Line
	14375 9225 14375 8875
Wire Wire Line
	14075 8725 13875 8725
Wire Wire Line
	13475 8725 13375 8725
Wire Wire Line
	13875 8825 13875 8725
Connection ~ 13875 8725
Wire Wire Line
	13875 8725 13775 8725
Wire Wire Line
	14375 9225 14375 9275
Connection ~ 14375 9225
Wire Wire Line
	14375 8475 14375 8325
$Comp
L appli_power:GND #PWR099
U 1 1 5AF786FF
P 14375 9275
F 0 "#PWR099" H 14375 9025 50  0001 C CNN
F 1 "GND" H 14380 9102 50  0000 C CNN
F 2 "" H 14375 9275 60  0000 C CNN
F 3 "" H 14375 9275 60  0000 C CNN
	1    14375 9275
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+12V #PWR098
U 1 1 5AF8C7A8
P 14375 8050
F 0 "#PWR098" H 14375 7900 50  0001 C CNN
F 1 "+12V" H 14390 8223 50  0000 C CNN
F 2 "" H 14375 8050 60  0000 C CNN
F 3 "" H 14375 8050 60  0000 C CNN
	1    14375 8050
	1    0    0    -1  
$EndComp
Wire Wire Line
	14375 8225 14375 8050
Wire Wire Line
	14375 8225 14475 8225
Wire Wire Line
	14475 8325 14375 8325
$Comp
L appli_transistor:2N7002P Q3
U 1 1 5B0B1DE7
P 10250 8750
F 0 "Q3" H 10441 8796 50  0000 L CNN
F 1 "2N7002P" H 10441 8705 50  0000 L CNN
F 2 "Applidyne_SOT:SOT95P240X110-3L16N" H 10450 8650 50  0001 L CIN
F 3 "http://docs-asia.electrocomponents.com/webdocs/1385/0900766b813856ab.pdf" H 8620 8480 50  0001 L CNN
F 4 "Fairchild" H 10250 8250 60  0001 C CNN "manf"
F 5 " 2N7002,215" H 10250 8130 60  0001 C CNN "manf#"
F 6 "RS Online" H 10250 8030 60  0001 C CNN "Supplier"
F 7 "http://au.rs-online.com/web/p/mosfet-transistors/4367379/" H 10250 7910 60  0001 C CNN "Supplier URL"
F 8 " 436-7379" H 10250 7780 60  0001 C CNN "Supplier Part No"
F 9 "0.232" H 10250 7660 60  0001 C CNN "Supplier Price"
F 10 "25" H 10250 7530 60  0001 C CNN "Supplier Price Break"
	1    10250 8750
	1    0    0    -1  
$EndComp
$Comp
L appli_resistor:10R_1608M R54
U 1 1 5B0BD8B8
P 9650 8800
F 0 "R54" V 9462 8650 50  0000 C CNN
F 1 "10R_1608M" V 9542 8650 35  0000 C CNN
F 2 "Applidyne_Resistor:RESC1608X50N" V 9760 8650 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 9785 8650 20  0001 C CNN
F 4 "MULTICOMP" V 9835 8650 20  0001 C CNN "manf"
F 5 "MCHP03W8F100JT5E" V 9860 8650 20  0001 C CNN "manf#"
F 6 "Element14" V 9885 8650 20  0001 C CNN "Supplier"
F 7 "1576250" V 9910 8650 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mchp03w8f100jt5e/res-thick-film-10r-1-0-125w-0603/dp/1576250" V 9935 8650 20  0001 C CNN "Supplier URL"
F 9 "0.025" V 9960 8650 20  0001 C CNN "Supplier Price"
F 10 "1" V 9985 8650 20  0001 C CNN "Supplier Price Break"
	1    9650 8800
	0    1    1    0   
$EndComp
$Comp
L appli_resistor:10K0_1608M R55
U 1 1 5B0BD968
P 9850 8900
F 0 "R55" H 9923 8785 50  0000 L CNN
F 1 "10K0_1608M" H 9923 8705 35  0000 L CNN
F 2 "Applidyne_Resistor:RESC1608X50N" V 9960 8750 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 9985 8750 20  0001 C CNN
F 4 "TE CONNECTIVITY" V 10035 8750 20  0001 C CNN "manf"
F 5 "CRGH0603F10K" V 10060 8750 20  0001 C CNN "manf#"
F 6 "Element14" V 10085 8750 20  0001 C CNN "Supplier"
F 7 "2332016" V 10110 8750 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/te-connectivity/crgh0603f10k/resistor-power-10k-0-2w-1-0603/dp/2332016" V 10135 8750 20  0001 C CNN "Supplier URL"
F 9 "0.02" V 10160 8750 20  0001 C CNN "Supplier Price"
F 10 "10" V 10185 8750 20  0001 C CNN "Supplier Price Break"
	1    9850 8900
	1    0    0    -1  
$EndComp
$Comp
L appli_transistor:NTR1P02T1G Q4
U 1 1 5B0C8ADE
P 10900 8400
F 0 "Q4" H 11091 8446 50  0000 L CNN
F 1 "NTR1P02T1G" H 11091 8355 50  0000 L CNN
F 2 "Applidyne_SOT:SOT95P240X110-3L16N" H 11148 8300 50  0001 L CIN
F 3 "http://www.farnell.com/datasheets/1708311.pdf" H 9270 8130 50  0001 L CNN
F 4 "On Semi" H 10900 7900 60  0001 C CNN "manf"
F 5 "NTR1P02T1G" H 10900 7780 60  0001 C CNN "manf#"
F 6 "Element14" H 10900 7680 60  0001 C CNN "Supplier"
F 7 "http://au.element14.com/on-semiconductor/ntr1p02t1g/mosfet-p-ch-20v-1a-sot-23-3/dp/2317617" H 10900 7560 60  0001 C CNN "Supplier URL"
F 8 "2317617" H 10900 7430 60  0001 C CNN "Supplier Part No"
F 9 "0.53" H 10900 7310 60  0001 C CNN "Supplier Price"
F 10 "1" H 10900 7180 60  0001 C CNN "Supplier Price Break"
	1    10900 8400
	1    0    0    -1  
$EndComp
$Comp
L appli_resistor:10K0_1608M R56
U 1 1 5B0C8C44
P 10350 8050
F 0 "R56" H 10423 7935 50  0000 L CNN
F 1 "10K0_1608M" H 10423 7855 35  0000 L CNN
F 2 "Applidyne_Resistor:RESC1608X50N" V 10460 7900 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 10485 7900 20  0001 C CNN
F 4 "TE CONNECTIVITY" V 10535 7900 20  0001 C CNN "manf"
F 5 "CRGH0603F10K" V 10560 7900 20  0001 C CNN "manf#"
F 6 "Element14" V 10585 7900 20  0001 C CNN "Supplier"
F 7 "2332016" V 10610 7900 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/te-connectivity/crgh0603f10k/resistor-power-10k-0-2w-1-0603/dp/2332016" V 10635 7900 20  0001 C CNN "Supplier URL"
F 9 "0.02" V 10660 7900 20  0001 C CNN "Supplier Price"
F 10 "10" V 10685 7900 20  0001 C CNN "Supplier Price Break"
	1    10350 8050
	1    0    0    -1  
$EndComp
Wire Wire Line
	10350 8550 10350 8450
Wire Wire Line
	10700 8450 10350 8450
Connection ~ 10350 8450
Wire Wire Line
	10350 8450 10350 8350
Wire Wire Line
	10050 8800 9850 8800
Wire Wire Line
	9850 8900 9850 8800
Connection ~ 9850 8800
Wire Wire Line
	9850 8800 9650 8800
Wire Wire Line
	9850 9200 9850 9300
Wire Wire Line
	9850 9300 10350 9300
Wire Wire Line
	10350 9300 10350 8950
Wire Wire Line
	10350 9300 10350 9400
Connection ~ 10350 9300
Wire Wire Line
	9350 8800 9200 8800
Wire Wire Line
	10350 8050 10350 7950
Wire Wire Line
	10350 7950 11000 7950
Wire Wire Line
	11000 7950 11000 8200
Wire Wire Line
	11000 8600 11000 8800
$Comp
L appli_power:GND #PWR095
U 1 1 5B1370F8
P 10350 9400
F 0 "#PWR095" H 10350 9150 50  0001 C CNN
F 1 "GND" H 10355 9227 50  0000 C CNN
F 2 "" H 10350 9400 60  0000 C CNN
F 3 "" H 10350 9400 60  0000 C CNN
	1    10350 9400
	1    0    0    -1  
$EndComp
Wire Wire Line
	11000 8800 11600 8800
$Comp
L appli_power:+3.3V #PWR096
U 1 1 5B15AAFC
P 11000 7900
F 0 "#PWR096" H 11000 7860 30  0001 C CNN
F 1 "+3.3V" H 11009 8038 30  0000 C CNN
F 2 "" H 11000 7900 60  0000 C CNN
F 3 "" H 11000 7900 60  0000 C CNN
	1    11000 7900
	1    0    0    -1  
$EndComp
Wire Wire Line
	11000 7900 11000 7950
Connection ~ 11000 7950
$Comp
L appli_power:GND #PWR097
U 1 1 5B17E8CF
P 11450 9400
F 0 "#PWR097" H 11450 9150 50  0001 C CNN
F 1 "GND" H 11455 9227 50  0000 C CNN
F 2 "" H 11450 9400 60  0000 C CNN
F 3 "" H 11450 9400 60  0000 C CNN
	1    11450 9400
	1    0    0    -1  
$EndComp
Wire Wire Line
	11450 9400 11450 9300
Wire Wire Line
	11450 9300 11600 9300
Wire Wire Line
	11450 9200 11600 9200
Wire Wire Line
	11450 9100 11600 9100
Wire Wire Line
	11450 9000 11600 9000
Wire Wire Line
	11450 8900 11600 8900
Text Notes 12275 7125 0    118  ~ 24
TODO find correct buzzer\n
Text Notes 11150 8125 0    50   ~ 0
Rated to 1A
$Comp
L appli_resistor:4K7_1608M R65
U 1 1 5B32017C
P 12850 4250
F 0 "R65" H 12777 4065 50  0000 R CNN
F 1 "4K7_1608M" H 12777 4145 35  0000 R CNN
F 2 "Applidyne_Resistor:RESC1608X50N" V 12960 4100 20  0001 C CNN
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
L appli_capacitor:1n_X7R_1608M C60
U 1 1 5ADB3DC4
P 2550 2150
F 0 "C60" H 2628 2085 50  0000 L CNN
F 1 "1n_X7R_1608M" H 2628 2005 35  0000 L CNN
F 2 "Applidyne_Capacitor:CAPC1608X90N" V 2665 2050 20  0001 C CNN
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
L appli_capacitor:1n_X7R_1608M C61
U 1 1 5ADB433B
P 2550 3200
F 0 "C61" H 2628 3135 50  0000 L CNN
F 1 "1n_X7R_1608M" H 2628 3055 35  0000 L CNN
F 2 "Applidyne_Capacitor:CAPC1608X90N" V 2665 3100 20  0001 C CNN
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
L appli_capacitor:1n_X7R_1608M C62
U 1 1 5ADB43F1
P 2550 4250
F 0 "C62" H 2628 4185 50  0000 L CNN
F 1 "1n_X7R_1608M" H 2628 4105 35  0000 L CNN
F 2 "Applidyne_Capacitor:CAPC1608X90N" V 2665 4150 20  0001 C CNN
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
L appli_capacitor:1n_X7R_1608M C63
U 1 1 5ADB44B5
P 2550 5300
F 0 "C63" H 2628 5235 50  0000 L CNN
F 1 "1n_X7R_1608M" H 2628 5155 35  0000 L CNN
F 2 "Applidyne_Capacitor:CAPC1608X90N" V 2665 5200 20  0001 C CNN
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
L appli_capacitor:1n_X7R_1608M C64
U 1 1 5ADB4748
P 2550 6350
F 0 "C64" H 2628 6285 50  0000 L CNN
F 1 "1n_X7R_1608M" H 2628 6205 35  0000 L CNN
F 2 "Applidyne_Capacitor:CAPC1608X90N" V 2665 6250 20  0001 C CNN
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
L appli_capacitor:1n_X7R_1608M C69
U 1 1 5ADB499A
P 5850 5300
F 0 "C69" H 5928 5235 50  0000 L CNN
F 1 "1n_X7R_1608M" H 5928 5155 35  0000 L CNN
F 2 "Applidyne_Capacitor:CAPC1608X90N" V 5965 5200 20  0001 C CNN
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
L appli_capacitor:1n_X7R_1608M C68
U 1 1 5ADB4FDB
P 5850 4250
F 0 "C68" H 5928 4185 50  0000 L CNN
F 1 "1n_X7R_1608M" H 5928 4105 35  0000 L CNN
F 2 "Applidyne_Capacitor:CAPC1608X90N" V 5965 4150 20  0001 C CNN
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
L appli_capacitor:1n_X7R_1608M C67
U 1 1 5ADB508F
P 5850 3200
F 0 "C67" H 5928 3135 50  0000 L CNN
F 1 "1n_X7R_1608M" H 5928 3055 35  0000 L CNN
F 2 "Applidyne_Capacitor:CAPC1608X90N" V 5965 3100 20  0001 C CNN
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
L appli_capacitor:1n_X7R_1608M C66
U 1 1 5ADB531E
P 5850 2150
F 0 "C66" H 5928 2085 50  0000 L CNN
F 1 "1n_X7R_1608M" H 5928 2005 35  0000 L CNN
F 2 "Applidyne_Capacitor:CAPC1608X90N" V 5965 2050 20  0001 C CNN
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
L appli_resistor:100R_1608M R50
U 1 1 5ADB81E7
P 5700 2100
F 0 "R50" V 5512 1950 50  0000 C CNN
F 1 "100R_1608M" V 5592 1950 35  0000 C CNN
F 2 "Applidyne_Resistor:RESC1608X50N" V 5810 1950 20  0001 C CNN
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
L appli_capacitor:100n_X7R_1608M C70
U 1 1 5ADB8912
P 9600 2550
F 0 "C70" H 9522 2415 50  0000 R CNN
F 1 "100n_X7R_1608M" H 9522 2495 35  0000 R CNN
F 2 "Applidyne_Capacitor:CAPC1608X90N" V 9715 2450 20  0001 C CNN
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
L appli_capacitor:100n_X7R_1608M C71
U 1 1 5ADB9376
P 11600 2550
F 0 "C71" H 11522 2415 50  0000 R CNN
F 1 "100n_X7R_1608M" H 11522 2495 35  0000 R CNN
F 2 "Applidyne_Capacitor:CAPC1608X90N" V 11715 2450 20  0001 C CNN
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
L appli_capacitor:100n_X7R_1608M C73
U 1 1 5ADB9430
P 13800 2550
F 0 "C73" H 13722 2415 50  0000 R CNN
F 1 "100n_X7R_1608M" H 13722 2495 35  0000 R CNN
F 2 "Applidyne_Capacitor:CAPC1608X90N" V 13915 2450 20  0001 C CNN
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
L appli_capacitor:100n_X7R_1608M C74
U 1 1 5ADB98F0
P 13925 5500
F 0 "C74" H 13847 5365 50  0000 R CNN
F 1 "100n_X7R_1608M" H 13847 5445 35  0000 R CNN
F 2 "Applidyne_Capacitor:CAPC1608X90N" V 14040 5400 20  0001 C CNN
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
L appli_capacitor:22u_X7R_3225M C72
U 1 1 5ADB9CC8
P 13700 5500
F 0 "C72" H 13623 5365 50  0000 R CNN
F 1 "22u_X7R_3225M" H 13623 5445 35  0000 R CNN
F 2 "Applidyne_Capacitor:CAPC3225X170N" V 13815 5400 20  0001 C CNN
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
L appli_capacitor:100n_X7R_1608M C65
U 1 1 5ADBB9EC
P 4600 8825
F 0 "C65" V 4725 8800 50  0000 R CNN
F 1 "100n_X7R_1608M" V 4475 8950 35  0000 R CNN
F 2 "Applidyne_Capacitor:CAPC1608X90N" V 4715 8725 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1732728.pdf" V 4740 8725 20  0001 C CNN
F 4 "KEMET" V 4790 8725 20  0001 C CNN "manf"
F 5 "C0603C104K5RACAUTO" V 4815 8725 20  0001 C CNN "manf#"
F 6 "Element14" V 4840 8725 20  0001 C CNN "Supplier"
F 7 "2070398" V 4865 8725 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c0603c104k5racauto/cap-mlcc-x7r-100nf-50v-0603/dp/2070398" V 4890 8725 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 4915 8725 20  0001 C CNN "Supplier Price"
F 10 "1" V 4940 8725 20  0001 C CNN "Supplier Price Break"
	1    4600 8825
	-1   0    0    1   
$EndComp
$Comp
L appli_fuse:0.5A_1210L_PTC F2
U 1 1 5ADD9F16
P 13950 4250
F 0 "F2" V 13762 4100 50  0000 C CNN
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
L appli_power:+5V #PWR092
U 1 1 5B4D5FB5
P 8300 3900
F 0 "#PWR092" H 8300 3750 50  0001 C CNN
F 1 "+5V" V 8300 4050 50  0000 L CNN
F 2 "" H 8300 3900 60  0000 C CNN
F 3 "" H 8300 3900 60  0000 C CNN
	1    8300 3900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6700 4300 6400 4300
Wire Wire Line
	5850 3150 6400 3150
Wire Wire Line
	6400 4100 6400 3150
Wire Wire Line
	6650 3750 6650 3900
Wire Wire Line
	6700 4100 6400 4100
Wire Wire Line
	6700 4000 6500 4000
Wire Wire Line
	6650 3900 6700 3900
$Comp
L appli_connector_wurth:CONN_01X06 P20
U 1 1 5B5370D4
P 11800 9050
F 0 "P20" H 11878 9091 50  0000 L CNN
F 1 "CONN_01X06" H 11878 9000 50  0000 L CNN
F 2 "Applidyne_Connector_Wurth:61300611121" V 11975 9050 50  0001 C CNN
F 3 "http://www.farnell.com/cad/2124416.pdf" H 11800 8700 50  0001 C CNN
F 4 "WURTH" H 11800 8600 60  0001 C CNN "manf"
F 5 "61300611121" H 11800 8500 60  0001 C CNN "manf#"
F 6 "Element 14" H 11800 8400 60  0001 C CNN "Supplier"
F 7 "2356158" H 11800 8325 60  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/wurth-elektronik/61300611121/header-2-54mm-pin-tht-vertical/dp/2356158" H 11800 8225 60  0001 C CNN "Supplier URL"
F 9 "0.17" H 11800 8125 60  0001 C CNN "Supplier Price"
F 10 "1" H 11800 8025 60  0001 C CNN "Supplier Price Break"
	1    11800 9050
	1    0    0    -1  
$EndComp
Wire Wire Line
	8650 3800 8300 3800
Wire Wire Line
	8650 3900 8300 3900
Wire Wire Line
	8650 4000 8300 4000
Text Notes 8000 3525 0    79   ~ 16
Power Outputs
$Comp
L appli_power:+5V #PWR0100
U 1 1 5B665B8C
P 12850 3800
F 0 "#PWR0100" H 12850 3650 50  0001 C CNN
F 1 "+5V" H 12865 3973 50  0000 C CNN
F 2 "" H 12850 3800 60  0000 C CNN
F 3 "" H 12850 3800 60  0000 C CNN
	1    12850 3800
	1    0    0    -1  
$EndComp
$Comp
L appli_connector_molex:MOLEX_KK2.54_1x4_WAY_FAN P19
U 1 1 5AE7A351
P 14350 4300
F 0 "P19" H 14428 4330 50  0000 L CNN
F 1 "MOLEX_KK2.54_1x4_WAY_FAN" H 14428 4250 35  0000 L CNN
F 2 "Applidyne_Connector_Molex:MOLEX_47053-1000" V 14525 4300 20  0001 C CNN
F 3 "https://au.mouser.com/datasheet/2/276/0470531000_PCB_HEADERS-146351.pdf" V 14550 4300 20  0001 C CNN
F 4 "MOLEX" V 14600 4300 20  0001 C CNN "manf"
F 5 "47053-1000" V 14625 4300 20  0001 C CNN "manf#"
F 6 "Mouser" V 14650 4300 20  0001 C CNN "Supplier"
F 7 "538-47053-1000" V 14675 4300 20  0001 C CNN "Supplier Part No"
F 8 "https://au.mouser.com/productdetail/?qs=ph4zPCVRuvqoDX7hrEhxNA%3D%3D" V 14700 4300 20  0001 C CNN "Supplier URL"
F 9 "0.59" V 14725 4300 20  0001 C CNN "Supplier Price"
F 10 "1" V 14750 4300 20  0001 C CNN "Supplier Price Break"
	1    14350 4300
	1    0    0    -1  
$EndComp
$Comp
L appli_resistor:220R_QUAD_ARRAY_1608M R48
U 1 1 5AE7D204
P 2400 2100
F 0 "R48" V 2200 1950 50  0000 C CNN
F 1 "220R_QUAD_ARRAY_1608M" V 2280 1950 35  0000 C CNN
F 2 "Applidyne_Resistor:R_Array_Concave_4x1608M" V 2600 1965 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2085571.pdf" V 2625 1965 20  0001 C CNN
F 4 "PANASONIC" V 2675 1965 20  0001 C CNN "manf"
F 5 "EXB38V221JV" V 2700 1965 20  0001 C CNN "manf#"
F 6 "Element14" V 2725 1965 20  0001 C CNN "Supplier"
F 7 "2060094" V 2750 1965 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/panasonic-electronic-components/exb38v221jv/resistor-cvex-array-0603x4-220r/dp/2060094" V 2775 1965 20  0001 C CNN "Supplier URL"
F 9 "0.081" V 2800 1965 20  0001 C CNN "Supplier Price"
F 10 "1" V 2825 1965 20  0001 C CNN "Supplier Price Break"
	1    2400 2100
	0    1    1    0   
$EndComp
$Comp
L appli_resistor:220R_QUAD_ARRAY_1608M R48
U 2 1 5AE7DA01
P 2400 3150
F 0 "R48" V 2200 3000 50  0000 C CNN
F 1 "220R_QUAD_ARRAY_1608M" V 2280 3000 35  0000 C CNN
F 2 "Applidyne_Resistor:R_Array_Concave_4x1608M" V 2600 3015 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2085571.pdf" V 2625 3015 20  0001 C CNN
F 4 "PANASONIC" V 2675 3015 20  0001 C CNN "manf"
F 5 "EXB38V221JV" V 2700 3015 20  0001 C CNN "manf#"
F 6 "Element14" V 2725 3015 20  0001 C CNN "Supplier"
F 7 "2060094" V 2750 3015 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/panasonic-electronic-components/exb38v221jv/resistor-cvex-array-0603x4-220r/dp/2060094" V 2775 3015 20  0001 C CNN "Supplier URL"
F 9 "0.081" V 2800 3015 20  0001 C CNN "Supplier Price"
F 10 "1" V 2825 3015 20  0001 C CNN "Supplier Price Break"
	2    2400 3150
	0    1    1    0   
$EndComp
$Comp
L appli_resistor:220R_QUAD_ARRAY_1608M R48
U 3 1 5AE7DAB3
P 2400 4200
F 0 "R48" V 2200 4050 50  0000 C CNN
F 1 "220R_QUAD_ARRAY_1608M" V 2280 4050 35  0000 C CNN
F 2 "Applidyne_Resistor:R_Array_Concave_4x1608M" V 2600 4065 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2085571.pdf" V 2625 4065 20  0001 C CNN
F 4 "PANASONIC" V 2675 4065 20  0001 C CNN "manf"
F 5 "EXB38V221JV" V 2700 4065 20  0001 C CNN "manf#"
F 6 "Element14" V 2725 4065 20  0001 C CNN "Supplier"
F 7 "2060094" V 2750 4065 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/panasonic-electronic-components/exb38v221jv/resistor-cvex-array-0603x4-220r/dp/2060094" V 2775 4065 20  0001 C CNN "Supplier URL"
F 9 "0.081" V 2800 4065 20  0001 C CNN "Supplier Price"
F 10 "1" V 2825 4065 20  0001 C CNN "Supplier Price Break"
	3    2400 4200
	0    1    1    0   
$EndComp
$Comp
L appli_resistor:220R_QUAD_ARRAY_1608M R48
U 4 1 5AE7DB69
P 2400 5250
F 0 "R48" V 2200 5100 50  0000 C CNN
F 1 "220R_QUAD_ARRAY_1608M" V 2280 5100 35  0000 C CNN
F 2 "Applidyne_Resistor:R_Array_Concave_4x1608M" V 2600 5115 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2085571.pdf" V 2625 5115 20  0001 C CNN
F 4 "PANASONIC" V 2675 5115 20  0001 C CNN "manf"
F 5 "EXB38V221JV" V 2700 5115 20  0001 C CNN "manf#"
F 6 "Element14" V 2725 5115 20  0001 C CNN "Supplier"
F 7 "2060094" V 2750 5115 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/panasonic-electronic-components/exb38v221jv/resistor-cvex-array-0603x4-220r/dp/2060094" V 2775 5115 20  0001 C CNN "Supplier URL"
F 9 "0.081" V 2800 5115 20  0001 C CNN "Supplier Price"
F 10 "1" V 2825 5115 20  0001 C CNN "Supplier Price Break"
	4    2400 5250
	0    1    1    0   
$EndComp
$Comp
L appli_resistor:1K_QUAD_ARRAY_1608M R57
U 1 1 5AE830B2
P 9450 2250
F 0 "R57" V 9250 2100 50  0000 C CNN
F 1 "1K_QUAD_ARRAY_1608M" V 9330 2100 35  0000 C CNN
F 2 "Applidyne_Resistor:R_Array_Concave_4x1608M" V 9665 2100 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1885635.pdf" V 9690 2100 20  0001 C CNN
F 4 "BOURNS" V 9740 2100 20  0001 C CNN "manf"
F 5 "CAY16-102J4LF" V 9765 2100 20  0001 C CNN "manf#"
F 6 "Element14" V 9790 2100 20  0001 C CNN "Supplier"
F 7 "1770133" V 9815 2100 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/bourns/cay16-102j4lf/resistor-array-4-1k-1206-5-smd/dp/1770133?CMP=GRHB-OCTOPART" V 9840 2100 20  0001 C CNN "Supplier URL"
F 9 "0.073" V 9865 2100 20  0001 C CNN "Supplier Price"
F 10 "50" V 9890 2100 20  0001 C CNN "Supplier Price Break"
	1    9450 2250
	0    1    1    0   
$EndComp
$Comp
L appli_resistor:1K_QUAD_ARRAY_1608M R57
U 3 1 5AE831ED
P 13650 2250
F 0 "R57" V 13450 2100 50  0000 C CNN
F 1 "1K_QUAD_ARRAY_1608M" V 13530 2100 35  0000 C CNN
F 2 "Applidyne_Resistor:R_Array_Concave_4x1608M" V 13865 2100 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1885635.pdf" V 13890 2100 20  0001 C CNN
F 4 "BOURNS" V 13940 2100 20  0001 C CNN "manf"
F 5 "CAY16-102J4LF" V 13965 2100 20  0001 C CNN "manf#"
F 6 "Element14" V 13990 2100 20  0001 C CNN "Supplier"
F 7 "1770133" V 14015 2100 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/bourns/cay16-102j4lf/resistor-array-4-1k-1206-5-smd/dp/1770133?CMP=GRHB-OCTOPART" V 14040 2100 20  0001 C CNN "Supplier URL"
F 9 "0.073" V 14065 2100 20  0001 C CNN "Supplier Price"
F 10 "50" V 14090 2100 20  0001 C CNN "Supplier Price Break"
	3    13650 2250
	0    1    1    0   
$EndComp
$Comp
L appli_resistor:1K_QUAD_ARRAY_1608M R57
U 2 1 5AE83789
P 11450 2250
F 0 "R57" V 11250 2100 50  0000 C CNN
F 1 "1K_QUAD_ARRAY_1608M" V 11330 2100 35  0000 C CNN
F 2 "Applidyne_Resistor:R_Array_Concave_4x1608M" V 11665 2100 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1885635.pdf" V 11690 2100 20  0001 C CNN
F 4 "BOURNS" V 11740 2100 20  0001 C CNN "manf"
F 5 "CAY16-102J4LF" V 11765 2100 20  0001 C CNN "manf#"
F 6 "Element14" V 11790 2100 20  0001 C CNN "Supplier"
F 7 "1770133" V 11815 2100 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/bourns/cay16-102j4lf/resistor-array-4-1k-1206-5-smd/dp/1770133?CMP=GRHB-OCTOPART" V 11840 2100 20  0001 C CNN "Supplier URL"
F 9 "0.073" V 11865 2100 20  0001 C CNN "Supplier Price"
F 10 "50" V 11890 2100 20  0001 C CNN "Supplier Price Break"
	2    11450 2250
	0    1    1    0   
$EndComp
$Comp
L appli_resistor:100R_1608M R51
U 1 1 5ADB7F7E
P 5700 3150
F 0 "R51" V 5512 3000 50  0000 C CNN
F 1 "100R_1608M" V 5592 3000 35  0000 C CNN
F 2 "Applidyne_Resistor:RESC1608X50N" V 5810 3000 20  0001 C CNN
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
L appli_resistor:10R_1608M R52
U 1 1 5ADB697D
P 5700 4200
F 0 "R52" V 5512 4050 50  0000 C CNN
F 1 "10R_1608M" V 5592 4050 35  0000 C CNN
F 2 "Applidyne_Resistor:RESC1608X50N" V 5810 4050 20  0001 C CNN
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
L appli_resistor:10R_1608M R53
U 1 1 5ADB6C2D
P 5700 5250
F 0 "R53" V 5512 5100 50  0000 C CNN
F 1 "10R_1608M" V 5592 5100 35  0000 C CNN
F 2 "Applidyne_Resistor:RESC1608X50N" V 5810 5100 20  0001 C CNN
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
Connection ~ 5850 5250
Connection ~ 5850 2100
Connection ~ 5850 3150
$Comp
L appli_resistor:NTC_10K_1608M TH2
U 1 1 5ADB606F
P 11850 1900
F 0 "TH2" H 11948 1785 50  0000 L CNN
F 1 "NTC_10K_1608M" H 11948 1705 35  0000 L CNN
F 2 "Applidyne_Resistor:RESC1608X50N" V 12000 1760 20  0001 C CNN
F 3 "http://au.rs-online.com/webdocs/1423/0900766b81423c04.pdf" V 12025 1760 20  0001 C CNN
F 4 "MURATA" V 12075 1760 20  0001 C CNN "manf"
F 5 "NCP18XH103J03RB" V 12100 1770 20  0001 C CNN "manf#"
F 6 "RS Online" V 12125 1760 20  0001 C CNN "Supplier"
F 7 "619-7740" V 12150 1760 20  0001 C CNN "Supplier Part No"
F 8 "http://au.rs-online.com/web/p/thermistors/6197740/" V 12175 1760 20  0001 C CNN "Supplier URL"
F 9 "0.327" V 12200 1760 20  0001 C CNN "Supplier Price"
F 10 "25" V 12225 1760 20  0001 C CNN "Supplier Price Break"
	1    11850 1900
	1    0    0    -1  
$EndComp
$Comp
L appli_resistor:1K_QUAD_ARRAY_1608M R29
U 1 1 5B1D2AA5
P 11100 4350
F 0 "R29" V 10900 4100 50  0000 L CNN
F 1 "1K_QUAD_ARRAY_1608M" V 10975 3925 35  0000 L CNN
F 2 "Applidyne_Resistor:R_Array_Concave_4x1608M" V 11315 4200 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1885635.pdf" V 11340 4200 20  0001 C CNN
F 4 "BOURNS" V 11390 4200 20  0001 C CNN "manf"
F 5 "CAY16-102J4LF" V 11415 4200 20  0001 C CNN "manf#"
F 6 "Element14" V 11440 4200 20  0001 C CNN "Supplier"
F 7 "1770133" V 11465 4200 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/bourns/cay16-102j4lf/resistor-array-4-1k-1206-5-smd/dp/1770133?CMP=GRHB-OCTOPART" V 11490 4200 20  0001 C CNN "Supplier URL"
F 9 "0.073" V 11515 4200 20  0001 C CNN "Supplier Price"
F 10 "50" V 11540 4200 20  0001 C CNN "Supplier Price Break"
	1    11100 4350
	0    -1   1    0   
$EndComp
$Comp
L appli_resistor:10K0_1608M R4
U 1 1 5AE7B8D9
P 9850 2300
F 0 "R4" H 9923 2185 50  0000 L CNN
F 1 "10K0_1608M" H 9923 2105 35  0000 L CNN
F 2 "Applidyne_Resistor:RESC1608X50N" V 9960 2150 20  0001 C CNN
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
L appli_resistor:10K0_1608M R58
U 1 1 5AE7C23D
P 11850 2300
F 0 "R58" H 11923 2185 50  0000 L CNN
F 1 "10K0_1608M" H 11923 2105 35  0000 L CNN
F 2 "Applidyne_Resistor:RESC1608X50N" V 11960 2150 20  0001 C CNN
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
L appli_resistor:10K0_1608M R59
U 1 1 5AE7C88F
P 14050 2300
F 0 "R59" H 14123 2185 50  0000 L CNN
F 1 "10K0_1608M" H 14123 2105 35  0000 L CNN
F 2 "Applidyne_Resistor:RESC1608X50N" V 14160 2150 20  0001 C CNN
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
$Comp
L appli_connector_molex:MOLEX_NANOFIT_2x4_WAY P15
U 1 1 5AE9292E
P 3700 4250
F 0 "P15" H 3625 4725 50  0000 L CNN
F 1 "MOLEX_NANOFIT_2x4_WAY" V 3850 3950 35  0000 L CNN
F 2 "Applidyne_Connector_Molex:Molex_Nano-Fit_105310-xx08_2x04_VERT" V 3875 4250 20  0001 C CNN
F 3 "https://www.molex.com/molex/products/datasheet.jsp?part=active/0039281083_PCB_HEADERS.xml&channel=Products&Lang=en-US" V 3900 4250 20  0001 C CNN
F 4 "MOLEX" V 3950 4250 20  0001 C CNN "manf"
F 5 "105310-1108" V 3975 4250 20  0001 C CNN "manf#"
F 6 "Mouser" V 4000 4250 20  0001 C CNN "Supplier"
F 7 "538-105310-1108" V 4025 4250 20  0001 C CNN "Supplier Part No"
F 8 "https://au.mouser.com/ProductDetail/Molex/105310-1108" V 4050 4250 20  0001 C CNN "Supplier URL"
F 9 "2.26" V 4075 4250 20  0001 C CNN "Supplier Price"
F 10 "1" V 4100 4250 20  0001 C CNN "Supplier Price Break"
	1    3700 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 4000 3500 4000
Wire Wire Line
	3100 4100 3500 4100
Wire Wire Line
	3500 4300 3300 4300
Wire Wire Line
	3300 4300 3300 4700
Wire Wire Line
	3200 4600 3500 4600
Wire Wire Line
	3500 4500 3100 4500
Wire Wire Line
	3500 3900 3300 3900
Wire Wire Line
	3300 3900 3300 3800
$Comp
L appli_power:+12V #PWR0198
U 1 1 5AF6E25B
P 3300 3800
F 0 "#PWR0198" H 3300 3650 50  0001 C CNN
F 1 "+12V" V 3300 4050 50  0000 C CNN
F 2 "" H 3300 3800 60  0000 C CNN
F 3 "" H 3300 3800 60  0000 C CNN
	1    3300 3800
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR0199
U 1 1 5AF6E896
P 3300 4700
F 0 "#PWR0199" H 3300 4450 50  0001 C CNN
F 1 "GND" H 3305 4527 50  0000 C CNN
F 2 "" H 3300 4700 60  0000 C CNN
F 3 "" H 3300 4700 60  0000 C CNN
	1    3300 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	3500 4400 3450 4400
Wire Wire Line
	3450 4400 3450 3800
$Comp
L appli_power:+3.3V #PWR0200
U 1 1 5AF8BA97
P 3450 3800
F 0 "#PWR0200" H 3450 3760 30  0001 C CNN
F 1 "+3.3V" V 3450 3975 30  0000 C CNN
F 2 "" H 3450 3800 60  0000 C CNN
F 3 "" H 3450 3800 60  0000 C CNN
	1    3450 3800
	1    0    0    -1  
$EndComp
$Comp
L appli_connector_molex:MOLEX_NANOFIT_2x3_WAY P17
U 1 1 5AF939B2
P 8850 4050
F 0 "P17" H 8927 4080 50  0000 L CNN
F 1 "MOLEX_NANOFIT_2x3_WAY" H 8927 4000 35  0000 L CNN
F 2 "Applidyne_Connector_Molex:Molex_Nano-Fit_105310-xx06_2x03_VERT" V 9025 4050 20  0001 C CNN
F 3 "https://www.molex.com/molex/products/datasheet.jsp?part=active/1053101204_PCB_HEADERS.xml&channel=Products&Lang=en-US" V 9050 4050 20  0001 C CNN
F 4 "MOLEX" V 9100 4050 20  0001 C CNN "manf"
F 5 "105310-1204" V 9125 4050 20  0001 C CNN "manf#"
F 6 "Mouser" V 9150 4050 20  0001 C CNN "Supplier"
F 7 "538-105310-1106" V 9175 4050 20  0001 C CNN "Supplier Part No"
F 8 "https://au.mouser.com/ProductDetail/Molex/105310-1106" V 9200 4050 20  0001 C CNN "Supplier URL"
F 9 "1.81" V 9225 4050 20  0001 C CNN "Supplier Price"
F 10 "1" V 9250 4050 20  0001 C CNN "Supplier Price Break"
	1    8850 4050
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR094
U 1 1 5AF06DAE
P 8250 4400
F 0 "#PWR094" H 8250 4150 50  0001 C CNN
F 1 "GND" H 8255 4227 50  0000 C CNN
F 2 "" H 8250 4400 60  0000 C CNN
F 3 "" H 8250 4400 60  0000 C CNN
	1    8250 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	8250 4400 8250 4300
Wire Wire Line
	8250 4100 8650 4100
Wire Wire Line
	8650 4200 8250 4200
Connection ~ 8250 4200
Wire Wire Line
	8250 4200 8250 4100
Wire Wire Line
	8650 4300 8250 4300
Connection ~ 8250 4300
Wire Wire Line
	8250 4300 8250 4200
$Comp
L appli_connector_molex:MOLEX_NANOFIT_1x6_WAY P16
U 1 1 5B00C8A6
P 6900 4150
F 0 "P16" H 6977 4180 50  0000 L CNN
F 1 "MOLEX_NANOFIT_1x6_WAY" H 6977 4100 35  0000 L CNN
F 2 "Applidyne_Connector_Molex:Molex_Nano-Fit_105309-xx06_1x06_VERT" V 7075 4150 20  0001 C CNN
F 3 "https://www.molex.com/molex/products/datasheet.jsp?part=active/1053091106_PCB_HEADERS.xml" V 7100 4150 20  0001 C CNN
F 4 "MOLEX" V 7150 4150 20  0001 C CNN "manf"
F 5 "105313-1106" V 7175 4150 20  0001 C CNN "manf#"
F 6 "Mouser" V 7200 4150 20  0001 C CNN "Supplier"
F 7 "538-105309-1106" V 7225 4150 20  0001 C CNN "Supplier Part No"
F 8 "https://au.mouser.com/ProductDetail/Molex/105309-1106" V 7250 4150 20  0001 C CNN "Supplier URL"
F 9 "1.74" V 7275 4150 20  0001 C CNN "Supplier Price"
F 10 "1" V 7300 4150 20  0001 C CNN "Supplier Price Break"
	1    6900 4150
	1    0    0    -1  
$EndComp
$Comp
L appli_resistor:22R_1608M R49
U 1 1 5B01397A
P 2400 6300
F 0 "R49" V 2212 6150 50  0000 C CNN
F 1 "22R_1608M" V 2292 6150 35  0000 C CNN
F 2 "Applidyne_Resistor:RESC1608X50N" V 2510 6150 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1788326.pdf" V 2535 6150 20  0001 C CNN
F 4 "MULTICOMP" V 2585 6150 20  0001 C CNN "manf"
F 5 "MC0063W060314R22" V 2610 6150 20  0001 C CNN "manf#"
F 6 "Element14" V 2635 6150 20  0001 C CNN "Supplier"
F 7 "2141539" V 2660 6150 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mc0063w060314r22/res-thick-film-4r22-1-0-063w-0603/dp/2141539" V 2685 6150 20  0001 C CNN "Supplier URL"
F 9 "0.015" V 2710 6150 20  0001 C CNN "Supplier Price"
F 10 "1" V 2735 6150 20  0001 C CNN "Supplier Price Break"
	1    2400 6300
	0    1    1    0   
$EndComp
$Comp
L appli_connector_molex:MOLEX_NANOFIT_2x5_WAY P21
U 1 1 5B019139
P 6750 8550
F 0 "P21" H 6827 8480 50  0000 L CNN
F 1 "MOLEX_NANOFIT_2x5_WAY" H 6827 8400 35  0000 L CNN
F 2 "Applidyne_Connector_Molex:Molex_Nano-Fit_105310-xx10_2x05_VERT" V 6925 8550 20  0001 C CNN
F 3 "https://www.molex.com/molex/products/datasheet.jsp?part=active/1053101110_PCB_HEADERS.xml" V 6950 8550 20  0001 C CNN
F 4 "MOLEX" V 7000 8550 20  0001 C CNN "manf"
F 5 "105310-1110" V 7025 8550 20  0001 C CNN "manf#"
F 6 "Mouser" V 7050 8550 20  0001 C CNN "Supplier"
F 7 "538-105310-1110" V 7075 8550 20  0001 C CNN "Supplier Part No"
F 8 "https://au.mouser.com/ProductDetail/Molex/105310-1110" V 7100 8550 20  0001 C CNN "Supplier URL"
F 9 "2.65" V 7125 8550 20  0001 C CNN "Supplier Price"
F 10 "1" V 7150 8550 20  0001 C CNN "Supplier Price Break"
	1    6750 8550
	1    0    0    -1  
$EndComp
Text GLabel 3125 7225 0    50   Input ~ 0
LEMO_USB_VBUS
Text GLabel 5900 8700 0    50   Input ~ 0
LEMO_USB_ID
Text GLabel 1275 8400 0    50   Input ~ 0
USB_D+
Text GLabel 1275 8300 0    50   Input ~ 0
USB_D-
Wire Wire Line
	5350 8200 6550 8200
Wire Wire Line
	5900 8700 6550 8700
Wire Wire Line
	6550 8800 6300 8800
Wire Wire Line
	6300 8800 6300 8900
Wire Wire Line
	6550 8900 6300 8900
Connection ~ 6300 8900
Wire Wire Line
	6300 8900 6300 9250
$Comp
L appli_special:ADUM3160 U?
U 1 1 5AE099EB
P 3200 7950
F 0 "U?" H 3200 8487 60  0000 C CNN
F 1 "ADUM3160" H 3200 8381 60  0000 C CNN
F 2 "Applidyne_SOIC:SOIC127P1032X265-16L87N" H 3200 6700 60  0001 C CNN
F 3 "http://www.analog.com/media/en/technical-documentation/data-sheets/ADuM3160.pdf" H 3200 6825 60  0001 C CNN
F 4 "Analog Devices" H 3200 6585 60  0001 C CNN "manf"
F 5 "ADUM3160BRWZ" H 3210 6475 60  0001 C CNN "manf#"
F 6 "Mouser" H 3200 6365 60  0001 C CNN "Supplier"
F 7 "584-ADUM3160BRWZ-R" H 3200 6255 60  0001 C CNN "Supplier Part No"
F 8 "https://au.mouser.com/ProductDetail/Analog-Devices/ADUM3160BRWZ-RL" H 3200 6155 60  0001 C CNN "Supplier URL"
F 9 "9.47" H 3200 6035 60  0001 C CNN "Supplier Price"
F 10 "1" H 3210 5925 60  0001 C CNN "Supplier Price Break"
	1    3200 7950
	-1   0    0    -1  
$EndComp
Text Label 5850 8400 0    50   ~ 0
ISOLATED_D+
Text Label 5850 8300 0    50   ~ 0
ISOLATED_D-
Wire Wire Line
	1400 8300 1275 8300
Wire Wire Line
	1275 8400 1400 8400
$Comp
L appli_resistor:22R_1608M R30
U 1 1 5AE71DC9
P 1700 8300
F 0 "R30" V 1512 8150 50  0000 C CNN
F 1 "22R_1608M" V 1592 8150 35  0000 C CNN
F 2 "Applidyne_Resistor:RESC1608X50N" V 1810 8150 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1788326.pdf" V 1835 8150 20  0001 C CNN
F 4 "MULTICOMP" V 1885 8150 20  0001 C CNN "manf"
F 5 "MC0063W060314R22" V 1910 8150 20  0001 C CNN "manf#"
F 6 "Element14" V 1935 8150 20  0001 C CNN "Supplier"
F 7 "2141539" V 1960 8150 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mc0063w060314r22/res-thick-film-4r22-1-0-063w-0603/dp/2141539" V 1985 8150 20  0001 C CNN "Supplier URL"
F 9 "0.015" V 2010 8150 20  0001 C CNN "Supplier Price"
F 10 "1" V 2035 8150 20  0001 C CNN "Supplier Price Break"
	1    1700 8300
	0    1    1    0   
$EndComp
$Comp
L appli_resistor:22R_1608M R31
U 1 1 5AE71DD7
P 1700 8400
F 0 "R31" V 1800 8250 50  0000 C CNN
F 1 "22R_1608M" V 1875 8250 35  0000 C CNN
F 2 "Applidyne_Resistor:RESC1608X50N" V 1810 8250 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1788326.pdf" V 1835 8250 20  0001 C CNN
F 4 "MULTICOMP" V 1885 8250 20  0001 C CNN "manf"
F 5 "MC0063W060314R22" V 1910 8250 20  0001 C CNN "manf#"
F 6 "Element14" V 1935 8250 20  0001 C CNN "Supplier"
F 7 "2141539" V 1960 8250 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mc0063w060314r22/res-thick-film-4r22-1-0-063w-0603/dp/2141539" V 1985 8250 20  0001 C CNN "Supplier URL"
F 9 "0.015" V 2010 8250 20  0001 C CNN "Supplier Price"
F 10 "1" V 2035 8250 20  0001 C CNN "Supplier Price Break"
	1    1700 8400
	0    1    1    0   
$EndComp
Text GLabel 2025 10825 0    50   Output ~ 0
USB_OTG_ID
$Comp
L appli_resistor:100R_1608M R32
U 1 1 5AEA8133
P 2325 10825
F 0 "R32" V 2137 10675 50  0000 C CNN
F 1 "100R_1608M" V 2217 10675 35  0000 C CNN
F 2 "Applidyne_Resistor:RESC1608X50N" V 2435 10675 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1788326.pdf" V 2460 10675 20  0001 C CNN
F 4 "MULTICOMP" V 2510 10675 20  0001 C CNN "manf"
F 5 "MC0063W06035100R" V 2535 10675 20  0001 C CNN "manf#"
F 6 "Element14" V 2560 10675 20  0001 C CNN "Supplier"
F 7 "9331689" V 2585 10675 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mc0063w06035100r/product-range-mc-series/dp/9331689" V 2610 10675 20  0001 C CNN "Supplier URL"
F 9 "0.022" V 2635 10675 20  0001 C CNN "Supplier Price"
F 10 "50" V 2660 10675 20  0001 C CNN "Supplier Price Break"
	1    2325 10825
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2025 10825 2325 10825
Wire Wire Line
	3125 10825 2625 10825
Text Notes 3125 10675 2    50   ~ 0
Provides OTG sensing on the micro
Text Notes 3800 11050 0    50   ~ 0
OTG contexts aren't expected for this board.\nTherefore 5V supply not included on USBVDD.\nShifting responsibility to a potential OTG device to have a power supply provided\nwhich will likely require a custom cable with 5V regulator and socket.\nI can't forsee any HOST applications that would need isolation (wireless dongles mostly?).\nThis is slightly out of spec with normal OTG devices, but ID is only added for futureproofing.
Text GLabel 3125 10825 2    50   Input ~ 0
LEMO_USB_ID
Wire Wire Line
	1700 8300 2250 8300
Wire Wire Line
	2250 8400 1700 8400
Wire Wire Line
	3400 8800 3400 8850
Wire Wire Line
	3400 8850 3500 8850
Wire Wire Line
	3500 8850 3500 8800
Wire Wire Line
	3000 8800 3000 8850
Wire Wire Line
	3000 8850 2900 8850
Wire Wire Line
	2900 8850 2900 8800
Wire Wire Line
	2900 8850 2900 8950
Connection ~ 2900 8850
Wire Wire Line
	3500 8850 3500 8950
Connection ~ 3500 8850
$Comp
L appli_power:GND #PWR?
U 1 1 5AF4CE29
P 2900 8950
F 0 "#PWR?" H 2900 8700 50  0001 C CNN
F 1 "GND" H 2905 8777 50  0000 C CNN
F 2 "" H 2900 8950 60  0000 C CNN
F 3 "" H 2900 8950 60  0000 C CNN
	1    2900 8950
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GNDA #PWR?
U 1 1 5AF4D359
P 6300 9250
F 0 "#PWR?" H 6300 9000 50  0001 C CNN
F 1 "GNDA" H 6305 9077 50  0000 C CNN
F 2 "" H 6300 9250 60  0000 C CNN
F 3 "" H 6300 9250 60  0000 C CNN
	1    6300 9250
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GNDA #PWR?
U 1 1 5AF4D3F9
P 3500 8950
F 0 "#PWR?" H 3500 8700 50  0001 C CNN
F 1 "GNDA" H 3505 8777 50  0000 C CNN
F 2 "" H 3500 8950 60  0000 C CNN
F 3 "" H 3500 8950 60  0000 C CNN
	1    3500 8950
	1    0    0    -1  
$EndComp
Wire Wire Line
	5350 8200 5350 7700
Wire Wire Line
	5350 7700 4600 7700
Wire Wire Line
	4150 8100 4200 8100
Wire Wire Line
	4200 8100 4200 8000
Wire Wire Line
	4200 7800 4150 7800
Wire Wire Line
	4150 8000 4200 8000
Connection ~ 4200 8000
Wire Wire Line
	4200 8000 4200 7800
Wire Wire Line
	2250 8100 2200 8100
Wire Wire Line
	2200 8100 2200 8000
Wire Wire Line
	2200 8000 2250 8000
Wire Wire Line
	2200 8000 2200 7800
Wire Wire Line
	2200 7800 2250 7800
Connection ~ 2200 8000
Wire Wire Line
	1875 7700 2250 7700
Wire Wire Line
	1875 7700 1875 7500
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5B029998
P 1875 7500
F 0 "#PWR?" H 1875 7460 30  0001 C CNN
F 1 "+3.3V" H 1884 7638 30  0000 C CNN
F 2 "" H 1875 7500 60  0000 C CNN
F 3 "" H 1875 7500 60  0000 C CNN
	1    1875 7500
	1    0    0    -1  
$EndComp
$Comp
L appli_resistor:22R_1608M R?
U 1 1 5B029FC4
P 5200 8300
F 0 "R?" V 5012 8150 50  0000 C CNN
F 1 "22R_1608M" V 5092 8150 35  0000 C CNN
F 2 "Applidyne_Resistor:RESC1608X50N" V 5310 8150 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1788326.pdf" V 5335 8150 20  0001 C CNN
F 4 "MULTICOMP" V 5385 8150 20  0001 C CNN "manf"
F 5 "MC0063W060314R22" V 5410 8150 20  0001 C CNN "manf#"
F 6 "Element14" V 5435 8150 20  0001 C CNN "Supplier"
F 7 "2141539" V 5460 8150 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mc0063w060314r22/res-thick-film-4r22-1-0-063w-0603/dp/2141539" V 5485 8150 20  0001 C CNN "Supplier URL"
F 9 "0.015" V 5510 8150 20  0001 C CNN "Supplier Price"
F 10 "1" V 5535 8150 20  0001 C CNN "Supplier Price Break"
	1    5200 8300
	0    1    1    0   
$EndComp
$Comp
L appli_resistor:22R_1608M R?
U 1 1 5B02A07C
P 5200 8400
F 0 "R?" V 5300 8250 50  0000 C CNN
F 1 "22R_1608M" V 5375 8250 35  0000 C CNN
F 2 "Applidyne_Resistor:RESC1608X50N" V 5310 8250 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1788326.pdf" V 5335 8250 20  0001 C CNN
F 4 "MULTICOMP" V 5385 8250 20  0001 C CNN "manf"
F 5 "MC0063W060314R22" V 5410 8250 20  0001 C CNN "manf#"
F 6 "Element14" V 5435 8250 20  0001 C CNN "Supplier"
F 7 "2141539" V 5460 8250 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mc0063w060314r22/res-thick-film-4r22-1-0-063w-0603/dp/2141539" V 5485 8250 20  0001 C CNN "Supplier URL"
F 9 "0.015" V 5510 8250 20  0001 C CNN "Supplier Price"
F 10 "1" V 5535 8250 20  0001 C CNN "Supplier Price Break"
	1    5200 8400
	0    1    1    0   
$EndComp
Wire Wire Line
	4900 8400 4150 8400
Wire Wire Line
	4150 8300 4900 8300
Wire Wire Line
	5200 8300 6550 8300
Wire Wire Line
	6550 8400 5200 8400
$Comp
L appli_capacitor:100n_X7R_1608M C?
U 1 1 5B0749D9
P 4200 8825
F 0 "C?" V 4325 8800 50  0000 R CNN
F 1 "100n_X7R_1608M" V 4075 8950 35  0000 R CNN
F 2 "Applidyne_Capacitor:CAPC1608X90N" V 4315 8725 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1732728.pdf" V 4340 8725 20  0001 C CNN
F 4 "KEMET" V 4390 8725 20  0001 C CNN "manf"
F 5 "C0603C104K5RACAUTO" V 4415 8725 20  0001 C CNN "manf#"
F 6 "Element14" V 4440 8725 20  0001 C CNN "Supplier"
F 7 "2070398" V 4465 8725 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c0603c104k5racauto/cap-mlcc-x7r-100nf-50v-0603/dp/2070398" V 4490 8725 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 4515 8725 20  0001 C CNN "Supplier Price"
F 10 "1" V 4540 8725 20  0001 C CNN "Supplier Price Break"
	1    4200 8825
	-1   0    0    1   
$EndComp
Wire Wire Line
	4600 7700 4600 8625
Connection ~ 4600 7700
Wire Wire Line
	4600 7700 4150 7700
Wire Wire Line
	4600 8825 4600 8950
Wire Wire Line
	4200 8825 4200 8950
$Comp
L appli_power:GNDA #PWR?
U 1 1 5B16B9D7
P 4200 8950
F 0 "#PWR?" H 4200 8700 50  0001 C CNN
F 1 "GNDA" H 4205 8777 50  0000 C CNN
F 2 "" H 4200 8950 60  0000 C CNN
F 3 "" H 4200 8950 60  0000 C CNN
	1    4200 8950
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GNDA #PWR?
U 1 1 5B16BA74
P 4600 8950
F 0 "#PWR?" H 4600 8700 50  0001 C CNN
F 1 "GNDA" H 4605 8777 50  0000 C CNN
F 2 "" H 4600 8950 60  0000 C CNN
F 3 "" H 4600 8950 60  0000 C CNN
	1    4600 8950
	1    0    0    -1  
$EndComp
$Comp
L appli_capacitor:100n_X7R_1608M C39
U 1 1 5B1BE782
P 1875 8825
F 0 "C39" V 2000 8800 50  0000 R CNN
F 1 "100n_X7R_1608M" V 1750 8950 35  0000 R CNN
F 2 "Applidyne_Capacitor:CAPC1608X90N" V 1990 8725 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1732728.pdf" V 2015 8725 20  0001 C CNN
F 4 "KEMET" V 2065 8725 20  0001 C CNN "manf"
F 5 "C0603C104K5RACAUTO" V 2090 8725 20  0001 C CNN "manf#"
F 6 "Element14" V 2115 8725 20  0001 C CNN "Supplier"
F 7 "2070398" V 2140 8725 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c0603c104k5racauto/cap-mlcc-x7r-100nf-50v-0603/dp/2070398" V 2165 8725 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 2190 8725 20  0001 C CNN "Supplier Price"
F 10 "1" V 2215 8725 20  0001 C CNN "Supplier Price Break"
	1    1875 8825
	-1   0    0    1   
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5B221FC4
P 1875 8950
F 0 "#PWR?" H 1875 8700 50  0001 C CNN
F 1 "GND" H 1880 8777 50  0000 C CNN
F 2 "" H 1875 8950 60  0000 C CNN
F 3 "" H 1875 8950 60  0000 C CNN
	1    1875 8950
	1    0    0    -1  
$EndComp
$Comp
L appli_capacitor:100n_X7R_1608M C?
U 1 1 5B2354E1
P 2200 8825
F 0 "C?" V 2300 8775 50  0000 R CNN
F 1 "100n_X7R_1608M" V 2075 8950 35  0000 R CNN
F 2 "Applidyne_Capacitor:CAPC1608X90N" V 2315 8725 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1732728.pdf" V 2340 8725 20  0001 C CNN
F 4 "KEMET" V 2390 8725 20  0001 C CNN "manf"
F 5 "C0603C104K5RACAUTO" V 2415 8725 20  0001 C CNN "manf#"
F 6 "Element14" V 2440 8725 20  0001 C CNN "Supplier"
F 7 "2070398" V 2465 8725 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c0603c104k5racauto/cap-mlcc-x7r-100nf-50v-0603/dp/2070398" V 2490 8725 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 2515 8725 20  0001 C CNN "Supplier Price"
F 10 "1" V 2540 8725 20  0001 C CNN "Supplier Price Break"
	1    2200 8825
	-1   0    0    1   
$EndComp
Wire Wire Line
	2200 8625 2200 8100
Connection ~ 2200 8100
Wire Wire Line
	2200 8825 2200 8950
$Comp
L appli_power:GND #PWR?
U 1 1 5B2D0AE4
P 2200 8950
F 0 "#PWR?" H 2200 8700 50  0001 C CNN
F 1 "GND" H 2205 8777 50  0000 C CNN
F 2 "" H 2200 8950 60  0000 C CNN
F 3 "" H 2200 8950 60  0000 C CNN
	1    2200 8950
	1    0    0    -1  
$EndComp
Wire Wire Line
	1875 7700 1875 8625
Wire Wire Line
	1875 8825 1875 8950
Wire Wire Line
	4200 8100 4200 8625
Connection ~ 4200 8100
Connection ~ 1875 7700
$EndSCHEMATC
