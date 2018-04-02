EESchema Schematic File Version 4
LIBS:zaphod-controller-cache
EELAYER 26 0
EELAYER END
$Descr A3 16535 11693
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
Text Notes 12925 1425 0    79   ~ 16
Input Voltage Monitoring
Text Notes 1850 3125 0    79   ~ 16
12V Regulation
Text Notes 1825 5050 0    79   ~ 16
3.3V Regulation
Text Notes 13375 8600 0    79   ~ 16
Testpoints
$Comp
L appli_test_points:CLIP_HOOK_BLACK TEST?
U 1 1 5AB8D09C
P 14550 9100
F 0 "TEST?" H 14591 9169 39  0000 L CNN
F 1 "CLIP_HOOK_BLACK" H 14600 9100 39  0001 L TNN
F 2 "Applidyne_Test:KEYSTONE_5001" H 14600 9050 39  0001 L TNN
F 3 "http://www.farnell.com/datasheets/1703983.pdf" H 14600 8900 39  0001 L BNN
F 4 "KEYSTONE" H 14600 9000 39  0001 L TNN "manf"
F 5 "5001" H 14600 8850 39  0001 L BNN "manf#"
F 6 "Element14" H 14600 8800 39  0001 L BNN "Supplier"
F 7 "2301278" H 14600 8750 39  0001 L BNN "Supplier Part No"
F 8 "http://au.element14.com/keystone/5001/test-point-pcb-through-hole/dp/2301278" H 14600 8700 39  0001 L BNN "Supplier URL"
F 9 "0.218" H 14600 8650 39  0001 L BNN "Supplier Price"
F 10 "100" H 14600 8600 39  0001 L BNN "Suppier Price Break"
	1    14550 9100
	1    0    0    -1  
$EndComp
$Comp
L appli_test_points:CLIP_HOOK_RED TEST?
U 1 1 5AB8D25D
P 12850 9225
F 0 "TEST?" H 12672 9293 39  0000 R CNN
F 1 "CLIP_HOOK_RED" H 12900 9225 39  0001 L TNN
F 2 "Applidyne_Test:KEYSTONE_5000" H 12900 9175 39  0001 L TNN
F 3 "http://www.farnell.com/datasheets/1703983.pdf" H 12900 9025 39  0001 L BNN
F 4 "KEYSTONE" H 12900 9125 39  0001 L TNN "manf"
F 5 "5000" H 12900 8975 39  0001 L BNN "manf#"
F 6 "Element14" H 12900 8925 39  0001 L BNN "Supplier"
F 7 "1463076" H 12900 8875 39  0001 L BNN "Supplier Part No"
F 8 "http://au.element14.com/keystone/5000/test-point-pcb-through-hole/dp/1463076" H 12900 8825 39  0001 L BNN "Supplier URL"
F 9 "0.23" H 12900 8775 39  0001 L BNN "Supplier Price"
F 10 "100" H 12900 8725 39  0001 L BNN "Suppier Price Break"
	1    12850 9225
	-1   0    0    1   
$EndComp
$Comp
L appli_power:GND #PWR0104
U 1 1 5AB8D36F
P 14550 9225
F 0 "#PWR0104" H 14550 8975 50  0001 C CNN
F 1 "GND" H 14555 9052 50  0000 C CNN
F 2 "" H 14550 9225 60  0000 C CNN
F 3 "" H 14550 9225 60  0000 C CNN
	1    14550 9225
	1    0    0    -1  
$EndComp
Wire Wire Line
	14550 9100 14550 9225
$Comp
L appli_power:+POWER #PWR0105
U 1 1 5AB8D4D9
P 12850 9100
F 0 "#PWR0105" H 12850 9050 20  0001 C CNN
F 1 "+POWER" H 12853 9238 30  0000 C CNN
F 2 "" H 12850 9100 60  0000 C CNN
F 3 "" H 12850 9100 60  0000 C CNN
	1    12850 9100
	1    0    0    -1  
$EndComp
Wire Wire Line
	12850 9225 12850 9100
Text Notes 12675 8850 0    50   ~ 0
75V INPUT
Text Notes 14450 8850 0    50   ~ 0
GND
Wire Wire Line
	13450 9225 13450 9100
Text Notes 13300 8850 0    50   ~ 0
12V REG
$Comp
L appli_test_points:CLIP_HOOK_YELLOW TEST?
U 1 1 5AB8D795
P 13450 9225
F 0 "TEST?" H 13272 9293 39  0000 R CNN
F 1 "CLIP_HOOK_YELLOW" H 13500 9225 39  0001 L TNN
F 2 "Applidyne_Test:KEYSTONE_5004" H 13500 9175 39  0001 L TNN
F 3 "http://www.farnell.com/datasheets/1703983.pdf" H 13500 9025 39  0001 L BNN
F 4 "KEYSTONE" H 13500 9125 39  0001 L TNN "manf"
F 5 "5004" H 13500 8975 39  0001 L BNN "manf#"
F 6 "Element14" H 13500 8925 39  0001 L BNN "Supplier"
F 7 "2301281" H 13500 8875 39  0001 L BNN "Supplier Part No"
F 8 "http://au.element14.com/keystone/5004/test-point-pcb-through-hole/dp/2301281" H 13500 8825 39  0001 L BNN "Supplier URL"
F 9 "0.238" H 13500 8775 39  0001 L BNN "Supplier Price"
F 10 "100" H 13500 8725 39  0001 L BNN "Suppier Price Break"
	1    13450 9225
	-1   0    0    1   
$EndComp
Wire Wire Line
	13975 9225 13975 9100
Text Notes 13825 8850 0    50   ~ 0
3V3 REG
$Comp
L appli_test_points:CLIP_HOOK_YELLOW TEST?
U 1 1 5AB8D90E
P 13975 9225
F 0 "TEST?" H 13797 9293 39  0000 R CNN
F 1 "CLIP_HOOK_YELLOW" H 14025 9225 39  0001 L TNN
F 2 "Applidyne_Test:KEYSTONE_5004" H 14025 9175 39  0001 L TNN
F 3 "http://www.farnell.com/datasheets/1703983.pdf" H 14025 9025 39  0001 L BNN
F 4 "KEYSTONE" H 14025 9125 39  0001 L TNN "manf"
F 5 "5004" H 14025 8975 39  0001 L BNN "manf#"
F 6 "Element14" H 14025 8925 39  0001 L BNN "Supplier"
F 7 "2301281" H 14025 8875 39  0001 L BNN "Supplier Part No"
F 8 "http://au.element14.com/keystone/5004/test-point-pcb-through-hole/dp/2301281" H 14025 8825 39  0001 L BNN "Supplier URL"
F 9 "0.238" H 14025 8775 39  0001 L BNN "Supplier Price"
F 10 "100" H 14025 8725 39  0001 L BNN "Suppier Price Break"
	1    13975 9225
	-1   0    0    1   
$EndComp
$Comp
L appli_power:+3.3V #PWR0106
U 1 1 5AB8D9EC
P 13975 9100
F 0 "#PWR0106" H 13975 9060 30  0001 C CNN
F 1 "+3.3V" H 13984 9238 30  0000 C CNN
F 2 "" H 13975 9100 60  0000 C CNN
F 3 "" H 13975 9100 60  0000 C CNN
	1    13975 9100
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+12V #PWR0107
U 1 1 5AB8DA40
P 13450 9100
F 0 "#PWR0107" H 13450 8950 50  0001 C CNN
F 1 "+12V" H 13450 9250 30  0000 C CNN
F 2 "" H 13450 9100 60  0000 C CNN
F 3 "" H 13450 9100 60  0000 C CNN
	1    13450 9100
	1    0    0    -1  
$EndComp
Text Notes 1275 1650 0    50   ~ 0
SABRE 2-pin connector
Text Notes 3000 1650 0    50   ~ 0
Polyfuse
$Comp
L appli_resistor:130K_1608M R?
U 1 1 5ABC59BC
P 12775 2125
F 0 "R?" H 12703 1940 50  0000 R CNN
F 1 "130K_1608M" H 12703 2020 35  0000 R CNN
F 2 "RESC1608*" V 12885 1975 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2059623.pdf" V 12910 1975 20  0001 C CNN
F 4 "PANASONIC" V 12960 1975 20  0001 C CNN "manf"
F 5 "ERJ3GEYJ134V" V 12985 1975 20  0001 C CNN "manf#"
F 6 "Element14" V 13010 1975 20  0001 C CNN "Supplier"
F 7 "2059658" V 13035 1975 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/panasonic-electronic-components/erj3geyj134v/res-thick-film-130k-5-0-1w-0603/dp/2059658" V 13060 1975 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 13085 1975 20  0001 C CNN "Supplier Price"
F 10 "50" V 13110 1975 20  0001 C CNN "Supplier Price Break"
	1    12775 2125
	1    0    0    1   
$EndComp
$Comp
L appli_resistor:130K_1608M R?
U 1 1 5ABC5A8A
P 12775 2525
F 0 "R?" H 12703 2340 50  0000 R CNN
F 1 "130K_1608M" H 12703 2420 35  0000 R CNN
F 2 "RESC1608*" V 12885 2375 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2059623.pdf" V 12910 2375 20  0001 C CNN
F 4 "PANASONIC" V 12960 2375 20  0001 C CNN "manf"
F 5 "ERJ3GEYJ134V" V 12985 2375 20  0001 C CNN "manf#"
F 6 "Element14" V 13010 2375 20  0001 C CNN "Supplier"
F 7 "2059658" V 13035 2375 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/panasonic-electronic-components/erj3geyj134v/res-thick-film-130k-5-0-1w-0603/dp/2059658" V 13060 2375 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 13085 2375 20  0001 C CNN "Supplier Price"
F 10 "50" V 13110 2375 20  0001 C CNN "Supplier Price Break"
	1    12775 2525
	1    0    0    1   
$EndComp
$Comp
L appli_resistor:10K0_1608M R?
U 1 1 5ABC5BF5
P 12775 2925
F 0 "R?" H 12702 2810 50  0000 R CNN
F 1 "10K0_1608M" H 12702 2730 35  0000 R CNN
F 2 "RESC1608*" V 12885 2775 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 12910 2775 20  0001 C CNN
F 4 "TE CONNECTIVITY" V 12960 2775 20  0001 C CNN "manf"
F 5 "CRGH0603F10K" V 12985 2775 20  0001 C CNN "manf#"
F 6 "Element14" V 13010 2775 20  0001 C CNN "Supplier"
F 7 "2332016" V 13035 2775 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/te-connectivity/crgh0603f10k/resistor-power-10k-0-2w-1-0603/dp/2332016" V 13060 2775 20  0001 C CNN "Supplier URL"
F 9 "0.02" V 13085 2775 20  0001 C CNN "Supplier Price"
F 10 "10" V 13110 2775 20  0001 C CNN "Supplier Price Break"
	1    12775 2925
	1    0    0    1   
$EndComp
$Comp
L appli_capacitor:100NF_X7R_1608M C?
U 1 1 5ABC5DA8
P 13125 2675
F 0 "C?" H 13203 2610 50  0000 L CNN
F 1 "100NF_X7R_1608M" H 13203 2530 35  0000 L CNN
F 2 "CAPC1608*" V 13240 2575 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1732728.pdf" V 13265 2575 20  0001 C CNN
F 4 "KEMET" V 13315 2575 20  0001 C CNN "manf"
F 5 "C0603C104K5RACAUTO" V 13340 2575 20  0001 C CNN "manf#"
F 6 "Element14" V 13365 2575 20  0001 C CNN "Supplier"
F 7 "2070398" V 13390 2575 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c0603c104k5racauto/cap-mlcc-x7r-100nf-50v-0603/dp/2070398" V 13415 2575 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 13440 2575 20  0001 C CNN "Supplier Price"
F 10 "1" V 13465 2575 20  0001 C CNN "Supplier Price Break"
	1    13125 2675
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+POWER #PWR0108
U 1 1 5ABC5F0E
P 12775 1775
F 0 "#PWR0108" H 12775 1725 20  0001 C CNN
F 1 "+POWER" H 12778 1913 30  0000 C CNN
F 2 "" H 12775 1775 60  0000 C CNN
F 3 "" H 12775 1775 60  0000 C CNN
	1    12775 1775
	1    0    0    -1  
$EndComp
Wire Wire Line
	12775 1775 12775 1825
Wire Wire Line
	12775 2125 12775 2225
Wire Wire Line
	12775 2525 12775 2575
Wire Wire Line
	13125 2675 13125 2575
Wire Wire Line
	13125 2575 12775 2575
Connection ~ 12775 2575
Wire Wire Line
	12775 2575 12775 2625
Wire Wire Line
	12775 2925 12775 2975
$Comp
L appli_power:GND #PWR0109
U 1 1 5ABC61AD
P 12775 3025
F 0 "#PWR0109" H 12775 2775 50  0001 C CNN
F 1 "GND" H 12780 2852 50  0000 C CNN
F 2 "" H 12775 3025 60  0000 C CNN
F 3 "" H 12775 3025 60  0000 C CNN
	1    12775 3025
	1    0    0    -1  
$EndComp
Wire Wire Line
	13125 2875 13125 2975
Wire Wire Line
	13125 2975 12775 2975
Connection ~ 12775 2975
Wire Wire Line
	12775 2975 12775 3025
Wire Wire Line
	13125 2575 13775 2575
Connection ~ 13125 2575
$Comp
L appli_device:BAV99 D?
U 1 1 5ABC7E1F
P 13775 2575
F 0 "D?" H 13883 2621 50  0000 L CNN
F 1 "BAV99" H 13883 2530 50  0000 L CNN
F 2 "SOT23" H 13975 2425 20  0001 C CNN
F 3 "http://datasheet.octopart.com/BAV99-/T3-NXP-datasheet-5318834.pdf" H 13975 2400 20  0001 C CNN
F 4 "NXP" H 13975 2350 20  0001 C CNN "manf"
F 5 "RS Online" H 13975 2300 20  0001 C CNN "Supplier"
F 6 " 544-4534" H 13975 2275 20  0001 C CNN "Supplier Part No"
F 7 "http://au.rs-online.com/web/p/rectifier-schottky-diodes/5444534/" H 13975 2250 20  0001 C CNN "Supplier URL"
F 8 "0.045" H 13975 2225 20  0001 C CNN "Supplier Price"
F 9 "1" H 13975 2200 20  0001 C CNN "Supplier Price Break"
F 10 "BAV99" H 13975 2325 20  0001 C CNN "manf#"
	1    13775 2575
	1    0    0    -1  
$EndComp
Wire Wire Line
	13775 2825 13775 3025
Wire Wire Line
	13775 2325 13775 2250
$Comp
L appli_power:GND #PWR0110
U 1 1 5ABC81D7
P 13775 3025
F 0 "#PWR0110" H 13775 2775 50  0001 C CNN
F 1 "GND" H 13780 2852 50  0000 C CNN
F 2 "" H 13775 3025 60  0000 C CNN
F 3 "" H 13775 3025 60  0000 C CNN
	1    13775 3025
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+3.3V #PWR0111
U 1 1 5ABC8212
P 13775 2250
F 0 "#PWR0111" H 13775 2210 30  0001 C CNN
F 1 "+3.3V" H 13784 2388 30  0000 C CNN
F 2 "" H 13775 2250 60  0000 C CNN
F 3 "" H 13775 2250 60  0000 C CNN
	1    13775 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	13775 2575 14200 2575
Connection ~ 13775 2575
$Comp
L appli_resistor:1K00_1608M R?
U 1 1 5ABC8785
P 14500 2575
F 0 "R?" V 14312 2425 50  0000 C CNN
F 1 "1K00_1608M" V 14392 2425 35  0000 C CNN
F 2 "RESC1608*" V 14610 2425 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 14635 2425 20  0001 C CNN
F 4 "MULTICOMP" V 14685 2425 20  0001 C CNN "manf"
F 5 "MCHP03W8F1001T5E" V 14710 2425 20  0001 C CNN "manf#"
F 6 "Element14" V 14735 2425 20  0001 C CNN "Supplier"
F 7 "1576283" V 14760 2425 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mchp03w8f1001t5e/resistor-thick-film-1k-0-125w/dp/1576283" V 14785 2425 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 14810 2425 20  0001 C CNN "Supplier Price"
F 10 "50+" V 14835 2425 20  0001 C CNN "Supplier Price Break"
	1    14500 2575
	0    1    1    0   
$EndComp
Wire Wire Line
	14500 2575 14650 2575
Text GLabel 14650 2575 2    50   Output ~ 0
VOLTAGE_INPUT
Text Notes 13275 1925 0    50   ~ 0
Teknic IPC-3 outputs 75VDC nominal.\nDivider is 27:1, so 75V = 2.78.\nProvides enough overhead for sensing 90VDC.
Text Notes 14650 2775 0    50   ~ 0
27:1 ratio
$Comp
L appli_power:+12V #PWR0112
U 1 1 5ABE329D
P 3350 3575
F 0 "#PWR0112" H 3350 3425 50  0001 C CNN
F 1 "+12V" H 3365 3748 50  0000 C CNN
F 2 "" H 3350 3575 60  0000 C CNN
F 3 "" H 3350 3575 60  0000 C CNN
	1    3350 3575
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+12V #PWR0113
U 1 1 5ABE32C3
P 1050 5625
F 0 "#PWR0113" H 1050 5475 50  0001 C CNN
F 1 "+12V" H 1065 5798 50  0000 C CNN
F 2 "" H 1050 5625 60  0000 C CNN
F 3 "" H 1050 5625 60  0000 C CNN
	1    1050 5625
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+3.3V #PWR0114
U 1 1 5ABE331C
P 3700 5625
F 0 "#PWR0114" H 3700 5585 30  0001 C CNN
F 1 "+3.3V" H 3709 5763 30  0000 C CNN
F 2 "" H 3700 5625 60  0000 C CNN
F 3 "" H 3700 5625 60  0000 C CNN
	1    3700 5625
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+POWER #PWR0115
U 1 1 5ABE3398
P 4300 2075
F 0 "#PWR0115" H 4300 2025 20  0001 C CNN
F 1 "+POWER" H 4303 2213 30  0000 C CNN
F 2 "" H 4300 2075 60  0000 C CNN
F 3 "" H 4300 2075 60  0000 C CNN
	1    4300 2075
	1    0    0    -1  
$EndComp
$EndSCHEMATC
