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
$EndSCHEMATC
