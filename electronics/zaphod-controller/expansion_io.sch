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
Text Notes 1475 1050 0    79   ~ 16
Expansion PWM Outputs
Text Notes 1275 3175 0    79   ~ 16
Expansion OptoIsolated Outputs
Text Notes 12800 4075 0    79   ~ 16
Fan Control Circuit
Text Notes 12675 1575 0    79   ~ 16
Temperature Sensors
Text Notes 12225 1875 0    50   ~ 0
Expansion temp sensor 0
Text Notes 10325 1875 0    50   ~ 0
On-board temp sensor
Text Notes 2025 7050 0    79   ~ 16
Add-in Card
$Comp
L appli_resistor:NTC_10K_1608M TH?
U 1 1 5ABC263A
P 10800 2250
F 0 "TH?" H 10898 2135 50  0000 L CNN
F 1 "NTC_10K_1608M" H 10898 2055 35  0000 L CNN
F 2 "RESC1608X50*" V 10950 2110 20  0001 C CNN
F 3 "http://au.rs-online.com/webdocs/1423/0900766b81423c04.pdf" V 10975 2110 20  0001 C CNN
F 4 "MURATA" V 11025 2110 20  0001 C CNN "manf"
F 5 "NCP18XH103J03RB" V 11050 2120 20  0001 C CNN "manf#"
F 6 "RS Online" V 11075 2110 20  0001 C CNN "Supplier"
F 7 "619-7740" V 11100 2110 20  0001 C CNN "Supplier Part No"
F 8 "http://au.rs-online.com/web/p/thermistors/6197740/" V 11125 2110 20  0001 C CNN "Supplier URL"
F 9 "0.327" V 11150 2110 20  0001 C CNN "Supplier Price"
F 10 "25" V 11175 2110 20  0001 C CNN "Supplier Price Break"
	1    10800 2250
	1    0    0    -1  
$EndComp
$Comp
L appli_connector_wurth:CONN_01X02 P?
U 1 1 5ABC2851
P 13100 2400
F 0 "P?" H 13178 2441 50  0000 L CNN
F 1 "CONN_01X02" H 13178 2350 50  0000 L CNN
F 2 "Applidyne_Connector_Wurth:61300211121" V 13275 2400 50  0001 C CNN
F 3 "http://www.farnell.com/cad/2124416.pdf" V 14050 2450 50  0001 C CNN
F 4 "WURTH" V 13375 2425 60  0001 C CNN "manf"
F 5 "61300211121" V 13475 2425 60  0001 C CNN "manf#"
F 6 "Element 14" V 13575 2425 60  0001 C CNN "Supplier"
F 7 "2356153" V 13650 2425 60  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/wurth-elektronik/61300211121/header-2-54mm-pin-tht-vertical/dp/2356153" V 13750 2425 60  0001 C CNN "Supplier URL"
F 9 "0.17" V 13850 2425 60  0001 C CNN "Supplier Price"
F 10 "1" V 13950 2425 60  0001 C CNN "Supplier Price Break"
	1    13100 2400
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+3.3V #PWR0185
U 1 1 5ABC2901
P 10800 2150
F 0 "#PWR0185" H 10800 2110 30  0001 C CNN
F 1 "+3.3V" H 10809 2288 30  0000 C CNN
F 2 "" H 10800 2150 60  0000 C CNN
F 3 "" H 10800 2150 60  0000 C CNN
	1    10800 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	10400 2600 10550 2600
Wire Wire Line
	10800 2600 10800 2650
Wire Wire Line
	10800 2550 10800 2600
Connection ~ 10800 2600
Wire Wire Line
	10800 2250 10800 2150
Wire Wire Line
	10800 2950 10800 3000
$Comp
L appli_power:GND #PWR0186
U 1 1 5ABC3416
P 10800 3050
F 0 "#PWR0186" H 10800 2800 50  0001 C CNN
F 1 "GND" H 10805 2877 50  0000 C CNN
F 2 "" H 10800 3050 60  0000 C CNN
F 3 "" H 10800 3050 60  0000 C CNN
	1    10800 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	10550 2700 10550 2600
Connection ~ 10550 2600
Wire Wire Line
	10550 2600 10800 2600
Wire Wire Line
	10550 2900 10550 3000
Wire Wire Line
	10550 3000 10800 3000
Connection ~ 10800 3000
Wire Wire Line
	10800 3000 10800 3050
Wire Wire Line
	10100 2600 9950 2600
$Comp
L appli_power:+3.3V #PWR0187
U 1 1 5ABC37B4
P 12800 2150
F 0 "#PWR0187" H 12800 2110 30  0001 C CNN
F 1 "+3.3V" H 12809 2288 30  0000 C CNN
F 2 "" H 12800 2150 60  0000 C CNN
F 3 "" H 12800 2150 60  0000 C CNN
	1    12800 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	12400 2600 12550 2600
Wire Wire Line
	12800 2600 12800 2650
Connection ~ 12800 2600
Wire Wire Line
	12800 2950 12800 3000
$Comp
L appli_power:GND #PWR0188
U 1 1 5ABC37EA
P 12800 3050
F 0 "#PWR0188" H 12800 2800 50  0001 C CNN
F 1 "GND" H 12805 2877 50  0000 C CNN
F 2 "" H 12800 3050 60  0000 C CNN
F 3 "" H 12800 3050 60  0000 C CNN
	1    12800 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	12550 2700 12550 2600
Connection ~ 12550 2600
Wire Wire Line
	12550 2600 12800 2600
Wire Wire Line
	12550 2900 12550 3000
Wire Wire Line
	12550 3000 12800 3000
Connection ~ 12800 3000
Wire Wire Line
	12800 3000 12800 3050
Wire Wire Line
	12100 2600 11950 2600
Wire Wire Line
	12800 2450 12900 2450
Wire Wire Line
	12800 2450 12800 2600
Wire Wire Line
	12900 2350 12800 2350
Wire Wire Line
	12800 2150 12800 2350
Text GLabel 9950 2600 0    50   Output ~ 0
TEMP_PCB
Text GLabel 11950 2600 0    50   Output ~ 0
TEMP_EXT_0
Text GLabel 11950 5875 0    50   Input ~ 0
FAN_PWM
Text GLabel 11950 4975 0    50   Output ~ 0
FAN_HALL
$Comp
L appli_power:+12V #PWR0189
U 1 1 5AD0DD6F
P 13975 4475
F 0 "#PWR0189" H 13975 4325 50  0001 C CNN
F 1 "+12V" H 13990 4648 50  0000 C CNN
F 2 "" H 13975 4475 60  0000 C CNN
F 3 "" H 13975 4475 60  0000 C CNN
	1    13975 4475
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR0190
U 1 1 5AD0DDC4
P 13575 6175
F 0 "#PWR0190" H 13575 5925 50  0001 C CNN
F 1 "GND" H 13580 6002 50  0000 C CNN
F 2 "" H 13575 6175 60  0000 C CNN
F 3 "" H 13575 6175 60  0000 C CNN
	1    13575 6175
	1    0    0    -1  
$EndComp
$Comp
L appli_transistor:BC847B Q?
U 1 1 5AD0E4D5
P 13475 5875
F 0 "Q?" H 13670 5913 40  0000 L CNN
F 1 "BC847B" H 13670 5837 40  0000 L CNN
F 2 "SOT95P240X110-3L16*" H 13400 5975 29  0001 R CNN
F 3 "http://www.farnell.com/datasheets/1685010.pdf" V 13700 5875 20  0001 C CNN
F 4 "Multicomp" V 13750 5875 20  0001 C CNN "manf"
F 5 "BC847B" V 13775 5875 20  0001 C CNN "manf#"
F 6 "Element14" V 13800 5875 20  0001 C CNN "Supplier"
F 7 "1574343" V 13825 5875 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/bc847b/transistor-npn-45v-100ma-sot-23/dp/1574343" V 13850 5875 20  0001 C CNN "Supplier URL"
F 9 "0.26" V 13875 5875 20  0001 C CNN "Supplier Price"
F 10 "10" V 13900 5875 20  0001 C CNN "Supplier Price Break"
	1    13475 5875
	1    0    0    -1  
$EndComp
$Comp
L appli_resistor:1K00_1608M R?
U 1 1 5AD0E62C
P 12325 5875
F 0 "R?" V 12137 5725 50  0000 C CNN
F 1 "1K00_1608M" V 12217 5725 35  0000 C CNN
F 2 "RESC1608*" V 12435 5725 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 12460 5725 20  0001 C CNN
F 4 "MULTICOMP" V 12510 5725 20  0001 C CNN "manf"
F 5 "MCHP03W8F1001T5E" V 12535 5725 20  0001 C CNN "manf#"
F 6 "Element14" V 12560 5725 20  0001 C CNN "Supplier"
F 7 "1576283" V 12585 5725 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mchp03w8f1001t5e/resistor-thick-film-1k-0-125w/dp/1576283" V 12610 5725 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 12635 5725 20  0001 C CNN "Supplier Price"
F 10 "50+" V 12660 5725 20  0001 C CNN "Supplier Price Break"
	1    12325 5875
	0    1    1    0   
$EndComp
Wire Wire Line
	11950 5875 12025 5875
Wire Wire Line
	13575 6075 13575 6175
$Comp
L appli_power:GND #PWR0191
U 1 1 5AD0F172
P 14475 5200
F 0 "#PWR0191" H 14475 4950 50  0001 C CNN
F 1 "GND" H 14480 5027 50  0000 C CNN
F 2 "" H 14475 5200 60  0000 C CNN
F 3 "" H 14475 5200 60  0000 C CNN
	1    14475 5200
	1    0    0    -1  
$EndComp
$Comp
L appli_connector_wurth:CONN_01X04 P?
U 1 1 5AD0F38F
P 14725 4925
F 0 "P?" H 14803 4966 50  0000 L CNN
F 1 "CONN_01X04" H 14803 4875 50  0000 L CNN
F 2 "Applidyne_Connector_Wurth:61300411121" V 14900 4925 50  0001 C CNN
F 3 "http://www.farnell.com/cad/2124416.pdf" H 14725 4600 50  0001 C CNN
F 4 "WURTH" H 14725 4475 60  0001 C CNN "manf"
F 5 "61300411121" H 14725 4375 60  0001 C CNN "manf#"
F 6 "Element 14" H 14725 4275 60  0001 C CNN "Supplier"
F 7 "2356155" H 14725 4200 60  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/wurth-elektronik/61300411121/header-2-54mm-pin-tht-vertical/dp/2356155" H 14725 4100 60  0001 C CNN "Supplier URL"
F 9 "0.125" H 14725 4000 60  0001 C CNN "Supplier Price"
F 10 "1" H 14725 3900 60  0001 C CNN "Supplier Price Break"
	1    14725 4925
	1    0    0    -1  
$EndComp
Wire Wire Line
	14525 4775 14475 4775
Wire Wire Line
	14475 4775 14475 5200
$Comp
L appli_device:BAV99 D?
U 1 1 5AD10A17
P 12575 4975
F 0 "D?" H 12683 5021 50  0000 L CNN
F 1 "BAV99" H 12683 4930 50  0000 L CNN
F 2 "SOT23" H 12775 4825 20  0001 C CNN
F 3 "http://datasheet.octopart.com/BAV99-/T3-NXP-datasheet-5318834.pdf" H 12775 4800 20  0001 C CNN
F 4 "NXP" H 12775 4750 20  0001 C CNN "manf"
F 5 "RS Online" H 12775 4700 20  0001 C CNN "Supplier"
F 6 " 544-4534" H 12775 4675 20  0001 C CNN "Supplier Part No"
F 7 "http://au.rs-online.com/web/p/rectifier-schottky-diodes/5444534/" H 12775 4650 20  0001 C CNN "Supplier URL"
F 8 "0.045" H 12775 4625 20  0001 C CNN "Supplier Price"
F 9 "1" H 12775 4600 20  0001 C CNN "Supplier Price Break"
F 10 "BAV99" H 12775 4725 20  0001 C CNN "manf#"
	1    12575 4975
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR0192
U 1 1 5AD10F65
P 12575 5475
F 0 "#PWR0192" H 12575 5225 50  0001 C CNN
F 1 "GND" H 12580 5302 50  0000 C CNN
F 2 "" H 12575 5475 60  0000 C CNN
F 3 "" H 12575 5475 60  0000 C CNN
	1    12575 5475
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+3.3V #PWR0193
U 1 1 5AD11030
P 12575 4675
F 0 "#PWR0193" H 12575 4635 30  0001 C CNN
F 1 "+3.3V" H 12584 4813 30  0000 C CNN
F 2 "" H 12575 4675 60  0000 C CNN
F 3 "" H 12575 4675 60  0000 C CNN
	1    12575 4675
	1    0    0    -1  
$EndComp
Wire Wire Line
	12575 4675 12575 4725
Wire Wire Line
	12575 5225 12575 5425
$Comp
L appli_resistor:1K00_1608M R?
U 1 1 5AD11EAB
P 12325 4975
F 0 "R?" V 12137 4825 50  0000 C CNN
F 1 "1K00_1608M" V 12217 4825 35  0000 C CNN
F 2 "RESC1608*" V 12435 4825 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 12460 4825 20  0001 C CNN
F 4 "MULTICOMP" V 12510 4825 20  0001 C CNN "manf"
F 5 "MCHP03W8F1001T5E" V 12535 4825 20  0001 C CNN "manf#"
F 6 "Element14" V 12560 4825 20  0001 C CNN "Supplier"
F 7 "1576283" V 12585 4825 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mchp03w8f1001t5e/resistor-thick-film-1k-0-125w/dp/1576283" V 12610 4825 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 12635 4825 20  0001 C CNN "Supplier Price"
F 10 "50+" V 12660 4825 20  0001 C CNN "Supplier Price Break"
	1    12325 4975
	0    1    1    0   
$EndComp
Wire Wire Line
	11950 4975 12025 4975
Wire Wire Line
	12325 4975 12575 4975
$Comp
L appli_capacitor:22UF_X7R_3225M C?
U 1 1 5AD13D98
P 14650 6100
F 0 "C?" H 14572 5965 50  0000 R CNN
F 1 "22UF_X7R_3225M" H 14572 6045 35  0000 R CNN
F 2 "CAPC3225*" V 14765 6000 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2111999.pdf?_ga=1.157172477.774814437.1442284889" V 14790 6000 20  0001 C CNN
F 4 "MURATA" V 14840 6000 20  0001 C CNN "manf"
F 5 "C1210C226K3RACTU" V 14865 6000 20  0001 C CNN "manf#"
F 6 "Element14" V 14890 6000 20  0001 C CNN "Supplier"
F 7 "2473550" V 14915 6000 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c1210c226k3ractu/cap-mlcc-x7r-22uf-25v-1210/dp/2473550" V 14940 6000 20  0001 C CNN "Supplier URL"
F 9 "1.02" V 14965 6000 20  0001 C CNN "Supplier Price"
F 10 "50" V 14990 6000 20  0001 C CNN "Supplier Price Break"
	1    14650 6100
	1    0    0    1   
$EndComp
$Comp
L appli_power:+12V #PWR0194
U 1 1 5AD13E6B
P 14650 5800
F 0 "#PWR0194" H 14650 5650 50  0001 C CNN
F 1 "+12V" H 14665 5973 50  0000 C CNN
F 2 "" H 14650 5800 60  0000 C CNN
F 3 "" H 14650 5800 60  0000 C CNN
	1    14650 5800
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR0195
U 1 1 5AD13E92
P 14875 6250
F 0 "#PWR0195" H 14875 6000 50  0001 C CNN
F 1 "GND" H 14880 6077 50  0000 C CNN
F 2 "" H 14875 6250 60  0000 C CNN
F 3 "" H 14875 6250 60  0000 C CNN
	1    14875 6250
	1    0    0    -1  
$EndComp
$Comp
L appli_capacitor:100NF_X7R_1608M C?
U 1 1 5AD14F52
P 14875 5900
F 0 "C?" H 14953 5835 50  0000 L CNN
F 1 "100NF_X7R_1608M" H 14953 5755 35  0000 L CNN
F 2 "CAPC1608*" V 14990 5800 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1732728.pdf" V 15015 5800 20  0001 C CNN
F 4 "KEMET" V 15065 5800 20  0001 C CNN "manf"
F 5 "C0603C104K5RACAUTO" V 15090 5800 20  0001 C CNN "manf#"
F 6 "Element14" V 15115 5800 20  0001 C CNN "Supplier"
F 7 "2070398" V 15140 5800 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c0603c104k5racauto/cap-mlcc-x7r-100nf-50v-0603/dp/2070398" V 15165 5800 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 15190 5800 20  0001 C CNN "Supplier Price"
F 10 "1" V 15215 5800 20  0001 C CNN "Supplier Price Break"
	1    14875 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	14650 5900 14650 5850
Wire Wire Line
	14650 6100 14650 6150
Wire Wire Line
	14650 6150 14875 6150
Wire Wire Line
	14875 6150 14875 6100
Wire Wire Line
	14875 5900 14875 5850
Wire Wire Line
	14875 5850 14650 5850
Connection ~ 14650 5850
Wire Wire Line
	14650 5850 14650 5800
Wire Wire Line
	14875 6250 14875 6150
Connection ~ 14875 6150
Text Notes 14050 6375 0    50   ~ 0
Bypass caps near \nfan connector
$Comp
L appli_resistor:4K7_1608M R?
U 1 1 5AD1ED64
P 13825 4875
F 0 "R?" H 13752 4690 50  0000 R CNN
F 1 "4K7_1608M" H 13752 4770 35  0000 R CNN
F 2 "RESC1608*" V 13935 4725 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1788326.pdf" V 13960 4725 20  0001 C CNN
F 4 "MULTICOMP" V 14010 4725 20  0001 C CNN "manf"
F 5 "MC0063W060354K7" V 14035 4725 20  0001 C CNN "manf#"
F 6 "Element14" V 14060 4725 20  0001 C CNN "Supplier"
F 7 "9332154" V 14085 4725 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mc0063w060354k7/res-thick-film-4k7-5-0-063w-0603/dp/9332154" V 14110 4725 20  0001 C CNN "Supplier URL"
F 9 "0.024" V 14135 4725 20  0001 C CNN "Supplier Price"
F 10 "1" V 14160 4725 20  0001 C CNN "Supplier Price Break"
	1    13825 4875
	1    0    0    1   
$EndComp
Wire Wire Line
	13825 4875 13825 4975
Wire Wire Line
	13825 4975 14525 4975
Wire Wire Line
	13425 4975 13825 4975
Connection ~ 13825 4975
Wire Wire Line
	13125 4975 13025 4975
Connection ~ 12575 4975
Wire Wire Line
	13975 4475 13975 4525
Wire Wire Line
	13825 4575 13825 4525
Wire Wire Line
	13825 4525 13975 4525
Connection ~ 13975 4525
Wire Wire Line
	13975 4525 13975 4875
Wire Wire Line
	13025 4975 13025 5050
Connection ~ 13025 4975
Wire Wire Line
	13025 4975 12575 4975
$Comp
L appli_resistor:10K0_1608M R?
U 1 1 5AD365D7
P 13025 5050
F 0 "R?" H 13098 4935 50  0000 L CNN
F 1 "10K0_1608M" H 13098 4855 35  0000 L CNN
F 2 "RESC1608*" V 13135 4900 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 13160 4900 20  0001 C CNN
F 4 "TE CONNECTIVITY" V 13210 4900 20  0001 C CNN "manf"
F 5 "CRGH0603F10K" V 13235 4900 20  0001 C CNN "manf#"
F 6 "Element14" V 13260 4900 20  0001 C CNN "Supplier"
F 7 "2332016" V 13285 4900 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/te-connectivity/crgh0603f10k/resistor-power-10k-0-2w-1-0603/dp/2332016" V 13310 4900 20  0001 C CNN "Supplier URL"
F 9 "0.02" V 13335 4900 20  0001 C CNN "Supplier Price"
F 10 "10" V 13360 4900 20  0001 C CNN "Supplier Price Break"
	1    13025 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	13025 5350 13025 5425
Wire Wire Line
	13025 5425 12575 5425
Connection ~ 12575 5425
Wire Wire Line
	12575 5425 12575 5475
Wire Wire Line
	13575 5075 14525 5075
Wire Wire Line
	13575 5075 13575 5675
Wire Wire Line
	13275 5875 12325 5875
$Comp
L appli_resistor:33K2_1608M R?
U 1 1 5AD500A2
P 13425 4975
F 0 "R?" V 13237 4825 50  0000 C CNN
F 1 "33K2_1608M" V 13317 4825 35  0000 C CNN
F 2 "RESC1608*" V 13535 4825 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2051174.pdf?_ga=1.145166194.774814437.1442284889" V 13560 4825 20  0001 C CNN
F 4 "PANASONIC " V 13610 4825 20  0001 C CNN "manf"
F 5 "ERJPA3F3322V" V 13635 4825 20  0001 C CNN "manf#"
F 6 "Element14" V 13660 4825 20  0001 C CNN "Supplier"
F 7 "2561973" V 13685 4825 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/panasonic-electronic-components/erjpa3f3322v/res-aec-q200-thick-film-33k2-0603/dp/2561973" V 13710 4825 20  0001 C CNN "Supplier URL"
F 9 "0.2" V 13735 4825 20  0001 C CNN "Supplier Price"
F 10 "1" V 13760 4825 20  0001 C CNN "Supplier Price Break"
	1    13425 4975
	0    1    1    0   
$EndComp
Text Notes 11225 4650 0    50   ~ 0
Tachometer uses 12V pullup.\n2.3:1 divider on tach.\nBAV99 prevents input damage.\n
Text Notes 11225 5575 0    50   ~ 0
Drive PWM at 22-25kHz.
$Comp
L appli_fuse:500mA_1210L_PTC F?
U 1 1 5AD5A6A5
P 14425 4875
F 0 "F?" V 14175 4725 50  0000 C CNN
F 1 "500mA_1210L_PTC" V 14250 4725 35  0000 C CNN
F 2 "Applidyne_Fuse:FUSC3225X85L50N" V 14535 4725 20  0001 C CNN
F 3 "http://docs-asia.electrocomponents.com/webdocs/1420/0900766b8142082b.pdf" V 14560 4725 20  0001 C CNN
F 4 "LittelFuse" V 14610 4725 20  0001 C CNN "manf"
F 5 " 1210L050YR" V 14635 4725 20  0001 C CNN "manf#"
F 6 "RS Online" V 14660 4725 20  0001 C CNN "Supplier"
F 7 "787-4202" V 14685 4725 20  0001 C CNN "Supplier Part No"
F 8 "http://au.rs-online.com/web/p/resettable-surface-mount-fuses/7874202/" V 14710 4725 20  0001 C CNN "Supplier URL"
F 9 "0.486" V 14735 4725 20  0001 C CNN "Supplier Price"
F 10 "10" V 14760 4725 20  0001 C CNN "Supplier Price Break"
	1    14425 4875
	0    1    1    0   
$EndComp
Wire Wire Line
	14525 4875 14425 4875
Wire Wire Line
	14125 4875 13975 4875
Text Notes 11575 6775 0    118  ~ 24
TODO pullup resistor on PWM control
Text Notes 10225 7175 0    50   ~ 0
https://electronics.stackexchange.com/questions/153846/arduino-how-to-read-and-control-the-speed-of-a-12v-four-wire-fan
Text Notes 14650 4550 0    50   Italic 0
TODO CONNECTOR
$Comp
L appli_connector_wurth:CONN_01X02 P?
U 1 1 5AC12FC5
P 15300 2400
F 0 "P?" H 15378 2441 50  0000 L CNN
F 1 "CONN_01X02" H 15378 2350 50  0000 L CNN
F 2 "Applidyne_Connector_Wurth:61300211121" V 15475 2400 50  0001 C CNN
F 3 "http://www.farnell.com/cad/2124416.pdf" V 16250 2450 50  0001 C CNN
F 4 "WURTH" V 15575 2425 60  0001 C CNN "manf"
F 5 "61300211121" V 15675 2425 60  0001 C CNN "manf#"
F 6 "Element 14" V 15775 2425 60  0001 C CNN "Supplier"
F 7 "2356153" V 15850 2425 60  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/wurth-elektronik/61300211121/header-2-54mm-pin-tht-vertical/dp/2356153" V 15950 2425 60  0001 C CNN "Supplier URL"
F 9 "0.17" V 16050 2425 60  0001 C CNN "Supplier Price"
F 10 "1" V 16150 2425 60  0001 C CNN "Supplier Price Break"
	1    15300 2400
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+3.3V #PWR0196
U 1 1 5AC12FCC
P 15000 2150
F 0 "#PWR0196" H 15000 2110 30  0001 C CNN
F 1 "+3.3V" H 15009 2288 30  0000 C CNN
F 2 "" H 15000 2150 60  0000 C CNN
F 3 "" H 15000 2150 60  0000 C CNN
	1    15000 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	14600 2600 14750 2600
Wire Wire Line
	15000 2600 15000 2650
Connection ~ 15000 2600
Wire Wire Line
	15000 2950 15000 3000
$Comp
L appli_power:GND #PWR0197
U 1 1 5AC13000
P 15000 3050
F 0 "#PWR0197" H 15000 2800 50  0001 C CNN
F 1 "GND" H 15005 2877 50  0000 C CNN
F 2 "" H 15000 3050 60  0000 C CNN
F 3 "" H 15000 3050 60  0000 C CNN
	1    15000 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	14750 2700 14750 2600
Connection ~ 14750 2600
Wire Wire Line
	14750 2600 15000 2600
Wire Wire Line
	14750 2900 14750 3000
Wire Wire Line
	14750 3000 15000 3000
Connection ~ 15000 3000
Wire Wire Line
	15000 3000 15000 3050
Wire Wire Line
	14300 2600 14150 2600
Wire Wire Line
	15000 2450 15100 2450
Wire Wire Line
	15000 2450 15000 2600
Wire Wire Line
	15100 2350 15000 2350
Wire Wire Line
	15000 2150 15000 2350
Text GLabel 14150 2600 0    50   Output ~ 0
TEMP_EXT_1
Text Notes 14500 1875 0    50   ~ 0
Expansion temp sensor 1
$Comp
L appli_resistor:1K00_1608M R?
U 1 1 5AC153F3
P 10400 2600
F 0 "R?" V 10212 2450 50  0000 C CNN
F 1 "1K00_1608M" V 10292 2450 35  0000 C CNN
F 2 "RESC1608*" V 10510 2450 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 10535 2450 20  0001 C CNN
F 4 "MULTICOMP" V 10585 2450 20  0001 C CNN "manf"
F 5 "MCHP03W8F1001T5E" V 10610 2450 20  0001 C CNN "manf#"
F 6 "Element14" V 10635 2450 20  0001 C CNN "Supplier"
F 7 "1576283" V 10660 2450 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mchp03w8f1001t5e/resistor-thick-film-1k-0-125w/dp/1576283" V 10685 2450 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 10710 2450 20  0001 C CNN "Supplier Price"
F 10 "50+" V 10735 2450 20  0001 C CNN "Supplier Price Break"
	1    10400 2600
	0    1    1    0   
$EndComp
$Comp
L appli_resistor:1K00_1608M R?
U 1 1 5AC15558
P 12400 2600
F 0 "R?" V 12212 2450 50  0000 C CNN
F 1 "1K00_1608M" V 12292 2450 35  0000 C CNN
F 2 "RESC1608*" V 12510 2450 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 12535 2450 20  0001 C CNN
F 4 "MULTICOMP" V 12585 2450 20  0001 C CNN "manf"
F 5 "MCHP03W8F1001T5E" V 12610 2450 20  0001 C CNN "manf#"
F 6 "Element14" V 12635 2450 20  0001 C CNN "Supplier"
F 7 "1576283" V 12660 2450 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mchp03w8f1001t5e/resistor-thick-film-1k-0-125w/dp/1576283" V 12685 2450 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 12710 2450 20  0001 C CNN "Supplier Price"
F 10 "50+" V 12735 2450 20  0001 C CNN "Supplier Price Break"
	1    12400 2600
	0    1    1    0   
$EndComp
$Comp
L appli_resistor:1K00_1608M R?
U 1 1 5AC155B6
P 14600 2600
F 0 "R?" V 14412 2450 50  0000 C CNN
F 1 "1K00_1608M" V 14492 2450 35  0000 C CNN
F 2 "RESC1608*" V 14710 2450 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 14735 2450 20  0001 C CNN
F 4 "MULTICOMP" V 14785 2450 20  0001 C CNN "manf"
F 5 "MCHP03W8F1001T5E" V 14810 2450 20  0001 C CNN "manf#"
F 6 "Element14" V 14835 2450 20  0001 C CNN "Supplier"
F 7 "1576283" V 14860 2450 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mchp03w8f1001t5e/resistor-thick-film-1k-0-125w/dp/1576283" V 14885 2450 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 14910 2450 20  0001 C CNN "Supplier Price"
F 10 "50+" V 14935 2450 20  0001 C CNN "Supplier Price Break"
	1    14600 2600
	0    1    1    0   
$EndComp
$Comp
L appli_capacitor:100NF_X7R_1608M C?
U 1 1 5AC157FF
P 10550 2900
F 0 "C?" H 10473 2765 50  0000 R CNN
F 1 "100NF_X7R_1608M" H 10473 2845 35  0000 R CNN
F 2 "CAPC1608*" V 10665 2800 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1732728.pdf" V 10690 2800 20  0001 C CNN
F 4 "KEMET" V 10740 2800 20  0001 C CNN "manf"
F 5 "C0603C104K5RACAUTO" V 10765 2800 20  0001 C CNN "manf#"
F 6 "Element14" V 10790 2800 20  0001 C CNN "Supplier"
F 7 "2070398" V 10815 2800 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c0603c104k5racauto/cap-mlcc-x7r-100nf-50v-0603/dp/2070398" V 10840 2800 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 10865 2800 20  0001 C CNN "Supplier Price"
F 10 "1" V 10890 2800 20  0001 C CNN "Supplier Price Break"
	1    10550 2900
	1    0    0    1   
$EndComp
$Comp
L appli_capacitor:100NF_X7R_1608M C?
U 1 1 5AC16B5C
P 12550 2900
F 0 "C?" H 12473 2765 50  0000 R CNN
F 1 "100NF_X7R_1608M" H 12473 2845 35  0000 R CNN
F 2 "CAPC1608*" V 12665 2800 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1732728.pdf" V 12690 2800 20  0001 C CNN
F 4 "KEMET" V 12740 2800 20  0001 C CNN "manf"
F 5 "C0603C104K5RACAUTO" V 12765 2800 20  0001 C CNN "manf#"
F 6 "Element14" V 12790 2800 20  0001 C CNN "Supplier"
F 7 "2070398" V 12815 2800 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c0603c104k5racauto/cap-mlcc-x7r-100nf-50v-0603/dp/2070398" V 12840 2800 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 12865 2800 20  0001 C CNN "Supplier Price"
F 10 "1" V 12890 2800 20  0001 C CNN "Supplier Price Break"
	1    12550 2900
	1    0    0    1   
$EndComp
$Comp
L appli_capacitor:100NF_X7R_1608M C?
U 1 1 5AC16BBA
P 14750 2900
F 0 "C?" H 14673 2765 50  0000 R CNN
F 1 "100NF_X7R_1608M" H 14673 2845 35  0000 R CNN
F 2 "CAPC1608*" V 14865 2800 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1732728.pdf" V 14890 2800 20  0001 C CNN
F 4 "KEMET" V 14940 2800 20  0001 C CNN "manf"
F 5 "C0603C104K5RACAUTO" V 14965 2800 20  0001 C CNN "manf#"
F 6 "Element14" V 14990 2800 20  0001 C CNN "Supplier"
F 7 "2070398" V 15015 2800 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c0603c104k5racauto/cap-mlcc-x7r-100nf-50v-0603/dp/2070398" V 15040 2800 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 15065 2800 20  0001 C CNN "Supplier Price"
F 10 "1" V 15090 2800 20  0001 C CNN "Supplier Price Break"
	1    14750 2900
	1    0    0    1   
$EndComp
$Comp
L appli_resistor:10K0_1608M R?
U 1 1 5AC16D64
P 10800 2650
F 0 "R?" H 10873 2535 50  0000 L CNN
F 1 "10K0_1608M" H 10873 2455 35  0000 L CNN
F 2 "RESC1608*" V 10910 2500 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 10935 2500 20  0001 C CNN
F 4 "TE CONNECTIVITY" V 10985 2500 20  0001 C CNN "manf"
F 5 "CRGH0603F10K" V 11010 2500 20  0001 C CNN "manf#"
F 6 "Element14" V 11035 2500 20  0001 C CNN "Supplier"
F 7 "2332016" V 11060 2500 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/te-connectivity/crgh0603f10k/resistor-power-10k-0-2w-1-0603/dp/2332016" V 11085 2500 20  0001 C CNN "Supplier URL"
F 9 "0.02" V 11110 2500 20  0001 C CNN "Supplier Price"
F 10 "10" V 11135 2500 20  0001 C CNN "Supplier Price Break"
	1    10800 2650
	1    0    0    -1  
$EndComp
$Comp
L appli_resistor:10K0_1608M R?
U 1 1 5AC16E9A
P 12800 2650
F 0 "R?" H 12873 2535 50  0000 L CNN
F 1 "10K0_1608M" H 12873 2455 35  0000 L CNN
F 2 "RESC1608*" V 12910 2500 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 12935 2500 20  0001 C CNN
F 4 "TE CONNECTIVITY" V 12985 2500 20  0001 C CNN "manf"
F 5 "CRGH0603F10K" V 13010 2500 20  0001 C CNN "manf#"
F 6 "Element14" V 13035 2500 20  0001 C CNN "Supplier"
F 7 "2332016" V 13060 2500 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/te-connectivity/crgh0603f10k/resistor-power-10k-0-2w-1-0603/dp/2332016" V 13085 2500 20  0001 C CNN "Supplier URL"
F 9 "0.02" V 13110 2500 20  0001 C CNN "Supplier Price"
F 10 "10" V 13135 2500 20  0001 C CNN "Supplier Price Break"
	1    12800 2650
	1    0    0    -1  
$EndComp
$Comp
L appli_resistor:10K0_1608M R?
U 1 1 5AC16EF4
P 15000 2650
F 0 "R?" H 15073 2535 50  0000 L CNN
F 1 "10K0_1608M" H 15073 2455 35  0000 L CNN
F 2 "RESC1608*" V 15110 2500 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 15135 2500 20  0001 C CNN
F 4 "TE CONNECTIVITY" V 15185 2500 20  0001 C CNN "manf"
F 5 "CRGH0603F10K" V 15210 2500 20  0001 C CNN "manf#"
F 6 "Element14" V 15235 2500 20  0001 C CNN "Supplier"
F 7 "2332016" V 15260 2500 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/te-connectivity/crgh0603f10k/resistor-power-10k-0-2w-1-0603/dp/2332016" V 15285 2500 20  0001 C CNN "Supplier URL"
F 9 "0.02" V 15310 2500 20  0001 C CNN "Supplier Price"
F 10 "10" V 15335 2500 20  0001 C CNN "Supplier Price Break"
	1    15000 2650
	1    0    0    -1  
$EndComp
Text GLabel 1425 1600 0    50   Input ~ 0
AUX_PWM_1
Text GLabel 1425 1700 0    50   Input ~ 0
AUX_PWM_2
Text GLabel 1425 1500 0    50   Input ~ 0
AUX_PWM_0
Text GLabel 11550 8575 0    50   Output ~ 0
BUZZER_PWM
Text GLabel 7275 4625 0    50   Output ~ 0
USART_1_TX
Text GLabel 7275 4725 0    50   Input ~ 0
USART_1_RX
Text GLabel 1925 7675 0    50   Output ~ 0
USART_2_TX
Text GLabel 1925 7775 0    50   Input ~ 0
USART_2_RX
Text GLabel 1925 7575 0    50   Output ~ 0
USART_2_RTS
Text GLabel 1925 7475 0    50   Input ~ 0
USART_2_CTS
Text GLabel 1900 8100 0    50   Output ~ 0
USART_2_PWR_EN
Text Notes 2000 8225 0    50   ~ 0
mosfet
Text Notes 12600 7850 0    79   ~ 16
Buzzer
Text Notes 11875 8075 0    50   ~ 0
Used for potential indication of state/warnings
Text GLabel 1425 4175 0    50   Input ~ 0
EXT_INPUT_0
Text GLabel 1425 5150 0    50   Input ~ 0
EXT_OUTPUT_1
Text GLabel 1400 5300 0    50   Output ~ 0
EXT_INPUT_1
Text GLabel 1425 3775 0    50   Output ~ 0
EXT_OUTPUT_0
Text Notes 1750 3925 0    50   ~ 0
EXT_0 is UART capable
Text Notes 2650 5400 2    50   ~ 0
EXT_1 is CAN capable\n(with added tranceiver)
Text Notes 1250 1250 0    50   ~ 0
To be used with LED drivers, servos or motors/pumps
Text Notes 1325 3425 0    50   ~ 0
Provide 2 optocoupled input/output pairs.\nPair 0 is UART capable, Pair 1 is CAN capable.
$EndSCHEMATC
