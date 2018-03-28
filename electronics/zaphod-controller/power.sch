EESchema Schematic File Version 4
LIBS:zaphod-controller-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 5
Title "Power"
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 1800 1150 0    79   ~ 16
75V Input Power
Text Notes 8400 1175 0    79   ~ 16
Input Voltage Monitoring
Text Notes 1850 3125 0    79   ~ 16
12V Regulation
Text Notes 1825 5050 0    79   ~ 16
3.3V Regulation
Text Notes 8800 5075 0    79   ~ 16
Testpoints
$Comp
L appli_test_points:CLIP_HOOK_BLACK TEST?
U 1 1 5AB8D09C
P 9975 5575
F 0 "TEST?" H 10016 5644 39  0000 L CNN
F 1 "CLIP_HOOK_BLACK" H 10025 5575 39  0001 L TNN
F 2 "Applidyne_Test:KEYSTONE_5001" H 10025 5525 39  0001 L TNN
F 3 "http://www.farnell.com/datasheets/1703983.pdf" H 10025 5375 39  0001 L BNN
F 4 "KEYSTONE" H 10025 5475 39  0001 L TNN "manf"
F 5 "5001" H 10025 5325 39  0001 L BNN "manf#"
F 6 "Element14" H 10025 5275 39  0001 L BNN "Supplier"
F 7 "2301278" H 10025 5225 39  0001 L BNN "Supplier Part No"
F 8 "http://au.element14.com/keystone/5001/test-point-pcb-through-hole/dp/2301278" H 10025 5175 39  0001 L BNN "Supplier URL"
F 9 "0.218" H 10025 5125 39  0001 L BNN "Supplier Price"
F 10 "100" H 10025 5075 39  0001 L BNN "Suppier Price Break"
	1    9975 5575
	1    0    0    -1  
$EndComp
$Comp
L appli_test_points:CLIP_HOOK_RED TEST?
U 1 1 5AB8D25D
P 8275 5700
F 0 "TEST?" H 8097 5768 39  0000 R CNN
F 1 "CLIP_HOOK_RED" H 8325 5700 39  0001 L TNN
F 2 "Applidyne_Test:KEYSTONE_5000" H 8325 5650 39  0001 L TNN
F 3 "http://www.farnell.com/datasheets/1703983.pdf" H 8325 5500 39  0001 L BNN
F 4 "KEYSTONE" H 8325 5600 39  0001 L TNN "manf"
F 5 "5000" H 8325 5450 39  0001 L BNN "manf#"
F 6 "Element14" H 8325 5400 39  0001 L BNN "Supplier"
F 7 "1463076" H 8325 5350 39  0001 L BNN "Supplier Part No"
F 8 "http://au.element14.com/keystone/5000/test-point-pcb-through-hole/dp/1463076" H 8325 5300 39  0001 L BNN "Supplier URL"
F 9 "0.23" H 8325 5250 39  0001 L BNN "Supplier Price"
F 10 "100" H 8325 5200 39  0001 L BNN "Suppier Price Break"
	1    8275 5700
	-1   0    0    1   
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5AB8D36F
P 9975 5700
F 0 "#PWR?" H 9975 5450 50  0001 C CNN
F 1 "GND" H 9980 5527 50  0000 C CNN
F 2 "" H 9975 5700 60  0000 C CNN
F 3 "" H 9975 5700 60  0000 C CNN
	1    9975 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	9975 5575 9975 5700
$Comp
L appli_power:+POWER #PWR?
U 1 1 5AB8D4D9
P 8275 5575
F 0 "#PWR?" H 8275 5525 20  0001 C CNN
F 1 "+POWER" H 8278 5713 30  0000 C CNN
F 2 "" H 8275 5575 60  0000 C CNN
F 3 "" H 8275 5575 60  0000 C CNN
	1    8275 5575
	1    0    0    -1  
$EndComp
Wire Wire Line
	8275 5700 8275 5575
Text Notes 8100 5325 0    50   ~ 0
75V INPUT
Text Notes 9875 5325 0    50   ~ 0
GND
Wire Wire Line
	8875 5700 8875 5575
Text Notes 8725 5325 0    50   ~ 0
12V REG
$Comp
L appli_test_points:CLIP_HOOK_YELLOW TEST?
U 1 1 5AB8D795
P 8875 5700
F 0 "TEST?" H 8697 5768 39  0000 R CNN
F 1 "CLIP_HOOK_YELLOW" H 8925 5700 39  0001 L TNN
F 2 "Applidyne_Test:KEYSTONE_5004" H 8925 5650 39  0001 L TNN
F 3 "http://www.farnell.com/datasheets/1703983.pdf" H 8925 5500 39  0001 L BNN
F 4 "KEYSTONE" H 8925 5600 39  0001 L TNN "manf"
F 5 "5004" H 8925 5450 39  0001 L BNN "manf#"
F 6 "Element14" H 8925 5400 39  0001 L BNN "Supplier"
F 7 "2301281" H 8925 5350 39  0001 L BNN "Supplier Part No"
F 8 "http://au.element14.com/keystone/5004/test-point-pcb-through-hole/dp/2301281" H 8925 5300 39  0001 L BNN "Supplier URL"
F 9 "0.238" H 8925 5250 39  0001 L BNN "Supplier Price"
F 10 "100" H 8925 5200 39  0001 L BNN "Suppier Price Break"
	1    8875 5700
	-1   0    0    1   
$EndComp
Wire Wire Line
	9400 5700 9400 5575
Text Notes 9250 5325 0    50   ~ 0
3V3 REG
$Comp
L appli_test_points:CLIP_HOOK_YELLOW TEST?
U 1 1 5AB8D90E
P 9400 5700
F 0 "TEST?" H 9222 5768 39  0000 R CNN
F 1 "CLIP_HOOK_YELLOW" H 9450 5700 39  0001 L TNN
F 2 "Applidyne_Test:KEYSTONE_5004" H 9450 5650 39  0001 L TNN
F 3 "http://www.farnell.com/datasheets/1703983.pdf" H 9450 5500 39  0001 L BNN
F 4 "KEYSTONE" H 9450 5600 39  0001 L TNN "manf"
F 5 "5004" H 9450 5450 39  0001 L BNN "manf#"
F 6 "Element14" H 9450 5400 39  0001 L BNN "Supplier"
F 7 "2301281" H 9450 5350 39  0001 L BNN "Supplier Part No"
F 8 "http://au.element14.com/keystone/5004/test-point-pcb-through-hole/dp/2301281" H 9450 5300 39  0001 L BNN "Supplier URL"
F 9 "0.238" H 9450 5250 39  0001 L BNN "Supplier Price"
F 10 "100" H 9450 5200 39  0001 L BNN "Suppier Price Break"
	1    9400 5700
	-1   0    0    1   
$EndComp
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5AB8D9EC
P 9400 5575
F 0 "#PWR?" H 9400 5535 30  0001 C CNN
F 1 "+3.3V" H 9409 5713 30  0000 C CNN
F 2 "" H 9400 5575 60  0000 C CNN
F 3 "" H 9400 5575 60  0000 C CNN
	1    9400 5575
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+12V #PWR?
U 1 1 5AB8DA40
P 8875 5575
F 0 "#PWR?" H 8875 5425 50  0001 C CNN
F 1 "+12V" H 8875 5725 30  0000 C CNN
F 2 "" H 8875 5575 60  0000 C CNN
F 3 "" H 8875 5575 60  0000 C CNN
	1    8875 5575
	1    0    0    -1  
$EndComp
Text Notes 1275 1650 0    50   ~ 0
SABRE 2-pin connector
Text Notes 3000 1650 0    50   ~ 0
Polyfuse
$Comp
L appli_resistor:130K_1608M R?
U 1 1 5ABC59BC
P 8250 1875
F 0 "R?" H 8178 1690 50  0000 R CNN
F 1 "130K_1608M" H 8178 1770 35  0000 R CNN
F 2 "RESC1608*" V 8360 1725 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2059623.pdf" V 8385 1725 20  0001 C CNN
F 4 "PANASONIC" V 8435 1725 20  0001 C CNN "manf"
F 5 "ERJ3GEYJ134V" V 8460 1725 20  0001 C CNN "manf#"
F 6 "Element14" V 8485 1725 20  0001 C CNN "Supplier"
F 7 "2059658" V 8510 1725 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/panasonic-electronic-components/erj3geyj134v/res-thick-film-130k-5-0-1w-0603/dp/2059658" V 8535 1725 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 8560 1725 20  0001 C CNN "Supplier Price"
F 10 "50" V 8585 1725 20  0001 C CNN "Supplier Price Break"
	1    8250 1875
	1    0    0    1   
$EndComp
$Comp
L appli_resistor:130K_1608M R?
U 1 1 5ABC5A8A
P 8250 2275
F 0 "R?" H 8178 2090 50  0000 R CNN
F 1 "130K_1608M" H 8178 2170 35  0000 R CNN
F 2 "RESC1608*" V 8360 2125 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2059623.pdf" V 8385 2125 20  0001 C CNN
F 4 "PANASONIC" V 8435 2125 20  0001 C CNN "manf"
F 5 "ERJ3GEYJ134V" V 8460 2125 20  0001 C CNN "manf#"
F 6 "Element14" V 8485 2125 20  0001 C CNN "Supplier"
F 7 "2059658" V 8510 2125 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/panasonic-electronic-components/erj3geyj134v/res-thick-film-130k-5-0-1w-0603/dp/2059658" V 8535 2125 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 8560 2125 20  0001 C CNN "Supplier Price"
F 10 "50" V 8585 2125 20  0001 C CNN "Supplier Price Break"
	1    8250 2275
	1    0    0    1   
$EndComp
$Comp
L appli_resistor:10K0_1608M R?
U 1 1 5ABC5BF5
P 8250 2675
F 0 "R?" H 8177 2560 50  0000 R CNN
F 1 "10K0_1608M" H 8177 2480 35  0000 R CNN
F 2 "RESC1608*" V 8360 2525 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 8385 2525 20  0001 C CNN
F 4 "TE CONNECTIVITY" V 8435 2525 20  0001 C CNN "manf"
F 5 "CRGH0603F10K" V 8460 2525 20  0001 C CNN "manf#"
F 6 "Element14" V 8485 2525 20  0001 C CNN "Supplier"
F 7 "2332016" V 8510 2525 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/te-connectivity/crgh0603f10k/resistor-power-10k-0-2w-1-0603/dp/2332016" V 8535 2525 20  0001 C CNN "Supplier URL"
F 9 "0.02" V 8560 2525 20  0001 C CNN "Supplier Price"
F 10 "10" V 8585 2525 20  0001 C CNN "Supplier Price Break"
	1    8250 2675
	1    0    0    1   
$EndComp
$Comp
L appli_capacitor:100NF_X7R_1608M C?
U 1 1 5ABC5DA8
P 8600 2425
F 0 "C?" H 8678 2360 50  0000 L CNN
F 1 "100NF_X7R_1608M" H 8678 2280 35  0000 L CNN
F 2 "CAPC1608*" V 8715 2325 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1732728.pdf" V 8740 2325 20  0001 C CNN
F 4 "KEMET" V 8790 2325 20  0001 C CNN "manf"
F 5 "C0603C104K5RACAUTO" V 8815 2325 20  0001 C CNN "manf#"
F 6 "Element14" V 8840 2325 20  0001 C CNN "Supplier"
F 7 "2070398" V 8865 2325 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c0603c104k5racauto/cap-mlcc-x7r-100nf-50v-0603/dp/2070398" V 8890 2325 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 8915 2325 20  0001 C CNN "Supplier Price"
F 10 "1" V 8940 2325 20  0001 C CNN "Supplier Price Break"
	1    8600 2425
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+POWER #PWR?
U 1 1 5ABC5F0E
P 8250 1525
F 0 "#PWR?" H 8250 1475 20  0001 C CNN
F 1 "+POWER" H 8253 1663 30  0000 C CNN
F 2 "" H 8250 1525 60  0000 C CNN
F 3 "" H 8250 1525 60  0000 C CNN
	1    8250 1525
	1    0    0    -1  
$EndComp
Wire Wire Line
	8250 1525 8250 1575
Wire Wire Line
	8250 1875 8250 1975
Wire Wire Line
	8250 2275 8250 2325
Wire Wire Line
	8600 2425 8600 2325
Wire Wire Line
	8600 2325 8250 2325
Connection ~ 8250 2325
Wire Wire Line
	8250 2325 8250 2375
Wire Wire Line
	8250 2675 8250 2725
$Comp
L appli_power:GND #PWR?
U 1 1 5ABC61AD
P 8250 2775
F 0 "#PWR?" H 8250 2525 50  0001 C CNN
F 1 "GND" H 8255 2602 50  0000 C CNN
F 2 "" H 8250 2775 60  0000 C CNN
F 3 "" H 8250 2775 60  0000 C CNN
	1    8250 2775
	1    0    0    -1  
$EndComp
Wire Wire Line
	8600 2625 8600 2725
Wire Wire Line
	8600 2725 8250 2725
Connection ~ 8250 2725
Wire Wire Line
	8250 2725 8250 2775
Wire Wire Line
	8600 2325 9250 2325
Connection ~ 8600 2325
$Comp
L appli_device:BAV99 D?
U 1 1 5ABC7E1F
P 9250 2325
F 0 "D?" H 9358 2371 50  0000 L CNN
F 1 "BAV99" H 9358 2280 50  0000 L CNN
F 2 "SOT23" H 9450 2175 20  0001 C CNN
F 3 "http://datasheet.octopart.com/BAV99-/T3-NXP-datasheet-5318834.pdf" H 9450 2150 20  0001 C CNN
F 4 "NXP" H 9450 2100 20  0001 C CNN "manf"
F 5 "RS Online" H 9450 2050 20  0001 C CNN "Supplier"
F 6 " 544-4534" H 9450 2025 20  0001 C CNN "Supplier Part No"
F 7 "http://au.rs-online.com/web/p/rectifier-schottky-diodes/5444534/" H 9450 2000 20  0001 C CNN "Supplier URL"
F 8 "0.045" H 9450 1975 20  0001 C CNN "Supplier Price"
F 9 "1" H 9450 1950 20  0001 C CNN "Supplier Price Break"
F 10 "BAV99" H 9450 2075 20  0001 C CNN "manf#"
	1    9250 2325
	1    0    0    -1  
$EndComp
Wire Wire Line
	9250 2575 9250 2775
Wire Wire Line
	9250 2075 9250 2000
$Comp
L appli_power:GND #PWR?
U 1 1 5ABC81D7
P 9250 2775
F 0 "#PWR?" H 9250 2525 50  0001 C CNN
F 1 "GND" H 9255 2602 50  0000 C CNN
F 2 "" H 9250 2775 60  0000 C CNN
F 3 "" H 9250 2775 60  0000 C CNN
	1    9250 2775
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5ABC8212
P 9250 2000
F 0 "#PWR?" H 9250 1960 30  0001 C CNN
F 1 "+3.3V" H 9259 2138 30  0000 C CNN
F 2 "" H 9250 2000 60  0000 C CNN
F 3 "" H 9250 2000 60  0000 C CNN
	1    9250 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	9250 2325 9675 2325
Connection ~ 9250 2325
$Comp
L appli_resistor:1K00_1608M R?
U 1 1 5ABC8785
P 9975 2325
F 0 "R?" V 9787 2175 50  0000 C CNN
F 1 "1K00_1608M" V 9867 2175 35  0000 C CNN
F 2 "RESC1608*" V 10085 2175 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 10110 2175 20  0001 C CNN
F 4 "MULTICOMP" V 10160 2175 20  0001 C CNN "manf"
F 5 "MCHP03W8F1001T5E" V 10185 2175 20  0001 C CNN "manf#"
F 6 "Element14" V 10210 2175 20  0001 C CNN "Supplier"
F 7 "1576283" V 10235 2175 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mchp03w8f1001t5e/resistor-thick-film-1k-0-125w/dp/1576283" V 10260 2175 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 10285 2175 20  0001 C CNN "Supplier Price"
F 10 "50+" V 10310 2175 20  0001 C CNN "Supplier Price Break"
	1    9975 2325
	0    1    1    0   
$EndComp
Wire Wire Line
	9975 2325 10125 2325
Text GLabel 10125 2325 2    50   Output ~ 0
VOLTAGE_INPUT
Text Notes 8750 1675 0    50   ~ 0
Teknic IPC-3 outputs 75VDC nominal.\nDivider is 27:1, so 75V = 2.78.\nProvides enough overhead for sensing 90VDC.
Text Notes 10125 2525 0    50   ~ 0
27:1 ratio
$EndSCHEMATC
