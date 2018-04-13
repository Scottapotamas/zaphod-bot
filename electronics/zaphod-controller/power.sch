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
Text Notes 1850 3350 0    79   ~ 16
12V Regulation
Text Notes 1950 7825 0    79   ~ 16
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
Text Notes 1075 1450 0    50   ~ 0
SABRE 2-pin connector
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
P 8875 4950
F 0 "#PWR0112" H 8875 4800 50  0001 C CNN
F 1 "+12V" H 8890 5123 50  0000 C CNN
F 2 "" H 8875 4950 60  0000 C CNN
F 3 "" H 8875 4950 60  0000 C CNN
	1    8875 4950
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+12V #PWR0113
U 1 1 5ABE32C3
P 1175 8400
F 0 "#PWR0113" H 1175 8250 50  0001 C CNN
F 1 "+12V" H 1190 8573 50  0000 C CNN
F 2 "" H 1175 8400 60  0000 C CNN
F 3 "" H 1175 8400 60  0000 C CNN
	1    1175 8400
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+3.3V #PWR0114
U 1 1 5ABE331C
P 3825 8400
F 0 "#PWR0114" H 3825 8360 30  0001 C CNN
F 1 "+3.3V" H 3834 8538 30  0000 C CNN
F 2 "" H 3825 8400 60  0000 C CNN
F 3 "" H 3825 8400 60  0000 C CNN
	1    3825 8400
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+POWER #PWR0115
U 1 1 5ABE3398
P 4000 1550
F 0 "#PWR0115" H 4000 1500 20  0001 C CNN
F 1 "+POWER" H 4003 1688 30  0000 C CNN
F 2 "" H 4000 1550 60  0000 C CNN
F 3 "" H 4000 1550 60  0000 C CNN
	1    4000 1550
	1    0    0    -1  
$EndComp
$Comp
L appli_capacitor:100NF_X7R_1608M C?
U 1 1 5B03FF12
P 4100 6500
F 0 "C?" H 4023 6365 50  0000 R CNN
F 1 "100NF_X7R_1608M" H 4023 6445 35  0000 R CNN
F 2 "CAPC1608*" V 4215 6400 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1732728.pdf" V 4240 6400 20  0001 C CNN
F 4 "KEMET" V 4290 6400 20  0001 C CNN "manf"
F 5 "C0603C104K5RACAUTO" V 4315 6400 20  0001 C CNN "manf#"
F 6 "Element14" V 4340 6400 20  0001 C CNN "Supplier"
F 7 "2070398" V 4365 6400 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c0603c104k5racauto/cap-mlcc-x7r-100nf-50v-0603/dp/2070398" V 4390 6400 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 4415 6400 20  0001 C CNN "Supplier Price"
F 10 "1" V 4440 6400 20  0001 C CNN "Supplier Price Break"
	1    4100 6500
	1    0    0    1   
$EndComp
$Comp
L appli_capacitor:1UF_X7R_2012M C?
U 1 1 5B040200
P 6400 4900
F 0 "C?" H 6322 4765 50  0000 R CNN
F 1 "1UF_X7R_2012M" H 6322 4845 35  0000 R CNN
F 2 "CAPC2012*" V 6515 4800 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2047889.pdf" V 6540 4800 20  0001 C CNN
F 4 "MURATA" V 6590 4800 20  0001 C CNN "manf"
F 5 "GRM21BR71C105KA01L" V 6615 4800 20  0001 C CNN "manf#"
F 6 "Element14" V 6640 4800 20  0001 C CNN "Supplier"
F 7 "9527710" V 6665 4800 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/murata/grm21br71c105ka01l/cap-mlcc-x7r-1uf-16v-0805/dp/9527710" V 6690 4800 20  0001 C CNN "Supplier URL"
F 9 "0.224" V 6715 4800 20  0001 C CNN "Supplier Price"
F 10 "1" V 6740 4800 20  0001 C CNN "Supplier Price Break"
	1    6400 4900
	1    0    0    1   
$EndComp
$Comp
L appli_capacitor:330PF_X7R_1608M C?
U 1 1 5B040331
P 8225 6100
F 0 "C?" H 8303 6035 50  0000 L CNN
F 1 "330PF_X7R_1608M" H 8303 5955 35  0000 L CNN
F 2 "CAPC1608*" V 8340 6000 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2068140.pdf" V 8365 6000 20  0001 C CNN
F 4 "AVX" V 8415 6000 20  0001 C CNN "manf"
F 5 "06035A331JAT2A" V 8440 6000 20  0001 C CNN "manf#"
F 6 "Element14" V 8465 6000 20  0001 C CNN "Supplier"
F 7 "316660" V 8490 6000 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/avx/06035a331jat2a/cap-mlcc-c0g-np0-330pf-50v-0603/dp/316660" V 8515 6000 20  0001 C CNN "Supplier URL"
F 9 "0.21" V 8540 6000 20  0001 C CNN "Supplier Price"
F 10 "10" V 8565 6000 20  0001 C CNN "Supplier Price Break"
	1    8225 6100
	1    0    0    -1  
$EndComp
$Comp
L appli_capacitor:1UF_X7R_1608M C?
U 1 1 5B040728
P 2550 4975
F 0 "C?" H 2472 4840 50  0000 R CNN
F 1 "1UF_X7R_1608M" H 2472 4920 35  0000 R CNN
F 2 "CAPC1608*" V 2665 4875 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2048611.pdf?_ga=1.116411968.774814437.1442284889" V 2690 4875 20  0001 C CNN
F 4 "MURATA" V 2740 4875 20  0001 C CNN "manf"
F 5 "GRM188R61H105KAALD" V 2765 4875 20  0001 C CNN "manf#"
F 6 "Element14" V 2790 4875 20  0001 C CNN "Supplier"
F 7 "1845736" V 2815 4875 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/murata/grm188r61h105kaald/cap-mlcc-x5r-1uf-50v-0603/dp/1845736" V 2840 4875 20  0001 C CNN "Supplier URL"
F 9 "0.092" V 2865 4875 20  0001 C CNN "Supplier Price"
F 10 "100" V 2890 4875 20  0001 C CNN "Supplier Price Break"
	1    2550 4975
	1    0    0    1   
$EndComp
$Comp
L appli_capacitor:2.2UF_X7R_1608M C?
U 1 1 5B0408F7
P 10375 5050
F 0 "C?" H 10453 4985 50  0000 L CNN
F 1 "2.2UF_X7R_1608M" H 10453 4905 35  0000 L CNN
F 2 "CAPC1608*" V 10490 4950 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2079169.pdf?_ga=1.116328000.774814437.1442284889" V 10515 4950 20  0001 C CNN
F 4 "MURATA" V 10565 4950 20  0001 C CNN "manf"
F 5 "GRM188R61H225KE11D" V 10590 4950 20  0001 C CNN "manf#"
F 6 "Element14" V 10615 4950 20  0001 C CNN "Supplier"
F 7 "2611925" V 10640 4950 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/murata/grm188r61h225ke11d/cap-mlcc-x5r-2-2uf-50v-0603/dp/2611925" V 10665 4950 20  0001 C CNN "Supplier URL"
F 9 "0.0256" V 10690 4950 20  0001 C CNN "Supplier Price"
F 10 "10" V 10715 4950 20  0001 C CNN "Supplier Price Break"
	1    10375 5050
	1    0    0    -1  
$EndComp
Text Notes 9925 4800 0    50   ~ 0
100 v rated part required
Text Notes 8125 4725 0    50   ~ 0
47uF electro
$Comp
L appli_capacitor:22UF_X7R_3225M C?
U 1 1 5B040B76
P 9425 5925
F 0 "C?" H 9503 5860 50  0000 L CNN
F 1 "22UF_X7R_3225M" H 9503 5780 35  0000 L CNN
F 2 "CAPC3225*" V 9540 5825 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2111999.pdf?_ga=1.157172477.774814437.1442284889" V 9565 5825 20  0001 C CNN
F 4 "MURATA" V 9615 5825 20  0001 C CNN "manf"
F 5 "C1210C226K3RACTU" V 9640 5825 20  0001 C CNN "manf#"
F 6 "Element14" V 9665 5825 20  0001 C CNN "Supplier"
F 7 "2473550" V 9690 5825 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c1210c226k3ractu/cap-mlcc-x7r-22uf-25v-1210/dp/2473550" V 9715 5825 20  0001 C CNN "Supplier URL"
F 9 "1.02" V 9740 5825 20  0001 C CNN "Supplier Price"
F 10 "50" V 9765 5825 20  0001 C CNN "Supplier Price Break"
	1    9425 5925
	1    0    0    -1  
$EndComp
$Comp
L appli_resistor:0R_1608M R?
U 1 1 5B040E00
P 6850 5700
F 0 "R?" V 6662 5550 50  0000 C CNN
F 1 "0R_1608M" V 6742 5550 35  0000 C CNN
F 2 "RESC1608*" V 6960 5550 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1739504.pdf" V 6985 5550 20  0001 C CNN
F 4 "MULTICOMP" V 7035 5550 20  0001 C CNN "manf"
F 5 "MC0063W06030R" V 7060 5550 20  0001 C CNN "manf#"
F 6 "Element14" V 7085 5550 20  0001 C CNN "Supplier"
F 7 "9331662" V 7110 5550 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mc0063w06030r/resistor-thick-film-0r-0-063w/dp/9331662" V 7135 5550 20  0001 C CNN "Supplier URL"
F 9 "0.023" V 7160 5550 20  0001 C CNN "Supplier Price"
F 10 "50" V 7185 5550 20  0001 C CNN "Supplier Price Break"
	1    6850 5700
	0    1    1    0   
$EndComp
$Comp
L appli_resistor:10K0_1608M R?
U 1 1 5B040F7F
P 6850 6300
F 0 "R?" V 6662 6150 50  0000 C CNN
F 1 "10K0_1608M" V 6742 6150 35  0000 C CNN
F 2 "RESC1608*" V 6960 6150 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 6985 6150 20  0001 C CNN
F 4 "TE CONNECTIVITY" V 7035 6150 20  0001 C CNN "manf"
F 5 "CRGH0603F10K" V 7060 6150 20  0001 C CNN "manf#"
F 6 "Element14" V 7085 6150 20  0001 C CNN "Supplier"
F 7 "2332016" V 7110 6150 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/te-connectivity/crgh0603f10k/resistor-power-10k-0-2w-1-0603/dp/2332016" V 7135 6150 20  0001 C CNN "Supplier URL"
F 9 "0.02" V 7160 6150 20  0001 C CNN "Supplier Price"
F 10 "10" V 7185 6150 20  0001 C CNN "Supplier Price Break"
	1    6850 6300
	0    1    1    0   
$EndComp
$Comp
L appli_resistor:39K_1608M R?
U 1 1 5B041168
P 10100 7500
F 0 "R?" H 10173 7385 50  0000 L CNN
F 1 "39K_1608M" H 10173 7305 35  0000 L CNN
F 2 "RESC1608*" V 10210 7350 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2310790.pdf" V 10235 7350 20  0001 C CNN
F 4 "PANASONIC" V 10285 7350 20  0001 C CNN "manf"
F 5 "MCMR06X2202FTL" V 10310 7350 20  0001 C CNN "manf#"
F 6 "Element14" V 10335 7350 20  0001 C CNN "Supplier"
F 7 "1469804" V 10360 7350 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/vishay/crcw060339k0fkea/res-thick-film-39k-1-0-1w-0603/dp/1469804" V 10385 7350 20  0001 C CNN "Supplier URL"
F 9 "0.176" V 10410 7350 20  0001 C CNN "Supplier Price"
F 10 "1" V 10435 7350 20  0001 C CNN "Supplier Price Break"
	1    10100 7500
	1    0    0    -1  
$EndComp
$Comp
L appli_resistor:1M_1608M R?
U 1 1 5B0412F7
P 1700 4800
F 0 "R?" H 1627 4615 50  0000 R CNN
F 1 "1M_1608M" H 1627 4695 35  0000 R CNN
F 2 "RESC1608*" V 1810 4650 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1720486.pdf" V 1835 4650 20  0001 C CNN
F 4 "VISHAY DRALORIC" V 1885 4650 20  0001 C CNN "manf"
F 5 "CRCW06031M00FKEA" V 1910 4650 20  0001 C CNN "manf#"
F 6 "Element14" V 1935 4650 20  0001 C CNN "Supplier"
F 7 "1469746" V 1960 4650 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/vishay-draloric/crcw06031m00fkea/product-range-aec-q200-crcw-series/dp/1469746" V 1985 4650 20  0001 C CNN "Supplier URL"
F 9 "0.02" V 2010 4650 20  0001 C CNN "Supplier Price"
F 10 "100" V 2035 4650 20  0001 C CNN "Supplier Price Break"
	1    1700 4800
	-1   0    0    1   
$EndComp
$Comp
L appli_capacitor:1UF_X7R_2012M C?
U 1 1 5B044635
P 5400 6025
F 0 "C?" H 5478 5960 50  0000 L CNN
F 1 "1UF_X7R_2012M" H 5478 5880 35  0000 L CNN
F 2 "CAPC2012*" V 5515 5925 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2047889.pdf" V 5540 5925 20  0001 C CNN
F 4 "MURATA" V 5590 5925 20  0001 C CNN "manf"
F 5 "GRM21BR71C105KA01L" V 5615 5925 20  0001 C CNN "manf#"
F 6 "Element14" V 5640 5925 20  0001 C CNN "Supplier"
F 7 "9527710" V 5665 5925 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/murata/grm21br71c105ka01l/cap-mlcc-x7r-1uf-16v-0805/dp/9527710" V 5690 5925 20  0001 C CNN "Supplier URL"
F 9 "0.224" V 5715 5925 20  0001 C CNN "Supplier Price"
F 10 "1" V 5740 5925 20  0001 C CNN "Supplier Price Break"
	1    5400 6025
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 5750 5400 5750
Wire Wire Line
	5400 5750 5400 6025
Wire Wire Line
	5400 6225 5400 6525
$Comp
L appli_power:GND #PWR?
U 1 1 5B047597
P 5400 6525
F 0 "#PWR?" H 5400 6275 50  0001 C CNN
F 1 "GND" H 5405 6352 50  0000 C CNN
F 2 "" H 5400 6525 60  0000 C CNN
F 3 "" H 5400 6525 60  0000 C CNN
	1    5400 6525
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5B0475D7
P 4500 6525
F 0 "#PWR?" H 4500 6275 50  0001 C CNN
F 1 "GND" H 4505 6352 50  0000 C CNN
F 2 "" H 4500 6525 60  0000 C CNN
F 3 "" H 4500 6525 60  0000 C CNN
	1    4500 6525
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 6050 4400 6100
Wire Wire Line
	4400 6100 4500 6100
Wire Wire Line
	4500 6100 4500 6050
Wire Wire Line
	4500 6100 4600 6100
Wire Wire Line
	4600 6100 4600 6050
Connection ~ 4500 6100
Wire Wire Line
	4500 6100 4500 6525
Wire Wire Line
	4100 6300 4100 6050
Wire Wire Line
	4200 6050 4200 6250
Wire Wire Line
	4200 6250 4750 6250
Wire Wire Line
	4100 6500 4100 6525
$Comp
L appli_power:GND #PWR?
U 1 1 5B049899
P 4100 6525
F 0 "#PWR?" H 4100 6275 50  0001 C CNN
F 1 "GND" H 4105 6352 50  0000 C CNN
F 2 "" H 4100 6525 60  0000 C CNN
F 3 "" H 4100 6525 60  0000 C CNN
	1    4100 6525
	1    0    0    -1  
$EndComp
Text Notes 4350 6600 1    50   ~ 0
330pF
Text Notes 4800 6275 0    50   ~ 0
750k
Wire Wire Line
	5025 6250 5250 6250
$Comp
L appli_resistor:12K7_1608M R?
U 1 1 5B04EAEE
P 1700 5675
F 0 "R?" H 1627 5490 50  0000 R CNN
F 1 "12K7_1608M" H 1627 5570 35  0000 R CNN
F 2 "Applidyne_Resistor:RESC1608X50N" V 1810 5525 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1911175.pdf" V 1835 5525 20  0001 C CNN
F 4 "PANASONIC" V 1885 5525 20  0001 C CNN "manf"
F 5 "ERJ3EKF1272V" V 1910 5525 20  0001 C CNN "manf#"
F 6 "Element14" V 1935 5525 20  0001 C CNN "Supplier"
F 7 "2059415" V 1960 5525 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/panasonic-electronic-components/erj3ekf1272v/res-thick-film-12k7-1-0-1w-0603/dp/2059415" V 1985 5525 20  0001 C CNN "Supplier URL"
F 9 "0.014" V 2010 5525 20  0001 C CNN "Supplier Price"
F 10 "50" V 2035 5525 20  0001 C CNN "Supplier Price Break"
	1    1700 5675
	-1   0    0    1   
$EndComp
Text Notes 2800 4575 0    50   ~ 0
102k
Text Notes 2825 4925 0    50   ~ 0
9.3k
Wire Wire Line
	2900 4750 2900 4625
Wire Wire Line
	2900 4750 2900 4825
Connection ~ 2900 4750
Wire Wire Line
	2900 4400 2900 4350
Wire Wire Line
	2900 4350 3600 4350
Text Notes 2600 4700 1    50   ~ 0
diode >
Connection ~ 2900 4350
Wire Wire Line
	2900 4750 2550 4750
Wire Wire Line
	2550 4750 2550 4775
Wire Wire Line
	2900 5000 2900 5100
Wire Wire Line
	2550 4975 2550 5025
Wire Wire Line
	3600 5050 3350 5050
Wire Wire Line
	3350 5050 3350 5150
Wire Wire Line
	3350 5150 1700 5150
Wire Wire Line
	1700 5150 1700 4800
Wire Wire Line
	1700 5250 1700 5375
Text Notes 2875 5700 0    50   ~ 0
39k
Text Notes 2800 6100 0    50   ~ 0
1500pF
Text Notes 2525 5875 0    50   ~ 0
15pf
Text Notes 2875 6500 0    50   ~ 0
1.2k
Text Notes 3375 6025 0    50   ~ 0
10.7k
Wire Wire Line
	2950 6275 2950 6175
Wire Wire Line
	2950 6275 2950 6350
Connection ~ 2950 6275
Wire Wire Line
	2950 6600 2950 6650
Wire Wire Line
	2950 5925 2950 5850
Wire Wire Line
	2950 5550 2950 5450
Wire Wire Line
	2950 5450 3600 5450
Wire Wire Line
	2950 5450 2600 5450
Wire Wire Line
	2600 5450 2600 5700
Connection ~ 2950 5450
Wire Wire Line
	2600 6000 2600 6275
Wire Wire Line
	2600 6275 2950 6275
$Comp
L appli_power:GND #PWR?
U 1 1 5B06CC85
P 2950 6650
F 0 "#PWR?" H 2950 6400 50  0001 C CNN
F 1 "GND" H 2955 6477 50  0000 C CNN
F 2 "" H 2950 6650 60  0000 C CNN
F 3 "" H 2950 6650 60  0000 C CNN
	1    2950 6650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3475 5750 3475 5850
Wire Wire Line
	3475 6125 3475 6275
$Comp
L appli_regulator:LM5116 U?
U 1 1 5B07707D
P 4400 5050
F 0 "U?" H 4400 6017 50  0000 C CNN
F 1 "LM5116" H 4400 5926 50  0000 C CNN
F 2 "Applidyne_SOP:SOP21P65_720X780X200L75X30N" H 4725 4150 20  0001 L CNN
F 3 "http://www.ti.com/lit/ds/snvs499h/snvs499h.pdf" H 4725 4125 20  0001 L CNN
F 4 "TEXAS INSTRUMENTS" H 4725 4075 20  0001 L CNN "manf"
F 5 "LM5116MHX" H 4725 4050 20  0001 L CNN "manf#"
F 6 "Digikey" H 4725 4025 20  0001 L CNN "Supplier"
F 7 "LM5116MHX/NOPBCT-ND" H 4725 4000 20  0001 L CNN "Supplier Part No"
F 8 "https://www.digikey.com.au/product-detail/en/LM5116MHX-NOPB/LM5116MHX-NOPBCT-ND/3440164" H 4725 3975 20  0001 L CNN "Supplier URL"
F 9 "7.79" H 4725 3950 20  0001 L CNN "Supplier Price"
F 10 "1" H 4725 3925 20  0001 L CNN "Supplier Price Break"
	1    4400 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 5250 3600 5250
Wire Wire Line
	2950 6275 3200 6275
Wire Wire Line
	3475 5750 3600 5750
Wire Wire Line
	3600 5550 3200 5550
Wire Wire Line
	3200 5550 3200 6275
Connection ~ 3200 6275
Wire Wire Line
	3200 6275 3475 6275
$Comp
L appli_power:+12V #PWR?
U 1 1 5B089A4B
P 3475 5725
F 0 "#PWR?" H 3475 5575 50  0001 C CNN
F 1 "+12V" H 3325 5775 50  0000 C CNN
F 2 "" H 3475 5725 60  0000 C CNN
F 3 "" H 3475 5725 60  0000 C CNN
	1    3475 5725
	1    0    0    -1  
$EndComp
Wire Wire Line
	3475 5725 3475 5750
Connection ~ 3475 5750
Text Notes 5600 5775 0    50   ~ 0
1R
Text Notes 5625 4375 0    50   ~ 0
diode >
Wire Wire Line
	6400 4650 6400 4350
Wire Wire Line
	5200 4650 5500 4650
Text Notes 5600 4675 0    50   ~ 0
2.2k
Wire Wire Line
	6400 4700 6400 4650
Connection ~ 6400 4650
Wire Wire Line
	6400 4900 6400 4950
Wire Wire Line
	6400 4350 6000 4350
$Comp
L appli_transistor:2N7002P Q?
U 1 1 5B0B7AAF
P 7200 4700
F 0 "Q?" H 7391 4746 50  0000 L CNN
F 1 "2N7002P" H 7391 4655 50  0000 L CNN
F 2 "SOT23" H 7400 4600 50  0001 L CIN
F 3 "http://docs-asia.electrocomponents.com/webdocs/1385/0900766b813856ab.pdf" H 5570 4430 50  0001 L CNN
F 4 "Fairchild" H 7200 4200 60  0001 C CNN "manf"
F 5 " 2N7002,215" H 7200 4080 60  0001 C CNN "manf#"
F 6 "RS Online" H 7200 3980 60  0001 C CNN "Supplier"
F 7 "http://au.rs-online.com/web/p/mosfet-transistors/4367379/" H 7200 3860 60  0001 C CNN "Supplier URL"
F 8 " 436-7379" H 7200 3730 60  0001 C CNN "Supplier Part No"
F 9 "0.232" H 7200 3610 60  0001 C CNN "Supplier Price"
F 10 "25" H 7200 3480 60  0001 C CNN "Supplier Price Break"
	1    7200 4700
	1    0    0    -1  
$EndComp
Text Notes 7475 4525 0    50   ~ 0
CHANGE
Wire Wire Line
	5200 4950 6400 4950
Wire Wire Line
	6400 4950 7300 4950
Connection ~ 6400 4950
Wire Wire Line
	5850 4650 6400 4650
Text Notes 7950 4975 0    50   ~ 0
inductor
Wire Wire Line
	8275 4950 8875 4950
Text Notes 8475 5175 0    50   ~ 0
caps
$Comp
L appli_transistor:2N7002P Q?
U 1 1 5B0CB451
P 7200 5200
F 0 "Q?" H 7391 5246 50  0000 L CNN
F 1 "2N7002P" H 7391 5155 50  0000 L CNN
F 2 "SOT23" H 7400 5100 50  0001 L CIN
F 3 "http://docs-asia.electrocomponents.com/webdocs/1385/0900766b813856ab.pdf" H 5570 4930 50  0001 L CNN
F 4 "Fairchild" H 7200 4700 60  0001 C CNN "manf"
F 5 " 2N7002,215" H 7200 4580 60  0001 C CNN "manf#"
F 6 "RS Online" H 7200 4480 60  0001 C CNN "Supplier"
F 7 "http://au.rs-online.com/web/p/mosfet-transistors/4367379/" H 7200 4360 60  0001 C CNN "Supplier URL"
F 8 " 436-7379" H 7200 4230 60  0001 C CNN "Supplier Part No"
F 9 "0.232" H 7200 4110 60  0001 C CNN "Supplier Price"
F 10 "25" H 7200 3980 60  0001 C CNN "Supplier Price Break"
	1    7200 5200
	1    0    0    -1  
$EndComp
Text Notes 7550 5175 0    50   ~ 0
CHANGE
Text Notes 7200 5875 0    50   ~ 0
0.01R
Wire Wire Line
	7300 5400 7300 5700
Wire Wire Line
	7050 5700 6850 5700
Wire Wire Line
	7050 6000 6850 6000
$Comp
L appli_resistor:0R_1608M R?
U 1 1 5B0D507E
P 6850 6000
F 0 "R?" V 6662 5850 50  0000 C CNN
F 1 "0R_1608M" V 6742 5850 35  0000 C CNN
F 2 "RESC1608*" V 6960 5850 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1739504.pdf" V 6985 5850 20  0001 C CNN
F 4 "MULTICOMP" V 7035 5850 20  0001 C CNN "manf"
F 5 "MC0063W06030R" V 7060 5850 20  0001 C CNN "manf#"
F 6 "Element14" V 7085 5850 20  0001 C CNN "Supplier"
F 7 "9331662" V 7110 5850 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mc0063w06030r/resistor-thick-film-0r-0-063w/dp/9331662" V 7135 5850 20  0001 C CNN "Supplier URL"
F 9 "0.023" V 7160 5850 20  0001 C CNN "Supplier Price"
F 10 "50" V 7185 5850 20  0001 C CNN "Supplier Price Break"
	1    6850 6000
	0    1    1    0   
$EndComp
Wire Wire Line
	6550 6000 6300 6000
Wire Wire Line
	5200 5250 6400 5250
Wire Wire Line
	6400 5250 6400 5700
Wire Wire Line
	6400 5700 6550 5700
Wire Wire Line
	5200 5350 6300 5350
Wire Wire Line
	6300 5350 6300 6000
Wire Wire Line
	5800 5750 5925 5750
$Comp
L appli_power:+POWER #PWR?
U 1 1 5B0E5346
P 600 4350
F 0 "#PWR?" H 600 4300 20  0001 C CNN
F 1 "+POWER" H 603 4488 30  0000 C CNN
F 2 "" H 600 4350 60  0000 C CNN
F 3 "" H 600 4350 60  0000 C CNN
	1    600  4350
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+POWER #PWR?
U 1 1 5B0E5387
P 7300 4325
F 0 "#PWR?" H 7300 4275 20  0001 C CNN
F 1 "+POWER" H 7303 4463 30  0000 C CNN
F 2 "" H 7300 4325 60  0000 C CNN
F 3 "" H 7300 4325 60  0000 C CNN
	1    7300 4325
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5B0E58BD
P 7300 6525
F 0 "#PWR?" H 7300 6275 50  0001 C CNN
F 1 "GND" H 7305 6352 50  0000 C CNN
F 2 "" H 7300 6525 60  0000 C CNN
F 3 "" H 7300 6525 60  0000 C CNN
	1    7300 6525
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 5450 6200 5450
Wire Wire Line
	6200 5450 6200 6300
Wire Wire Line
	6200 6300 6550 6300
Wire Wire Line
	6850 6300 7300 6300
Wire Wire Line
	7300 6000 7300 6300
Wire Wire Line
	7300 6525 7300 6300
Connection ~ 7300 6300
Wire Wire Line
	6500 5150 6500 5250
Wire Wire Line
	6500 5250 7000 5250
Wire Wire Line
	5200 5150 6500 5150
$Comp
L appli_capacitor:1UF_X7R_2012M C?
U 1 1 5B130DCA
P 5250 6500
F 0 "C?" H 5172 6365 50  0000 R CNN
F 1 "1UF_X7R_2012M" H 5172 6445 35  0000 R CNN
F 2 "CAPC2012*" V 5365 6400 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2047889.pdf" V 5390 6400 20  0001 C CNN
F 4 "MURATA" V 5440 6400 20  0001 C CNN "manf"
F 5 "GRM21BR71C105KA01L" V 5465 6400 20  0001 C CNN "manf#"
F 6 "Element14" V 5490 6400 20  0001 C CNN "Supplier"
F 7 "9527710" V 5515 6400 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/murata/grm21br71c105ka01l/cap-mlcc-x7r-1uf-16v-0805/dp/9527710" V 5540 6400 20  0001 C CNN "Supplier URL"
F 9 "0.224" V 5565 6400 20  0001 C CNN "Supplier Price"
F 10 "1" V 5590 6400 20  0001 C CNN "Supplier Price Break"
	1    5250 6500
	1    0    0    1   
$EndComp
Wire Wire Line
	5200 4350 5250 4350
Wire Wire Line
	5250 6300 5250 6250
Connection ~ 5250 6250
$Comp
L appli_power:GND #PWR?
U 1 1 5B142139
P 5250 6550
F 0 "#PWR?" H 5250 6300 50  0001 C CNN
F 1 "GND" H 5255 6377 50  0000 C CNN
F 2 "" H 5250 6550 60  0000 C CNN
F 3 "" H 5250 6550 60  0000 C CNN
	1    5250 6550
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 5675 1700 6600
Wire Wire Line
	1700 4500 1700 4350
Wire Wire Line
	1700 4350 2900 4350
Wire Wire Line
	1700 4350 1500 4350
Connection ~ 1700 4350
Wire Wire Line
	2900 4750 3600 4750
Wire Wire Line
	7300 5000 7300 4950
Connection ~ 7300 4950
Wire Wire Line
	7300 4950 7900 4950
Wire Wire Line
	7300 4950 7300 4900
$Comp
L appli_power:+12V #PWR?
U 1 1 5B1AC942
P 5925 5750
F 0 "#PWR?" H 5925 5600 50  0001 C CNN
F 1 "+12V" H 5940 5923 50  0000 C CNN
F 2 "" H 5925 5750 60  0000 C CNN
F 3 "" H 5925 5750 60  0000 C CNN
	1    5925 5750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 6250 5250 4350
Connection ~ 5250 4350
Wire Wire Line
	5250 4350 5500 4350
Wire Wire Line
	5250 6550 5250 6500
Text Notes 850  5250 0    50   ~ 0
Input caps
Wire Wire Line
	5400 5750 5500 5750
Connection ~ 5400 5750
Wire Wire Line
	6675 4550 6675 4750
Wire Wire Line
	6675 4750 7000 4750
Wire Wire Line
	5200 4550 6675 4550
Wire Wire Line
	6400 4350 6675 4350
Connection ~ 6400 4350
$Comp
L appli_connector_molex:MOLEX_43160-0102 P?
U 1 1 5AD2F837
P 1400 1900
F 0 "P?" H 1319 2165 50  0000 C CNN
F 1 "MOLEX_43160-0102" H 1319 2074 50  0000 C CNN
F 2 "Applidyne_Connector_Molex:Molex_Sabre_43160-0102_VERT" V 1575 1900 50  0001 C CNN
F 3 "https://www.molex.com/molex/products/datasheet.jsp?part=active/0431600102_PCB_HEADERS.xml" V 2350 1950 50  0001 C CNN
F 4 "MOLEX" V 1675 1925 60  0001 C CNN "manf"
F 5 "043160-0102" V 1775 1925 60  0001 C CNN "manf#"
F 6 "DigiKey" V 1875 1925 60  0001 C CNN "Supplier"
F 7 "WM18473-ND" V 1950 1925 60  0001 C CNN "Supplier Part No"
F 8 "https://www.digikey.com.au/product-detail/en/0431600102/WM18473-ND/300106" V 2050 1925 60  0001 C CNN "Supplier URL"
F 9 "1.66" V 2150 1925 60  0001 C CNN "Supplier Price"
F 10 "1" V 2250 1925 60  0001 C CNN "Supplier Price Break"
	1    1400 1900
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1600 1850 2150 1850
Wire Wire Line
	1600 1950 2000 1950
Wire Wire Line
	2000 1950 2000 2000
$Comp
L appli_power:GND #PWR?
U 1 1 5AD4886E
P 4000 2250
F 0 "#PWR?" H 4000 2000 50  0001 C CNN
F 1 "GND" H 4005 2077 50  0000 C CNN
F 2 "" H 4000 2250 60  0000 C CNN
F 3 "" H 4000 2250 60  0000 C CNN
	1    4000 2250
	1    0    0    -1  
$EndComp
$EndSCHEMATC
