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
P 3850 6500
F 0 "C?" V 3725 6450 50  0000 R CNN
F 1 "100NF_X7R_1608M" V 3975 6775 35  0000 R CNN
F 2 "CAPC1608*" V 3965 6400 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1732728.pdf" V 3990 6400 20  0001 C CNN
F 4 "KEMET" V 4040 6400 20  0001 C CNN "manf"
F 5 "C0603C104K5RACAUTO" V 4065 6400 20  0001 C CNN "manf#"
F 6 "Element14" V 4090 6400 20  0001 C CNN "Supplier"
F 7 "2070398" V 4115 6400 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c0603c104k5racauto/cap-mlcc-x7r-100nf-50v-0603/dp/2070398" V 4140 6400 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 4165 6400 20  0001 C CNN "Supplier Price"
F 10 "1" V 4190 6400 20  0001 C CNN "Supplier Price Break"
	1    3850 6500
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
P 4200 6300
F 0 "C?" V 4075 6150 50  0000 L CNN
F 1 "330PF_X7R_1608M" V 4325 5825 35  0000 L CNN
F 2 "CAPC1608*" V 4315 6200 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2068140.pdf" V 4340 6200 20  0001 C CNN
F 4 "AVX" V 4390 6200 20  0001 C CNN "manf"
F 5 "06035A331JAT2A" V 4415 6200 20  0001 C CNN "manf#"
F 6 "Element14" V 4440 6200 20  0001 C CNN "Supplier"
F 7 "316660" V 4465 6200 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/avx/06035a331jat2a/cap-mlcc-c0g-np0-330pf-50v-0603/dp/316660" V 4490 6200 20  0001 C CNN "Supplier URL"
F 9 "0.21" V 4515 6200 20  0001 C CNN "Supplier Price"
F 10 "10" V 4540 6200 20  0001 C CNN "Supplier Price Break"
	1    4200 6300
	1    0    0    -1  
$EndComp
$Comp
L appli_capacitor:1UF_X7R_1608M C?
U 1 1 5B040728
P 2550 5050
F 0 "C?" H 2472 4915 50  0000 R CNN
F 1 "1UF_X7R_1608M" H 2472 4995 35  0000 R CNN
F 2 "CAPC1608*" V 2665 4950 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2048611.pdf?_ga=1.116411968.774814437.1442284889" V 2690 4950 20  0001 C CNN
F 4 "MURATA" V 2740 4950 20  0001 C CNN "manf"
F 5 "GRM188R61H105KAALD" V 2765 4950 20  0001 C CNN "manf#"
F 6 "Element14" V 2790 4950 20  0001 C CNN "Supplier"
F 7 "1845736" V 2815 4950 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/murata/grm188r61h105kaald/cap-mlcc-x5r-1uf-50v-0603/dp/1845736" V 2840 4950 20  0001 C CNN "Supplier URL"
F 9 "0.092" V 2865 4950 20  0001 C CNN "Supplier Price"
F 10 "100" V 2890 4950 20  0001 C CNN "Supplier Price Break"
	1    2550 5050
	1    0    0    1   
$EndComp
Text Notes 8125 4725 0    50   ~ 0
47uF electro
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
P 2950 5500
F 0 "R?" H 3023 5385 50  0000 L CNN
F 1 "39K_1608M" H 3023 5305 35  0000 L CNN
F 2 "RESC1608*" V 3060 5350 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2310790.pdf" V 3085 5350 20  0001 C CNN
F 4 "PANASONIC" V 3135 5350 20  0001 C CNN "manf"
F 5 "MCMR06X2202FTL" V 3160 5350 20  0001 C CNN "manf#"
F 6 "Element14" V 3185 5350 20  0001 C CNN "Supplier"
F 7 "1469804" V 3210 5350 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/vishay/crcw060339k0fkea/res-thick-film-39k-1-0-1w-0603/dp/1469804" V 3235 5350 20  0001 C CNN "Supplier URL"
F 9 "0.176" V 3260 5350 20  0001 C CNN "Supplier Price"
F 10 "1" V 3285 5350 20  0001 C CNN "Supplier Price Break"
	1    2950 5500
	-1   0    0    -1  
$EndComp
$Comp
L appli_resistor:1M_1608M R?
U 1 1 5B0412F7
P 1700 4900
F 0 "R?" H 1627 4715 50  0000 R CNN
F 1 "1M_1608M" H 1627 4795 35  0000 R CNN
F 2 "RESC1608*" V 1810 4750 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1720486.pdf" V 1835 4750 20  0001 C CNN
F 4 "VISHAY DRALORIC" V 1885 4750 20  0001 C CNN "manf"
F 5 "CRCW06031M00FKEA" V 1910 4750 20  0001 C CNN "manf#"
F 6 "Element14" V 1935 4750 20  0001 C CNN "Supplier"
F 7 "1469746" V 1960 4750 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/vishay-draloric/crcw06031m00fkea/product-range-aec-q200-crcw-series/dp/1469746" V 1985 4750 20  0001 C CNN "Supplier URL"
F 9 "0.02" V 2010 4750 20  0001 C CNN "Supplier Price"
F 10 "100" V 2035 4750 20  0001 C CNN "Supplier Price Break"
	1    1700 4900
	-1   0    0    1   
$EndComp
$Comp
L appli_capacitor:1UF_X7R_2012M C?
U 1 1 5B044635
P 5450 6025
F 0 "C?" H 5528 5960 50  0000 L CNN
F 1 "1UF_X7R_2012M" H 5528 5880 35  0000 L CNN
F 2 "CAPC2012*" V 5565 5925 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2047889.pdf" V 5590 5925 20  0001 C CNN
F 4 "MURATA" V 5640 5925 20  0001 C CNN "manf"
F 5 "GRM21BR71C105KA01L" V 5665 5925 20  0001 C CNN "manf#"
F 6 "Element14" V 5690 5925 20  0001 C CNN "Supplier"
F 7 "9527710" V 5715 5925 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/murata/grm21br71c105ka01l/cap-mlcc-x7r-1uf-16v-0805/dp/9527710" V 5740 5925 20  0001 C CNN "Supplier URL"
F 9 "0.224" V 5765 5925 20  0001 C CNN "Supplier Price"
F 10 "1" V 5790 5925 20  0001 C CNN "Supplier Price Break"
	1    5450 6025
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 5750 5450 5750
Wire Wire Line
	5450 5750 5450 6025
Wire Wire Line
	5450 6225 5450 6750
$Comp
L appli_power:GND #PWR?
U 1 1 5B047597
P 5450 6750
F 0 "#PWR?" H 5450 6500 50  0001 C CNN
F 1 "GND" H 5455 6577 50  0000 C CNN
F 2 "" H 5450 6750 60  0000 C CNN
F 3 "" H 5450 6750 60  0000 C CNN
	1    5450 6750
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5B0475D7
P 4500 6750
F 0 "#PWR?" H 4500 6500 50  0001 C CNN
F 1 "GND" H 4505 6577 50  0000 C CNN
F 2 "" H 4500 6750 60  0000 C CNN
F 3 "" H 4500 6750 60  0000 C CNN
	1    4500 6750
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
	4100 6200 4100 6050
Wire Wire Line
	4200 6050 4200 6250
Wire Wire Line
	4200 6250 4725 6250
Wire Wire Line
	3850 6500 3850 6750
$Comp
L appli_power:GND #PWR?
U 1 1 5B049899
P 3850 6750
F 0 "#PWR?" H 3850 6500 50  0001 C CNN
F 1 "GND" H 3855 6577 50  0000 C CNN
F 2 "" H 3850 6750 60  0000 C CNN
F 3 "" H 3850 6750 60  0000 C CNN
	1    3850 6750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5025 6250 5250 6250
$Comp
L appli_resistor:12K7_1608M R?
U 1 1 5B04EAEE
P 1700 5800
F 0 "R?" H 1627 5615 50  0000 R CNN
F 1 "12K7_1608M" H 1627 5695 35  0000 R CNN
F 2 "Applidyne_Resistor:RESC1608X50N" V 1810 5650 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1911175.pdf" V 1835 5650 20  0001 C CNN
F 4 "PANASONIC" V 1885 5650 20  0001 C CNN "manf"
F 5 "ERJ3EKF1272V" V 1910 5650 20  0001 C CNN "manf#"
F 6 "Element14" V 1935 5650 20  0001 C CNN "Supplier"
F 7 "2059415" V 1960 5650 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/panasonic-electronic-components/erj3ekf1272v/res-thick-film-12k7-1-0-1w-0603/dp/2059415" V 1985 5650 20  0001 C CNN "Supplier URL"
F 9 "0.014" V 2010 5650 20  0001 C CNN "Supplier Price"
F 10 "50" V 2035 5650 20  0001 C CNN "Supplier Price Break"
	1    1700 5800
	-1   0    0    1   
$EndComp
Wire Wire Line
	2900 4750 2900 4700
Wire Wire Line
	2900 4750 2900 4800
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
	2550 4750 2550 4850
Wire Wire Line
	2550 5050 2550 5100
Wire Wire Line
	3600 5050 3350 5050
Wire Wire Line
	3350 5050 3350 5150
Wire Wire Line
	3350 5150 1700 5150
Wire Wire Line
	1700 5150 1700 4900
Wire Wire Line
	1700 5250 1700 5500
Wire Wire Line
	2950 6275 2950 6200
Wire Wire Line
	2950 6275 2950 6400
Connection ~ 2950 6275
Wire Wire Line
	2950 6700 2950 6750
Wire Wire Line
	2950 6000 2950 5800
Wire Wire Line
	2950 5500 2950 5450
Wire Wire Line
	2950 5450 3600 5450
Wire Wire Line
	2950 5450 2200 5450
Wire Wire Line
	2200 5450 2200 5775
Connection ~ 2950 5450
Wire Wire Line
	2200 5975 2200 6275
Wire Wire Line
	2200 6275 2950 6275
$Comp
L appli_power:GND #PWR?
U 1 1 5B06CC85
P 2950 6750
F 0 "#PWR?" H 2950 6500 50  0001 C CNN
F 1 "GND" H 2955 6577 50  0000 C CNN
F 2 "" H 2950 6750 60  0000 C CNN
F 3 "" H 2950 6750 60  0000 C CNN
	1    2950 6750
	1    0    0    -1  
$EndComp
Wire Wire Line
	3475 5750 3475 5850
Wire Wire Line
	3475 6150 3475 6275
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
	2950 6275 3150 6275
Wire Wire Line
	3475 5750 3600 5750
Wire Wire Line
	3600 5550 3150 5550
Wire Wire Line
	3150 5550 3150 6275
Connection ~ 3150 6275
Wire Wire Line
	3150 6275 3475 6275
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
Text Notes 5625 4375 0    50   ~ 0
diode >
Wire Wire Line
	6400 4650 6400 4350
Wire Wire Line
	5200 4650 5500 4650
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
	5800 4650 6400 4650
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
Wire Wire Line
	7300 5400 7300 5700
Wire Wire Line
	7300 5700 6850 5700
Wire Wire Line
	7300 6000 6850 6000
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
	5850 5750 6000 5750
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
P 5250 6750
F 0 "#PWR?" H 5250 6500 50  0001 C CNN
F 1 "GND" H 5255 6577 50  0000 C CNN
F 2 "" H 5250 6750 60  0000 C CNN
F 3 "" H 5250 6750 60  0000 C CNN
	1    5250 6750
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 5800 1700 6600
Wire Wire Line
	1700 4600 1700 4350
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
P 6000 5750
F 0 "#PWR?" H 6000 5600 50  0001 C CNN
F 1 "+12V" H 6015 5923 50  0000 C CNN
F 2 "" H 6000 5750 60  0000 C CNN
F 3 "" H 6000 5750 60  0000 C CNN
	1    6000 5750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 6250 5250 4350
Connection ~ 5250 4350
Wire Wire Line
	5250 4350 5500 4350
Wire Wire Line
	5250 6750 5250 6500
Text Notes 850  5250 0    50   ~ 0
Input caps
Wire Wire Line
	5450 5750 5550 5750
Connection ~ 5450 5750
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
$Comp
L appli_resistor:0.01R_3216M R?
U 1 1 5AD42A5D
P 7300 5700
F 0 "R?" H 7373 5585 50  0000 L CNN
F 1 "0.01R_3216M" H 7373 5505 35  0000 L CNN
F 2 "" V 7410 5550 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2049155.pdf?" V 7435 5550 20  0001 C CNN
F 4 "Vishay" V 7485 5550 20  0001 C CNN "manf"
F 5 "WSLP1206R0100FEA" V 7510 5550 20  0001 C CNN "manf#"
F 6 "Element14" V 7535 5550 20  0001 C CNN "Supplier"
F 7 "683-6215" V 7560 5550 20  0001 C CNN "Supplier Part No"
F 8 "http://au.rs-online.com/web/p/surface-mount-fixed-resistors/6836215" V 7585 5550 20  0001 C CNN "Supplier URL"
F 9 "1.15" V 7610 5550 20  0001 C CNN "Supplier Price"
F 10 "1" V 7635 5550 20  0001 C CNN "Supplier Price Break"
	1    7300 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 6200 3850 6200
Wire Wire Line
	3850 6200 3850 6300
Wire Wire Line
	4200 6300 4200 6250
Connection ~ 4200 6250
Wire Wire Line
	4200 6500 4200 6750
$Comp
L appli_power:GND #PWR?
U 1 1 5AD84D28
P 4200 6750
F 0 "#PWR?" H 4200 6500 50  0001 C CNN
F 1 "GND" H 4205 6577 50  0000 C CNN
F 2 "" H 4200 6750 60  0000 C CNN
F 3 "" H 4200 6750 60  0000 C CNN
	1    4200 6750
	1    0    0    -1  
$EndComp
Wire Wire Line
	4500 6750 4500 6100
$Comp
L appli_resistor:2K2_1608M R?
U 1 1 5ADAD352
P 5500 4650
F 0 "R?" V 5375 4500 50  0000 C CNN
F 1 "2K2_1608M" V 5300 4500 35  0000 C CNN
F 2 "RESC1608*" V 5610 4500 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2310790.pdf" V 5635 4500 20  0001 C CNN
F 4 "VISHAY" V 5685 4500 20  0001 C CNN "manf"
F 5 "CRCW06032K20FKEA" V 5710 4500 20  0001 C CNN "manf#"
F 6 "Element14" V 5735 4500 20  0001 C CNN "Supplier"
F 7 "1469765" V 5760 4500 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/vishay/crcw06032k20fkea/res-thick-film-2k2-1-0-1w-0603/dp/1469765" V 5785 4500 20  0001 C CNN "Supplier URL"
F 9 "0.062" V 5810 4500 20  0001 C CNN "Supplier Price"
F 10 "10" V 5835 4500 20  0001 C CNN "Supplier Price Break"
	1    5500 4650
	0    -1   -1   0   
$EndComp
Connection ~ 7300 5700
Connection ~ 7300 6000
Text Notes 7425 6150 0    50   ~ 0
Route with Kelvin \nconnection
$Comp
L appli_resistor:1K2_1608M R?
U 1 1 5ADD2F03
P 2950 6400
F 0 "R?" H 3023 6285 50  0000 L CNN
F 1 "1K2_1608M" H 3023 6205 35  0000 L CNN
F 2 "RESC1608*" V 3060 6250 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2310790.pdf" V 3085 6250 20  0001 C CNN
F 4 "VISHAY" V 3135 6250 20  0001 C CNN "manf"
F 5 "CRCW06031K20FKEA" V 3160 6250 20  0001 C CNN "manf#"
F 6 "Element14" V 3185 6250 20  0001 C CNN "Supplier"
F 7 "1469741" V 3210 6250 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/vishay/crcw06031k20fkea/res-thick-film-1k2-1-0-1w-0603/dp/1469741" V 3235 6250 20  0001 C CNN "Supplier URL"
F 9 "0.013" V 3260 6250 20  0001 C CNN "Supplier Price"
F 10 "10" V 3285 6250 20  0001 C CNN "Supplier Price Break"
	1    2950 6400
	1    0    0    -1  
$EndComp
$Comp
L appli_resistor:10K7_1608M R?
U 1 1 5ADD30A8
P 3475 5850
F 0 "R?" H 3548 5735 50  0000 L CNN
F 1 "10K7_1608M" H 3548 5655 35  0000 L CNN
F 2 "RESC1608*" V 3585 5700 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1911175.pdf" V 3610 5700 20  0001 C CNN
F 4 "PANASONIC" V 3660 5700 20  0001 C CNN "manf"
F 5 "ERJ3EKF1072V" V 3685 5700 20  0001 C CNN "manf#"
F 6 "Element14" V 3710 5700 20  0001 C CNN "Supplier"
F 7 "2303193" V 3735 5700 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/panasonic-electronic-components/erj3ekf1072v/res-thick-film-10k7-1-0-1w-0603/dp/2303193" V 3760 5700 20  0001 C CNN "Supplier URL"
F 9 "0.014" V 3785 5700 20  0001 C CNN "Supplier Price"
F 10 "5" V 3810 5700 20  0001 C CNN "Supplier Price Break"
	1    3475 5850
	1    0    0    -1  
$EndComp
$Comp
L appli_resistor:750K_1608M R?
U 1 1 5ADD3253
P 5025 6250
F 0 "R?" V 4837 6100 50  0000 C CNN
F 1 "750K_1608M" V 4917 6100 35  0000 C CNN
F 2 "RESC1608*" V 5135 6100 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1716711.pdf" V 5160 6100 20  0001 C CNN
F 4 "MULTICOMP" V 5210 6100 20  0001 C CNN "manf"
F 5 "MCSR06X7503FTL" V 5235 6100 20  0001 C CNN "manf#"
F 6 "Element14" V 5260 6100 20  0001 C CNN "Supplier"
F 7 "2074305" V 5285 6100 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mcsr06x7503ftl/res-ceramic-750k-1-0-1w-0603/dp/2074305" V 5310 6100 20  0001 C CNN "Supplier URL"
F 9 "0.01" V 5335 6100 20  0001 C CNN "Supplier Price"
F 10 "100" V 5360 6100 20  0001 C CNN "Supplier Price Break"
	1    5025 6250
	0    1    1    0   
$EndComp
$Comp
L appli_capacitor:1500PF_X7R_1608M C?
U 1 1 5ADE9DE3
P 2950 6200
F 0 "C?" H 2873 6065 50  0000 R CNN
F 1 "1500PF_X7R_1608M" H 2873 6145 35  0000 R CNN
F 2 "CAPC1608*" V 3065 6100 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1955686.pdf" V 3090 6100 20  0001 C CNN
F 4 "MURATA" V 3140 6100 20  0001 C CNN "manf"
F 5 "GRM188R71H152KA01D" V 3165 6100 20  0001 C CNN "manf#"
F 6 "Element14" V 3190 6100 20  0001 C CNN "Supplier"
F 7 "2494239RL" V 3215 6100 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/murata/grm188r71h152ka01d/cap-mlcc-x7r-1500pf-50v-0603/dp/2494239RL" V 3240 6100 20  0001 C CNN "Supplier URL"
F 9 "0.209" V 3265 6100 20  0001 C CNN "Supplier Price"
F 10 "10" V 3290 6100 20  0001 C CNN "Supplier Price Break"
	1    2950 6200
	1    0    0    1   
$EndComp
$Comp
L appli_capacitor:15PF_X7R_1608M C?
U 1 1 5ADFAAE4
P 2200 5775
F 0 "C?" H 2278 5710 50  0000 L CNN
F 1 "15PF_X7R_1608M" H 2278 5630 35  0000 L CNN
F 2 "CAPC1608*" V 2315 5675 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2291921.pdf" V 2340 5675 20  0001 C CNN
F 4 "TDK" V 2390 5675 20  0001 C CNN "manf"
F 5 "C1608C0G1H150J080AA" V 2415 5675 20  0001 C CNN "manf#"
F 6 "Element14" V 2440 5675 20  0001 C CNN "Supplier"
F 7 "1907286RL" V 2465 5675 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/tdk/c1608c0g1h150j080aa/cap-mlcc-c0g-np0-15pf-50v-0603/dp/1907286RL" V 2490 5675 20  0001 C CNN "Supplier URL"
F 9 "0.0121" V 2515 5675 20  0001 C CNN "Supplier Price"
F 10 "50" V 2540 5675 20  0001 C CNN "Supplier Price Break"
	1    2200 5775
	1    0    0    -1  
$EndComp
$Comp
L appli_resistor:102K_1608M R?
U 1 1 5AE1914E
P 2900 4400
F 0 "R?" H 2973 4285 50  0000 L CNN
F 1 "102K_1608M" H 2973 4205 35  0000 L CNN
F 2 "RESC1608*" V 3010 4250 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1729042.pdf" V 3035 4250 20  0001 C CNN
F 4 "MULTICOMP" V 3085 4250 20  0001 C CNN "manf"
F 5 "MCWF06R1023BTL" V 3110 4250 20  0001 C CNN "manf#"
F 6 "Element14" V 3135 4250 20  0001 C CNN "Supplier"
F 7 "2338699" V 3160 4250 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mcwf06r1023btl/res-thin-film-102k-0-1-0-1w-0603/dp/2338699" V 3185 4250 20  0001 C CNN "Supplier URL"
F 9 "0.045" V 3210 4250 20  0001 C CNN "Supplier Price"
F 10 "10" V 3235 4250 20  0001 C CNN "Supplier Price Break"
	1    2900 4400
	1    0    0    -1  
$EndComp
$Comp
L appli_resistor:1R0_1608M R?
U 1 1 5AE215F9
P 5850 5750
F 0 "R?" V 5662 5600 50  0000 C CNN
F 1 "1R0_1608M" V 5742 5600 35  0000 C CNN
F 2 "RESC1608*" V 5960 5600 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2549522.pdf" V 5985 5600 20  0001 C CNN
F 4 "MULTICOMP" V 6035 5600 20  0001 C CNN "manf"
F 5 "MCMR06X1R0JTL" V 6060 5600 20  0001 C CNN "manf#"
F 6 "Element14" V 6085 5600 20  0001 C CNN "Supplier"
F 7 "2073416" V 6110 5600 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mcmr06x1r0-jtl/res-ceramic-1r-5-0-1w-0603/dp/2073416" V 6135 5600 20  0001 C CNN "Supplier URL"
F 9 "0.005" V 6160 5600 20  0001 C CNN "Supplier Price"
F 10 "100" V 6185 5600 20  0001 C CNN "Supplier Price Break"
	1    5850 5750
	0    1    1    0   
$EndComp
$Comp
L appli_resistor:9K53_1608M R?
U 1 1 5AE2DC3A
P 2900 4800
F 0 "R?" H 2973 4685 50  0000 L CNN
F 1 "9K53_1608M" H 2973 4605 35  0000 L CNN
F 2 "Applidyne_Resistor:RESC1608X50N" V 3010 4650 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2111203.pdf" V 3035 4650 20  0001 C CNN
F 4 "VISHAY" V 3085 4650 20  0001 C CNN "manf"
F 5 "CRCW06039K53FKEA" V 3110 4650 20  0001 C CNN "manf#"
F 6 "Element14" V 3135 4650 20  0001 C CNN "Supplier"
F 7 "1469844" V 3160 4650 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/vishay/crcw06039k53fkea/res-thick-film-9k53-1-0-1w-0603/dp/1469844?CMP=GRHB-OCTOPART" V 3185 4650 20  0001 C CNN "Supplier URL"
F 9 "0.087" V 3210 4650 20  0001 C CNN "Supplier Price"
F 10 "10" V 3235 4650 20  0001 C CNN "Supplier Price Break"
	1    2900 4800
	1    0    0    -1  
$EndComp
Text Notes 3175 3450 0    157  ~ 31
check for 100V rated part requirements
$EndSCHEMATC
