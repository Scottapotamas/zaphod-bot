EESchema Schematic File Version 4
LIBS:zaphod-controller-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
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
Effector Expansion
Text Notes 1275 3175 0    79   ~ 16
Expansion OptoIsolated Outputs
Text Notes 1275 4425 0    79   ~ 16
Expansion OptoIsolated Inputs
Text Notes 4075 3800 0    50   ~ 0
Isolated IO Connector
Text Notes 8425 3125 0    79   ~ 16
Fan Control Circuit
Text Notes 8375 1075 0    79   ~ 16
Temperature Sensors
Text Notes 9475 1375 0    50   ~ 0
Expansion temp sensor
Text Notes 7575 1375 0    50   ~ 0
On-board temp sensor
Text Notes 1300 6025 0    79   ~ 16
Wireless Add-in Card
$Comp
L appli_resistor:NTC_10K_1608M TH?
U 1 1 5ABC263A
P 8050 1750
F 0 "TH?" H 8148 1635 50  0000 L CNN
F 1 "NTC_10K_1608M" H 8148 1555 35  0000 L CNN
F 2 "RESC1608X50*" V 8200 1610 20  0001 C CNN
F 3 "http://au.rs-online.com/webdocs/1423/0900766b81423c04.pdf" V 8225 1610 20  0001 C CNN
F 4 "MURATA" V 8275 1610 20  0001 C CNN "manf"
F 5 "NCP18XH103J03RB" V 8300 1620 20  0001 C CNN "manf#"
F 6 "RS Online" V 8325 1610 20  0001 C CNN "Supplier"
F 7 "619-7740" V 8350 1610 20  0001 C CNN "Supplier Part No"
F 8 "http://au.rs-online.com/web/p/thermistors/6197740/" V 8375 1610 20  0001 C CNN "Supplier URL"
F 9 "0.327" V 8400 1610 20  0001 C CNN "Supplier Price"
F 10 "25" V 8425 1610 20  0001 C CNN "Supplier Price Break"
	1    8050 1750
	1    0    0    -1  
$EndComp
$Comp
L appli_connector_wurth:CONN_01X02 P?
U 1 1 5ABC2851
P 10350 1900
F 0 "P?" H 10428 1941 50  0000 L CNN
F 1 "CONN_01X02" H 10428 1850 50  0000 L CNN
F 2 "Applidyne_Connector_Wurth:61300211121" V 10525 1900 50  0001 C CNN
F 3 "http://www.farnell.com/cad/2124416.pdf" V 11300 1950 50  0001 C CNN
F 4 "WURTH" V 10625 1925 60  0001 C CNN "manf"
F 5 "61300211121" V 10725 1925 60  0001 C CNN "manf#"
F 6 "Element 14" V 10825 1925 60  0001 C CNN "Supplier"
F 7 "2356153" V 10900 1925 60  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/wurth-elektronik/61300211121/header-2-54mm-pin-tht-vertical/dp/2356153" V 11000 1925 60  0001 C CNN "Supplier URL"
F 9 "0.17" V 11100 1925 60  0001 C CNN "Supplier Price"
F 10 "1" V 11200 1925 60  0001 C CNN "Supplier Price Break"
	1    10350 1900
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5ABC2901
P 8050 1650
F 0 "#PWR?" H 8050 1610 30  0001 C CNN
F 1 "+3.3V" H 8059 1788 30  0000 C CNN
F 2 "" H 8050 1650 60  0000 C CNN
F 3 "" H 8050 1650 60  0000 C CNN
	1    8050 1650
	1    0    0    -1  
$EndComp
$Comp
L appli_capacitor:100NF_X7R_1005M C?
U 1 1 5ABC2D40
P 7800 2400
F 0 "C?" H 7723 2265 50  0000 R CNN
F 1 "100NF_X7R_1005M" H 7723 2345 35  0000 R CNN
F 2 "Applidyne_Capacitor:CAPC1005X60N" V 7915 2300 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1936565.pdf" V 7940 2300 20  0001 C CNN
F 4 "TDK" V 7990 2300 20  0001 C CNN "manf"
F 5 "CGA2B3X7R1H104K050BB" V 8015 2300 20  0001 C CNN "manf#"
F 6 "Element14" V 8040 2300 20  0001 C CNN "Supplier"
F 7 "2210822" V 8065 2300 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/tdk/cga2b3x7r1h104k050bb/cap-mlcc-x7r-100nf-50v-0402/dp/2210822" V 8090 2300 20  0001 C CNN "Supplier URL"
F 9 "0.087" V 8115 2300 20  0001 C CNN "Supplier Price"
F 10 "100" V 8140 2300 20  0001 C CNN "Supplier Price Break"
	1    7800 2400
	1    0    0    1   
$EndComp
$Comp
L appli_resistor:1K_1005M R?
U 1 1 5ABC2ED9
P 7650 2100
F 0 "R?" V 7462 1950 50  0000 C CNN
F 1 "1K_1005M" V 7542 1950 35  0000 C CNN
F 2 "Applidyne_Resistor:RESC1005X40N" V 7760 1950 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2111203.pdf" V 7785 1950 20  0001 C CNN
F 4 "VISHAY" V 7835 1950 20  0001 C CNN "manf"
F 5 "CRCW04021K00FKED" V 7860 1950 20  0001 C CNN "manf#"
F 6 "Element14" V 7885 1950 20  0001 C CNN "Supplier"
F 7 "1469669" V 7910 1950 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/vishay/crcw040210k0fked/res-thick-film-10k-1-0-063w-0402/dp/1469669" V 7935 1950 20  0001 C CNN "Supplier URL"
F 9 "0.09" V 7960 1950 20  0001 C CNN "Supplier Price"
F 10 "10" V 7985 1950 20  0001 C CNN "Supplier Price Break"
	1    7650 2100
	0    1    1    0   
$EndComp
$Comp
L appli_resistor:10K_1005M R?
U 1 1 5ABC31B5
P 8050 2150
F 0 "R?" H 8123 2035 50  0000 L CNN
F 1 "10K_1005M" H 8123 1955 35  0000 L CNN
F 2 "Applidyne_Resistor:RESC1005X40N" V 8160 2000 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2111203.pdf" V 8185 2000 20  0001 C CNN
F 4 "VISHAY" V 8235 2000 20  0001 C CNN "manf"
F 5 "CRCW040210K0FKED" V 8260 2000 20  0001 C CNN "manf#"
F 6 "Element14" V 8285 2000 20  0001 C CNN "Supplier"
F 7 "1469669" V 8310 2000 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/vishay/crcw040210k0fked/res-thick-film-10k-1-0-063w-0402/dp/1469669" V 8335 2000 20  0001 C CNN "Supplier URL"
F 9 "0.009" V 8360 2000 20  0001 C CNN "Supplier Price"
F 10 "10" V 8385 2000 20  0001 C CNN "Supplier Price Break"
	1    8050 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	7650 2100 7800 2100
Wire Wire Line
	8050 2100 8050 2150
Wire Wire Line
	8050 2050 8050 2100
Connection ~ 8050 2100
Wire Wire Line
	8050 1750 8050 1650
Wire Wire Line
	8050 2450 8050 2500
$Comp
L appli_power:GND #PWR?
U 1 1 5ABC3416
P 8050 2550
F 0 "#PWR?" H 8050 2300 50  0001 C CNN
F 1 "GND" H 8055 2377 50  0000 C CNN
F 2 "" H 8050 2550 60  0000 C CNN
F 3 "" H 8050 2550 60  0000 C CNN
	1    8050 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	7800 2200 7800 2100
Connection ~ 7800 2100
Wire Wire Line
	7800 2100 8050 2100
Wire Wire Line
	7800 2400 7800 2500
Wire Wire Line
	7800 2500 8050 2500
Connection ~ 8050 2500
Wire Wire Line
	8050 2500 8050 2550
Wire Wire Line
	7350 2100 7200 2100
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5ABC37B4
P 10050 1650
F 0 "#PWR?" H 10050 1610 30  0001 C CNN
F 1 "+3.3V" H 10059 1788 30  0000 C CNN
F 2 "" H 10050 1650 60  0000 C CNN
F 3 "" H 10050 1650 60  0000 C CNN
	1    10050 1650
	1    0    0    -1  
$EndComp
$Comp
L appli_capacitor:100NF_X7R_1005M C?
U 1 1 5ABC37C1
P 9800 2400
F 0 "C?" H 9723 2265 50  0000 R CNN
F 1 "100NF_X7R_1005M" H 9723 2345 35  0000 R CNN
F 2 "Applidyne_Capacitor:CAPC1005X60N" V 9915 2300 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1936565.pdf" V 9940 2300 20  0001 C CNN
F 4 "TDK" V 9990 2300 20  0001 C CNN "manf"
F 5 "CGA2B3X7R1H104K050BB" V 10015 2300 20  0001 C CNN "manf#"
F 6 "Element14" V 10040 2300 20  0001 C CNN "Supplier"
F 7 "2210822" V 10065 2300 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/tdk/cga2b3x7r1h104k050bb/cap-mlcc-x7r-100nf-50v-0402/dp/2210822" V 10090 2300 20  0001 C CNN "Supplier URL"
F 9 "0.087" V 10115 2300 20  0001 C CNN "Supplier Price"
F 10 "100" V 10140 2300 20  0001 C CNN "Supplier Price Break"
	1    9800 2400
	1    0    0    1   
$EndComp
$Comp
L appli_resistor:1K_1005M R?
U 1 1 5ABC37CF
P 9650 2100
F 0 "R?" V 9462 1950 50  0000 C CNN
F 1 "1K_1005M" V 9542 1950 35  0000 C CNN
F 2 "Applidyne_Resistor:RESC1005X40N" V 9760 1950 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2111203.pdf" V 9785 1950 20  0001 C CNN
F 4 "VISHAY" V 9835 1950 20  0001 C CNN "manf"
F 5 "CRCW04021K00FKED" V 9860 1950 20  0001 C CNN "manf#"
F 6 "Element14" V 9885 1950 20  0001 C CNN "Supplier"
F 7 "1469669" V 9910 1950 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/vishay/crcw040210k0fked/res-thick-film-10k-1-0-063w-0402/dp/1469669" V 9935 1950 20  0001 C CNN "Supplier URL"
F 9 "0.09" V 9960 1950 20  0001 C CNN "Supplier Price"
F 10 "10" V 9985 1950 20  0001 C CNN "Supplier Price Break"
	1    9650 2100
	0    1    1    0   
$EndComp
$Comp
L appli_resistor:10K_1005M R?
U 1 1 5ABC37DD
P 10050 2150
F 0 "R?" H 10123 2035 50  0000 L CNN
F 1 "10K_1005M" H 10123 1955 35  0000 L CNN
F 2 "Applidyne_Resistor:RESC1005X40N" V 10160 2000 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2111203.pdf" V 10185 2000 20  0001 C CNN
F 4 "VISHAY" V 10235 2000 20  0001 C CNN "manf"
F 5 "CRCW040210K0FKED" V 10260 2000 20  0001 C CNN "manf#"
F 6 "Element14" V 10285 2000 20  0001 C CNN "Supplier"
F 7 "1469669" V 10310 2000 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/vishay/crcw040210k0fked/res-thick-film-10k-1-0-063w-0402/dp/1469669" V 10335 2000 20  0001 C CNN "Supplier URL"
F 9 "0.009" V 10360 2000 20  0001 C CNN "Supplier Price"
F 10 "10" V 10385 2000 20  0001 C CNN "Supplier Price Break"
	1    10050 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	9650 2100 9800 2100
Wire Wire Line
	10050 2100 10050 2150
Connection ~ 10050 2100
Wire Wire Line
	10050 2450 10050 2500
$Comp
L appli_power:GND #PWR?
U 1 1 5ABC37EA
P 10050 2550
F 0 "#PWR?" H 10050 2300 50  0001 C CNN
F 1 "GND" H 10055 2377 50  0000 C CNN
F 2 "" H 10050 2550 60  0000 C CNN
F 3 "" H 10050 2550 60  0000 C CNN
	1    10050 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	9800 2200 9800 2100
Connection ~ 9800 2100
Wire Wire Line
	9800 2100 10050 2100
Wire Wire Line
	9800 2400 9800 2500
Wire Wire Line
	9800 2500 10050 2500
Connection ~ 10050 2500
Wire Wire Line
	10050 2500 10050 2550
Wire Wire Line
	9350 2100 9200 2100
Wire Wire Line
	10050 1950 10150 1950
Wire Wire Line
	10050 1950 10050 2100
Wire Wire Line
	10150 1850 10050 1850
Wire Wire Line
	10050 1650 10050 1850
Text GLabel 7200 2100 0    50   Output ~ 0
TEMP_PCB
Text GLabel 9200 2100 0    50   Output ~ 0
TEMP_EXT
Text GLabel 7575 4925 0    50   Input ~ 0
FAN_PWM
Text GLabel 7575 4025 0    50   Output ~ 0
FAN_HALL
$Comp
L appli_power:+12V #PWR?
U 1 1 5AD0DD6F
P 9600 3525
F 0 "#PWR?" H 9600 3375 50  0001 C CNN
F 1 "+12V" H 9615 3698 50  0000 C CNN
F 2 "" H 9600 3525 60  0000 C CNN
F 3 "" H 9600 3525 60  0000 C CNN
	1    9600 3525
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5AD0DDC4
P 9200 5225
F 0 "#PWR?" H 9200 4975 50  0001 C CNN
F 1 "GND" H 9205 5052 50  0000 C CNN
F 2 "" H 9200 5225 60  0000 C CNN
F 3 "" H 9200 5225 60  0000 C CNN
	1    9200 5225
	1    0    0    -1  
$EndComp
$Comp
L appli_transistor:BC847B Q?
U 1 1 5AD0E4D5
P 9100 4925
F 0 "Q?" H 9295 4963 40  0000 L CNN
F 1 "BC847B" H 9295 4887 40  0000 L CNN
F 2 "SOT95P240X110-3L16*" H 9025 5025 29  0001 R CNN
F 3 "http://www.farnell.com/datasheets/1685010.pdf" V 9325 4925 20  0001 C CNN
F 4 "Multicomp" V 9375 4925 20  0001 C CNN "manf"
F 5 "BC847B" V 9400 4925 20  0001 C CNN "manf#"
F 6 "Element14" V 9425 4925 20  0001 C CNN "Supplier"
F 7 "1574343" V 9450 4925 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/bc847b/transistor-npn-45v-100ma-sot-23/dp/1574343" V 9475 4925 20  0001 C CNN "Supplier URL"
F 9 "0.26" V 9500 4925 20  0001 C CNN "Supplier Price"
F 10 "10" V 9525 4925 20  0001 C CNN "Supplier Price Break"
	1    9100 4925
	1    0    0    -1  
$EndComp
$Comp
L appli_resistor:1K00_1608M R?
U 1 1 5AD0E62C
P 7950 4925
F 0 "R?" V 7762 4775 50  0000 C CNN
F 1 "1K00_1608M" V 7842 4775 35  0000 C CNN
F 2 "RESC1608*" V 8060 4775 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 8085 4775 20  0001 C CNN
F 4 "MULTICOMP" V 8135 4775 20  0001 C CNN "manf"
F 5 "MCHP03W8F1001T5E" V 8160 4775 20  0001 C CNN "manf#"
F 6 "Element14" V 8185 4775 20  0001 C CNN "Supplier"
F 7 "1576283" V 8210 4775 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mchp03w8f1001t5e/resistor-thick-film-1k-0-125w/dp/1576283" V 8235 4775 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 8260 4775 20  0001 C CNN "Supplier Price"
F 10 "50+" V 8285 4775 20  0001 C CNN "Supplier Price Break"
	1    7950 4925
	0    1    1    0   
$EndComp
Wire Wire Line
	7575 4925 7650 4925
Wire Wire Line
	9200 5125 9200 5225
$Comp
L appli_power:GND #PWR?
U 1 1 5AD0F172
P 10100 4250
F 0 "#PWR?" H 10100 4000 50  0001 C CNN
F 1 "GND" H 10105 4077 50  0000 C CNN
F 2 "" H 10100 4250 60  0000 C CNN
F 3 "" H 10100 4250 60  0000 C CNN
	1    10100 4250
	1    0    0    -1  
$EndComp
$Comp
L appli_connector_wurth:CONN_01X04 P?
U 1 1 5AD0F38F
P 10350 3975
F 0 "P?" H 10428 4016 50  0000 L CNN
F 1 "CONN_01X04" H 10428 3925 50  0000 L CNN
F 2 "Applidyne_Connector_Wurth:61300411121" V 10525 3975 50  0001 C CNN
F 3 "http://www.farnell.com/cad/2124416.pdf" H 10350 3650 50  0001 C CNN
F 4 "WURTH" H 10350 3525 60  0001 C CNN "manf"
F 5 "61300411121" H 10350 3425 60  0001 C CNN "manf#"
F 6 "Element 14" H 10350 3325 60  0001 C CNN "Supplier"
F 7 "2356155" H 10350 3250 60  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/wurth-elektronik/61300411121/header-2-54mm-pin-tht-vertical/dp/2356155" H 10350 3150 60  0001 C CNN "Supplier URL"
F 9 "0.125" H 10350 3050 60  0001 C CNN "Supplier Price"
F 10 "1" H 10350 2950 60  0001 C CNN "Supplier Price Break"
	1    10350 3975
	1    0    0    -1  
$EndComp
Wire Wire Line
	10150 3825 10100 3825
Wire Wire Line
	10100 3825 10100 4250
$Comp
L appli_device:BAV99 D?
U 1 1 5AD10A17
P 8200 4025
F 0 "D?" H 8308 4071 50  0000 L CNN
F 1 "BAV99" H 8308 3980 50  0000 L CNN
F 2 "SOT23" H 8400 3875 20  0001 C CNN
F 3 "http://datasheet.octopart.com/BAV99-/T3-NXP-datasheet-5318834.pdf" H 8400 3850 20  0001 C CNN
F 4 "NXP" H 8400 3800 20  0001 C CNN "manf"
F 5 "RS Online" H 8400 3750 20  0001 C CNN "Supplier"
F 6 " 544-4534" H 8400 3725 20  0001 C CNN "Supplier Part No"
F 7 "http://au.rs-online.com/web/p/rectifier-schottky-diodes/5444534/" H 8400 3700 20  0001 C CNN "Supplier URL"
F 8 "0.045" H 8400 3675 20  0001 C CNN "Supplier Price"
F 9 "1" H 8400 3650 20  0001 C CNN "Supplier Price Break"
F 10 "BAV99" H 8400 3775 20  0001 C CNN "manf#"
	1    8200 4025
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5AD10F65
P 8200 4525
F 0 "#PWR?" H 8200 4275 50  0001 C CNN
F 1 "GND" H 8205 4352 50  0000 C CNN
F 2 "" H 8200 4525 60  0000 C CNN
F 3 "" H 8200 4525 60  0000 C CNN
	1    8200 4525
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5AD11030
P 8200 3725
F 0 "#PWR?" H 8200 3685 30  0001 C CNN
F 1 "+3.3V" H 8209 3863 30  0000 C CNN
F 2 "" H 8200 3725 60  0000 C CNN
F 3 "" H 8200 3725 60  0000 C CNN
	1    8200 3725
	1    0    0    -1  
$EndComp
Wire Wire Line
	8200 3725 8200 3775
Wire Wire Line
	8200 4275 8200 4475
$Comp
L appli_resistor:1K00_1608M R?
U 1 1 5AD11EAB
P 7950 4025
F 0 "R?" V 7762 3875 50  0000 C CNN
F 1 "1K00_1608M" V 7842 3875 35  0000 C CNN
F 2 "RESC1608*" V 8060 3875 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 8085 3875 20  0001 C CNN
F 4 "MULTICOMP" V 8135 3875 20  0001 C CNN "manf"
F 5 "MCHP03W8F1001T5E" V 8160 3875 20  0001 C CNN "manf#"
F 6 "Element14" V 8185 3875 20  0001 C CNN "Supplier"
F 7 "1576283" V 8210 3875 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mchp03w8f1001t5e/resistor-thick-film-1k-0-125w/dp/1576283" V 8235 3875 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 8260 3875 20  0001 C CNN "Supplier Price"
F 10 "50+" V 8285 3875 20  0001 C CNN "Supplier Price Break"
	1    7950 4025
	0    1    1    0   
$EndComp
Wire Wire Line
	7575 4025 7650 4025
Wire Wire Line
	7950 4025 8200 4025
$Comp
L appli_capacitor:22UF_X7R_3225M C?
U 1 1 5AD13D98
P 10275 5150
F 0 "C?" H 10197 5015 50  0000 R CNN
F 1 "22UF_X7R_3225M" H 10197 5095 35  0000 R CNN
F 2 "CAPC3225*" V 10390 5050 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2111999.pdf?_ga=1.157172477.774814437.1442284889" V 10415 5050 20  0001 C CNN
F 4 "MURATA" V 10465 5050 20  0001 C CNN "manf"
F 5 "C1210C226K3RACTU" V 10490 5050 20  0001 C CNN "manf#"
F 6 "Element14" V 10515 5050 20  0001 C CNN "Supplier"
F 7 "2473550" V 10540 5050 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c1210c226k3ractu/cap-mlcc-x7r-22uf-25v-1210/dp/2473550" V 10565 5050 20  0001 C CNN "Supplier URL"
F 9 "1.02" V 10590 5050 20  0001 C CNN "Supplier Price"
F 10 "50" V 10615 5050 20  0001 C CNN "Supplier Price Break"
	1    10275 5150
	1    0    0    1   
$EndComp
$Comp
L appli_power:+12V #PWR?
U 1 1 5AD13E6B
P 10275 4850
F 0 "#PWR?" H 10275 4700 50  0001 C CNN
F 1 "+12V" H 10290 5023 50  0000 C CNN
F 2 "" H 10275 4850 60  0000 C CNN
F 3 "" H 10275 4850 60  0000 C CNN
	1    10275 4850
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5AD13E92
P 10500 5300
F 0 "#PWR?" H 10500 5050 50  0001 C CNN
F 1 "GND" H 10505 5127 50  0000 C CNN
F 2 "" H 10500 5300 60  0000 C CNN
F 3 "" H 10500 5300 60  0000 C CNN
	1    10500 5300
	1    0    0    -1  
$EndComp
$Comp
L appli_capacitor:100NF_X7R_1608M C?
U 1 1 5AD14F52
P 10500 4950
F 0 "C?" H 10578 4885 50  0000 L CNN
F 1 "100NF_X7R_1608M" H 10578 4805 35  0000 L CNN
F 2 "CAPC1608*" V 10615 4850 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1732728.pdf" V 10640 4850 20  0001 C CNN
F 4 "KEMET" V 10690 4850 20  0001 C CNN "manf"
F 5 "C0603C104K5RACAUTO" V 10715 4850 20  0001 C CNN "manf#"
F 6 "Element14" V 10740 4850 20  0001 C CNN "Supplier"
F 7 "2070398" V 10765 4850 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c0603c104k5racauto/cap-mlcc-x7r-100nf-50v-0603/dp/2070398" V 10790 4850 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 10815 4850 20  0001 C CNN "Supplier Price"
F 10 "1" V 10840 4850 20  0001 C CNN "Supplier Price Break"
	1    10500 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	10275 4950 10275 4900
Wire Wire Line
	10275 5150 10275 5200
Wire Wire Line
	10275 5200 10500 5200
Wire Wire Line
	10500 5200 10500 5150
Wire Wire Line
	10500 4950 10500 4900
Wire Wire Line
	10500 4900 10275 4900
Connection ~ 10275 4900
Wire Wire Line
	10275 4900 10275 4850
Wire Wire Line
	10500 5300 10500 5200
Connection ~ 10500 5200
Text Notes 9675 5425 0    50   ~ 0
Bypass caps near \nfan connector
$Comp
L appli_resistor:4K7_1608M R?
U 1 1 5AD1ED64
P 9450 3925
F 0 "R?" H 9377 3740 50  0000 R CNN
F 1 "4K7_1608M" H 9377 3820 35  0000 R CNN
F 2 "RESC1608*" V 9560 3775 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1788326.pdf" V 9585 3775 20  0001 C CNN
F 4 "MULTICOMP" V 9635 3775 20  0001 C CNN "manf"
F 5 "MC0063W060354K7" V 9660 3775 20  0001 C CNN "manf#"
F 6 "Element14" V 9685 3775 20  0001 C CNN "Supplier"
F 7 "9332154" V 9710 3775 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mc0063w060354k7/res-thick-film-4k7-5-0-063w-0603/dp/9332154" V 9735 3775 20  0001 C CNN "Supplier URL"
F 9 "0.024" V 9760 3775 20  0001 C CNN "Supplier Price"
F 10 "1" V 9785 3775 20  0001 C CNN "Supplier Price Break"
	1    9450 3925
	1    0    0    1   
$EndComp
Wire Wire Line
	9450 3925 9450 4025
Wire Wire Line
	9450 4025 10150 4025
Wire Wire Line
	9050 4025 9450 4025
Connection ~ 9450 4025
Wire Wire Line
	8750 4025 8650 4025
Connection ~ 8200 4025
Wire Wire Line
	9600 3525 9600 3575
Wire Wire Line
	9450 3625 9450 3575
Wire Wire Line
	9450 3575 9600 3575
Connection ~ 9600 3575
Wire Wire Line
	9600 3575 9600 3925
Wire Wire Line
	8650 4025 8650 4100
Connection ~ 8650 4025
Wire Wire Line
	8650 4025 8200 4025
$Comp
L appli_resistor:10K0_1608M R?
U 1 1 5AD365D7
P 8650 4100
F 0 "R?" H 8723 3985 50  0000 L CNN
F 1 "10K0_1608M" H 8723 3905 35  0000 L CNN
F 2 "RESC1608*" V 8760 3950 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 8785 3950 20  0001 C CNN
F 4 "TE CONNECTIVITY" V 8835 3950 20  0001 C CNN "manf"
F 5 "CRGH0603F10K" V 8860 3950 20  0001 C CNN "manf#"
F 6 "Element14" V 8885 3950 20  0001 C CNN "Supplier"
F 7 "2332016" V 8910 3950 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/te-connectivity/crgh0603f10k/resistor-power-10k-0-2w-1-0603/dp/2332016" V 8935 3950 20  0001 C CNN "Supplier URL"
F 9 "0.02" V 8960 3950 20  0001 C CNN "Supplier Price"
F 10 "10" V 8985 3950 20  0001 C CNN "Supplier Price Break"
	1    8650 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	8650 4400 8650 4475
Wire Wire Line
	8650 4475 8200 4475
Connection ~ 8200 4475
Wire Wire Line
	8200 4475 8200 4525
Wire Wire Line
	9200 4125 10150 4125
Wire Wire Line
	9200 4125 9200 4725
Wire Wire Line
	8900 4925 7950 4925
$Comp
L appli_resistor:33K2_1608M R?
U 1 1 5AD500A2
P 9050 4025
F 0 "R?" V 8862 3875 50  0000 C CNN
F 1 "33K2_1608M" V 8942 3875 35  0000 C CNN
F 2 "RESC1608*" V 9160 3875 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2051174.pdf?_ga=1.145166194.774814437.1442284889" V 9185 3875 20  0001 C CNN
F 4 "PANASONIC " V 9235 3875 20  0001 C CNN "manf"
F 5 "ERJPA3F3322V" V 9260 3875 20  0001 C CNN "manf#"
F 6 "Element14" V 9285 3875 20  0001 C CNN "Supplier"
F 7 "2561973" V 9310 3875 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/panasonic-electronic-components/erjpa3f3322v/res-aec-q200-thick-film-33k2-0603/dp/2561973" V 9335 3875 20  0001 C CNN "Supplier URL"
F 9 "0.2" V 9360 3875 20  0001 C CNN "Supplier Price"
F 10 "1" V 9385 3875 20  0001 C CNN "Supplier Price Break"
	1    9050 4025
	0    1    1    0   
$EndComp
Text Notes 6850 3700 0    50   ~ 0
Tachometer uses 12V pullup.\n2.3:1 divider on tach.\nBAV99 prevents input damage.\n
Text Notes 6850 4625 0    50   ~ 0
Drive PWM at 22-25kHz.
$Comp
L appli_fuse:500mA_1210L_PTC F?
U 1 1 5AD5A6A5
P 10050 3925
F 0 "F?" V 9800 3775 50  0000 C CNN
F 1 "500mA_1210L_PTC" V 9875 3775 35  0000 C CNN
F 2 "Applidyne_Fuse:FUSC3225X85L50N" V 10160 3775 20  0001 C CNN
F 3 "http://docs-asia.electrocomponents.com/webdocs/1420/0900766b8142082b.pdf" V 10185 3775 20  0001 C CNN
F 4 "LittelFuse" V 10235 3775 20  0001 C CNN "manf"
F 5 " 1210L050YR" V 10260 3775 20  0001 C CNN "manf#"
F 6 "RS Online" V 10285 3775 20  0001 C CNN "Supplier"
F 7 "787-4202" V 10310 3775 20  0001 C CNN "Supplier Part No"
F 8 "http://au.rs-online.com/web/p/resettable-surface-mount-fuses/7874202/" V 10335 3775 20  0001 C CNN "Supplier URL"
F 9 "0.486" V 10360 3775 20  0001 C CNN "Supplier Price"
F 10 "10" V 10385 3775 20  0001 C CNN "Supplier Price Break"
	1    10050 3925
	0    1    1    0   
$EndComp
Wire Wire Line
	10150 3925 10050 3925
Wire Wire Line
	9750 3925 9600 3925
Text Notes 7200 5825 0    118  ~ 24
TODO pullup resistor on PWM control
Text Notes 5850 6225 0    50   ~ 0
https://electronics.stackexchange.com/questions/153846/arduino-how-to-read-and-control-the-speed-of-a-12v-four-wire-fan
$EndSCHEMATC
