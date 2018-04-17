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
Text Notes 2675 1525 0    118  ~ 24
75VDC Input Power
Text Notes 12350 1675 0    118  ~ 24
Input Voltage Monitoring
Text Notes 6025 3450 0    118  ~ 24
12V Regulation
Text Notes 6000 8125 0    118  ~ 24
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
Text Notes 1950 1825 0    50   ~ 0
SABRE 2-pin connector
$Comp
L appli_resistor:130K_1608M R?
U 1 1 5ABC59BC
P 12200 2375
F 0 "R?" H 12128 2190 50  0000 R CNN
F 1 "130K_1608M" H 12128 2270 35  0000 R CNN
F 2 "RESC1608*" V 12310 2225 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2059623.pdf" V 12335 2225 20  0001 C CNN
F 4 "PANASONIC" V 12385 2225 20  0001 C CNN "manf"
F 5 "ERJ3GEYJ134V" V 12410 2225 20  0001 C CNN "manf#"
F 6 "Element14" V 12435 2225 20  0001 C CNN "Supplier"
F 7 "2059658" V 12460 2225 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/panasonic-electronic-components/erj3geyj134v/res-thick-film-130k-5-0-1w-0603/dp/2059658" V 12485 2225 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 12510 2225 20  0001 C CNN "Supplier Price"
F 10 "50" V 12535 2225 20  0001 C CNN "Supplier Price Break"
	1    12200 2375
	1    0    0    1   
$EndComp
$Comp
L appli_resistor:130K_1608M R?
U 1 1 5ABC5A8A
P 12200 2775
F 0 "R?" H 12128 2590 50  0000 R CNN
F 1 "130K_1608M" H 12128 2670 35  0000 R CNN
F 2 "RESC1608*" V 12310 2625 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2059623.pdf" V 12335 2625 20  0001 C CNN
F 4 "PANASONIC" V 12385 2625 20  0001 C CNN "manf"
F 5 "ERJ3GEYJ134V" V 12410 2625 20  0001 C CNN "manf#"
F 6 "Element14" V 12435 2625 20  0001 C CNN "Supplier"
F 7 "2059658" V 12460 2625 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/panasonic-electronic-components/erj3geyj134v/res-thick-film-130k-5-0-1w-0603/dp/2059658" V 12485 2625 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 12510 2625 20  0001 C CNN "Supplier Price"
F 10 "50" V 12535 2625 20  0001 C CNN "Supplier Price Break"
	1    12200 2775
	1    0    0    1   
$EndComp
$Comp
L appli_resistor:10K0_1608M R?
U 1 1 5ABC5BF5
P 12200 3175
F 0 "R?" H 12127 3060 50  0000 R CNN
F 1 "10K0_1608M" H 12127 2980 35  0000 R CNN
F 2 "RESC1608*" V 12310 3025 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 12335 3025 20  0001 C CNN
F 4 "TE CONNECTIVITY" V 12385 3025 20  0001 C CNN "manf"
F 5 "CRGH0603F10K" V 12410 3025 20  0001 C CNN "manf#"
F 6 "Element14" V 12435 3025 20  0001 C CNN "Supplier"
F 7 "2332016" V 12460 3025 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/te-connectivity/crgh0603f10k/resistor-power-10k-0-2w-1-0603/dp/2332016" V 12485 3025 20  0001 C CNN "Supplier URL"
F 9 "0.02" V 12510 3025 20  0001 C CNN "Supplier Price"
F 10 "10" V 12535 3025 20  0001 C CNN "Supplier Price Break"
	1    12200 3175
	1    0    0    1   
$EndComp
$Comp
L appli_power:+POWER #PWR0108
U 1 1 5ABC5F0E
P 12200 2025
F 0 "#PWR0108" H 12200 1975 20  0001 C CNN
F 1 "+POWER" H 12203 2163 30  0000 C CNN
F 2 "" H 12200 2025 60  0000 C CNN
F 3 "" H 12200 2025 60  0000 C CNN
	1    12200 2025
	1    0    0    -1  
$EndComp
Wire Wire Line
	12200 2025 12200 2075
Wire Wire Line
	12200 2375 12200 2475
Wire Wire Line
	12200 2775 12200 2825
Wire Wire Line
	12550 2925 12550 2825
Wire Wire Line
	12550 2825 12200 2825
Connection ~ 12200 2825
Wire Wire Line
	12200 2825 12200 2875
Wire Wire Line
	12200 3175 12200 3225
$Comp
L appli_power:GND #PWR0109
U 1 1 5ABC61AD
P 12200 3275
F 0 "#PWR0109" H 12200 3025 50  0001 C CNN
F 1 "GND" H 12205 3102 50  0000 C CNN
F 2 "" H 12200 3275 60  0000 C CNN
F 3 "" H 12200 3275 60  0000 C CNN
	1    12200 3275
	1    0    0    -1  
$EndComp
Wire Wire Line
	12550 3125 12550 3225
Wire Wire Line
	12550 3225 12200 3225
Connection ~ 12200 3225
Wire Wire Line
	12200 3225 12200 3275
Wire Wire Line
	12550 2825 13200 2825
Connection ~ 12550 2825
$Comp
L appli_device:BAV99 D?
U 1 1 5ABC7E1F
P 13200 2825
F 0 "D?" H 13308 2871 50  0000 L CNN
F 1 "BAV99" H 13308 2780 50  0000 L CNN
F 2 "SOT23" H 13400 2675 20  0001 C CNN
F 3 "http://datasheet.octopart.com/BAV99-/T3-NXP-datasheet-5318834.pdf" H 13400 2650 20  0001 C CNN
F 4 "NXP" H 13400 2600 20  0001 C CNN "manf"
F 5 "RS Online" H 13400 2550 20  0001 C CNN "Supplier"
F 6 " 544-4534" H 13400 2525 20  0001 C CNN "Supplier Part No"
F 7 "http://au.rs-online.com/web/p/rectifier-schottky-diodes/5444534/" H 13400 2500 20  0001 C CNN "Supplier URL"
F 8 "0.045" H 13400 2475 20  0001 C CNN "Supplier Price"
F 9 "1" H 13400 2450 20  0001 C CNN "Supplier Price Break"
F 10 "BAV99" H 13400 2575 20  0001 C CNN "manf#"
	1    13200 2825
	1    0    0    -1  
$EndComp
Wire Wire Line
	13200 3075 13200 3275
Wire Wire Line
	13200 2575 13200 2500
$Comp
L appli_power:GND #PWR0110
U 1 1 5ABC81D7
P 13200 3275
F 0 "#PWR0110" H 13200 3025 50  0001 C CNN
F 1 "GND" H 13205 3102 50  0000 C CNN
F 2 "" H 13200 3275 60  0000 C CNN
F 3 "" H 13200 3275 60  0000 C CNN
	1    13200 3275
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+3.3V #PWR0111
U 1 1 5ABC8212
P 13200 2500
F 0 "#PWR0111" H 13200 2460 30  0001 C CNN
F 1 "+3.3V" H 13209 2638 30  0000 C CNN
F 2 "" H 13200 2500 60  0000 C CNN
F 3 "" H 13200 2500 60  0000 C CNN
	1    13200 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	13200 2825 13625 2825
Connection ~ 13200 2825
$Comp
L appli_resistor:1K00_1608M R?
U 1 1 5ABC8785
P 13925 2825
F 0 "R?" V 13737 2675 50  0000 C CNN
F 1 "1K00_1608M" V 13817 2675 35  0000 C CNN
F 2 "RESC1608*" V 14035 2675 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 14060 2675 20  0001 C CNN
F 4 "MULTICOMP" V 14110 2675 20  0001 C CNN "manf"
F 5 "MCHP03W8F1001T5E" V 14135 2675 20  0001 C CNN "manf#"
F 6 "Element14" V 14160 2675 20  0001 C CNN "Supplier"
F 7 "1576283" V 14185 2675 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mchp03w8f1001t5e/resistor-thick-film-1k-0-125w/dp/1576283" V 14210 2675 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 14235 2675 20  0001 C CNN "Supplier Price"
F 10 "50+" V 14260 2675 20  0001 C CNN "Supplier Price Break"
	1    13925 2825
	0    1    1    0   
$EndComp
Wire Wire Line
	13925 2825 14075 2825
Text GLabel 14075 2825 2    50   Output ~ 0
VOLTAGE_INPUT
Text Notes 12700 2175 0    50   ~ 0
Teknic IPC-3/5 outputs 75VDC nominal.\nDivider is 27:1, so 75V = 2.78.\nProvides enough overhead for sensing 90VDC.
Text Notes 14075 3025 0    50   ~ 0
27:1 ratio
$Comp
L appli_power:+12V #PWR0112
U 1 1 5ABE329D
P 12800 5000
F 0 "#PWR0112" H 12800 4850 50  0001 C CNN
F 1 "+12V" H 12815 5173 50  0000 C CNN
F 2 "" H 12800 5000 60  0000 C CNN
F 3 "" H 12800 5000 60  0000 C CNN
	1    12800 5000
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+12V #PWR0113
U 1 1 5ABE32C3
P 5225 8700
F 0 "#PWR0113" H 5225 8550 50  0001 C CNN
F 1 "+12V" H 5240 8873 50  0000 C CNN
F 2 "" H 5225 8700 60  0000 C CNN
F 3 "" H 5225 8700 60  0000 C CNN
	1    5225 8700
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+3.3V #PWR0114
U 1 1 5ABE331C
P 7875 8700
F 0 "#PWR0114" H 7875 8660 30  0001 C CNN
F 1 "+3.3V" H 7884 8838 30  0000 C CNN
F 2 "" H 7875 8700 60  0000 C CNN
F 3 "" H 7875 8700 60  0000 C CNN
	1    7875 8700
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+POWER #PWR0115
U 1 1 5ABE3398
P 3025 2200
F 0 "#PWR0115" H 3025 2150 20  0001 C CNN
F 1 "+POWER" H 3028 2338 30  0000 C CNN
F 2 "" H 3025 2200 60  0000 C CNN
F 3 "" H 3025 2200 60  0000 C CNN
	1    3025 2200
	1    0    0    -1  
$EndComp
$Comp
L appli_resistor:0R_1608M R?
U 1 1 5B040E00
P 9150 5925
F 0 "R?" V 8962 5775 50  0000 C CNN
F 1 "0R_1608M" V 9042 5775 35  0000 C CNN
F 2 "RESC1608*" V 9260 5775 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1739504.pdf" V 9285 5775 20  0001 C CNN
F 4 "MULTICOMP" V 9335 5775 20  0001 C CNN "manf"
F 5 "MC0063W06030R" V 9360 5775 20  0001 C CNN "manf#"
F 6 "Element14" V 9385 5775 20  0001 C CNN "Supplier"
F 7 "9331662" V 9410 5775 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mc0063w06030r/resistor-thick-film-0r-0-063w/dp/9331662" V 9435 5775 20  0001 C CNN "Supplier URL"
F 9 "0.023" V 9460 5775 20  0001 C CNN "Supplier Price"
F 10 "50" V 9485 5775 20  0001 C CNN "Supplier Price Break"
	1    9150 5925
	0    1    1    0   
$EndComp
$Comp
L appli_resistor:10K0_1608M R?
U 1 1 5B040F7F
P 9150 6550
F 0 "R?" V 8962 6400 50  0000 C CNN
F 1 "10K0_1608M" V 9042 6400 35  0000 C CNN
F 2 "RESC1608*" V 9260 6400 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 9285 6400 20  0001 C CNN
F 4 "TE CONNECTIVITY" V 9335 6400 20  0001 C CNN "manf"
F 5 "CRGH0603F10K" V 9360 6400 20  0001 C CNN "manf#"
F 6 "Element14" V 9385 6400 20  0001 C CNN "Supplier"
F 7 "2332016" V 9410 6400 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/te-connectivity/crgh0603f10k/resistor-power-10k-0-2w-1-0603/dp/2332016" V 9435 6400 20  0001 C CNN "Supplier URL"
F 9 "0.02" V 9460 6400 20  0001 C CNN "Supplier Price"
F 10 "10" V 9485 6400 20  0001 C CNN "Supplier Price Break"
	1    9150 6550
	0    1    1    0   
$EndComp
$Comp
L appli_resistor:39K_1608M R?
U 1 1 5B041168
P 5250 5600
F 0 "R?" H 5323 5485 50  0000 L CNN
F 1 "39K_1608M" H 5323 5405 35  0000 L CNN
F 2 "RESC1608*" V 5360 5450 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2310790.pdf" V 5385 5450 20  0001 C CNN
F 4 "PANASONIC" V 5435 5450 20  0001 C CNN "manf"
F 5 "MCMR06X2202FTL" V 5460 5450 20  0001 C CNN "manf#"
F 6 "Element14" V 5485 5450 20  0001 C CNN "Supplier"
F 7 "1469804" V 5510 5450 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/vishay/crcw060339k0fkea/res-thick-film-39k-1-0-1w-0603/dp/1469804" V 5535 5450 20  0001 C CNN "Supplier URL"
F 9 "0.176" V 5560 5450 20  0001 C CNN "Supplier Price"
F 10 "1" V 5585 5450 20  0001 C CNN "Supplier Price Break"
	1    5250 5600
	-1   0    0    -1  
$EndComp
$Comp
L appli_resistor:1M_1608M R?
U 1 1 5B0412F7
P 4000 5075
F 0 "R?" H 3927 4890 50  0000 R CNN
F 1 "1M_1608M" H 3927 4970 35  0000 R CNN
F 2 "RESC1608*" V 4110 4925 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1720486.pdf" V 4135 4925 20  0001 C CNN
F 4 "VISHAY DRALORIC" V 4185 4925 20  0001 C CNN "manf"
F 5 "CRCW06031M00FKEA" V 4210 4925 20  0001 C CNN "manf#"
F 6 "Element14" V 4235 4925 20  0001 C CNN "Supplier"
F 7 "1469746" V 4260 4925 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/vishay-draloric/crcw06031m00fkea/product-range-aec-q200-crcw-series/dp/1469746" V 4285 4925 20  0001 C CNN "Supplier URL"
F 9 "0.02" V 4310 4925 20  0001 C CNN "Supplier Price"
F 10 "100" V 4335 4925 20  0001 C CNN "Supplier Price Break"
	1    4000 5075
	-1   0    0    1   
$EndComp
Wire Wire Line
	7500 5850 7750 5850
Wire Wire Line
	7750 5850 7750 6100
Wire Wire Line
	7750 6300 7750 6850
$Comp
L appli_power:GND #PWR?
U 1 1 5B047597
P 7750 6850
F 0 "#PWR?" H 7750 6600 50  0001 C CNN
F 1 "GND" H 7755 6677 50  0000 C CNN
F 2 "" H 7750 6850 60  0000 C CNN
F 3 "" H 7750 6850 60  0000 C CNN
	1    7750 6850
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5B0475D7
P 6800 6850
F 0 "#PWR?" H 6800 6600 50  0001 C CNN
F 1 "GND" H 6805 6677 50  0000 C CNN
F 2 "" H 6800 6850 60  0000 C CNN
F 3 "" H 6800 6850 60  0000 C CNN
	1    6800 6850
	1    0    0    -1  
$EndComp
Wire Wire Line
	6700 6150 6700 6200
Wire Wire Line
	6700 6200 6800 6200
Wire Wire Line
	6800 6200 6800 6150
Wire Wire Line
	6800 6200 6900 6200
Wire Wire Line
	6900 6200 6900 6150
Connection ~ 6800 6200
Wire Wire Line
	6400 6300 6400 6150
Wire Wire Line
	6500 6150 6500 6350
Wire Wire Line
	6500 6350 7025 6350
Wire Wire Line
	6150 6600 6150 6850
$Comp
L appli_power:GND #PWR?
U 1 1 5B049899
P 6150 6850
F 0 "#PWR?" H 6150 6600 50  0001 C CNN
F 1 "GND" H 6155 6677 50  0000 C CNN
F 2 "" H 6150 6850 60  0000 C CNN
F 3 "" H 6150 6850 60  0000 C CNN
	1    6150 6850
	1    0    0    -1  
$EndComp
Wire Wire Line
	7325 6350 7550 6350
$Comp
L appli_resistor:12K7_1608M R?
U 1 1 5B04EAEE
P 4000 5900
F 0 "R?" H 3927 5715 50  0000 R CNN
F 1 "12K7_1608M" H 3927 5795 35  0000 R CNN
F 2 "Applidyne_Resistor:RESC1608X50N" V 4110 5750 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1911175.pdf" V 4135 5750 20  0001 C CNN
F 4 "PANASONIC" V 4185 5750 20  0001 C CNN "manf"
F 5 "ERJ3EKF1272V" V 4210 5750 20  0001 C CNN "manf#"
F 6 "Element14" V 4235 5750 20  0001 C CNN "Supplier"
F 7 "2059415" V 4260 5750 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/panasonic-electronic-components/erj3ekf1272v/res-thick-film-12k7-1-0-1w-0603/dp/2059415" V 4285 5750 20  0001 C CNN "Supplier URL"
F 9 "0.014" V 4310 5750 20  0001 C CNN "Supplier Price"
F 10 "50" V 4335 5750 20  0001 C CNN "Supplier Price Break"
	1    4000 5900
	-1   0    0    1   
$EndComp
Wire Wire Line
	5200 4850 5200 4800
Wire Wire Line
	5200 4850 5200 4900
Connection ~ 5200 4850
Wire Wire Line
	5200 4500 5200 4450
Wire Wire Line
	5200 4450 5900 4450
Connection ~ 5200 4450
Wire Wire Line
	5200 4850 4850 4850
Wire Wire Line
	4850 4850 4850 4950
Wire Wire Line
	5900 5150 5650 5150
Wire Wire Line
	5650 5150 5650 5350
Wire Wire Line
	5650 5350 4000 5350
Wire Wire Line
	5250 6375 5250 6300
Wire Wire Line
	5250 6375 5250 6500
Connection ~ 5250 6375
Wire Wire Line
	5250 6800 5250 6850
Wire Wire Line
	5250 6100 5250 5900
Wire Wire Line
	5250 5600 5250 5550
Wire Wire Line
	5250 5550 5900 5550
Wire Wire Line
	5250 5550 4500 5550
Wire Wire Line
	4500 5550 4500 5875
Connection ~ 5250 5550
Wire Wire Line
	4500 6075 4500 6375
Wire Wire Line
	4500 6375 5250 6375
$Comp
L appli_power:GND #PWR?
U 1 1 5B06CC85
P 5250 6850
F 0 "#PWR?" H 5250 6600 50  0001 C CNN
F 1 "GND" H 5255 6677 50  0000 C CNN
F 2 "" H 5250 6850 60  0000 C CNN
F 3 "" H 5250 6850 60  0000 C CNN
	1    5250 6850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 5850 5800 5950
Wire Wire Line
	5800 6250 5800 6375
$Comp
L appli_regulator:LM5116 U?
U 1 1 5B07707D
P 6700 5150
F 0 "U?" H 6700 6117 50  0000 C CNN
F 1 "LM5116" H 6700 6026 50  0000 C CNN
F 2 "Applidyne_SOP:SOP21P65_720X780X200L75X30N" H 7025 4250 20  0001 L CNN
F 3 "http://www.ti.com/lit/ds/snvs499h/snvs499h.pdf" H 7025 4225 20  0001 L CNN
F 4 "TEXAS INSTRUMENTS" H 7025 4175 20  0001 L CNN "manf"
F 5 "LM5116MHX" H 7025 4150 20  0001 L CNN "manf#"
F 6 "Digikey" H 7025 4125 20  0001 L CNN "Supplier"
F 7 "LM5116MHX/NOPBCT-ND" H 7025 4100 20  0001 L CNN "Supplier Part No"
F 8 "https://www.digikey.com.au/product-detail/en/LM5116MHX-NOPB/LM5116MHX-NOPBCT-ND/3440164" H 7025 4075 20  0001 L CNN "Supplier URL"
F 9 "7.79" H 7025 4050 20  0001 L CNN "Supplier Price"
F 10 "1" H 7025 4025 20  0001 L CNN "Supplier Price Break"
	1    6700 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 6375 5450 6375
Wire Wire Line
	5800 5850 5900 5850
Wire Wire Line
	5900 5650 5450 5650
Wire Wire Line
	5450 5650 5450 6375
Connection ~ 5450 6375
Wire Wire Line
	5450 6375 5800 6375
$Comp
L appli_power:+12V #PWR?
U 1 1 5B089A4B
P 5800 5825
F 0 "#PWR?" H 5800 5675 50  0001 C CNN
F 1 "+12V" H 5650 5875 50  0000 C CNN
F 2 "" H 5800 5825 60  0000 C CNN
F 3 "" H 5800 5825 60  0000 C CNN
	1    5800 5825
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 5825 5800 5850
Connection ~ 5800 5850
Wire Wire Line
	8700 4750 8700 4450
Wire Wire Line
	7500 4750 7800 4750
Wire Wire Line
	8700 4800 8700 4750
Connection ~ 8700 4750
Wire Wire Line
	8700 5000 8700 5050
Wire Wire Line
	8700 4450 8050 4450
Wire Wire Line
	7500 5050 8700 5050
Wire Wire Line
	8700 5050 9250 5050
Connection ~ 8700 5050
Wire Wire Line
	8100 4750 8700 4750
Wire Wire Line
	10550 5050 10800 5050
Wire Wire Line
	9600 5925 9150 5925
Wire Wire Line
	9600 6225 9150 6225
$Comp
L appli_resistor:0R_1608M R?
U 1 1 5B0D507E
P 9150 6225
F 0 "R?" V 8962 6075 50  0000 C CNN
F 1 "0R_1608M" V 9042 6075 35  0000 C CNN
F 2 "RESC1608*" V 9260 6075 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1739504.pdf" V 9285 6075 20  0001 C CNN
F 4 "MULTICOMP" V 9335 6075 20  0001 C CNN "manf"
F 5 "MC0063W06030R" V 9360 6075 20  0001 C CNN "manf#"
F 6 "Element14" V 9385 6075 20  0001 C CNN "Supplier"
F 7 "9331662" V 9410 6075 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mc0063w06030r/resistor-thick-film-0r-0-063w/dp/9331662" V 9435 6075 20  0001 C CNN "Supplier URL"
F 9 "0.023" V 9460 6075 20  0001 C CNN "Supplier Price"
F 10 "50" V 9485 6075 20  0001 C CNN "Supplier Price Break"
	1    9150 6225
	0    1    1    0   
$EndComp
Wire Wire Line
	8850 6225 8600 6225
Wire Wire Line
	7500 5350 8700 5350
Wire Wire Line
	8700 5350 8700 5600
Wire Wire Line
	8700 5925 8850 5925
Wire Wire Line
	7500 5450 8600 5450
Wire Wire Line
	8600 5450 8600 6225
Wire Wire Line
	8150 5850 8300 5850
$Comp
L appli_power:+POWER #PWR?
U 1 1 5B0E5346
P 1550 4350
F 0 "#PWR?" H 1550 4300 20  0001 C CNN
F 1 "+POWER" H 1553 4488 30  0000 C CNN
F 2 "" H 1550 4350 60  0000 C CNN
F 3 "" H 1550 4350 60  0000 C CNN
	1    1550 4350
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5B0E58BD
P 9600 6625
F 0 "#PWR?" H 9600 6375 50  0001 C CNN
F 1 "GND" H 9605 6452 50  0000 C CNN
F 2 "" H 9600 6625 60  0000 C CNN
F 3 "" H 9600 6625 60  0000 C CNN
	1    9600 6625
	1    0    0    -1  
$EndComp
Wire Wire Line
	7500 5550 8500 5550
Wire Wire Line
	8500 5550 8500 6550
Wire Wire Line
	8500 6550 8850 6550
Wire Wire Line
	9150 6550 9600 6550
Wire Wire Line
	9600 6225 9600 6550
Wire Wire Line
	9600 6625 9600 6550
Connection ~ 9600 6550
Wire Wire Line
	8800 5250 8800 5350
Wire Wire Line
	7500 5250 8800 5250
Wire Wire Line
	7500 4450 7550 4450
Wire Wire Line
	7550 6400 7550 6350
Connection ~ 7550 6350
$Comp
L appli_power:GND #PWR?
U 1 1 5B142139
P 7550 6850
F 0 "#PWR?" H 7550 6600 50  0001 C CNN
F 1 "GND" H 7555 6677 50  0000 C CNN
F 2 "" H 7550 6850 60  0000 C CNN
F 3 "" H 7550 6850 60  0000 C CNN
	1    7550 6850
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 5900 4000 6850
Wire Wire Line
	4000 4775 4000 4450
Wire Wire Line
	4000 4450 4850 4450
Wire Wire Line
	4000 4450 3300 4450
Connection ~ 4000 4450
Wire Wire Line
	5200 4850 5900 4850
Wire Wire Line
	9600 5100 9600 5050
Connection ~ 9600 5050
Wire Wire Line
	9600 5050 10250 5050
$Comp
L appli_power:+12V #PWR?
U 1 1 5B1AC942
P 8300 5850
F 0 "#PWR?" H 8300 5700 50  0001 C CNN
F 1 "+12V" H 8315 6023 50  0000 C CNN
F 2 "" H 8300 5850 60  0000 C CNN
F 3 "" H 8300 5850 60  0000 C CNN
	1    8300 5850
	1    0    0    -1  
$EndComp
Wire Wire Line
	7550 6350 7550 4450
Connection ~ 7550 4450
Wire Wire Line
	7550 4450 7850 4450
Wire Wire Line
	7550 6850 7550 6600
Wire Wire Line
	7750 5850 7850 5850
Connection ~ 7750 5850
Wire Wire Line
	8975 4650 8975 4850
Wire Wire Line
	8975 4850 9300 4850
Wire Wire Line
	7500 4650 8975 4650
$Comp
L appli_connector_molex:MOLEX_43160-0102 P?
U 1 1 5AD2F837
P 2275 2300
F 0 "P?" H 2194 2565 50  0000 C CNN
F 1 "MOLEX_43160-0102" H 2194 2474 50  0000 C CNN
F 2 "Applidyne_Connector_Molex:Molex_Sabre_43160-0102_VERT" V 2450 2300 50  0001 C CNN
F 3 "https://www.molex.com/molex/products/datasheet.jsp?part=active/0431600102_PCB_HEADERS.xml" V 3225 2350 50  0001 C CNN
F 4 "MOLEX" V 2550 2325 60  0001 C CNN "manf"
F 5 "043160-0102" V 2650 2325 60  0001 C CNN "manf#"
F 6 "DigiKey" V 2750 2325 60  0001 C CNN "Supplier"
F 7 "WM18473-ND" V 2825 2325 60  0001 C CNN "Supplier Part No"
F 8 "https://www.digikey.com.au/product-detail/en/0431600102/WM18473-ND/300106" V 2925 2325 60  0001 C CNN "Supplier URL"
F 9 "1.66" V 3025 2325 60  0001 C CNN "Supplier Price"
F 10 "1" V 3125 2325 60  0001 C CNN "Supplier Price Break"
	1    2275 2300
	-1   0    0    -1  
$EndComp
Wire Wire Line
	2475 2250 3025 2250
Wire Wire Line
	2475 2350 2950 2350
Wire Wire Line
	2950 2350 2950 2400
$Comp
L appli_power:GND #PWR?
U 1 1 5AD4886E
P 2950 2400
F 0 "#PWR?" H 2950 2150 50  0001 C CNN
F 1 "GND" H 2955 2227 50  0000 C CNN
F 2 "" H 2950 2400 60  0000 C CNN
F 3 "" H 2950 2400 60  0000 C CNN
	1    2950 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 6300 6150 6300
Wire Wire Line
	6150 6300 6150 6400
Wire Wire Line
	6500 6400 6500 6350
Connection ~ 6500 6350
Wire Wire Line
	6500 6600 6500 6850
$Comp
L appli_power:GND #PWR?
U 1 1 5AD84D28
P 6500 6850
F 0 "#PWR?" H 6500 6600 50  0001 C CNN
F 1 "GND" H 6505 6677 50  0000 C CNN
F 2 "" H 6500 6850 60  0000 C CNN
F 3 "" H 6500 6850 60  0000 C CNN
	1    6500 6850
	1    0    0    -1  
$EndComp
Wire Wire Line
	6800 6850 6800 6200
Text Notes 9725 6375 0    50   ~ 0
Route with Kelvin \nconnection
$Comp
L appli_resistor:1K2_1608M R?
U 1 1 5ADD2F03
P 5250 6500
F 0 "R?" H 5323 6385 50  0000 L CNN
F 1 "1K2_1608M" H 5323 6305 35  0000 L CNN
F 2 "RESC1608*" V 5360 6350 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2310790.pdf" V 5385 6350 20  0001 C CNN
F 4 "VISHAY" V 5435 6350 20  0001 C CNN "manf"
F 5 "CRCW06031K20FKEA" V 5460 6350 20  0001 C CNN "manf#"
F 6 "Element14" V 5485 6350 20  0001 C CNN "Supplier"
F 7 "1469741" V 5510 6350 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/vishay/crcw06031k20fkea/res-thick-film-1k2-1-0-1w-0603/dp/1469741" V 5535 6350 20  0001 C CNN "Supplier URL"
F 9 "0.013" V 5560 6350 20  0001 C CNN "Supplier Price"
F 10 "10" V 5585 6350 20  0001 C CNN "Supplier Price Break"
	1    5250 6500
	1    0    0    -1  
$EndComp
$Comp
L appli_resistor:10K7_1608M R?
U 1 1 5ADD30A8
P 5800 5950
F 0 "R?" H 5873 5835 50  0000 L CNN
F 1 "10K7_1608M" H 5873 5755 35  0000 L CNN
F 2 "RESC1608*" V 5910 5800 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1911175.pdf" V 5935 5800 20  0001 C CNN
F 4 "PANASONIC" V 5985 5800 20  0001 C CNN "manf"
F 5 "ERJ3EKF1072V" V 6010 5800 20  0001 C CNN "manf#"
F 6 "Element14" V 6035 5800 20  0001 C CNN "Supplier"
F 7 "2303193" V 6060 5800 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/panasonic-electronic-components/erj3ekf1072v/res-thick-film-10k7-1-0-1w-0603/dp/2303193" V 6085 5800 20  0001 C CNN "Supplier URL"
F 9 "0.014" V 6110 5800 20  0001 C CNN "Supplier Price"
F 10 "5" V 6135 5800 20  0001 C CNN "Supplier Price Break"
	1    5800 5950
	1    0    0    -1  
$EndComp
$Comp
L appli_resistor:750K_1608M R?
U 1 1 5ADD3253
P 7325 6350
F 0 "R?" V 7137 6200 50  0000 C CNN
F 1 "750K_1608M" V 7217 6200 35  0000 C CNN
F 2 "RESC1608*" V 7435 6200 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1716711.pdf" V 7460 6200 20  0001 C CNN
F 4 "MULTICOMP" V 7510 6200 20  0001 C CNN "manf"
F 5 "MCSR06X7503FTL" V 7535 6200 20  0001 C CNN "manf#"
F 6 "Element14" V 7560 6200 20  0001 C CNN "Supplier"
F 7 "2074305" V 7585 6200 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mcsr06x7503ftl/res-ceramic-750k-1-0-1w-0603/dp/2074305" V 7610 6200 20  0001 C CNN "Supplier URL"
F 9 "0.01" V 7635 6200 20  0001 C CNN "Supplier Price"
F 10 "100" V 7660 6200 20  0001 C CNN "Supplier Price Break"
	1    7325 6350
	0    1    1    0   
$EndComp
$Comp
L appli_resistor:102K_1608M R?
U 1 1 5AE1914E
P 5200 4500
F 0 "R?" H 5273 4385 50  0000 L CNN
F 1 "102K_1608M" H 5273 4305 35  0000 L CNN
F 2 "RESC1608*" V 5310 4350 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1729042.pdf" V 5335 4350 20  0001 C CNN
F 4 "MULTICOMP" V 5385 4350 20  0001 C CNN "manf"
F 5 "MCWF06R1023BTL" V 5410 4350 20  0001 C CNN "manf#"
F 6 "Element14" V 5435 4350 20  0001 C CNN "Supplier"
F 7 "2338699" V 5460 4350 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mcwf06r1023btl/res-thin-film-102k-0-1-0-1w-0603/dp/2338699" V 5485 4350 20  0001 C CNN "Supplier URL"
F 9 "0.045" V 5510 4350 20  0001 C CNN "Supplier Price"
F 10 "10" V 5535 4350 20  0001 C CNN "Supplier Price Break"
	1    5200 4500
	1    0    0    -1  
$EndComp
$Comp
L appli_resistor:1R0_1608M R?
U 1 1 5AE215F9
P 8150 5850
F 0 "R?" V 7962 5700 50  0000 C CNN
F 1 "1R0_1608M" V 8042 5700 35  0000 C CNN
F 2 "RESC1608*" V 8260 5700 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2549522.pdf" V 8285 5700 20  0001 C CNN
F 4 "MULTICOMP" V 8335 5700 20  0001 C CNN "manf"
F 5 "MCMR06X1R0JTL" V 8360 5700 20  0001 C CNN "manf#"
F 6 "Element14" V 8385 5700 20  0001 C CNN "Supplier"
F 7 "2073416" V 8410 5700 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mcmr06x1r0-jtl/res-ceramic-1r-5-0-1w-0603/dp/2073416" V 8435 5700 20  0001 C CNN "Supplier URL"
F 9 "0.005" V 8460 5700 20  0001 C CNN "Supplier Price"
F 10 "100" V 8485 5700 20  0001 C CNN "Supplier Price Break"
	1    8150 5850
	0    1    1    0   
$EndComp
$Comp
L appli_resistor:9K53_1608M R?
U 1 1 5AE2DC3A
P 5200 4900
F 0 "R?" H 5273 4785 50  0000 L CNN
F 1 "9K53_1608M" H 5273 4705 35  0000 L CNN
F 2 "Applidyne_Resistor:RESC1608X50N" V 5310 4750 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2111203.pdf" V 5335 4750 20  0001 C CNN
F 4 "VISHAY" V 5385 4750 20  0001 C CNN "manf"
F 5 "CRCW06039K53FKEA" V 5410 4750 20  0001 C CNN "manf#"
F 6 "Element14" V 5435 4750 20  0001 C CNN "Supplier"
F 7 "1469844" V 5460 4750 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/vishay/crcw06039k53fkea/res-thick-film-9k53-1-0-1w-0603/dp/1469844?CMP=GRHB-OCTOPART" V 5485 4750 20  0001 C CNN "Supplier URL"
F 9 "0.087" V 5510 4750 20  0001 C CNN "Supplier Price"
F 10 "10" V 5535 4750 20  0001 C CNN "Supplier Price Break"
	1    5200 4900
	1    0    0    -1  
$EndComp
$Comp
L appli_power:PWR_FLAG #FLG?
U 1 1 5AD551CC
P 8125 8750
F 0 "#FLG?" H 8125 8800 30  0001 C CNN
F 1 "PWR_FLAG" H 8125 8883 30  0000 C CNN
F 2 "" H 8125 8750 60  0000 C CNN
F 3 "" H 8125 8750 60  0000 C CNN
	1    8125 8750
	1    0    0    -1  
$EndComp
$Comp
L appli_power:PWR_FLAG #FLG?
U 1 1 5AD55228
P 3350 2200
F 0 "#FLG?" H 3350 2250 30  0001 C CNN
F 1 "PWR_FLAG" H 3350 2333 30  0000 C CNN
F 2 "" H 3350 2200 60  0000 C CNN
F 3 "" H 3350 2200 60  0000 C CNN
	1    3350 2200
	1    0    0    -1  
$EndComp
$Comp
L appli_power:PWR_FLAG #FLG?
U 1 1 5AD5527D
P 13125 5000
F 0 "#FLG?" H 13125 5050 30  0001 C CNN
F 1 "PWR_FLAG" H 13125 5133 30  0000 C CNN
F 2 "" H 13125 5000 60  0000 C CNN
F 3 "" H 13125 5000 60  0000 C CNN
	1    13125 5000
	1    0    0    -1  
$EndComp
$Comp
L appli_capacitor:100n_X7R_1608M C?
U 1 1 5ADBDD4B
P 12550 2925
F 0 "C?" H 12628 2860 50  0000 L CNN
F 1 "100n_X7R_1608M" H 12628 2780 35  0000 L CNN
F 2 "CAPC1608*" V 12665 2825 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1732728.pdf" V 12690 2825 20  0001 C CNN
F 4 "KEMET" V 12740 2825 20  0001 C CNN "manf"
F 5 "C0603C104K5RACAUTO" V 12765 2825 20  0001 C CNN "manf#"
F 6 "Element14" V 12790 2825 20  0001 C CNN "Supplier"
F 7 "2070398" V 12815 2825 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c0603c104k5racauto/cap-mlcc-x7r-100nf-50v-0603/dp/2070398" V 12840 2825 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 12865 2825 20  0001 C CNN "Supplier Price"
F 10 "1" V 12890 2825 20  0001 C CNN "Supplier Price Break"
	1    12550 2925
	1    0    0    -1  
$EndComp
$Comp
L appli_capacitor:1u_X7R_2012M C?
U 1 1 5ADBE365
P 7550 6600
F 0 "C?" H 7473 6465 50  0000 R CNN
F 1 "1u_X7R_2012M" H 7473 6545 35  0000 R CNN
F 2 "CAPC2012*" V 7665 6500 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2047889.pdf" V 7690 6500 20  0001 C CNN
F 4 "muRata" V 7740 6500 20  0001 C CNN "manf"
F 5 "GRM21BR71C105KA01L" V 7765 6500 20  0001 C CNN "manf#"
F 6 "Element14" V 7790 6500 20  0001 C CNN "Supplier"
F 7 "9527710" V 7815 6500 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/murata/grm21br71c105ka01l/cap-mlcc-x7r-1uf-16v-0805/dp/9527710" V 7840 6500 20  0001 C CNN "Supplier URL"
F 9 "0.224" V 7865 6500 20  0001 C CNN "Supplier Price"
F 10 "1" V 7890 6500 20  0001 C CNN "Supplier Price Break"
	1    7550 6600
	1    0    0    1   
$EndComp
$Comp
L appli_capacitor:1u_X7R_2012M C?
U 1 1 5ADCA287
P 7750 6300
F 0 "C?" H 7672 6165 50  0000 R CNN
F 1 "1u_X7R_2012M" H 7672 6245 35  0000 R CNN
F 2 "CAPC2012*" V 7865 6200 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2047889.pdf" V 7890 6200 20  0001 C CNN
F 4 "muRata" V 7940 6200 20  0001 C CNN "manf"
F 5 "GRM21BR71C105KA01L" V 7965 6200 20  0001 C CNN "manf#"
F 6 "Element14" V 7990 6200 20  0001 C CNN "Supplier"
F 7 "9527710" V 8015 6200 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/murata/grm21br71c105ka01l/cap-mlcc-x7r-1uf-16v-0805/dp/9527710" V 8040 6200 20  0001 C CNN "Supplier URL"
F 9 "0.224" V 8065 6200 20  0001 C CNN "Supplier Price"
F 10 "1" V 8090 6200 20  0001 C CNN "Supplier Price Break"
	1    7750 6300
	-1   0    0    1   
$EndComp
$Comp
L appli_capacitor:330p_X7R_1608M C?
U 1 1 5ADCA53F
P 6500 6400
F 0 "C?" V 6375 6250 50  0000 L CNN
F 1 "330p_X7R_1608M" V 6600 5900 35  0000 L CNN
F 2 "CAPC1608*" V 6615 6300 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2068140.pdf" V 6640 6300 20  0001 C CNN
F 4 "AVX" V 6690 6300 20  0001 C CNN "manf"
F 5 "06035A331JAT2A" V 6715 6300 20  0001 C CNN "manf#"
F 6 "Element14" V 6740 6300 20  0001 C CNN "Supplier"
F 7 "316660" V 6765 6300 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/avx/06035a331jat2a/cap-mlcc-c0g-np0-330pf-50v-0603/dp/316660" V 6790 6300 20  0001 C CNN "Supplier URL"
F 9 "0.21" V 6815 6300 20  0001 C CNN "Supplier Price"
F 10 "10" V 6840 6300 20  0001 C CNN "Supplier Price Break"
	1    6500 6400
	1    0    0    -1  
$EndComp
$Comp
L appli_capacitor:100n_X7R_1608M C?
U 1 1 5ADCA97C
P 6150 6400
F 0 "C?" V 6025 6250 50  0000 L CNN
F 1 "100n_X7R_1608M" V 6250 5925 35  0000 L CNN
F 2 "CAPC1608*" V 6265 6300 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1732728.pdf" V 6290 6300 20  0001 C CNN
F 4 "KEMET" V 6340 6300 20  0001 C CNN "manf"
F 5 "C0603C104K5RACAUTO" V 6365 6300 20  0001 C CNN "manf#"
F 6 "Element14" V 6390 6300 20  0001 C CNN "Supplier"
F 7 "2070398" V 6415 6300 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c0603c104k5racauto/cap-mlcc-x7r-100nf-50v-0603/dp/2070398" V 6440 6300 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 6465 6300 20  0001 C CNN "Supplier Price"
F 10 "1" V 6490 6300 20  0001 C CNN "Supplier Price Break"
	1    6150 6400
	1    0    0    -1  
$EndComp
$Comp
L appli_capacitor:1n5_X7R_1608M C?
U 1 1 5ADCAF13
P 5250 6300
F 0 "C?" H 5173 6165 50  0000 R CNN
F 1 "1n5_X7R_1608M" H 5173 6245 35  0000 R CNN
F 2 "CAPC1608*" V 5365 6200 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1955686.pdf" V 5390 6200 20  0001 C CNN
F 4 "muRata" V 5440 6200 20  0001 C CNN "manf"
F 5 "GRM188R71H152KA01D" V 5465 6200 20  0001 C CNN "manf#"
F 6 "Element14" V 5490 6200 20  0001 C CNN "Supplier"
F 7 "2494239RL" V 5515 6200 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/murata/grm188r71h152ka01d/cap-mlcc-x7r-1500pf-50v-0603/dp/2494239RL" V 5540 6200 20  0001 C CNN "Supplier URL"
F 9 "0.209" V 5565 6200 20  0001 C CNN "Supplier Price"
F 10 "10" V 5590 6200 20  0001 C CNN "Supplier Price Break"
	1    5250 6300
	1    0    0    1   
$EndComp
$Comp
L appli_capacitor:15p_X7R_1608M C?
U 1 1 5ADD6F55
P 4500 5875
F 0 "C?" H 4578 5810 50  0000 L CNN
F 1 "15p_X7R_1608M" H 4578 5730 35  0000 L CNN
F 2 "CAPC1608*" V 4615 5775 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2291921.pdf" V 4640 5775 20  0001 C CNN
F 4 "TDK" V 4690 5775 20  0001 C CNN "manf"
F 5 "C1608C0G1H150J080AA" V 4715 5775 20  0001 C CNN "manf#"
F 6 "Element14" V 4740 5775 20  0001 C CNN "Supplier"
F 7 "1907286RL" V 4765 5775 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/tdk/c1608c0g1h150j080aa/cap-mlcc-c0g-np0-15pf-50v-0603/dp/1907286RL" V 4790 5775 20  0001 C CNN "Supplier URL"
F 9 "0.0121" V 4815 5775 20  0001 C CNN "Supplier Price"
F 10 "50" V 4840 5775 20  0001 C CNN "Supplier Price Break"
	1    4500 5875
	1    0    0    -1  
$EndComp
$Comp
L appli_capacitor:1u_X7R_1608M C?
U 1 1 5ADD72D1
P 4850 5150
F 0 "C?" H 4773 5015 50  0000 R CNN
F 1 "1u_X7R_1608M" H 4773 5095 35  0000 R CNN
F 2 "CAPC1608*" V 4965 5050 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2048611.pdf?_ga=1.116411968.774814437.1442284889" V 4990 5050 20  0001 C CNN
F 4 "muRata" V 5040 5050 20  0001 C CNN "manf"
F 5 "GRM188R61H105KAALD" V 5065 5050 20  0001 C CNN "manf#"
F 6 "Element14" V 5090 5050 20  0001 C CNN "Supplier"
F 7 "1845736" V 5115 5050 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/murata/grm188r61h105kaald/cap-mlcc-x5r-1uf-50v-0603/dp/1845736" V 5140 5050 20  0001 C CNN "Supplier URL"
F 9 "0.092" V 5165 5050 20  0001 C CNN "Supplier Price"
F 10 "100" V 5190 5050 20  0001 C CNN "Supplier Price Break"
	1    4850 5150
	1    0    0    1   
$EndComp
$Comp
L appli_capacitor:1u_X7R_2012M C?
U 1 1 5ADD78B1
P 8700 5000
F 0 "C?" H 8623 4865 50  0000 R CNN
F 1 "1u_X7R_2012M" H 8623 4945 35  0000 R CNN
F 2 "CAPC2012*" V 8815 4900 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2047889.pdf" V 8840 4900 20  0001 C CNN
F 4 "muRata" V 8890 4900 20  0001 C CNN "manf"
F 5 "GRM21BR71C105KA01L" V 8915 4900 20  0001 C CNN "manf#"
F 6 "Element14" V 8940 4900 20  0001 C CNN "Supplier"
F 7 "9527710" V 8965 4900 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/murata/grm21br71c105ka01l/cap-mlcc-x7r-1uf-16v-0805/dp/9527710" V 8990 4900 20  0001 C CNN "Supplier URL"
F 9 "0.224" V 9015 4900 20  0001 C CNN "Supplier Price"
F 10 "1" V 9040 4900 20  0001 C CNN "Supplier Price Break"
	1    8700 5000
	1    0    0    1   
$EndComp
$Comp
L appli_resistor:R01_3216M R?
U 1 1 5ADD7A8A
P 9600 5925
F 0 "R?" H 9673 5810 50  0000 L CNN
F 1 "R01_3216M" H 9673 5730 35  0000 L CNN
F 2 "" V 9710 5775 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2049155.pdf?" V 9735 5775 20  0001 C CNN
F 4 "Vishay" V 9785 5775 20  0001 C CNN "manf"
F 5 "WSLP1206R0100FEA" V 9810 5775 20  0001 C CNN "manf#"
F 6 "Element14" V 9835 5775 20  0001 C CNN "Supplier"
F 7 "683-6215" V 9860 5775 20  0001 C CNN "Supplier Part No"
F 8 "http://au.rs-online.com/web/p/surface-mount-fixed-resistors/6836215" V 9885 5775 20  0001 C CNN "Supplier URL"
F 9 "1.15" V 9910 5775 20  0001 C CNN "Supplier Price"
F 10 "1" V 9935 5775 20  0001 C CNN "Supplier Price Break"
	1    9600 5925
	1    0    0    -1  
$EndComp
Connection ~ 9600 6225
$Comp
L appli_inductor:10u2_PA2050 L?
U 1 1 5ADD7CF3
P 10550 5050
F 0 "L?" V 10363 4900 55  0000 C CNN
F 1 "10u2_PA2050" V 10447 4900 35  0000 C CNN
F 2 "Applidyne_Inductor:PULSE_PA2050" V 10665 4900 20  0001 C CNN
F 3 "https://au.mouser.com/datasheet/2/336/-369081.pdf" V 10690 4900 20  0001 C CNN
F 4 "PULSE ELECTRONICS" V 10740 4900 20  0001 C CNN "manf"
F 5 "PA2050.103NL" V 11050 4900 20  0001 C CNN "manf#"
F 6 "Mouser" V 10790 4900 20  0001 C CNN "Supplier"
F 7 "673-PA2050.103NL" V 10950 4900 20  0001 C CNN "Supplier Part No"
F 8 "https://au.mouser.com/ProductDetail/Pulse-Electronics/PA2050103NL" V 11000 4900 20  0001 C CNN "Supplier URL"
F 9 "5.59" V 10865 4900 20  0001 C CNN "Supplier Price"
F 10 "1" V 10890 4900 20  0001 C CNN "Supplier Price Break"
	1    10550 5050
	0    1    1    0   
$EndComp
Wire Wire Line
	9600 5050 9600 5000
$Comp
L appli_device:200V_350mW_SOT23 D?
U 1 1 5AD656F9
P 7950 4450
F 0 "D?" H 7950 4245 50  0000 C CNN
F 1 "200V_350mW_SOT23" H 7950 4336 50  0000 C CNN
F 2 "Applidyne_SOT:SOT95P240X110-3L16N" H 7950 4300 20  0001 C CNN
F 3 "https://au.mouser.com/datasheet/2/68/cmpd2003s_3c_4c-55024.pdf" H 7950 4275 20  0001 C CNN
F 4 "Central Semiconductor" H 7950 4225 20  0001 C CNN "manf"
F 5 "CMPD2003C-TR" H 7950 4200 20  0001 C CNN "manf#"
F 6 "Mouser" H 7950 4175 20  0001 C CNN "Supplier"
F 7 "610-CMPD2003CTR" H 7950 4150 20  0001 C CNN "Supplier Part No"
F 8 "https://au.mouser.com/ProductDetail/Central-Semiconductor/CMPD2003C-TR" H 7950 4125 20  0001 C CNN "Supplier URL"
F 9 "0.59" H 7950 4100 20  0001 C CNN "Supplier Price"
F 10 "1" H 7950 4075 20  0001 C CNN "Supplier Price Break"
	1    7950 4450
	-1   0    0    1   
$EndComp
$Comp
L appli_device:200V_350mW_SOT23 D?
U 1 1 5AD65862
P 4850 4650
F 0 "D?" V 4800 4425 50  0000 L CNN
F 1 "200V_350mW_SOT23" V 4900 3825 50  0000 L CNN
F 2 "Applidyne_SOT:SOT95P240X110-3L16N" H 4850 4500 20  0001 C CNN
F 3 "https://au.mouser.com/datasheet/2/68/cmpd2003s_3c_4c-55024.pdf" H 4850 4475 20  0001 C CNN
F 4 "Central Semiconductor" H 4850 4425 20  0001 C CNN "manf"
F 5 "CMPD2003C-TR" H 4850 4400 20  0001 C CNN "manf#"
F 6 "Mouser" H 4850 4375 20  0001 C CNN "Supplier"
F 7 "610-CMPD2003CTR" H 4850 4350 20  0001 C CNN "Supplier Part No"
F 8 "https://au.mouser.com/ProductDetail/Central-Semiconductor/CMPD2003C-TR" H 4850 4325 20  0001 C CNN "Supplier URL"
F 9 "0.59" H 4850 4300 20  0001 C CNN "Supplier Price"
F 10 "1" H 4850 4275 20  0001 C CNN "Supplier Price Break"
	1    4850 4650
	0    1    1    0   
$EndComp
Wire Wire Line
	4850 4850 4850 4750
Connection ~ 4850 4850
Wire Wire Line
	4850 4550 4850 4450
Connection ~ 4850 4450
Wire Wire Line
	4850 4450 5200 4450
$Comp
L appli_power:GND #PWR?
U 1 1 5AD7AEE5
P 5200 5225
F 0 "#PWR?" H 5200 4975 50  0001 C CNN
F 1 "GND" H 5350 5175 50  0000 C CNN
F 2 "" H 5200 5225 60  0000 C CNN
F 3 "" H 5200 5225 60  0000 C CNN
	1    5200 5225
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 5200 5200 5225
Wire Wire Line
	4000 5350 4000 5075
Wire Wire Line
	4000 5450 5800 5450
Wire Wire Line
	5800 5450 5800 5350
Wire Wire Line
	4000 5450 4000 5600
Wire Wire Line
	5800 5350 5900 5350
Wire Wire Line
	4850 5225 5200 5225
Wire Wire Line
	4850 5150 4850 5225
Connection ~ 5200 5225
$Comp
L appli_power:GND #PWR?
U 1 1 5ADCECDF
P 4000 6850
F 0 "#PWR?" H 4000 6600 50  0001 C CNN
F 1 "GND" H 4005 6677 50  0000 C CNN
F 2 "" H 4000 6850 60  0000 C CNN
F 3 "" H 4000 6850 60  0000 C CNN
	1    4000 6850
	1    0    0    -1  
$EndComp
$Comp
L appli_transistor:Si7852DP Q?
U 1 1 5ADE3674
P 9500 4800
F 0 "Q?" H 9691 4846 50  0000 L CNN
F 1 "Si7852DP" H 9691 4755 50  0000 L CNN
F 2 "Applidyne_SON:SON127P490X575X90-9T313X381N" H 9700 4700 50  0001 L CIN
F 3 "https://au.mouser.com/datasheet/2/427/si7852dp-241219.pdf" H 7870 4530 50  0001 L CNN
F 4 "Vishay" H 9500 4300 60  0001 C CNN "manf"
F 5 "SI7852DP-T1-GE3" H 9500 4180 60  0001 C CNN "manf#"
F 6 "Mouser" H 9500 4080 60  0001 C CNN "Supplier"
F 7 "781-SI7852DP-GE3" H 9500 3830 60  0001 C CNN "Supplier Part No"
F 8 "https://au.mouser.com/ProductDetail/Vishay-Siliconix/SI7852DP-T1-GE3" H 9500 3960 60  0001 C CNN "Supplier URL"
F 9 "4.1" H 9500 3710 60  0001 C CNN "Supplier Price"
F 10 "1" H 9500 3580 60  0001 C CNN "Supplier Price Break"
	1    9500 4800
	1    0    0    -1  
$EndComp
$Comp
L appli_transistor:Si7852DP Q?
U 1 1 5ADE380B
P 9500 5300
F 0 "Q?" H 9691 5346 50  0000 L CNN
F 1 "Si7852DP" H 9691 5255 50  0000 L CNN
F 2 "Applidyne_SON:SON127P490X575X90-9T313X381N" H 9700 5200 50  0001 L CIN
F 3 "https://au.mouser.com/datasheet/2/427/si7852dp-241219.pdf" H 7870 5030 50  0001 L CNN
F 4 "Vishay" H 9500 4800 60  0001 C CNN "manf"
F 5 "SI7852DP-T1-GE3" H 9500 4680 60  0001 C CNN "manf#"
F 6 "Mouser" H 9500 4580 60  0001 C CNN "Supplier"
F 7 "781-SI7852DP-GE3" H 9500 4330 60  0001 C CNN "Supplier Part No"
F 8 "https://au.mouser.com/ProductDetail/Vishay-Siliconix/SI7852DP-T1-GE3" H 9500 4460 60  0001 C CNN "Supplier URL"
F 9 "4.1" H 9500 4210 60  0001 C CNN "Supplier Price"
F 10 "1" H 9500 4080 60  0001 C CNN "Supplier Price Break"
	1    9500 5300
	1    0    0    -1  
$EndComp
$Comp
L appli_capacitor:22u_X7R_3225M C?
U 1 1 5ADF1E40
P 12400 5550
F 0 "C?" H 12225 5450 50  0000 L CNN
F 1 "22u_X7R_3225M" V 12525 5250 35  0000 L CNN
F 2 "CAPC3225*" V 12515 5450 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2111999.pdf?_ga=1.157172477.774814437.1442284889" V 12540 5450 20  0001 C CNN
F 4 "muRata" V 12590 5450 20  0001 C CNN "manf"
F 5 "C1210C226K3RACTU" V 12615 5450 20  0001 C CNN "manf#"
F 6 "Element14" V 12640 5450 20  0001 C CNN "Supplier"
F 7 "2473550" V 12665 5450 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c1210c226k3ractu/cap-mlcc-x7r-22uf-25v-1210/dp/2473550" V 12690 5450 20  0001 C CNN "Supplier URL"
F 9 "1.02" V 12715 5450 20  0001 C CNN "Supplier Price"
F 10 "50" V 12740 5450 20  0001 C CNN "Supplier Price Break"
	1    12400 5550
	1    0    0    -1  
$EndComp
$Comp
L appli_capacitor:47u_TANT_7343M C?
U 1 1 5ADF1FAB
P 10800 5550
F 0 "C?" H 10600 5450 50  0000 L CNN
F 1 "47u_TANT_7343M" V 10925 5225 35  0000 L CNN
F 2 "CAPPM7343*" V 10915 5450 20  0001 C CNN
F 3 "https://au.mouser.com/datasheet/2/212/Datasheet-T520D476M016ATE035-1079046.pdf" V 10940 5450 20  0001 C CNN
F 4 "KEMET" V 10990 5450 20  0001 C CNN "manf"
F 5 "T520D476M016ATE035" V 11015 5450 20  0001 C CNN "manf#"
F 6 "Mouser" V 11040 5450 20  0001 C CNN "Supplier"
F 7 "80-T520D476M16E35" V 11065 5450 20  0001 C CNN "Supplier Part No"
F 8 "https://au.mouser.com/ProductDetail/?qs=23lJVmSKTBTa5vz24B8T6A%3D%3D" V 11090 5450 20  0001 C CNN "Supplier URL"
F 9 "3.07" V 11115 5450 20  0001 C CNN "Supplier Price"
F 10 "10" V 11140 5450 20  0001 C CNN "Supplier Price Break"
	1    10800 5550
	1    0    0    -1  
$EndComp
$Comp
L appli_capacitor:47u_TANT_7343M C?
U 1 1 5ADF2433
P 11200 5550
F 0 "C?" H 11000 5450 50  0000 L CNN
F 1 "47u_TANT_7343M" V 11325 5225 35  0000 L CNN
F 2 "CAPPM7343*" V 11315 5450 20  0001 C CNN
F 3 "https://au.mouser.com/datasheet/2/212/Datasheet-T520D476M016ATE035-1079046.pdf" V 11340 5450 20  0001 C CNN
F 4 "KEMET" V 11390 5450 20  0001 C CNN "manf"
F 5 "T520D476M016ATE035" V 11415 5450 20  0001 C CNN "manf#"
F 6 "Mouser" V 11440 5450 20  0001 C CNN "Supplier"
F 7 "80-T520D476M16E35" V 11465 5450 20  0001 C CNN "Supplier Part No"
F 8 "https://au.mouser.com/ProductDetail/?qs=23lJVmSKTBTa5vz24B8T6A%3D%3D" V 11490 5450 20  0001 C CNN "Supplier URL"
F 9 "3.07" V 11515 5450 20  0001 C CNN "Supplier Price"
F 10 "10" V 11540 5450 20  0001 C CNN "Supplier Price Break"
	1    11200 5550
	1    0    0    -1  
$EndComp
$Comp
L appli_capacitor:47u_TANT_7343M C?
U 1 1 5ADF24BD
P 11600 5550
F 0 "C?" H 11400 5450 50  0000 L CNN
F 1 "47u_TANT_7343M" V 11725 5225 35  0000 L CNN
F 2 "CAPPM7343*" V 11715 5450 20  0001 C CNN
F 3 "https://au.mouser.com/datasheet/2/212/Datasheet-T520D476M016ATE035-1079046.pdf" V 11740 5450 20  0001 C CNN
F 4 "KEMET" V 11790 5450 20  0001 C CNN "manf"
F 5 "T520D476M016ATE035" V 11815 5450 20  0001 C CNN "manf#"
F 6 "Mouser" V 11840 5450 20  0001 C CNN "Supplier"
F 7 "80-T520D476M16E35" V 11865 5450 20  0001 C CNN "Supplier Part No"
F 8 "https://au.mouser.com/ProductDetail/?qs=23lJVmSKTBTa5vz24B8T6A%3D%3D" V 11890 5450 20  0001 C CNN "Supplier URL"
F 9 "3.07" V 11915 5450 20  0001 C CNN "Supplier Price"
F 10 "10" V 11940 5450 20  0001 C CNN "Supplier Price Break"
	1    11600 5550
	1    0    0    -1  
$EndComp
$Comp
L appli_capacitor:47u_TANT_7343M C?
U 1 1 5ADF2543
P 12000 5550
F 0 "C?" H 11800 5450 50  0000 L CNN
F 1 "47u_TANT_7343M" V 12125 5225 35  0000 L CNN
F 2 "CAPPM7343*" V 12115 5450 20  0001 C CNN
F 3 "https://au.mouser.com/datasheet/2/212/Datasheet-T520D476M016ATE035-1079046.pdf" V 12140 5450 20  0001 C CNN
F 4 "KEMET" V 12190 5450 20  0001 C CNN "manf"
F 5 "T520D476M016ATE035" V 12215 5450 20  0001 C CNN "manf#"
F 6 "Mouser" V 12240 5450 20  0001 C CNN "Supplier"
F 7 "80-T520D476M16E35" V 12265 5450 20  0001 C CNN "Supplier Part No"
F 8 "https://au.mouser.com/ProductDetail/?qs=23lJVmSKTBTa5vz24B8T6A%3D%3D" V 12290 5450 20  0001 C CNN "Supplier URL"
F 9 "3.07" V 12315 5450 20  0001 C CNN "Supplier Price"
F 10 "10" V 12340 5450 20  0001 C CNN "Supplier Price Break"
	1    12000 5550
	1    0    0    -1  
$EndComp
Wire Wire Line
	10800 5550 10800 5050
Connection ~ 10800 5050
Wire Wire Line
	10800 5050 11200 5050
Wire Wire Line
	11200 5550 11200 5050
Connection ~ 11200 5050
Wire Wire Line
	11200 5050 11600 5050
Wire Wire Line
	11600 5550 11600 5050
Connection ~ 11600 5050
Wire Wire Line
	11600 5050 12000 5050
Wire Wire Line
	12000 5550 12000 5050
Connection ~ 12000 5050
Wire Wire Line
	12000 5050 12400 5050
Wire Wire Line
	12400 5550 12400 5050
Connection ~ 12400 5050
Wire Wire Line
	12400 5750 12400 6850
Wire Wire Line
	12000 5750 12000 6850
Wire Wire Line
	11600 5750 11600 6850
Wire Wire Line
	11200 5750 11200 6850
Wire Wire Line
	10800 5750 10800 6850
$Comp
L appli_power:GND #PWR?
U 1 1 5AE2FCFB
P 10800 6850
F 0 "#PWR?" H 10800 6600 50  0001 C CNN
F 1 "GND" H 10805 6677 50  0000 C CNN
F 2 "" H 10800 6850 60  0000 C CNN
F 3 "" H 10800 6850 60  0000 C CNN
	1    10800 6850
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5AE2FD65
P 11200 6850
F 0 "#PWR?" H 11200 6600 50  0001 C CNN
F 1 "GND" H 11205 6677 50  0000 C CNN
F 2 "" H 11200 6850 60  0000 C CNN
F 3 "" H 11200 6850 60  0000 C CNN
	1    11200 6850
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5AE2FDC8
P 11600 6850
F 0 "#PWR?" H 11600 6600 50  0001 C CNN
F 1 "GND" H 11605 6677 50  0000 C CNN
F 2 "" H 11600 6850 60  0000 C CNN
F 3 "" H 11600 6850 60  0000 C CNN
	1    11600 6850
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5AE2FE2B
P 12000 6850
F 0 "#PWR?" H 12000 6600 50  0001 C CNN
F 1 "GND" H 12005 6677 50  0000 C CNN
F 2 "" H 12000 6850 60  0000 C CNN
F 3 "" H 12000 6850 60  0000 C CNN
	1    12000 6850
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5AE2FE8E
P 12400 6850
F 0 "#PWR?" H 12400 6600 50  0001 C CNN
F 1 "GND" H 12405 6677 50  0000 C CNN
F 2 "" H 12400 6850 60  0000 C CNN
F 3 "" H 12400 6850 60  0000 C CNN
	1    12400 6850
	1    0    0    -1  
$EndComp
$Comp
L appli_capacitor:47u_ELEC_100V_1350X1350M C?
U 1 1 5AE30345
P 1550 5350
F 0 "C?" H 1375 5250 50  0000 L CNN
F 1 "47u_ELEC_100V_1350X1350M" V 1675 4850 35  0000 L CNN
F 2 "Applidyne_Capacitor:CAPAE1350X1350D1490DD450L470N" V 1665 5250 20  0001 C CNN
F 3 "https://au.mouser.com/datasheet/2/315/ABA0000C1181-947564.pdf" V 1690 5250 20  0001 C CNN
F 4 "PANASONIC" V 1740 5250 20  0001 C CNN "manf"
F 5 "EEV-FK2A470Q" V 1765 5250 20  0001 C CNN "manf#"
F 6 "Mouser" V 1790 5250 20  0001 C CNN "Supplier"
F 7 "667-EEV-FK2A470Q" V 1815 5250 20  0001 C CNN "Supplier Part No"
F 8 "https://au.mouser.com/productdetail/?qs=f%252bPvLx1Hi0lKaMF67jB0og%3D%3D" V 1840 5250 20  0001 C CNN "Supplier URL"
F 9 "1.66" V 1865 5250 20  0001 C CNN "Supplier Price"
F 10 "1" V 1890 5250 20  0001 C CNN "Supplier Price Break"
	1    1550 5350
	1    0    0    -1  
$EndComp
$Comp
L appli_capacitor:2u2_X7R_4532M C?
U 1 1 5AE32FC1
P 2100 5350
F 0 "C?" H 1925 5250 50  0000 L CNN
F 1 "2u2_X7R_4532M" V 2225 5025 35  0000 L CNN
F 2 "Applidyne_Capacitor:CAPC4532X180N" V 2215 5250 20  0001 C CNN
F 3 "https://au.mouser.com/datasheet/2/400/lcc_commercial_highvoltage_en-520036.pdf" V 2240 5250 20  0001 C CNN
F 4 "TDK" V 2290 5250 20  0001 C CNN "manf"
F 5 "C4532X7R2A225M230KA" V 2315 5250 20  0001 C CNN "manf#"
F 6 "Mouser" V 2340 5250 20  0001 C CNN "Supplier"
F 7 "810-C4532X7R2A225M" V 2365 5250 20  0001 C CNN "Supplier Part No"
F 8 "https://au.mouser.com/ProductDetail/TDK/C4532X7R2A225M230KA" V 2390 5250 20  0001 C CNN "Supplier URL"
F 9 "1.41" V 2415 5250 20  0001 C CNN "Supplier Price"
F 10 "10" V 2440 5250 20  0001 C CNN "Supplier Price Break"
	1    2100 5350
	1    0    0    -1  
$EndComp
$Comp
L appli_capacitor:2u2_X7R_4532M C?
U 1 1 5AE33709
P 2500 5350
F 0 "C?" H 2325 5250 50  0000 L CNN
F 1 "2u2_X7R_4532M" V 2625 5025 35  0000 L CNN
F 2 "Applidyne_Capacitor:CAPC4532X180N" V 2615 5250 20  0001 C CNN
F 3 "https://au.mouser.com/datasheet/2/400/lcc_commercial_highvoltage_en-520036.pdf" V 2640 5250 20  0001 C CNN
F 4 "TDK" V 2690 5250 20  0001 C CNN "manf"
F 5 "C4532X7R2A225M230KA" V 2715 5250 20  0001 C CNN "manf#"
F 6 "Mouser" V 2740 5250 20  0001 C CNN "Supplier"
F 7 "810-C4532X7R2A225M" V 2765 5250 20  0001 C CNN "Supplier Part No"
F 8 "https://au.mouser.com/ProductDetail/TDK/C4532X7R2A225M230KA" V 2790 5250 20  0001 C CNN "Supplier URL"
F 9 "1.41" V 2815 5250 20  0001 C CNN "Supplier Price"
F 10 "10" V 2840 5250 20  0001 C CNN "Supplier Price Break"
	1    2500 5350
	1    0    0    -1  
$EndComp
$Comp
L appli_capacitor:2u2_X7R_4532M C?
U 1 1 5AE33795
P 2900 5350
F 0 "C?" H 2725 5250 50  0000 L CNN
F 1 "2u2_X7R_4532M" V 3025 5025 35  0000 L CNN
F 2 "Applidyne_Capacitor:CAPC4532X180N" V 3015 5250 20  0001 C CNN
F 3 "https://au.mouser.com/datasheet/2/400/lcc_commercial_highvoltage_en-520036.pdf" V 3040 5250 20  0001 C CNN
F 4 "TDK" V 3090 5250 20  0001 C CNN "manf"
F 5 "C4532X7R2A225M230KA" V 3115 5250 20  0001 C CNN "manf#"
F 6 "Mouser" V 3140 5250 20  0001 C CNN "Supplier"
F 7 "810-C4532X7R2A225M" V 3165 5250 20  0001 C CNN "Supplier Part No"
F 8 "https://au.mouser.com/ProductDetail/TDK/C4532X7R2A225M230KA" V 3190 5250 20  0001 C CNN "Supplier URL"
F 9 "1.41" V 3215 5250 20  0001 C CNN "Supplier Price"
F 10 "10" V 3240 5250 20  0001 C CNN "Supplier Price Break"
	1    2900 5350
	1    0    0    -1  
$EndComp
$Comp
L appli_capacitor:2u2_X7R_4532M C?
U 1 1 5AE3382F
P 3300 5350
F 0 "C?" H 3125 5250 50  0000 L CNN
F 1 "2u2_X7R_4532M" V 3425 5025 35  0000 L CNN
F 2 "Applidyne_Capacitor:CAPC4532X180N" V 3415 5250 20  0001 C CNN
F 3 "https://au.mouser.com/datasheet/2/400/lcc_commercial_highvoltage_en-520036.pdf" V 3440 5250 20  0001 C CNN
F 4 "TDK" V 3490 5250 20  0001 C CNN "manf"
F 5 "C4532X7R2A225M230KA" V 3515 5250 20  0001 C CNN "manf#"
F 6 "Mouser" V 3540 5250 20  0001 C CNN "Supplier"
F 7 "810-C4532X7R2A225M" V 3565 5250 20  0001 C CNN "Supplier Part No"
F 8 "https://au.mouser.com/ProductDetail/TDK/C4532X7R2A225M230KA" V 3590 5250 20  0001 C CNN "Supplier URL"
F 9 "1.41" V 3615 5250 20  0001 C CNN "Supplier Price"
F 10 "10" V 3640 5250 20  0001 C CNN "Supplier Price Break"
	1    3300 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	1550 5350 1550 4450
Connection ~ 1550 4450
Wire Wire Line
	1550 4450 1550 4350
Wire Wire Line
	2100 5350 2100 4450
Connection ~ 2100 4450
Wire Wire Line
	2100 4450 1550 4450
Wire Wire Line
	2500 5350 2500 4450
Connection ~ 2500 4450
Wire Wire Line
	2500 4450 2100 4450
Wire Wire Line
	2900 5350 2900 4450
Connection ~ 2900 4450
Wire Wire Line
	2900 4450 2500 4450
Wire Wire Line
	3300 5350 3300 4450
Connection ~ 3300 4450
Wire Wire Line
	3300 4450 2900 4450
Wire Wire Line
	3300 5550 3300 6850
Wire Wire Line
	2900 5550 2900 6850
Wire Wire Line
	2500 5550 2500 6850
Wire Wire Line
	2100 5550 2100 6850
Wire Wire Line
	1550 5550 1550 6850
$Comp
L appli_power:GND #PWR?
U 1 1 5AE7973A
P 3300 6850
F 0 "#PWR?" H 3300 6600 50  0001 C CNN
F 1 "GND" H 3305 6677 50  0000 C CNN
F 2 "" H 3300 6850 60  0000 C CNN
F 3 "" H 3300 6850 60  0000 C CNN
	1    3300 6850
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5AE797A7
P 2900 6850
F 0 "#PWR?" H 2900 6600 50  0001 C CNN
F 1 "GND" H 2905 6677 50  0000 C CNN
F 2 "" H 2900 6850 60  0000 C CNN
F 3 "" H 2900 6850 60  0000 C CNN
	1    2900 6850
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5AE79814
P 2500 6850
F 0 "#PWR?" H 2500 6600 50  0001 C CNN
F 1 "GND" H 2505 6677 50  0000 C CNN
F 2 "" H 2500 6850 60  0000 C CNN
F 3 "" H 2500 6850 60  0000 C CNN
	1    2500 6850
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5AE79881
P 2100 6850
F 0 "#PWR?" H 2100 6600 50  0001 C CNN
F 1 "GND" H 2105 6677 50  0000 C CNN
F 2 "" H 2100 6850 60  0000 C CNN
F 3 "" H 2100 6850 60  0000 C CNN
	1    2100 6850
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5AE798EE
P 1550 6850
F 0 "#PWR?" H 1550 6600 50  0001 C CNN
F 1 "GND" H 1555 6677 50  0000 C CNN
F 2 "" H 1550 6850 60  0000 C CNN
F 3 "" H 1550 6850 60  0000 C CNN
	1    1550 6850
	1    0    0    -1  
$EndComp
$Comp
L appli_resistor:2R2_1608M R?
U 1 1 5AEA5393
P 7800 4750
F 0 "R?" V 7925 4600 50  0000 C CNN
F 1 "2R2_1608M" V 8000 4600 35  0000 C CNN
F 2 "RESC1608*" V 7910 4600 20  0001 C CNN
F 3 "http://industrial.panasonic.com/www-cgi/jvcr13pz.cgi?E+PZ+3+AOA0005+ERJS06J821V+7+WW" V 7935 4600 20  0001 C CNN
F 4 "PANASONIC" V 7985 4600 20  0001 C CNN "manf"
F 5 "ERJ-S03J2R2V" V 8010 4600 20  0001 C CNN "manf#"
F 6 "Mouser" V 8035 4600 20  0001 C CNN "Supplier"
F 7 "667-ERJ-S03J2R2V" V 8060 4600 20  0001 C CNN "Supplier Part No"
F 8 "https://au.mouser.com/ProductDetail/Panasonic/ERJ-S03J2R2V" V 8085 4600 20  0001 C CNN "Supplier URL"
F 9 "0.084" V 8110 4600 20  0001 C CNN "Supplier Price"
F 10 "50" V 8135 4600 20  0001 C CNN "Supplier Price Break"
	1    7800 4750
	0    -1   1    0   
$EndComp
Text Notes 1925 4300 0    59   ~ 12
15-80V Input
Text Notes 11750 4925 0    59   ~ 12
12V, 5A output
Connection ~ 9600 5925
Wire Wire Line
	9600 5500 9600 5925
$Comp
L appli_resistor:0R_1608M R?
U 1 1 5AF05644
P 9150 5600
F 0 "R?" V 8962 5450 50  0000 C CNN
F 1 "0R_1608M" V 9042 5450 35  0000 C CNN
F 2 "RESC1608*" V 9260 5450 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1739504.pdf" V 9285 5450 20  0001 C CNN
F 4 "MULTICOMP" V 9335 5450 20  0001 C CNN "manf"
F 5 "MC0063W06030R" V 9360 5450 20  0001 C CNN "manf#"
F 6 "Element14" V 9385 5450 20  0001 C CNN "Supplier"
F 7 "9331662" V 9410 5450 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mc0063w06030r/resistor-thick-film-0r-0-063w/dp/9331662" V 9435 5450 20  0001 C CNN "Supplier URL"
F 9 "0.023" V 9460 5450 20  0001 C CNN "Supplier Price"
F 10 "50" V 9485 5450 20  0001 C CNN "Supplier Price Break"
	1    9150 5600
	0    1    1    0   
$EndComp
Wire Wire Line
	8850 5600 8700 5600
Connection ~ 8700 5600
Wire Wire Line
	8700 5600 8700 5925
Wire Wire Line
	9150 5600 9250 5600
Wire Wire Line
	9250 5600 9250 5050
Connection ~ 9250 5050
Wire Wire Line
	9250 5050 9600 5050
Text Notes 9275 5650 0    50   ~ 10
<-DNP
Text Notes 8950 5625 0    35   ~ 7
DNP
Wire Wire Line
	8800 5350 9300 5350
Wire Wire Line
	9600 4600 9600 4000
Wire Wire Line
	9600 4000 3300 4000
Wire Wire Line
	3300 4000 3300 4450
Text Notes 3675 4400 0    50   ~ 0
High current tracks not required into regulator IC
Wire Wire Line
	13125 5000 13125 5050
Wire Wire Line
	12400 5050 12800 5050
Wire Wire Line
	12800 5000 12800 5050
Connection ~ 12800 5050
Wire Wire Line
	12800 5050 13125 5050
Text Notes 5625 3800 0    50   ~ 0
Follows the TI AN-1713 application note for the LM5116.\n250kHz, should achieve >90% efficiency at operating voltage.
Text Notes 8625 6700 0    50   ~ 0
diode bias setting
$Bitmap
Pos 14625 6100
Scale 1.000000
Data
89 50 4E 47 0D 0A 1A 0A 00 00 00 0D 49 48 44 52 00 00 02 A2 00 00 02 07 08 02 00 00 00 CB B2 A9 
F9 00 00 00 03 73 42 49 54 08 08 08 DB E1 4F E0 00 00 00 09 70 48 59 73 00 00 0E 74 00 00 0E 74 
01 6B 24 B3 D6 00 00 20 00 49 44 41 54 78 9C EC DD 67 60 14 D5 1A 06 E0 6F 76 37 BB 49 36 65 D3 
1B E9 21 24 21 21 74 08 BD 23 01 23 E1 A2 34 41 50 B0 01 82 02 36 B0 70 01 41 51 AA A2 D2 11 10 
41 29 52 A4 13 84 4B AF A1 85 92 DE 7B 6F DB 66 EE 8F F4 B6 E9 D9 CD E6 7D 7E 85 D9 99 33 DF 4C 
C2 BE 73 CE 34 86 E3 38 02 00 00 00 6D C4 53 77 01 00 00 00 D0 5C 10 F3 00 00 00 5A 0B 31 0F 00 
00 A0 B5 10 F3 00 00 00 5A 0B 31 0F 00 00 A0 B5 04 AA 3F BE 78 F1 E2 37 DF 7C 93 92 92 D2 32 D5 
80 86 30 34 34 9C 33 67 CE 6B AF BD A6 EE 42 00 00 A0 51 18 D5 37 D4 B9 BB BB BF 78 F1 A2 C5 AA 
01 CD A1 A7 A7 97 90 90 60 6C 6C AC EE 42 00 00 A0 E1 6A 19 B4 47 C6 B7 59 05 05 05 71 71 71 EA 
AE 02 00 00 1A A5 96 41 FB 52 7E 7E 7E FE FE FE CD 5A 0A 68 82 88 88 88 ED DB B7 AB 98 21 29 29 
29 31 31 91 65 D9 16 2B A9 BE 94 4A A5 54 2A D5 E4 0A A1 09 E9 E8 E8 08 85 42 86 61 D4 5D 08 68 
0A 1E 8F 67 65 65 65 6D 6D AD EE 42 34 45 5D 63 7E C9 92 25 88 F9 B6 40 75 CC 4B A5 D2 A7 4F 9F 
E2 C9 89 A0 39 94 4A 65 61 61 A1 BA AB 00 CD 92 91 91 61 64 64 A4 AF AF AF EE 42 34 02 AE B4 87 
7A 28 28 28 40 C6 03 80 E6 CB CF CF 57 77 09 9A 02 31 0F 00 00 A0 B5 10 F3 00 00 00 5A 0B 31 0F 
00 00 A0 B5 10 F3 00 00 00 5A 0B 31 0F 00 00 A0 B5 10 F3 00 00 00 5A 0B 31 0F 00 00 A0 B5 10 F3 
00 00 00 5A 0B 31 0F 00 00 A0 B5 10 F3 00 00 00 5A 0B 31 0F 00 00 A0 B5 10 F3 00 00 00 5A 0B 31 
0F 00 00 A0 B5 10 F3 00 00 00 5A 0B 31 0F 00 00 A0 B5 10 F3 00 00 00 5A 0B 31 0F 00 00 A0 B5 10 
F3 00 00 00 5A 0B 31 0F 00 00 A0 B5 10 F3 00 00 00 5A 0B 31 0F 00 00 A0 B5 10 F3 00 00 00 5A 0B 
31 0F 00 00 A0 B5 10 F3 00 00 00 5A 4B A0 B6 35 B3 D2 CC E4 F8 84 A4 2C 85 9E A9 B5 9D 9D 85 98 
AF B6 4A 00 00 00 B4 54 CB F7 E6 95 C9 37 77 7C 36 79 88 97 B5 B1 A9 AD 8B 57 E7 2E 9D 3A 38 5A 
99 58 B8 F9 FD 67 E1 96 FF C5 C9 5A BC 9C 52 8A 84 EB 57 9F 4A 9B AD 79 65 F8 A6 71 EE CE CE CE 
CE CE CE ED 47 7C FB 40 A1 6A 2A 00 00 40 D3 68 D9 98 97 85 FF 39 C7 CF AB EF 5B AB F6 05 85 A4 
48 39 AE 78 32 27 CF 08 BB 7E E8 87 77 06 75 1A 30 FF 50 78 CB 47 BD 34 EA DC EA D7 7B 78 0F FD 
E6 4A 41 F3 AD 44 96 15 1F 59 2C 3E B3 90 55 39 15 00 00 A0 49 B4 60 CC 2B C2 76 4E 19 3C 65 D3 
AD 34 05 57 FD 0C 9C 32 FD C6 86 D7 C7 2D F9 37 AB E5 8A 22 C5 E3 AD 93 BB FA 8E FC 64 EF FD 74 
65 0D 75 01 00 00 B4 52 2D 16 F3 85 F7 BE 7B 63 FE A1 68 39 47 44 C4 F0 4D BA CD F8 66 C7 DF 97 
1F 85 85 DE FF F7 CF D5 6F F5 B6 D6 61 88 88 88 2B 78 B0 6E E6 E2 F3 B9 2D 55 16 29 9E 06 9D 7A 
9A C5 22 E1 01 00 40 0B B5 D0 25 78 6C C2 BE A5 DF 5F CB 62 89 88 18 9D F6 93 B7 1E D9 3C CD 4B 
BF E8 33 17 57 DF 01 2F FB 77 7F 75 F8 9C E3 F1 4A 8E 38 79 D8 DE 8D 7F 7D 39 78 BA 25 EE 02 00 
00 00 68 94 96 89 52 E5 B3 9D 9B 4F 65 16 9D 79 E6 DB 4F 5B F3 63 69 C6 17 13 79 BD BD F1 B3 41 
FA 3C A1 A9 5B 4F FF A9 6F 0E 6A 27 AD D4 BD CE 0D 3B BF 65 C9 B4 11 3D 3A 3A DB 98 18 9B 58 3B 
7A F5 79 65 D6 D2 BD 77 D2 94 95 56 14 BD FF A3 71 01 01 01 01 01 63 67 FE FA 40 41 39 0F 0F 2C 
9D 31 BC B3 B3 B5 A9 B9 BD 77 DF D1 AF 2F DC 74 29 BE F4 DC BF EC FA DA D7 5F 19 FF DD E5 DC E2 
75 29 EE 6D 9A 1C 10 10 10 38 7B D7 73 25 91 EC E6 BA A9 63 03 02 02 02 02 C6 7D 72 28 21 F3 D6 
AF EF 8F E9 E9 62 69 D1 CE 6B E0 D4 6F CF 27 B0 F5 AD AC 01 A4 D1 E7 37 CE 1F DF BF A3 A3 A5 89 
B1 89 B5 63 C7 7E E3 66 7F F7 F7 B3 4A 03 1D 75 2D 13 00 00 DA A2 16 E9 CD B3 C9 FF 5E 0C 96 15 
85 A9 C0 69 EC D4 E1 92 AA F3 F0 EC A7 6E BB F3 8A 59 7B 7B 83 2A 47 1E 59 F7 7E 9D 33 65 E1 DE 
90 DC B2 E8 CF CE 4C 8A 0E B9 76 6C E7 4F 9B E6 6D DB B7 EA 65 87 92 CD E0 72 9E 5F 3A 7E EC 8E 
9C 88 EF 64 3B 76 EF 9B 13 E7 EC 2C 5D 2C E3 6A EC E3 AB 27 0F 1C BC BC 23 68 CF 14 27 3E 11 9B 
78 EF D4 F1 13 69 A5 51 A8 4C B8 7B F2 D8 5D 12 78 7A 7E AA 24 62 13 EF 9F 39 76 2C 99 25 D2 49 
71 E9 14 B2 7F F9 F1 44 25 47 44 29 FF 3B 79 27 79 3D AF BE 95 D5 93 32 E6 E8 82 71 6F 6C BC 93 
59 7A 3A 21 3B 33 29 FA C9 95 23 3B B7 4D DC 70 68 FB 5B 1D 75 4B F6 6D 1D CA 04 00 80 B6 AA 45 
72 40 76 E7 D6 83 E2 EE 39 A3 D7 A9 47 67 51 B5 95 18 3B 76 A8 26 E3 95 91 3B A6 FB CF DE 53 96 
A4 0C C3 63 8A CE E3 13 A7 48 B9 BA E6 35 FF 8F 4E A7 55 ED B5 B2 89 FB E6 BD BD 33 24 8F 6F 60 
ED E2 6C A5 C7 2B 59 44 1E F9 E7 67 2B CE D4 AB 7C C5 DD 5F 56 16 87 27 11 F1 2C FC 27 F9 9B 36 
A2 B2 3A C8 BF B9 FC B5 D7 37 DC 2E CA 78 46 20 B6 76 76 B6 12 0B 18 22 E2 F2 9F FF F1 DE B8 05 
A7 33 EA 5C 26 00 00 B4 61 2D 12 F3 D2 94 E4 CC 92 F4 31 B5 B1 D1 55 3D 77 79 6C F4 CE 79 8B 8E 
26 2A 89 88 18 61 BB E1 4B F6 DF 8C C8 C8 49 7D 7E 69 EB 7B DD 4D F8 0C 11 71 85 4F 7E 7E 7F C9 
D9 9C CA 4B 72 85 D9 39 3A 1D 67 EE 0E 8E 8D 0B 0B 4F 4C 09 F9 6D 8A 8B A0 28 83 95 09 97 CE 13 
11 91 B0 FF 67 7F 5F 38 B3 62 84 51 71 36 EB F4 F9 EC 44 50 50 D0 B9 5D EF 78 09 2B 34 25 93 CA 
F9 36 03 E7 AE D9 B5 7B CB 8A 77 27 BE FB C6 08 E3 C6 54 56 2B 65 C4 F6 CF 7F B8 91 C3 11 11 A3 
EB 35 63 F7 83 A4 84 F0 F0 C4 B8 5B 3F 8F 77 D6 61 88 38 F9 8B AD 8B 37 54 BD C7 BE DA 32 01 00 
A0 4D 6B 89 41 7B 36 23 33 BB 64 EC 99 D1 D5 D3 63 EA BC A4 F2 E9 9E 2D A7 D3 8B 2E DC 33 E8 BB 
EC E4 F1 8F BD 85 44 44 C6 FD DF DA 74 D6 91 E9 35 EA E7 E7 0A 8E 53 44 EC DD B8 FF AB 91 33 AD 
2B 2E 2C 70 9B F9 E3 FA 29 45 17 01 88 DD 27 7E 3C 75 ED 81 A5 77 E5 44 C4 A6 24 10 11 11 CF CC 
B3 EF 40 E7 C4 CD 7C 86 88 23 22 9E C4 B5 CF A0 41 D5 9C 50 20 BE CD A4 9F 0E 6F 08 34 21 A2 D7 
67 D6 BB B2 E1 33 AD EB 73 34 A5 0C DD BF F7 52 D1 10 01 DF 71 DA 0F EB A7 78 8A 89 88 8C 3B CF 
FA 69 E5 C5 FF 4D FE 23 91 E5 64 C1 07 7E BF FB 59 A7 9E C2 8A 4B 56 2D 13 00 00 DA B6 16 E9 CD 
EB EB 95 0E D3 73 85 05 05 75 BE 79 4D 19 7A F4 D8 DD A2 D1 7E C6 68 F8 DB 33 BD CB C7 9A 64 F0 
EC A9 9D 8B 8E 52 B8 BC AB E7 2F 57 5E 98 67 D4 A3 7F 8F B2 0B FD 04 8E 8E B6 C5 1B CB 49 EB F9 
14 1C C6 B0 FF 98 11 26 0D AE 2C BF 7E 6B CB B9 71 E3 51 D1 A3 05 18 FD AE FD 7B 8B 64 25 14 92 
81 03 3A 15 B5 AB 0C BB 73 BB CA F9 80 AA 65 02 00 40 1B D7 12 BD 79 9E 81 A9 89 2E 43 32 8E 88 
D8 F4 C4 44 19 51 B5 67 E7 AB 50 44 84 C5 14 67 99 C0 D9 C7 C7 A8 E2 A7 7C 17 6F 4F 63 DE ED 54 
96 88 CD 8D 8D AA B2 56 53 4B CB F2 5B A7 A3 2B D2 61 48 CA 11 51 7D EF 92 17 38 B6 6F 5F B1 E2 
7A 55 96 AC 24 83 BA 3F B1 5F 99 14 9F 58 7C 21 03 97 73 68 AA 89 68 6A 35 33 71 CA E8 D0 30 05 
D9 54 E8 CE 57 2D 13 5A BB BC BC BC E8 E8 68 75 57 D1 5A E9 E8 E8 B8 BA BA 32 4C DD C7 0F 01 B4 
50 8B 5C 69 2F F2 F6 71 17 1C B8 2D 27 22 AE E0 C9 FD 47 B2 29 7E C2 2A 33 29 1E 7E FF EA EC 6B 
76 2F 05 8C 1D F7 F2 20 0F 53 01 11 11 57 FA 3C 5C 12 E8 08 AB FC 67 D5 11 EA 94 2E 2D AF FC 21 
A3 AB A7 5B 61 AC 42 20 68 E8 C6 32 12 33 D3 4A C3 1E F5 A9 AC 4A 69 2A 71 D2 82 3A 3C 59 9F CB 
CF CB AB 7C B0 52 4D 99 D0 9A 3D 7A F4 68 E1 C2 85 52 69 F3 BD 69 41 FB 39 3B 3B 6F DE BC B9 E1 
FF F7 01 5A BF 16 F9 EB E7 BB 0D E8 E3 20 B8 1D A6 20 22 45 E8 F1 83 B7 96 F9 F5 AD 7C 1D 9E EC 
DE 1F BF 9D F8 DF 43 F9 E5 43 3F 2D D1 F3 5D 1C 74 73 59 2F 21 DF D2 DA 8C 47 F1 4A 22 62 13 A2 
A3 15 E4 55 BE 53 CC 66 C5 C6 64 14 5F BF AF 63 51 E9 C4 3C 11 F1 F8 F5 DF B8 1A BA F9 42 51 A5 
72 EB 55 99 65 BD 5E BE C7 93 98 18 17 1F 36 F0 AC 02 D7 FE F1 51 D7 EA 36 83 11 5A 7B E9 54 9E 
58 A5 4C 68 D5 4E 9E 3C 89 8C 6F A4 88 88 88 E0 E0 E0 6E DD BA A9 BB 10 00 B5 69 99 DE 9F B0 CF 
9B AF FB 16 F7 79 15 CF 36 2F 5C 7E A9 D2 89 E5 BC 9B 2B 3F D8 F8 A8 E8 49 B8 C4 B3 EA 3B C8 57 
48 44 82 0E 5D 3B 49 8A 2A 54 26 9E 39 74 A1 C2 B3 EE D9 98 C3 87 6F 14 9F 1E 17 F8 F8 F9 35 B4 
36 1E AF 64 1F 70 52 A9 B4 DA FB DF 18 A1 48 54 71 47 D5 AB B2 4A 43 FA B5 D5 63 E5 ED 61 5D 74 
60 C0 65 26 4B DB F9 F5 2B E5 E7 2C CC 95 EB 5A 77 E8 DE A7 5F DF 9E 6E 26 95 7F 77 55 CB 84 56 
AD B0 B0 50 DD 25 68 03 EC 46 68 E3 5A 68 2C 4B E0 F3 DE 67 13 77 4C DC 1D AD E0 88 CB B9 FE CD 
4B 7E 4F 97 AC 5C 30 7E 50 37 77 C3 FC 88 9B 07 D7 7E FE C5 2F 45 77 90 11 31 E2 3E B3 E7 0E 2A 
EA 96 1A 0C 9D 30 C6 E6 8F 1D 71 4A 22 65 F4 AE 39 6F 76 DA B7 F5 FD 9E 26 3C 22 65 FC E9 AF A6 
7D 79 36 8B 23 22 E2 19 0F 79 63 52 FB 86 96 26 12 95 9C 40 E0 B2 D2 D2 38 AA 3A 2C 40 C4 E3 55 
EE 90 D7 A7 B2 7A 75 E6 89 44 7E 2F 8F B0 D9 B2 35 56 49 9C F4 FA BA CF F6 BE BA 77 9A 93 90 88 
D8 F4 53 5F 4D 7C 65 5B 84 92 D1 31 B0 EE 3E FF C0 85 65 FD 2A 75 DE AB 96 09 5A A1 5B B7 6E 4B 
96 2C 51 77 15 AD 4C 60 60 A0 BA 4B 00 D0 08 2D 75 CA 8A 67 15 B8 76 EB EC 7B 63 37 3C CC E7 88 
B8 82 17 07 17 8F 3F B8 84 A7 23 E4 2B 65 F2 72 97 C4 F1 8C FC 3E 5D 3B C7 A3 24 AE 8C FD BF 5A 
15 78 66 FA C1 38 25 C7 C9 C2 0F 7D 30 E0 DF 0D BE 9D 5D C5 19 21 77 1F 46 67 15 75 FE 79 92 7E 
9F AE 9C E1 D4 D0 80 13 58 58 9A F1 29 81 25 22 F9 DD EF 46 F7 FA C7 38 55 3A 62 F7 F5 D5 FD 6B 
59 AE 19 2B 13 8F 98 3F D7 6F FF A7 FF CB E1 48 19 7F 70 66 EF A8 7F 26 8E F2 31 CD B9 7F 68 D7 
E1 48 25 11 71 0A 85 F5 D0 97 7B 60 80 BE CD 90 48 24 12 49 75 77 7A 42 CD 18 86 29 77 05 0D 40 
DB D5 72 57 A6 F0 4C 87 AF 39 7D 48 FF B5 A9 AB AF A4 14 BF 8A 96 63 E5 E5 47 C9 19 81 69 CF 79 
BF FD F9 59 B7 72 01 C6 77 9C FC CB EE B0 F4 A9 2B 82 E2 E4 1C 71 D2 B4 17 37 CF BF 28 D7 A8 A4 
E7 FC 3D FB 16 75 69 78 E4 09 BC FC 7A 58 F0 1F 15 9D 66 CF 8D BA 73 9D 18 43 F7 38 39 51 6D E9 
DC 8C 95 09 3A 7E B4 E3 C7 BB C3 67 1E 88 94 73 9C 3C E9 D6 FE F5 B7 F6 97 7E C8 E8 38 8D DB B0 
67 71 4F 5C 53 0F 00 00 B5 6A D1 0B 50 79 36 23 BF 09 7A F0 F2 8E 55 DF 6C FA F3 C2 83 84 FC D2 
67 E6 30 3A 26 1D 47 BF F3 F1 E2 0F 27 77 B7 A8 1C AF 3C B3 C1 5F 9D B9 3D 68 D3 7F BF DD FC E7 
F9 C7 A9 C5 8F C6 67 18 91 79 27 FF 77 16 2F FF E4 55 2F 83 46 15 25 F6 FF 72 FD 7B 8F 67 FF 7A 
2B B9 F8 25 B9 02 91 B2 20 9B A5 DA 6F 40 6F C6 CA 04 6E D3 F6 5C 71 E8 B5 78 C9 BA 83 D7 63 72 
4A 1E 60 CB E8 DA F5 9E 30 7B C9 97 0B FC 5D AA DE A9 00 6D 4C 6A 6A 6A 46 46 35 0F 3D 6E 6B 18 
86 69 D7 AE 9D AE 2E 46 B7 00 AA 57 CB B8 56 E9 2D A7 27 4E 9C F0 F7 F7 6F BA F5 B2 79 71 4F 1E 
3C 8F 49 CA 54 18 DB B9 B9 B9 BB DA 4B EA 10 5C D2 B4 B0 27 21 E1 D1 C9 79 42 13 BB 0E 5D BA BA 
48 9A EE 54 B4 22 2B EA D1 9D 07 91 05 FA 56 CE DE 5D BD AC EA DB 55 6E BE CA F2 13 1E DD 0F 89 
4A CE 22 63 3B 27 57 F7 F6 0E 75 D9 4F 8D 11 11 11 E1 E2 E2 52 F4 F3 E3 C7 8F BD BC BC CA 7F 9A 
99 99 79 FF FE FD E6 AD 00 4A 2C 5B B6 EC C2 85 0B 44 34 74 E8 D0 4A E7 E6 9F 3E 7D 7A E9 D2 25 
35 D5 A5 71 04 02 C1 C4 89 13 F5 F5 2B BC F6 72 C8 90 21 45 5F 6E CB 97 2F EF DB B7 AF 9A 4A 03 
B5 F1 F6 F6 36 37 37 57 77 15 1A 41 5D B7 93 F2 C4 76 DE 7E 76 DE F5 5B 48 64 E6 DA A5 9F 6B 97 
E6 A8 47 60 EC D8 79 88 63 E7 86 2E DE 7C 95 E9 DB 78 F7 B1 A9 E7 7E 02 AD 17 11 11 A1 EE 12 34 
88 42 A1 88 8D 8D 75 77 77 57 77 21 00 9A 08 37 60 01 B4 3E 2C DB A0 57 1F 6A 2F EC 10 80 9A 20 
E6 01 00 00 B4 16 62 1E 00 00 40 6B E1 51 CF 00 00 15 FC FB EF BF 78 D2 70 6B 27 16 8B 85 42 0D 
BA 25 A9 7B F7 EE 9F 7E FA A9 A9 A9 69 CB AF 1A 31 0F 00 50 26 2B 2B 6B C5 8A 15 F2 7A BE 72 0A 
40 B5 A0 A0 20 A1 50 B8 7C F9 F2 96 5F 35 62 1E 00 A0 4C 4C 4C 4C AB C8 78 33 33 B3 76 ED DA E1 
35 BB 44 94 9D 9D 1D 19 19 A9 F9 97 61 3E 7E FC 58 2D EB 45 CC 03 00 54 EF A5 97 5E 32 34 34 54 
77 15 D5 E3 F3 F9 C8 F8 52 3D 7A F4 A8 14 F3 E6 E6 E6 7A 7A 7A EA AA A7 BC 35 6B D6 A8 B7 00 C4 
3C 00 40 F5 DE 7F FF 7D CD 8C F9 F0 F0 F0 73 E7 CE A9 BB 0A 0D 62 65 65 55 E9 65 45 9A F3 78 9C 
CC CC CC ED DB B7 AB B1 00 5C 69 0F 00 00 A0 B5 10 F3 00 00 00 5A 0B 31 0F 00 00 A0 B5 10 F3 00 
00 00 5A 0B 31 0F 00 00 A0 B5 10 F3 00 00 00 5A 0B 31 0F 00 00 A0 B5 10 F3 00 00 00 5A 0B 31 0F 
00 00 A0 B5 10 F3 00 00 00 5A 0B 31 0F 00 00 A0 B5 10 F3 00 00 00 5A 0B 31 0F 00 00 A0 B5 10 F3 
00 00 00 5A 0B 31 0F 00 00 A0 B5 10 F3 00 00 00 5A 0B 31 0F 00 00 A0 B5 10 F3 00 00 00 5A 0B 31 
0F 00 00 A0 B5 10 F3 00 00 00 5A 0B 31 0F 00 00 A0 B5 10 F3 00 00 00 5A 0B 31 0F 00 00 A0 B5 10 
F3 00 00 00 5A 4B 50 C7 F9 CE 9C 39 23 10 D4 75 66 68 BD 22 22 22 D4 5D 02 00 00 34 99 BA 26 F7 
FA F5 EB D7 AF 5F DF AC A5 00 40 2B C3 CA 0B 0B 0A 0B 65 0A 12 08 F5 F4 F4 44 02 8C 0E 02 68 9C 
5A 62 DE CC CC 2C 2D 2D AD 65 4A 01 8D C2 E7 F3 CD CC CC D4 5D 05 68 24 65 41 4A E4 B3 17 51 F1 
49 A9 D9 85 4A AE 78 22 23 D0 33 36 B7 B2 73 72 77 77 34 D3 45 DE 03 68 8A 5A FE 37 AE 5C B9 12 
DF F5 6D 90 9E 9E DE 27 9F 7C 62 65 65 A5 EE 42 40 E3 C8 33 42 AF 9E 3A 7E EE 66 48 54 52 56 59 
C6 13 11 A7 28 C8 4C 8C 7C 7C FD EC 89 33 B7 A2 B2 95 EA AB 10 00 CA AB A5 37 3F 6B D6 AC 99 33 
67 2A 14 8A 96 A9 06 34 04 9F CF E7 F1 D0 21 83 CA E4 A9 8F 2E 5E 7C 98 2A 57 35 0F 27 CB 08 BD 
76 21 5F 36 B8 5F 7B 63 7E 4B 15 06 00 35 A9 FD DC 3C C3 30 3A 3A 3A 2D 50 0A 00 68 32 4E 1A 77 
EF FA A3 B2 8C 17 18 D8 BA B8 B5 B3 34 33 95 E8 72 79 99 E9 69 89 51 A1 11 C9 F9 2C 11 11 57 10 
7F EF DA 63 93 61 9D CC 71 DD 2E 80 9A E1 3F 21 00 D4 05 9B F5 E2 51 44 4E F1 28 3D A3 6B ED 3B 
A0 AF A7 99 B0 F8 43 43 23 53 6B 07 57 57 87 E0 4B FF 0B 49 93 13 11 29 33 9E 3F 8A 74 1D E8 26 
66 D4 54 2F 00 10 11 EE 9B 07 80 3A 51 26 87 86 67 B0 45 3F 33 BA ED BA F4 F2 28 CD F8 12 8C AE 
75 A7 DE DE 96 25 7D 07 79 62 68 44 16 DB 92 35 02 40 55 88 79 00 A8 1D 9B 9D 9C 92 5F D2 95 37 
70 74 B7 D7 AF B6 97 CE 33 72 71 B3 2D 89 7F 2E 2B 39 B9 90 AB 6E 36 00 68 31 88 79 00 A8 9D 3C 
3D A3 64 C0 9E 84 E6 96 A6 35 5E 5C A7 63 69 69 5A F2 B5 C2 66 A6 67 A0 3B 0F A0 5E 88 79 00 A8 
15 2B 95 4A 4B 02 9B D1 37 30 A8 F9 8B 83 11 8A F5 45 25 3D 7D B9 54 8A 3B EB 00 D4 AB 25 2F C1 
93 DD D8 F4 E1 D6 BB D2 92 81 3F 46 20 32 30 36 B1 70 EE F9 F2 AB A3 7C CC 04 44 44 CA 67 FB BE 
F8 E1 92 ED 8C 75 73 FC 44 2D 58 58 D3 53 A4 24 67 9B 95 75 6A EA AE DC 1E 10 68 CB CE 00 6D 20 
97 97 DE 55 CB 08 04 3A AA AE AB 13 E8 94 75 F5 E5 32 39 47 42 5C 85 07 A0 3E 2D 19 F3 8A D0 73 
3B B7 1D 51 88 25 E2 A2 FB F3 38 65 61 5E 6E A1 9C FB 62 61 87 37 B6 9E DC 32 D1 89 AF 48 B8 B6 
7F FB 0E EF BE AB 5B 73 B2 29 23 0E CC 79 75 61 DA 47 4F 0F 4C D6 AF F7 C2 E5 F6 00 69 C3 CE 00 
6D A1 23 28 CD 6E 4E A9 54 70 44 35 66 B7 52 51 36 50 5F F4 F8 05 4E 96 9B 95 27 E7 48 A0 67 64 
A8 8B 7B E9 01 5A 54 8B 0F DA 0B 47 6C 88 48 2F 92 91 95 2F CB 8F 39 B7 A4 9F E8 F9 AE 8F BE 3C 
9A D5 D2 A5 34 13 45 58 D0 89 FB E9 18 AA 04 6D C2 13 8A CA FA E4 05 79 F9 2A 2E AC 53 E4 17 94 
0C D9 11 5F A4 AB C3 10 C9 63 6E 9F 3E 75 EA D4 E9 AB 61 B9 38 55 0F D0 C2 D4 7D DF BC C8 6E E8 
E2 A5 53 F6 0E DB 74 F9 DC 1D 69 60 DF AA 33 28 73 13 5E 3C 09 09 4D 52 9A 3A BB 7B BA 3B 9A 54 
BC 87 27 2F FA CE 8D 07 B1 85 86 0E 1D BB FA 3A 1A 96 3F 66 91 A5 87 3D B8 FF 34 55 D7 B9 B3 AF 
87 B5 58 F5 D1 4C CD AD B0 B9 F1 21 0F 1E 44 E4 48 DC 7C 3B B9 5B EB 97 7C 24 4D 8F 8B CF D3 B5 
B5 37 13 E4 44 DD BF F9 38 D3 D8 A3 4B 27 17 53 21 11 B1 B9 89 91 49 79 2C 71 05 69 51 11 31 56 
B6 F6 A6 94 1E 13 9F 27 B6 B5 E5 C7 5E BF F6 42 C7 B3 7F 4F 47 B1 8A 96 6B 51 FD 56 49 AB 5F 45 
3D 76 25 9B 19 71 E7 DE 93 04 A9 C4 D9 B3 63 07 47 49 E5 1B A5 00 74 24 12 31 93 98 C3 11 11 57 
98 9E 9A C3 DA 48 4A FE FE 94 99 F1 09 AC A9 AD 69 D1 83 EC E5 A9 29 A5 97 DD 31 86 C6 46 B8 FC 
07 40 BD 34 E0 FF 20 A3 AF AF CB 90 4C 2A AD FC 81 F4 F9 BE 39 7D DA 99 D9 79 F9 8D 1C 37 6E 64 
3F 1F 67 07 9F 57 37 DE CA 2D FE 34 37 78 CB F4 2E 0E 6E 3D 86 BD F2 9F 80 C1 DD DA BB F4 7C 67 
5F 78 D1 D9 C3 C2 D0 83 8B 06 39 59 B7 EF 39 FC 95 31 FD BC DB D9 F7 9C B5 23 B8 A6 A1 82 9A 5B 
61 D3 AF FF 38 BD A7 B3 A3 4F BF 31 63 47 F7 F1 6A 67 D7 FD AD CD 77 32 59 22 22 D9 BD 55 C3 3C 
06 7C F8 FD 92 A1 8E E6 CE DD 87 8D 1E D6 A3 BD 53 D7 37 76 85 48 89 B2 F7 BF E5 FB FA EE 38 65 
FE C9 0F BC DC 87 2C BB 29 93 DD 5A 3E CC 63 F0 BC 2F A7 75 F1 EC 3F 6A 94 9F DF 9C A3 B9 2A 5A 
56 A1 E6 AD AA 66 15 F5 D9 95 8A F0 7D B3 BA BA B8 F7 1A 1A 30 6E CC 00 5F 67 9B 0E 63 56 04 25 
A1 CF 05 15 F1 25 16 E6 25 27 8F B8 CC A8 B0 D4 92 53 F5 5C 5E C4 FD AB 97 4F 1F 3B 11 74 2F 2A 
53 CE E6 45 87 C6 15 94 DE 78 67 61 65 A8 01 5F 31 00 6D 9A DA FF 0F 2A 12 4E 6E 39 F4 82 35 EA 
D1 B7 73 C5 27 EA 2A 9F 6D 7A E7 9D 5F C2 7C BE 3C 17 9E 5D 28 CB 4B 09 DE FF 9E 7B DC A1 4F 3F 
DF 15 C3 12 51 E6 E9 8F 5F 9D FD 7B BA DF 7F 4F BE 48 CB CF 8D 3A B7 A4 53 D4 B6 77 66 FE 14 AA 
A4 CC 53 1F 8D 9E B4 3E B2 FB B2 D3 A1 99 F9 59 B1 57 36 BD 5C B0 EF ED 31 B3 FF AC 2E B9 6A 6E 
85 8D DB 3B 2B 70 FE 1F E9 FD 56 9E 0D 4B CB CB 88 08 5A 33 22 6F DF FB FE 6F EC 88 28 1E 8D E7 
62 F7 AD D8 2F F9 E8 C8 83 A8 E8 C7 E7 D6 8C 35 79 BE 67 E1 57 87 D2 C8 68 E2 CE 27 FB A6 D9 F1 
F5 47 FF F4 22 FC D2 57 BD 84 44 44 6C F4 FE 4D B7 7C 16 FD B8 65 ED 97 5F CF 1A 9C 55 4B CB D5 
AA 6D AB 2A AC 62 A8 41 C5 85 55 ED 4A 36 6E FB 9C F7 76 E5 BE B4 E1 C2 E3 C4 BC C2 B4 47 7B 26 
EB 9E FB 7A D6 8A CB 55 0E BA A0 8D 13 D8 B8 3A 1A 16 8F DB 73 39 A1 B7 83 E3 0B 38 22 62 73 A2 
23 93 E5 44 8A DC C4 A7 57 CF 9C 3A 7B E9 41 82 AC 78 09 BE A9 B3 8B 19 CE C4 03 A8 59 8B 0F DA 
2B 82 B7 BF 3B F5 82 80 88 88 93 E5 A4 44 3D BC 71 3B 22 C7 70 C0 AA FF 4E B1 E1 51 F9 6C 29 4C 
17 79 BE 32 6B E2 A2 4F 86 38 09 88 C8 CC 67 FC E7 D3 7F D9 FA 51 7C 4C BC 82 EC 32 0F 6F DA 1B 
6E 1A B8 7B FB C7 23 2D 79 44 92 21 9F AF FB F2 E9 47 41 59 91 59 F1 E7 D7 EF 0E B3 98 B0 7F E7 
27 C3 4D 89 C8 D0 EF ED 5F 37 3F BF 31 E8 A7 8D 7B 96 8E 5B E0 5A E1 3B 87 4D AE A9 95 1C 45 E1 
F6 F5 C7 52 DA 4D FB 7B C7 C2 A1 12 22 32 1E F8 C1 B6 9F 9F DF 1D B9 79 ED 96 BB 6F 7D E3 4B 44 
1C 67 F1 9F 6F B7 7C 34 CA 94 88 1C E6 FD 30 EF F0 B1 4F 1E DC 7D 2E 9B E4 67 61 67 AE CF 23 46 
24 B1 B5 B7 D1 27 92 11 11 29 25 2F AF D8 B5 E2 55 09 11 29 1E 2D FB A0 E6 96 7B 54 BF D3 D8 F8 
FD 2A B6 CA BE D2 2A AA 50 B5 2B AD A2 43 23 F2 18 13 B7 1E DD 3D 2D 44 44 5E 13 57 6C 61 7B C7 
38 38 34 FC 37 0C 5A 8A 6F E6 EE 65 1B 75 23 AE 90 88 88 CD 7A 7E E5 7C BE 67 27 4F 47 2B F7 81 
2F 19 3E 7B 10 1C 12 93 AD 50 E6 A6 67 96 CC CE 18 3A 7B B7 97 A8 BD 1F 01 D0 E6 B5 78 CC B3 29 
8F 2E 9E 0F 63 88 18 86 AF 2B B1 B2 6B 3F EC ED D9 EF 7E 3A 7B 98 43 E5 F3 C1 62 BF F7 37 F9 11 
9B 9F FC FC 66 F0 FD FB 77 EF DF 0B BE 7B 39 58 41 66 52 19 91 FC C1 CD FB 79 A2 5E FE A3 2C 4B 
BE 45 04 3E 73 7F 3F 3B 97 A8 F0 D8 DE 7B F9 8C 44 FA 68 DB EA F0 92 96 14 69 62 81 EC C1 CD 3B 
52 72 AD 70 E9 7B CD AD 50 E6 CE 5B 4F 58 CB 49 81 43 CB 62 D3 A0 EF B8 11 76 BF 6C BB 7D 9B C8 
97 88 88 EF D1 BD 47 C9 A7 3C 0B 73 13 1E 17 55 E3 85 49 02 CF EE 3D 8C 8A 7E CC BD AD AA E5 1A 
62 5E 76 E7 BA 8A AD B2 AF B4 8A 2A 54 ED 4A 61 97 C0 D7 BA FC BA 7C F9 00 F7 A3 FD 87 8F 1C 35 
E6 95 C0 51 93 67 F5 C3 97 33 54 C5 33 70 EE D1 23 25 FB 4A 78 0E 4B 44 A4 C8 89 7D 78 25 F6 21 
C3 17 8A 04 AC 54 5A E9 2D 96 42 1B 4F 2F 6B 5C E4 01 A0 7E 2D 1E F3 C2 61 6B 9E 1E 9B 61 5C 87 
39 95 31 47 97 CC 5C F0 D3 B9 B0 5C 4E 47 E2 DC A9 47 4F 5F 07 73 DE 33 96 88 48 99 99 99 4D 7A 
46 46 BA 55 17 CA CA CA E6 A8 F0 EA 9E CD F7 CA 4F 6E E7 62 67 C0 4A 89 2A C4 7C CD AD B0 D9 59 
39 4A C6 D0 C4 A4 7C DC F1 4D 4C 8D 79 6C 41 7E F1 3F 19 A1 AE 6E E9 C7 0C C3 10 71 5C 4D 97 1F 
33 06 C6 C6 BC 3A B7 5C CD AE 50 BD 55 15 57 51 DD F2 35 EF 4A D2 F5 FB EA D4 69 FB 95 EB F7 1C 
3D B3 6F F5 99 DF 56 2F 34 70 1A B9 70 CB B6 25 43 AD 31 DC 0A 95 30 7A ED 7A 0C E9 27 BA 72 E3 
69 6A E9 C5 F4 9C 52 56 58 CD F9 26 59 FC ED B3 17 32 3A 7A 7B 38 5B 1B A8 FB 42 5F 80 36 4D 63 
FF 03 2A 9F 6D 7C 63 DA EA 3B 2E 6F 6E 3A 3B E7 E5 DE DE B6 62 1E 1B B3 61 D8 EE B3 D1 2C 47 C4 
97 48 8C 28 3F 3D 3D 8F A8 24 A3 95 31 D7 4F 3D 65 3C 3B 19 1A 1B F2 44 7E 2B 6E 1D 79 A3 DA F1 
EB F2 6A 6E A5 A7 BB 89 44 87 7D 9C 9A 5A FE 7C BE 32 29 31 8D E5 3B 9A 35 6C 7B 8A 4F 6A F2 8C 
1B D2 32 DF 58 D5 56 95 9C 0A AD E9 3E 66 95 BB 92 88 78 A6 BD 67 AE EE 3D 73 B5 2C E5 E1 B9 C3 
7F ED DB BC 69 DF 7F 27 2F F2 49 DA 3D AE DE 1B 09 DA 8F A7 6F D7 79 E8 68 87 A8 A7 21 A1 D1 89 
E9 B9 B2 F2 7F C8 3C A1 91 85 B5 84 4B 8D 4D CE 67 89 D8 82 94 17 B7 2F C6 A6 F6 F5 F7 B3 52 5B 
B9 00 A0 B1 A3 B3 B9 D7 2E 5C CB D6 1D FA D1 0F EF 0C ED 64 2B E6 11 B1 69 D7 AE 85 28 38 85 42 
C1 91 C8 D7 AF 9B A1 EC DA 91 23 71 25 5F 32 B2 07 5B DE 0D 1C 3D 7B 6F 52 17 BF 6E FA 85 97 FE 
F8 23 BC B4 83 21 BB BD 74 A0 B3 67 E0 86 C7 95 46 15 55 B4 12 A3 DF BB 4F 17 61 FA E9 DF 8F 94 
5D B8 97 FE CF BE 93 89 3C 9F 5E 3D 6B 2D 9D C7 E7 13 CB 2A AB BF 58 DD A0 21 2D 0B BB D5 7D AB 
AA 52 B9 2B 73 4E 7C D8 DD A5 DB 82 A0 42 22 A1 85 8F FF DB 4B B7 7E 3F C9 91 C9 8C 8B AD B5 59 
68 B3 78 22 53 67 DF BE C3 5F 0E 1C 17 30 6A F8 D0 41 03 FA F5 EB 3F 68 E8 88 51 01 81 81 A3 87 
F4 ED 3B E4 A5 61 DD 5D 4C 8A 9E 93 C7 93 B8 74 B0 C5 D0 3D 80 3A 69 6C CC EB B9 77 70 14 14 5E 
F9 6D 63 50 44 66 46 D4 ED 7F 36 2F 7C 75 DE C1 54 86 CB CB C9 E6 88 2C C6 CD 9F E9 59 70 6A F1 
E4 4F F7 5C 7E F4 FC C9 A5 1D F3 67 6D 78 62 19 F8 D1 74 6F 87 09 0B 66 B8 E7 9F F9 6C FC 3B 1B 
4F DE 79 16 1A 7C 6A FD F4 A9 DF 5E C9 6E 37 64 B8 47 95 91 8B 1A 5B F1 12 B9 4D 5F 34 C5 31 ED 
E0 BC F1 0B 77 FE FB F0 D9 A3 CB 7B 3E 1D 3F 7B 5F 92 ED F8 05 33 3C 6A AB 9C 6F 62 62 CC 93 DD 
FD F3 87 AD FB AF C4 55 19 CD E4 37 A4 65 9E 6D 3D B6 AA 7E BB 52 DC A5 B3 43 4E F0 AF F3 DE FB 
F9 6C 70 58 C4 B3 9B 87 57 AD 3A 18 2D EA 3A 7C 70 AD CD 42 9B C7 D3 11 4B CC 2D 6D EC EC ED DB 
D9 58 9A 49 C4 42 1E 11 11 23 32 6B DF 6B F8 A8 21 3D 3C ED 6D DC 3A 76 30 29 77 F2 87 53 C8 0A 
0B 2A 92 E1 41 52 00 CD 4B 63 07 ED 85 7D 16 6F F9 6F C8 F4 E5 4B 86 B8 2C 26 46 60 D8 7E D4 47 
BF 9E 1A FE 9D FF D7 37 FF 77 97 C6 F6 31 18 B0 F2 D4 51 A3 F7 E7 AC 9E 31 70 B5 82 63 F8 26 BE 
93 D6 1C DC 38 D1 9E 47 34 F4 87 B3 27 AC 3F 98 FB DD 82 D1 DB E4 1C 31 02 53 9F C0 95 87 7E 9A 
ED 59 CD A9 66 15 AD 58 04 FC 74 EE 90 ED EC F9 6B 67 0D 5E AB E0 18 BE A1 D3 C0 F7 77 FD F8 CD 
04 DB 5A 0F 8C 04 1D 27 BD 33 66 EF A2 A3 4B DF 3E 7F 7B EB F0 83 EE 95 3E E6 35 A8 65 63 15 5B 
55 DB F7 A4 AA 5D 29 1B DB 67 EA A6 3D 11 B3 E6 AD 9B 3B 72 A7 92 63 78 42 89 6B FF B9 BB 7E 5D 
E4 53 DB 66 02 A8 C0 17 5B BA 75 B6 74 AB 38 91 CB 78 72 EE EF 27 15 26 E9 38 F6 1B DF C7 BE 05 
0B 03 68 73 98 9A AF 1B D3 04 8A 9C 84 B0 F0 64 CE D2 B5 BD 8D 41 B5 57 84 29 73 E3 9F 87 65 E8 
3B B4 AF FC 7C 3C 62 F3 93 C3 43 E3 E5 26 4E AE F6 B5 3E D5 AD E6 56 48 99 1B FF 3C 34 95 6F ED 
EA 56 DB B3 F4 2A 92 65 26 A4 B2 26 D6 A6 BA 35 5F 17 D7 90 96 EB B3 55 15 A9 DE 95 6C 61 7A 6C 
44 74 06 DF AE 83 BB 45 D5 4B 12 4B 65 66 66 DE BF 7F BF 5E EB 85 06 5B B6 6C D9 85 0B 17 88 68 
E8 D0 A1 4B 96 2C 29 FF D1 89 13 27 E2 E2 E2 D4 54 57 FD C9 C2 2E 1E BE 99 50 DD 69 AC A6 8A F9 
01 03 06 78 78 54 18 0F 1B 32 64 48 D1 97 DB F2 E5 CB FB F6 AD E6 01 9B 35 79 F4 E8 D1 DC B9 73 
8B 7E 3E 7A F4 A8 A1 A1 61 E3 CB 6B 72 E1 E1 E1 E7 CE 9D 53 77 15 1A C4 C2 C2 22 30 30 B0 FC 14 
6F 6F 6F 73 73 73 75 D5 53 DE 5B 6F BD B5 7D FB 76 22 1A 3B 76 EC E1 C3 87 5B BE 00 8D ED CD 17 
11 18 DA 74 F0 B5 51 31 03 DF C0 D6 D3 D7 B6 BA 4F 78 FA 96 6E 9D 2C EB B6 9A 9A 5B 21 BE 81 AD 
67 E7 EA 3F 52 49 28 B1 A9 65 A9 06 B5 5C 9F AD AA 48 F5 AE E4 E9 9A 3A 78 9A E2 6E 79 68 16 42 
D7 41 13 5C D5 5D 04 40 1B A5 B1 E7 E6 01 00 00 A0 B1 10 F3 00 00 00 5A 4B C3 07 ED 41 53 3C 7C 
F8 F0 97 5F 7E 49 4C 4C CC C8 C8 50 77 2D AD 92 B5 B5 75 60 60 A0 85 85 85 BA 0B 01 80 B6 05 31 
0F 75 32 61 C2 84 90 90 10 75 57 D1 BA 45 44 44 AC 5C B9 52 DD 55 00 40 DB 82 41 7B A8 5D 76 76 
36 32 BE F1 9E 3F 7F AE EE 12 00 A0 CD 41 6F 1E EA C7 D2 D2 B2 53 A7 4E EA AE A2 35 C1 8D 4F 00 
A0 46 4D 1D F3 CA 67 FB BE F8 E1 92 ED 8C 75 73 FC 44 4D DC 74 CD 14 29 C9 D9 66 96 A6 0D 18 99 
28 5B 52 19 FE D7 D2 6F CF 9B 4E 59 3D 7F 80 41 ED CB 35 A7 CA 95 D4 65 EB 5A B0 FA C5 8B 17 23 
E6 EB C5 D7 D7 F7 87 1F 7E 50 77 15 00 D0 46 35 F5 A0 BD 22 E1 DA FE ED 3B CE 3E 97 37 71 BB 35 
52 46 1C 78 AF 77 B7 39 A7 0A 1B B9 A4 32 F9 E6 9F DB 77 9C 0A 29 68 EA 02 EB AD 7C 25 75 DD 3A 
CD A9 1E 00 00 34 49 AB 1F B4 57 84 05 9D B8 9F 5E C3 7B DA 9B 69 C9 E6 25 F4 7E FF F7 8B 63 85 
6E 26 A4 B9 35 56 A0 54 2A E3 E2 E2 E4 F2 7A 1C DA 49 24 12 33 B3 B2 37 F2 29 14 8A B8 B8 38 85 
A2 F6 D7 F0 94 32 31 31 31 35 35 AD 3A 9D 65 D9 F8 F8 78 A9 54 5A F7 A6 0C 0D 0D 2D 2D AB 79 E4 
10 C7 71 09 09 09 05 05 F5 38 76 12 8B C5 D6 D6 D6 75 9F 1F 00 A0 B9 35 2E E6 F3 A2 EF DC 78 10 
5B 68 E8 D0 B1 AB AF A3 61 0D 23 03 6C 6E 7C C8 83 07 11 39 12 37 DF 4E EE D6 FA E5 E7 62 33 23 
EE DC 7B 92 20 95 38 7B 76 EC E0 58 F9 E9 AD B2 F4 B0 07 F7 9F A6 EA 3A 77 F6 F5 A8 FE 79 B0 6C 
6E 62 64 52 1E 4B 5C 41 5A 54 44 8C 95 AD BD A9 A8 96 35 D6 B8 64 E9 58 37 2B 4B 0B BF 7B F7 85 
CC A6 4B 77 AF CA CB D6 5E 91 8A 0D CB 4F 89 4A CA D3 B5 74 B2 12 17 37 96 19 17 97 41 12 3B BB 
92 07 EC E6 25 47 26 4B 8D 6C ED C5 46 96 B6 1C 5F 9F 57 C3 D6 A9 DC ED AA AA 57 51 BE EA 5F 44 
2D 8E 1D 3B 96 9C 9C 5C AF 45 88 A8 77 EF DE A5 83 FF 47 8E 1C 49 4F 4F AF 6F 0B FD FA F5 F3 F2 
F2 AA 34 F1 CC 99 33 D1 D1 D1 F5 6D AA 73 E7 CE 3D 7B 56 7E 43 E0 C5 8B 17 5F BC 78 51 DF A6 3C 
3D 3D FB F7 EF 5F DF A5 00 00 9A 49 83 07 ED 73 83 B7 4C EF E2 E0 D6 63 D8 2B FF 09 18 DC AD BD 
4B CF 77 F6 85 57 E9 8C B1 E9 D7 7F 9C DE D3 D9 D1 A7 DF 98 B1 A3 FB 78 B5 B3 EB FE D6 E6 3B 99 
45 8F B6 56 84 EF 9B D5 D5 C5 BD D7 D0 80 71 63 06 F8 3A DB 74 18 B3 22 A8 F4 E5 AC 85 A1 07 17 
0D 72 B2 6E DF 73 F8 2B 63 FA 79 B7 B3 EF 39 6B 47 70 56 D5 22 B2 F7 BF E5 FB FA EE 38 65 FE C9 
0F BC DC 87 2C BB 29 53 B9 46 D5 4B 12 11 B1 59 77 37 04 7A B4 73 F7 1B E1 3F B0 93 9D 45 FB 71 
3F 05 97 F4 0A EB 58 51 CD 1B A6 8C DD 3E C9 C3 7D F8 CA 3B C5 7B 29 F7 F8 9C 2E 6E 6E 9D DE FF 
3B A7 78 B1 87 DF 8F 72 F7 7D F7 50 46 E1 BD 55 C3 DC 3D A7 FF 9E 56 5D 8D AA 76 BB 8A EA 55 95 
AF F2 17 51 AB 9C 9C 9C 06 64 3C 11 85 87 87 17 FD 90 91 91 D1 80 8C 2F DF 42 29 B9 5C DE 80 8C 
AF B6 A9 9A 26 D6 2A 2C 2C AC 01 4B 01 DC 57 FE DD 00 00 20 00 49 44 41 54 00 34 93 06 C6 7C E6 
E9 8F 5F 9D FD 7B BA DF 7F 4F BE 48 CB CF 8D 3A B7 A4 53 D4 B6 77 66 FE 14 5A E1 65 69 6C DC DE 
59 81 F3 FF 48 EF B7 F2 6C 58 5A 5E 46 44 D0 9A 11 79 FB DE F7 7F 63 47 84 92 D8 B8 ED 73 DE DB 
95 FB D2 86 0B 8F 13 F3 0A D3 1E ED 99 AC 7B EE EB 59 2B 2E 4B 89 88 32 4F 7D 34 7A D2 FA C8 EE 
CB 4E 87 66 E6 67 C5 5E D9 F4 72 C1 BE B7 C7 CC FE B3 4A F8 18 4D DC F9 64 DF 34 3B BE FE E8 9F 
5E 84 5F FA AA 97 40 C5 1A 55 2F 59 D4 7F 95 DF D8 B5 8F 3F 6B EF F5 A8 D4 84 BB BB DE 70 88 FD 
7B C9 D2 FD 69 F5 AA A8 E6 0D E3 BB 8C 7E C9 87 9E 05 9D 8F 54 12 11 49 6F 9E BF 92 CE B2 59 D7 
2E DE 92 12 11 29 C3 4F 9E 79 24 EC 3D 66 94 25 AF E6 1A 55 EF F6 1A AB 57 55 BE CA 5F 44 1D B0 
6C 9D 8F 08 6A 58 B0 F1 2D 34 61 31 E5 29 95 0D 79 47 6A 83 6B 00 00 68 0E 0D 8A 79 36 F9 F0 A6 
BD E1 A6 81 AB B6 7F 3C D2 55 22 D4 B5 1F F2 F9 BA 2F 5F EB 25 C9 8A CC 29 37 97 E2 C9 F6 F5 C7 
52 DA BD BE 71 C7 C2 A1 CE C6 22 43 C7 81 1F 6C FB F9 4D E7 F4 93 6B B7 DC 55 28 A2 43 23 F2 18 
13 B7 1E DD 3D 2D 44 02 89 D7 C4 15 5B B6 6E 5A 36 DE 81 88 D8 F8 FD EB 77 87 59 4C F8 61 E7 27 
C3 5D 0C 85 62 5B BF B7 7F DD FC AE 6B E2 C1 8D 7B 22 2A 57 2F B6 B0 33 D7 E7 11 23 92 D8 DA DB 
48 78 AA D6 A8 72 C9 E2 51 6A 9E D5 6B AB 77 7C 36 AE A7 83 99 75 97 29 4B DF EB 2B C8 7D 7C 3F 
44 A6 B2 A2 CA 49 50 F3 86 09 3C 46 8F F2 E4 EE 07 9D 4F 61 89 14 0F CF 5F 4E 74 EB DB DB 24 E1 
EA BF 21 0A 22 36 EE F4 A9 7B FC 9E 63 FC 6D 78 35 D6 58 DB 6E AF A1 7A D5 E5 AB F8 45 00 00 40 
AB D7 A0 73 F3 F2 07 37 EF E7 89 7A F9 97 F5 3C 05 3E 73 7F 3F 3B 97 88 A4 17 4B E7 CA BD 7D EB 
09 6B 39 29 70 A8 A4 74 92 41 DF 71 23 EC 7E D9 76 FB 76 9A 60 46 E0 6B 5D 7E 5D BE 7C 80 FB D1 
FE C3 47 8E 1A F3 4A E0 A8 C9 B3 FA F1 88 88 0A EF 5C BF 97 CF 48 A4 8F B6 AD 2E 1D 34 55 A4 89 
05 B2 07 37 EF 10 A9 7A CF 95 CA 35 B2 3D AC 6A 7F 55 7C 87 AE DD 4A DE 3A C9 33 B3 32 D7 E5 9E 
E7 E4 72 24 53 51 91 94 5C F5 CB 37 21 EC 52 E3 86 91 8F FF 48 D7 6F B7 9E BF 98 F3 CE AB 29 41 
FF BE B0 1A B2 7B 7A CC 8C F9 97 FE 8D 63 3B E9 9E 39 75 8B ED F6 D5 E8 76 BC 9A 5F 1F AF 62 B7 
CB AE D7 5C BD EA F2 85 01 35 D7 0B 00 00 AD 5D 83 62 5E 99 99 99 4D 7A 46 46 2A 5E 4C 4E C4 66 
67 E5 28 19 43 13 93 F2 91 C1 37 31 35 E6 B1 05 F9 F9 9C AE DF 57 A7 4E DB AF 5C BF E7 E8 99 7D 
AB CF FC B6 7A A1 81 D3 C8 85 5B B6 2D 19 6A AD CC CA CA E6 A8 F0 EA 9E CD F7 CA B7 D7 CE C5 CE 
40 F5 70 68 2D 6B AC C3 86 31 BA 7A 7A 65 0B 33 0C 11 11 47 A4 AA 22 29 51 85 98 A7 9A 37 8C 2F 
EC 36 66 84 C3 BA 83 E7 AE E6 0F 8A 0D 0A 16 FB CD 1B 36 2C CA 97 5B FE EF E5 CC A9 CC C9 AB B2 
CE 1F 8F 71 E2 53 CD 31 5F DB 6E AF A1 FA 5A CA 57 51 6F 1D F6 18 00 00 68 B4 06 C5 3C 5F 22 31 
A2 FC F4 F4 3C A2 92 C8 51 C6 5C 3F F5 94 F1 EC 59 F6 DC 14 9E B1 89 44 87 7D 9C 9A 5A 3E 9B 95 
49 89 69 2C DF D1 CC 94 4F 44 A6 BD 67 AE EE 3D 73 B5 2C E5 E1 B9 C3 7F ED DB BC 69 DF 7F 27 2F 
F2 49 DA ED 6F 6C 6C C8 13 F9 AD B8 75 E4 0D 09 D5 47 ED 6B 6C 20 7E FD 2A E2 55 BB 61 A1 BB C7 
19 0A 7B 8E 19 66 F3 73 D0 B9 2B 41 09 37 B9 1E DF F4 37 72 74 1E D8 7E F1 CE 8B E7 4F 15 5E CE 
F7 99 3D C6 4D 65 91 2A 76 BB 6F 63 CA AF BE DE A4 DD E3 EA B0 AD 00 00 A0 D1 1A 34 3A 2B F2 F5 
EB 66 28 BB 76 E4 48 5C 49 A0 CA 1E 6C 79 37 70 F4 EC BD 31 E5 12 D6 A0 77 9F 2E C2 F4 D3 BF 1F 
29 BB 4E 2D FD 9F 7D 27 13 79 3E BD 7A 1A E4 9C F8 B0 BB 4B B7 05 41 85 44 42 0B 1F FF B7 97 6E 
FD 7E 92 23 93 19 17 4B 24 EC E6 D7 4D BF F0 D2 1F 7F 84 97 F6 6B 65 B7 97 0E 74 F6 0C DC F0 B8 
BA 2D E0 F3 89 65 95 6C 6D 6B 54 B9 A4 6A AA 2A AA 7C 77 41 8D 1B 96 C9 12 91 6E 9F 31 C3 2C 22 
CF AE FB ED 6A 9E CF 80 81 16 3C 81 CF E0 7E 56 29 17 56 AF 39 9B E5 39 2A C0 B3 EA 41 57 F9 1A 
EB B6 DB EB 59 BE 8A 5F 04 00 00 B4 7A 0D 3B 09 6B 31 6E FE 4C CF 82 53 8B 27 7F BA E7 F2 A3 E7 
4F 2E ED 98 3F 6B C3 13 CB C0 8F A6 7B 95 0B 2A BE DB F4 45 53 1C D3 0E CE 1B BF 70 E7 BF 0F 9F 
3D BA BC E7 D3 F1 B3 F7 25 D9 8E 5F 30 C3 83 2F EE D2 D9 21 27 F8 D7 79 EF FD 7C 36 38 2C E2 D9 
CD C3 AB 56 1D 8C 16 75 1D 3E 98 88 67 3B 61 C1 0C F7 FC 33 9F 8D 7F 67 E3 C9 3B CF 42 83 4F AD 
9F 3E F5 DB 2B D9 ED 86 0C F7 A8 5A 0A DF C4 C4 98 27 BB FB E7 0F 5B F7 5F 49 74 56 B1 46 95 4B 
C6 A9 BC A6 5A 55 45 95 93 B9 C6 0D 2B BA B6 4E 3C 70 F4 60 E3 27 27 4F C7 39 F5 1F EC CC 27 12 
F5 1A DC C7 30 EA D6 9D 74 B7 97 C6 74 AC 9A F2 15 6B AC D3 6E AF 67 F9 2A 7E 11 00 A0 A1 8C 8C 
8C D4 5D 82 66 31 36 36 56 77 09 9A AB 81 8F C7 31 18 B0 F2 D4 51 A3 F7 E7 AC 9E 31 70 B5 82 63 
F8 26 BE 93 D6 1C DC 38 D1 9E 27 2F 77 D3 30 CF 22 E0 A7 73 87 6C 67 CF 5F 3B 6B F0 5A 05 C7 F0 
0D 9D 06 BE BF EB C7 6F 26 D8 F2 88 6C A7 6E DA 13 31 6B DE BA B9 23 77 2A 39 86 27 94 B8 F6 9F 
BB EB D7 45 3E 44 44 C6 43 7F 38 7B C2 FA 83 B9 DF 2D 18 BD 4D CE 11 23 30 F5 09 5C 79 E8 A7 D9 
9E D5 8C 69 0B 3A 4E 7A 67 CC DE 45 47 97 BE 7D FE F6 D6 E1 C7 DE AC 79 8D AA 97 3C 58 F9 39 2B 
15 D4 BD 22 5E 4D 1B 56 BC A7 0D 07 8F 19 64 BC EF 90 6E DF 41 9D 85 44 44 E2 01 83 7B E8 1E 38 
DF 6E E4 98 2E D5 3C 94 A6 F2 D6 D5 B4 DB 15 71 AA AA 57 59 BE AA 5F 04 00 68 24 73 73 F3 DE BD 
7B 87 86 86 E2 06 4E 22 32 36 36 EE D6 AD 9B BA AB D0 5C 0C C7 D5 E5 DA B4 9A 28 73 E3 9F 87 65 
E8 3B B4 77 34 51 F1 E4 34 65 6E FC F3 D0 54 BE B5 AB 5B E5 47 C7 B1 85 E9 B1 11 D1 19 7C BB 0E 
EE 16 55 AF 2C 63 F3 93 C3 43 E3 E5 26 4E AE F6 B5 3C 98 4D 96 99 90 CA 9A 58 9B EA F2 6A 5B 63 
6D 4B AA 56 E7 8A 54 6F 58 3D 55 A9 B1 6E BB BD 9A AA 6A 2C 5F 75 BD D9 D9 D9 A5 07 CB EB D7 AF 
2F FF EA 9A AC AC AC FD FB F7 D7 6F 7B 88 88 C8 DC DC 7C DC B8 71 44 94 96 96 76 F0 E0 C1 06 B4 
60 6D 6D 1D 10 10 50 7E 8A 54 2A DD B5 6B 57 03 9A 32 30 30 98 3C 79 72 A5 89 9B 37 6F 6E 40 53 
02 81 E0 CD 37 DF 2C 3F E5 F8 F1 E3 45 AF AE 31 35 35 AD D7 96 2E 5B B6 EC C2 85 0B 44 34 74 E8 
D0 25 4B 96 94 FF E8 C4 89 13 71 71 AA 8F ED DA 96 01 03 06 78 78 54 18 ED 1B 32 64 48 D1 97 DB 
F2 E5 CB FB F6 ED 5B F7 A6 1E 3D 7A 34 77 EE DC A2 9F 8F 1E 3D 6A 68 68 A8 7A 7E D0 4C DE DE DE 
E6 E6 E6 EA AE 82 88 E8 AD B7 DE DA BE 7D 3B 11 8D 1D 3B F6 F0 E1 C3 2D 5F 40 23 9F 69 CF 37 B0 
F5 F4 B5 AD CB 5C 9D AB 9D 8B A7 6B EA E0 69 5A D3 4D DA 3C 7D 4B B7 4E D5 3C 6C BC 2A A1 C4 A6 
42 FB 35 AF B1 B6 25 55 AB 73 45 AA 37 AC 9E AA D4 58 B7 DD 5E 4D 55 35 96 DF A4 F5 02 00 80 A6 
C0 0D D2 00 00 00 5A 0B 31 0F 00 00 A0 B5 10 F3 00 00 00 5A 0B 31 0F 00 00 A0 B5 10 F3 D0 5A 31 
45 CF F3 55 39 A5 69 DB 6F BE A5 00 00 9A 09 62 1E 1A C5 D0 D0 50 4F 4F AF 01 0B DA D8 D8 14 FD 
60 6C 6C 2C 14 D6 EB BE C0 62 D6 D6 D6 95 A6 08 85 C2 86 3D 25 A3 B4 98 F2 2C 2C 2C 9A A4 2A 00 
00 35 6A E4 0D 75 D0 D6 F1 78 BC 31 63 C6 3C 7B F6 4C 26 93 D5 7D 29 89 44 52 7A 97 B3 40 20 08 
08 08 78 F6 EC 99 5C 2E AF 7B 0B 26 26 26 95 EE 93 2E 32 6A D4 A8 A7 4F 9F 16 16 16 D6 BD 29 43 
43 C3 6A 9B 1A 3E 7C 78 48 48 48 7E 7E 7E DD 9B 12 8B C5 D5 36 05 00 A0 2E 88 79 68 2C 13 13 93 
DE BD 7B 37 A6 05 53 53 53 3F 3F BF 26 29 C6 C8 C8 A8 67 CF 9E 4D D2 94 58 2C EE DE BD 7B 93 34 
05 00 2D 4C 24 12 A9 BB 04 4D 81 98 87 FA B1 B6 B6 16 89 44 4A A5 CA B7 00 A8 15 C7 71 9A 5C 5E 
93 90 48 24 78 0A 5E 79 E6 E6 E6 02 41 F5 DF 66 7C 3E BF A6 8F 6A 9A BF F4 67 81 40 50 AF 65 41 
13 F0 78 3C 2B 2B 2B 3C BE B0 14 FE 82 A1 7E 6C 6C 6C 9A AA E7 DD 46 BC 78 F1 A2 E8 07 91 48 34 
68 D0 A0 BA 2F 58 FA B4 5D 2B 2B AB 4A 0B FA FA FA 1E 39 72 24 3A 3A BA 69 4A 6C CD 78 3C 5E D7 
AE 5D 47 8F 1E 5D 69 3A C3 14 3F C9 DB CB CB AB 5F BF 7E F5 6A B0 F4 E7 DE BD 7B 9B 98 98 34 49 
9D 00 EA 82 98 07 68 7D 4C 4C 4C 66 CC 98 A1 EE 2A 00 A0 15 C0 95 F6 00 00 00 5A 0B 31 0F 00 00 
A0 B5 10 F3 00 00 00 5A 0B E7 E6 A1 B1 1E 3D 7A 74 FE FC F9 82 82 02 75 17 A2 7E 26 26 26 FE FE 
FE F6 F6 F6 EA 2E 04 00 A0 18 62 1E 1A 45 26 93 6D D8 B0 21 37 37 57 DD 85 68 8A C8 C8 C8 95 2B 
57 AA BB 0A 00 80 62 88 79 68 94 B4 B4 34 64 7C 79 31 31 31 2D B6 2E A5 52 59 74 CF 58 9B C5 30 
4C F9 DB DC B5 9D EC C6 D2 11 53 76 46 29 89 88 67 1A F8 CB D5 35 C3 9B F8 09 30 E5 56 20 EC FE 
F9 D9 FD B3 1C 5A EC B4 AE 22 E1 FA CD AC 2E 7D 3C F0 4C 9B A6 87 98 07 68 7D 64 32 D9 5F 7F FD 
15 19 19 59 F7 45 04 02 41 FB F6 ED AB DE 05 CE B2 6C 58 58 58 4A 4A 4A DD 9B E2 F1 78 2E 2E 2E 
96 96 96 55 3F 8A 8C 8C 8C 8F 8F AF 7B 53 0C C3 D8 DB DB B7 6B D7 AE E8 9F 1C C7 45 44 44 24 26 
26 D6 BD 05 5D 5D DD 11 23 46 78 7B 7B D7 7D 91 D6 4B 9A 1E 1B 15 19 A9 20 22 5E 4E 6A 5E 33 1C 
E1 95 AD 40 A7 5D 96 A2 85 0E 21 A5 51 E7 36 2C 5E B4 EA B0 DD 77 71 C7 11 F3 CD 00 31 0F D0 FA 
FC FD F7 DF C7 8F 1F AF EF 52 4F 9E 3C 79 FD F5 D7 2B 3D D6 2D 34 34 F4 E2 C5 8B F5 6D EA E1 C3 
87 93 27 4F 16 8B C5 E5 27 26 24 24 9C 39 73 A6 01 4D 8D 1D 3B B6 E8 A0 21 26 26 E6 DC B9 73 F5 
6D E1 F6 ED DB DF 7D F7 5D E9 B1 02 B4 22 8A C7 5B A7 BD B6 70 7F 48 16 CB 89 6C D5 5D 8C B6 C2 
95 F6 00 AD CF D3 A7 4F 1B B0 94 4C 26 CB C8 C8 A8 34 31 21 21 A1 01 4D 71 1C 97 9C 9C DC 24 4D 
11 51 69 F7 BD C1 2D 3C 7B F6 AC 61 0B 82 7A 29 9E 06 9D 7A 9A C5 B6 E9 53 4F CD 0E 31 0F D0 FA 
34 F8 94 7C 13 9E CB AF DA 94 1A 2F 14 68 E3 D7 28 00 A8 80 98 07 00 68 C5 94 D1 FB 3F 1A 17 10 
10 10 10 30 76 E6 AF 0F 14 94 F3 F0 C0 D2 19 C3 3B 3B 5B 9B 9A DB 7B F7 1D FD FA C2 4D 97 E2 AB 
79 4B 74 EE D3 23 AB 66 4F 1C D9 CB C3 D6 D4 CC AE 43 9F FF 2C DA 76 33 8D AD B6 FD E7 BB 66 07 
06 04 04 04 04 04 8C FB F4 70 52 F9 79 B2 4E 7E 55 B4 E6 80 B1 D3 37 DE AE BC 92 BC B0 93 EB E6 
BD 36 D8 D7 C5 D6 CC 58 62 61 DF A1 73 1F FF B7 96 1F 08 4E 2B 7E AF 94 EC FA DA D7 5F 19 FF DD 
E5 DC E2 23 34 C5 BD 4D 93 03 02 02 02 67 EF 7A 5E E1 CD 53 B9 61 E7 B7 2C 99 36 A2 47 47 67 1B 
13 63 13 6B 47 AF 3E AF CC 5A BA F7 4E 5A A5 D7 53 C9 6E AE 9B 3A B6 A8 CA 4F 0E 25 64 DE FA F5 
FD 31 3D 5D 2C 2D DA 79 0D 9C FA ED F9 84 6A B7 AC AD C0 B9 79 00 80 56 8C CB 79 7E E9 F8 B1 3B 
72 22 BE 93 ED D8 BD 6F 4E 9C B3 33 A4 24 3A 33 AE C6 3E BE 7A F2 C0 C1 CB 3B 82 F6 4C 71 2A BD 
27 41 19 75 70 76 C0 8C 2D 0F 72 4A D2 2F E3 DA A1 EF AF 9F 38 74 E2 8D 8E B2 2A C3 22 CA 8C 27 
41 C7 8F 85 28 88 48 27 63 C0 F7 E5 3E 67 A5 51 D7 4F 1C 3B 23 23 22 9E 8D E5 DB E5 A3 34 FF F1 
8E 77 C7 CF DD F3 B4 EC 3A C1 EC AC D4 D8 E7 C1 D7 4E FD F6 E3 F0 6F 4F 1E FA A8 8B 98 4D BC 77 
EA F8 89 B2 43 0B 65 C2 DD 93 C7 EE 92 C0 D3 F3 53 25 51 51 AD 59 F7 7E 9D 33 65 E1 DE D2 ED 21 
A2 EC CC A4 E8 90 6B C7 76 FE B4 69 DE B6 7D AB 5E 76 28 C9 30 36 F1 FE 99 63 C7 92 59 22 9D 14 
97 4E 21 FB 97 1F 4F 54 72 44 94 F2 BF 93 77 92 D7 B7 E9 0E 6D 9B DE 78 00 00 ED C1 26 EE 9B F7 
F6 CE 90 3C BE 81 B5 8B B3 95 1E 8F 29 9A CC C9 23 FF FC 6C C5 99 BC 92 D9 14 8F D6 BE 31 B3 34 
E3 19 46 28 B1 77 B6 33 D2 21 69 F8 E1 CD C7 62 9A E0 1D CE 6C DA 3F 0B C6 BD 53 9A F1 0C 4F 64 
6C 6A 2C E2 33 44 44 9C 22 E9 EC A7 81 B3 0F 25 D5 DE BF 56 46 EE 98 EE 3F 7B 4F 59 C6 33 0C 8F 
29 D9 26 45 CA D5 35 AF F9 7F 74 BA 9A 11 08 C5 DD 5F 56 16 67 3C 11 F1 2C FC 27 F9 9B 36 7E A3 
5A 31 C4 3C 00 80 56 E0 0A B3 73 74 3A CE DC 1D 1C 1B 17 16 9E 98 12 F2 DB 14 17 41 51 2C 2A 13 
2E 9D 0F 2E 1E 54 CF 3A F1 FD DA FF 65 B2 44 44 8C D0 69 EC 9A AB F1 A9 D1 E1 B1 E9 31 17 96 8F 
B0 2B 9E BD 71 14 F7 D6 7D BE ED 85 9C 23 22 46 E4 1A B8 E6 DF B8 AC CC B4 CC B4 A7 7F BE DF 45 
CC 10 11 27 8F FE 7D F9 A6 07 BC FE 9F FD 7D E1 CC 8A 11 46 C5 EB D4 E9 F3 D9 89 A0 A0 A0 73 BB 
DE F1 12 12 11 1B BD 73 DE A2 A3 89 CA A2 3A DB 0D 5F B2 FF 66 44 46 4E EA F3 4B 5B DF EB 6E C2 
67 88 88 2B 7C F2 F3 FB 4B CE E6 54 D9 0D 32 A9 9C 6F 33 70 EE 9A 5D BB B7 AC 78 77 E2 BB 6F 8C 
30 6E 82 AD 6A C5 30 68 0F 00 A0 25 04 6E 33 7F 5C 3F C5 4B 9F 88 48 EC 3E F1 E3 A9 6B 0F 2C BD 
2B 27 22 36 25 A1 F8 FC 74 FE BF 87 4F 16 85 27 F1 9D 66 FC F2 DB FC DE 86 44 44 7C AB 41 9F FD 
B6 EA 9E F7 1B 07 53 1B 79 1E 5B 76 7D EF 81 C7 72 8E 88 88 EF 38 FD A7 DF 3E EC 6F 40 44 24 76 
FB CF 77 CB 4F 9F 99 71 4E DC A1 63 C7 8E 9D 5C 14 32 B3 CE 7D 07 3A 27 6E E6 33 44 1C 11 F1 24 
AE 7D 06 0D 92 14 B7 A2 7C BA 67 CB E9 F4 A2 83 11 83 BE CB 4E 1E FF D8 5B 48 44 64 DC FF AD 4D 
67 1D 99 5E A3 7E 7E AE E0 38 45 C4 DE 8D FB BF 1A 39 D3 BA 62 09 7C 9B 49 3F 1D DE 10 68 42 44 
AF CF 6C DC D6 68 03 C4 3C 00 80 76 E0 19 F5 E8 DF 43 BF F4 9F 02 47 47 5B 1E DD 25 22 E2 A4 05 
85 2C 11 91 22 EC DE C3 8C A2 20 E7 5B 0F 0F 1C 64 58 6E 69 AB D1 63 FA E8 1D 3A DA B8 E7 EE B0 
F1 37 6F 47 28 8A 1A 94 F4 1B 59 94 F1 45 C4 FE 9B 5F 24 D5 AD 15 65 E8 D1 63 77 A5 1C 11 11 63 
34 FC ED 99 45 19 5F 4C 32 78 F6 D4 CE 5B BE B8 2D 27 E2 F2 AE 9E BF 4C 33 5F AD B0 30 63 D8 7F 
CC 88 CA CF 81 6A C3 30 68 0F 00 A0 1D 78 A6 96 96 E5 BB 6E 3A BA 22 9D E2 21 71 B6 F8 96 43 65 
6A 4A 71 CA 13 CF C6 DE BE 62 47 4F DF C9 C1 52 65 26 54 3A 00 50 2A AB 9E C9 57 24 24 A6 94 9C 
16 37 B7 B6 6E 60 4F 52 11 11 16 53 5C A6 C0 D9 C7 C7 A8 E2 A7 7C 17 6F 4F E3 A2 42 D9 DC D8 A8 
CA 0B 0B 1C DB B7 C7 D3 F4 CA 20 E6 01 00 B4 03 A3 AB A7 5B E1 3B BD D2 23 0F 89 88 94 4A 45 E9 
CF 95 9F 36 C0 E8 08 75 54 9E 9D 57 C8 E5 E5 FE C5 15 16 56 BD 30 9F 14 B2 D2 15 34 E2 71 06 E5 
16 15 E8 08 AB 14 A5 23 D4 29 AB A9 F2 87 8C C4 CC 14 D1 56 06 FB 02 00 40 4B F0 F8 B5 F5 9E F9 
12 D3 E2 7E 30 B1 C9 09 89 8A 0A 1F 2A 12 13 AB 3B 33 CF 94 A6 AC 5C 56 3E D7 B9 BC BC FC AA 25 
98 98 1A 97 8C 20 A4 26 56 5C 01 9B 16 FA 20 34 B9 B0 C6 93 FF 65 6D F3 2D AD CD 4A CA 4C 88 8E 
AE 58 26 B1 59 B1 31 19 C5 97 F1 EB 58 54 3A 31 4F 44 42 91 6E 4D AB 68 8B 10 F3 00 00 6D 86 A0 
83 B7 87 B8 E4 F2 FB 6B 97 42 CA 07 68 DE D5 73 57 B3 AA 64 30 4F 4F 24 2A C9 ED F4 E4 94 72 C3 
F4 D2 D0 D0 AA F7 DF 09 5C 3A 79 49 8A 07 D4 B3 6F 5E BA 5D 58 F6 91 F2 E9 CF 93 BB BB 5B 1B 49 
6C 3B F4 7C 6D 63 B0 82 88 88 C7 2B 09 21 4E 2A 95 96 AE 5C D0 A1 6B A7 E2 56 94 89 67 0E 5D C8 
2A BF 0A 36 E6 F0 E1 1B C5 27 EE 05 3E 7E 7E 95 4B 60 84 22 11 A2 AD 0C F6 05 00 40 DB 21 1E 34 
AA 7F 71 7F 5E FE 60 C3 7B 5F 05 15 DF 79 AE 88 39 FE E9 A2 DD 51 55 4F B6 F3 4C CC 24 25 31 1F 
77 ED C2 83 92 DC CE B9 BD 7E ED B1 6A 3A FF FA 03 C6 0C 35 2F 5A 81 22 6C E7 A2 25 27 E3 8A DA 
94 45 FE F1 DF CD 77 15 1C 27 CF 49 08 8D E4 5B 15 3D D8 46 24 2A B9 B8 8E CB 4A 4B 2B EB CE 1B 
0C 9D 30 C6 A6 E8 19 39 CA E8 5D 73 DE FC F1 66 F1 25 05 CA F8 D3 5F 4E FB F2 6C 16 47 44 C4 33 
1E F2 C6 A4 F6 55 4B E6 B5 9D B7 13 D7 01 AE B4 07 00 68 3B 78 36 AF 7D F8 C6 77 A7 37 3C 93 71 
C4 66 5E 5F 39 A6 D3 09 BF BE 9D CC B2 EE 5F 0C 7A 94 2C AF E6 5C 3A CF AA 57 0F 57 41 50 88 82 
88 93 DF FF 6E 4C EF FB A3 87 F8 98 64 DC 3E 76 F0 42 68 4E 75 E3 EF A6 AF 2C 78 BF DB B1 A5 B7 
F2 38 62 B3 6E AE 79 C5 FB B0 6F 37 0F D3 BC A7 37 6E 45 64 29 39 22 62 74 7D 66 CD 7B C5 84 88 
48 60 61 69 C6 A7 04 96 88 E4 77 BF 1B DD EB 1F E3 54 E9 88 DD D7 57 F7 17 1A FB 7F B5 2A F0 CC 
F4 83 71 4A 8E 93 85 1F FA 60 C0 BF 1B 7C 3B BB 8A 33 42 EE 3E 8C CE 2A AA 93 27 E9 F7 E9 CA 19 
4E 88 F4 5A A0 37 0F 00 D0 96 18 0C 5E B6 7D 71 1F 93 A2 2F 7F 2E 3F 3E F8 FC 9F BB 0F 9C 79 98 
AC D0 71 9D F0 D6 88 AA 17 AF 09 BB BF 3D 6F 44 71 FF 9C 93 27 07 9F D8 B1 76 D5 9A 9D E7 42 F3 
CD 06 CF 9D D6 45 A7 F2 EC 44 A2 EE 9F ED 5A 37 CE B9 68 A8 9F 93 67 86 DF 3E FF CF 99 EB E1 C5 
19 CF B7 1C F2 DF AD 9F F7 2E BA 14 5E E0 E5 D7 C3 A2 38 A7 D9 DC A8 3B D7 1F 44 87 46 C6 C9 89 
88 F8 8E 93 7F D9 FD C5 60 BB A2 8B 02 39 69 DA 8B 9B E7 4F 05 DD 8D 2A CD F8 9E F3 F7 EC 5B D4 
05 67 E1 6B 85 98 07 68 7D CA 4E 68 36 7A 41 86 69 E0 83 CF AA 36 D5 F8 AA 1A 5C 4C 35 57 94 43 
CD 0C FB 7C 79 FC EC 2F B3 FC 6C 4A 4E BA 33 3A 26 5E 81 4B 8F 06 6D 79 D9 BA 9A BE 31 DF 75 D6 
9E E3 6B 27 FB 9A 95 5C 86 CF 08 8C DB 8F 5C F0 FB E5 23 EF 7B 08 AB CE 4E 44 42 CF 99 7F 5C FE 
67 F5 F4 3E 76 FA BC B2 5F 29 23 B2 EB FB D6 0F A7 6E 1C 5F D4 5D 5C 32 4D EC FF E5 FA F7 7A 5A 
96 5E E0 CF 08 44 CA 82 EC A2 41 02 9E D9 E0 AF CE DC 3E BB FE BD 51 DE E6 65 17 DB 33 8C C8 C2 
37 70 C9 1F 57 CE FF 30 DA 16 09 56 07 F8 BF 01 D0 FA 74 EE DC F9 C9 93 27 F5 5D CA C8 C8 C8 CC 
CC AC D2 44 27 27 A7 06 BC BD 5E 28 14 DA D9 D9 55 9A E8 E0 E0 70 FB F6 ED FA DE 44 C5 30 8C A3 
A3 63 D1 CF 8E 8E 8E C1 C1 C1 F5 6D 41 20 10 74 E9 D2 A5 5E 8B B4 4E C2 01 EB 43 E5 EB 2B 4F 15 
74 FC E2 B6 EC 8B 6A 17 D0 7F F5 8F 8C 57 FF A8 F6 23 49 B7 59 9B AF CE 58 F9 F4 C6 AD 67 C9 0A 
23 27 DF 9E 5D EC C5 44 44 BB 92 95 BB AA CE CD 33 E9 F5 C1 DE FB 33 57 3F B8 1D 1C 9E CE 99 38 
7A 77 F3 75 30 E0 11 D1 67 D7 65 9F 55 5F AC C0 6E C8 82 1D 43 16 FC 18 F7 E0 EE 93 A8 D4 7C A1 
A5 53 FB F6 ED 1D 2D F5 2B 1F 46 F0 9D C6 6F BC 31 76 79 D4 A3 3B 0F 22 0B F4 AD 9C BD BB 7A 59 
95 BB E9 9D 6F 3D 70 EE A6 81 73 D7 A6 85 3D 09 09 8F 4E CE 13 9A D8 75 E8 D2 D5 45 52 E5 68 44 
37 60 67 92 72 67 F5 B5 B4 71 88 79 80 D6 67 C0 80 01 85 85 85 E9 E9 E9 75 5F 44 20 10 D8 DB DB 
57 ED 2E 3B 38 38 4C 98 30 21 29 A9 8E 4F 27 23 22 E2 F1 78 ED DA B5 D3 D1 A9 3C 5C 6B 66 66 36 
79 F2 E4 B8 B8 B8 BA 37 C5 30 8C AD AD AD 58 5C DC B9 B3 B2 B2 9A 34 69 52 7C 7C 7C BD 5A E8 D7 
AF 9F B1 71 1B 7F 6C 79 C3 08 CC 3C FA BE E4 51 E7 D9 F5 6D 3B 0D B0 ED 54 BF 55 88 ED 3A F5 B7 
AB 75 19 81 B1 63 E7 21 8E 9D 6B 9E 41 64 E6 DA A5 9F 6B 5B 38 96 6B 06 88 79 80 56 C9 DC DC DC 
DC DC BC 49 9A 32 36 36 6E AA 98 14 8B C5 EE EE EE 8D 69 C1 C0 C0 A0 BE 2D 18 1A 1A D6 3E 13 40 
5B 85 33 1B 00 00 00 5A 0B BD 79 68 9D 58 79 61 41 61 A1 4C 41 02 A1 9E 9E 9E 48 A0 CD 07 AC BF 
FF FE 7B A5 0E AB 4C 26 4B 4D 4D 55 57 3D 9A C6 C8 C8 C8 C0 C0 A0 D2 44 96 6D E4 8B D6 00 B4 04 
62 1E 5A 15 65 41 4A E4 B3 17 51 F1 49 A9 D9 85 CA 92 0B B5 18 81 9E B1 B9 95 9D 93 BB BB A3 99 
AE 76 E6 FD AF BF FE AA EE 12 00 A0 55 D2 CE EF 44 D0 4A F2 8C D0 AB A7 8E 9F BB 19 12 95 94 55 
96 F1 44 C4 29 0A 32 13 23 1F 5F 3F 7B E2 CC AD A8 EC AA 8F F1 6A A5 3A 75 AA E7 D5 4E 50 05 9F 
CF F7 F5 F5 55 77 15 00 EA 84 DE 3C B4 0E F2 D4 47 17 2F 3E 4C AD F2 32 AA F2 38 59 46 E8 B5 0B 
F9 B2 C1 FD DA 1B 6B C1 93 B1 66 CF 9E AD 54 2A AF 5F BF 5E F5 23 B9 5C 9E 95 95 55 75 7A DB 24 
16 8B F5 F4 F4 AA 4E 17 89 44 13 27 4E 2C BD 5B 0F A0 6D 42 CC 43 2B C0 49 E3 EE 5D 7F 54 96 F1 
02 03 5B 17 B7 76 96 66 A6 12 5D 2E 2F 33 3D 2D 31 2A 34 22 39 9F 25 22 E2 0A E2 EF 5D 7B 6C 32 
AC 93 79 AB FF DB 36 34 34 5C BC 78 71 B5 1F 65 67 67 DF BD 7B B7 85 EB D1 58 6E 6E 6E ED DA B5 
53 77 15 00 1A AA D5 7F 15 42 1B C0 66 BD 78 14 91 53 3C 4A CF E8 5A FB 0E E8 EB 69 56 F2 F4 2D 
43 23 53 6B 07 57 57 87 E0 4B FF 0B 49 93 13 11 29 33 9E 3F 8A 74 1D E8 26 6E E0 13 D5 00 00 B4 
07 CE CD 83 C6 53 26 87 86 17 BF 9D 8A 18 DD 76 5D 7A 79 98 55 7E C2 26 A3 6B DD A9 B7 B7 65 C9 
51 AB 3C 31 34 A2 EA 0B 35 01 00 DA 1E C4 3C 68 3A 36 3B 39 25 BF A4 2B 6F E0 E8 6E AF 5F 6D 2F 
9D 67 E4 E2 66 5B F6 4E CB E4 E4 C2 FA 3D 31 15 00 40 1B 21 E6 41 D3 C9 D3 33 4A 06 EC 49 68 6E 
69 5A E3 C5 75 3A 96 96 A5 6F D7 62 33 D3 33 D0 9D 07 00 C0 B9 79 D0 70 AC 54 2A 2D 09 6C 46 DF 
C0 A0 E6 23 53 46 28 D6 17 31 54 C0 11 11 C9 A5 52 25 91 16 5C 6F 2F 97 CB 37 6F DE 7C FD FA F5 
F2 2F 74 91 CB E5 F5 7A A0 BD 76 33 34 34 D4 D7 D7 AF 3A DD D8 D8 F8 ED B7 DF C6 0D 75 D0 C6 21 
E6 41 D3 C9 E5 8A 92 1F 19 81 40 47 D5 75 75 02 9D B2 5C 97 CB E4 1C 09 95 B9 29 49 59 32 22 91 
B1 B5 B9 41 E9 87 8A D2 A9 76 E6 95 9F 9E A6 69 F6 ED DB 37 67 CE 1C 75 57 D1 5A 1D 3C 78 30 26 
26 A6 EA 5B 76 00 DA 0E C4 3C 68 3A 1D 41 69 3C 73 4A A5 82 23 AA 31 E9 95 8A B2 81 7A 1E 8F 47 
C4 16 C6 04 5F BE 9F C2 F1 2C BB 8E 19 D2 A1 E4 D2 7B B6 30 E6 FE E5 FB A9 1C CF B2 EB 84 A1 1D 
9A B1 F4 A6 70 E3 C6 0D 75 97 D0 8A 25 25 25 C5 C5 C5 39 39 39 35 A6 91 A8 A8 A8 C4 C4 44 3C 3D 
B7 B5 E0 F1 78 56 56 56 8D FC A5 6B 13 C4 3C 68 38 9E 50 24 64 88 8A 06 AC 0B F2 F2 39 AA F1 5D 
6A 8A FC 02 69 C9 C0 36 5F A4 AB B2 E3 DF 2A BD F6 DA 6B 19 19 19 05 05 05 4A A5 B2 A0 A0 40 DD 
E5 68 0A 91 48 54 BE BF 2E 16 8B E3 E3 E3 1F 3E 7C 58 F4 CF FA BE BD BE 92 FC FC FC 88 88 88 46 
D5 07 2D 2E 32 32 D2 DC DC BC EA 9B 0E DA 26 C4 3C 68 3A 1D 89 44 CC 24 E6 70 44 C4 15 A6 A7 E6 
B0 36 92 92 F3 F3 CA CC F8 04 D6 D4 D6 B4 E8 41 F6 F2 D4 94 D2 CB EE 18 43 63 23 2D BB BE 74 D4 
A8 51 33 66 CC F8 ED B7 DF D4 5D 48 2B 30 7E FC F8 59 B3 66 35 49 53 52 A9 B4 49 DA 81 16 56 58 
58 88 98 2F A2 65 DF 84 A0 85 F8 12 0B 73 51 F1 CF 5C 66 54 58 6A C9 A9 7A 2E 2F E2 FE D5 CB A7 
8F 9D 08 BA 17 95 29 67 F3 A2 43 E3 0A 4A 6F BC B3 B0 32 D4 BE 3F EE E0 E0 60 75 97 D0 3A 3C 7F 
FE 5C DD 25 00 68 0A F4 E6 41 E3 09 6C 5C 1D 0D 23 9F 15 F5 E7 73 42 6F 07 DB 0C EE 6A AB C7 B0 
39 D1 91 C9 72 22 CA 4D 7C 7A F5 4C AC A9 A1 22 43 56 BC 04 DF D4 D9 C5 AC FC 55 F6 5C 7E 52 E8 
13 45 E9 5F BB 3C B5 75 0E 78 37 72 FC B9 ED C0 8E 02 28 85 98 07 CD C7 37 73 F7 B2 8D BA 11 57 
48 44 C4 66 3D BF 72 3E DF B3 93 A7 A3 95 FB C0 97 0C 9F 3D 08 0E 89 C9 56 28 73 D3 33 4B 66 67 
0C 9D BD DB 4B 2A F4 E5 B9 DC B8 27 0F E2 5A BA 6E 00 00 B5 43 CC 43 2B C0 33 70 EE D1 23 25 FB 
4A 78 0E 4B 44 A4 C8 89 7D 78 25 F6 21 C3 17 8A 04 AC 54 AA A8 38 B3 D0 C6 D3 CB BA D2 D3 70 19 
5D 13 1B 8B B2 1B EA 94 B9 C9 09 19 52 74 F8 00 40 FB 21 E6 A1 55 60 F4 DA F5 18 D2 4F 74 E5 C6 
D3 D4 D2 78 E6 94 B2 C2 6A 5E 2E 2F 8B BF 7D F6 42 46 47 6F 0F 67 EB D2 67 E9 30 46 CE DD FB 96 
DD 50 97 1B 72 FE 78 06 AE AC 02 80 36 40 FB AE 52 82 66 C7 71 5C 61 09 99 4C 56 FB 02 4D 83 A7 
6F D7 79 E8 E8 11 BD BD 1C CC 0D 84 95 FE 70 79 42 23 2B 07 07 4B FD A2 C9 6C 41 CA 8B DB 17 CF 
DD 8A 69 B1 DA 2A 28 DD 39 B8 48 1B 00 D4 0E BD 79 A8 9F FC FC FC 6B D7 AE 95 A6 7B 56 56 56 4B 
AE 9D 27 32 75 F6 ED EB EC CB CA F3 B2 B3 F2 0A A4 52 05 27 10 E9 E9 89 0D 8D C4 42 1E 71 D2 B4 
D0 FB B7 82 23 32 E4 1C F1 24 2E 1D 6C 85 D4 E2 97 DA 71 1C 77 FD FA F5 D2 7F 8A C5 62 85 42 A1 
62 7E 00 80 66 85 98 87 FA 49 4B 4B 13 89 44 B5 CF D7 BC 78 3A 62 89 B9 58 52 69 2A 23 32 6B DF 
6B B8 AD 73 C4 8B E7 B1 4A C7 0E 26 7C 52 FB E9 F7 BC BC BC 9C 9C 1C 75 57 01 5A 28 36 36 36 32 
32 12 CF E6 23 22 89 44 E2 E1 E1 21 14 56 7E 3F 35 14 41 CC 43 FD 28 95 D5 9C 0E D7 24 7C B1 A5 
5B 67 4B B7 A2 7F A8 3D E6 89 08 5F C4 D0 E4 72 73 73 4F 9E 3C 89 FB 06 4B E5 E4 E4 F4 ED DB 57 
DD 55 68 28 C4 3C 68 33 9E 81 E7 B0 89 9E D5 4C 1D 5E 75 2A 40 AB 91 9C 9C 8C 8C 2F 2F 39 39 59 
DD 25 68 2E 5C 82 07 00 00 A0 B5 10 F3 00 00 00 5A 0B 31 0F 00 00 A0 B5 10 F3 00 00 00 5A 0B 31 
0F 00 00 A0 B5 10 F3 00 00 00 5A 0B 31 0F 00 00 A0 B5 10 F3 00 00 00 5A 0B 31 0F 00 00 A0 B5 10 
F3 00 00 00 5A 0B 31 0F 00 00 A0 B5 10 F3 00 00 00 5A 0B 31 0F 00 00 A0 B5 10 F3 00 00 00 5A 0B 
31 0F 00 00 A0 B5 10 F3 00 AD 06 C3 30 EA 2E A1 75 C0 8E 02 28 85 98 07 68 35 7C 7C 7C D4 5D 42 
EB E0 E1 E1 A1 EE 12 00 34 85 40 DD 05 00 40 5D 8D 1A 35 CA CE CE 2E 2A 2A 8A 88 4D 3B FB FD 92 
03 2F 14 55 67 62 F8 3A 22 5D 5D 5D 3D 03 13 6B 07 17 8F EE 03 07 75 B6 11 B5 7C A9 6A E4 E8 E8 
C8 E7 F3 D5 5D 05 80 A6 40 CC 03 B4 1A 0C C3 F8 FA FA FA FA FA 12 B1 91 61 6B 53 93 93 65 AA 66 
7F 12 7C E3 C2 F1 A3 E7 26 AC 3B B2 73 A6 67 9B 8A FA 27 4F 9E A8 BB 04 00 4D 81 41 7B 00 6D C6 
E5 3D DF 3F 67 FC E2 7F F3 D4 5D 08 00 A8 07 7A F3 00 AD 9D C0 63 EA F7 9F 0D 37 65 88 88 53 CA 
A4 52 A9 B4 20 23 E2 7F 7F EE 38 F2 20 43 C9 11 71 D2 A7 3B D6 1F 5C D4 7F 9A 15 8E EA 01 DA 1E 
C4 3C 40 6B C7 58 F8 06 4C 99 EA 5C F1 74 F4 FC 0F C6 CC ED 3E E2 C7 E7 0A 22 62 33 2F 9F BF 2E 
9B F6 8A AE 7A EA 03 00 35 C2 E1 3D 80 76 32 F4 1B D6 DB A4 F8 3F 38 97 9B 91 29 57 6F 39 00 A0 
1E 88 79 00 ED C4 A6 C5 C6 E7 72 45 3F F3 4C 2D 2D 85 EA 2D 07 00 D4 03 83 F6 00 5A 86 95 66 A5 
26 47 5E FA 69 DE B2 8B 85 45 31 CF 33 1B 30 A2 57 A5 4B ED 65 29 8F FE FD E7 D4 A5 87 91 89 69 
05 02 53 3B 47 27 E7 F6 BE 03 46 F4 73 35 AC 70 E8 AF 0C BF F0 FB A5 28 05 A7 6A 7D 3C B3 AE AF 
8C F1 95 F0 88 94 11 17 F7 FD 1B 21 E7 88 D1 71 E8 F7 DA 50 B7 8A 47 16 79 4F 4E 1E BC 91 A8 24 
46 E0 34 60 F2 60 97 F2 E7 18 9A BA 18 55 73 00 B4 31 88 79 80 D6 4E 71 63 99 9F FD 0F 3C 22 E2 
14 05 39 99 D9 F9 72 B6 7C 14 F2 24 03 3E FE 7C AC 69 D9 04 36 F9 DF EF DF 7F 77 C5 E1 67 D9 15 
E6 23 46 60 DA F9 F5 55 DB 36 CC EA 62 58 32 49 7A 75 E3 DB 6F 1E 29 54 99 AC 3A DD BE EE EA EF 
2B E1 11 49 AF FF F8 DE 9B 07 73 39 22 C6 B0 7F C2 B5 0B 8B 3A 96 FB 82 61 D3 CE 7C 3B EB C3 7F 
65 C4 E8 07 EE FE CF 60 17 FD 66 2C 06 77 CD 03 94 C2 A0 3D 40 6B C7 C9 B2 92 12 12 12 12 12 12 
12 53 32 F3 2A 64 3C CF D0 EB F5 1F CF 1E FE D0 A7 AC 63 2D 0B FE E1 B5 C0 CF 0E 3E AD 14 AB 44 
C4 29 D2 EF ED 7A 6F F4 D4 6D 61 CA 46 97 94 73 65 D5 C2 AD E1 B5 B6 D3 22 C5 00 B4 69 E8 CD 03 
68 23 9E A1 CF 6B 1F CD 9D F2 F2 E8 61 DD 6C 2B 5C 60 9F 71 F4 BB 1F 2E 67 B0 44 8C D0 69 F8 DC 
45 EF BD DC DD 59 42 19 61 D7 F6 7D B7 7C EB 8D 54 96 53 26 1C FF 7A D5 E9 89 5B FC C5 15 DA 63 
F4 BB CD F8 62 82 47 75 DD 64 BE 75 3F EB AA FD 05 36 FD EC D2 4F FE 08 D8 3F C5 56 45 5F A2 B9 
8A 49 A9 79 95 00 6D 0D 62 1E A0 B5 D3 E9 B1 F8 CC F6 F1 C2 A4 98 88 5B FB 57 AD DC F7 28 9B 25 
62 73 9F 5E BE 96 30 EB 5D AB 4A 37 D1 C9 42 AE DF 49 63 89 88 67 3C 76 D5 5F DF 4F 28 1E 11 F7 
ED 39 A0 8F 69 AA CF F4 43 A9 2C 29 13 83 4E DF 96 F9 0F AC 70 66 9D 11 75 F0 9F BF E8 3F F5 B9 
25 4F 99 74 64 F1 17 C7 5F DA 12 60 56 53 D0 37 5B 31 88 79 80 52 88 79 80 56 4F D7 C4 D1 B3 B3 
B3 77 E7 3E 43 5F 1E D3 B7 DD E0 97 BE BB 97 CF 71 F2 B8 33 5F FB 0F 8C DD 7F F9 97 57 2C 2B E4 
6C D1 F0 38 97 77 EF D4 3F 61 2F 4F 70 2D 3E 47 CE B3 1E FB C9 9A 15 3D 92 44 62 7D B1 A4 A3 53 
53 9C DD E6 14 D1 BB 3F FE EF 94 C1 EB 87 18 D6 3C 4F 4B 15 03 D0 56 21 E6 01 B4 89 71 FF A5 3B 
BE BC 36 E8 F3 4B 99 2C 11 27 7D B1 7D CE FC A1 3D F6 4C 28 1D 39 17 76 1A D0 C7 62 ED F3 04 96 
93 BF D8 39 C9 E7 DC DA 51 01 A3 46 8C F0 1F 33 B2 87 9D 61 CF A9 9F F6 6C A8 B6 C1 87 00 00 20 
00 49 44 41 54 9A 2A F8 D6 0E 36 19 31 B1 52 4E FE 7C F3 C2 6F 27 5F 5D DE AB DA 9E 77 CB 14 03 
D0 B6 E1 12 3C 00 ED 22 F2 5D B0 6D D5 A8 E2 0E 3C A7 8C 3D F0 E1 DC 3D 51 65 97 B1 19 F8 7F BA 
64 84 45 F1 A7 05 B1 37 0E 6D FA FA DD B1 BD 9C 6D DA 0F 9C B4 68 DD C1 9B 71 85 D5 B7 CA 65 1E 
98 62 26 AE 86 71 F7 2F 6E 55 79 4F 1E CF 79 C6 8A F7 3C 85 0C 11 57 78 7F FD 82 75 0F 6B 78 C5 
4E 8B 14 03 D0 B6 21 E6 01 B4 8D C0 6D E6 4F DF BE 52 F2 04 7B 65 C2 91 45 F3 76 95 05 BD A0 C3 
BB BF 9F F8 79 66 4F 4B 1D A6 74 09 4E 9E 19 7A E9 8F EF 3F 1C DF DB DD 27 F0 87 AB 19 6C 95 36 
39 4E 21 CD AF 9E 8C AD 3A 3B E9 F5 5E F2 FD 0C 17 01 43 C4 E5 5E FB 76 C1 96 D0 EA AF 97 6F 99 
62 00 DA 32 0C DA 03 68 1F BE E3 D4 B5 AB FE BE FA D6 91 24 96 88 D8 E4 E3 5F 7C BC FF A5 7D 93 
8B 87 EE 79 26 3D DE DE 72 F5 D5 0F CF 1C 3C 70 F8 E8 F1 93 41 C1 71 B9 25 CF 9C E1 F2 43 FF FE 
64 FC 5B 66 57 FF 9C 5E F1 8C 38 23 30 73 ED E4 64 CC 54 5E 13 F1 9D ED C4 D5 F5 16 78 92 91 4B 
57 4D 38 3E 71 6F 9C 92 CD 3C BF EC E3 BD 2F AF A9 B6 D4 16 29 06 A0 0D 43 CC 03 68 23 BE E3 D4 
75 2B FF BE 36 B3 28 E8 95 F1 07 3F FD EC C8 B0 1D E3 CA 2E C6 E3 9B 78 8D 9A F9 F5 A8 99 5F 53 
61 42 70 D0 E9 93 27 FE D8 BA F3 4C 58 1E 47 9C 32 F1 F8 9A AD F7 A6 2F EF 5E BE 3D C6 70 E4 AA 
2B 7B EB 75 A5 3D CF 6A DC 37 CB 03 CE BE 75 38 99 65 93 8E 2E 59 32 6C 46 8D 0F B6 69 FE 62 00 
DA 2C 1C F9 02 68 27 BE E3 B4 75 2B 03 4A 87 EE 63 7F FF F8 8B 53 69 E9 F7 0E FD B2 66 C5 17 8B 
E6 BC F3 F5 91 B8 E2 E1 6D 5D 1B DF 51 D3 3F FD F1 54 F0 D5 15 03 0C 18 22 E2 14 CF EF DE 6B 8A 
12 78 0E 53 57 7D 31 44 C2 23 22 65 EC BE AF 7F 7D 52 61 E4 9E 6D D9 62 00 DA 28 F4 E6 01 5A 23 
9E D3 87 FF 4A 3F 54 3D 0F DF 71 C6 E1 C4 19 15 26 A5 6C DD B6 60 E1 3F F9 1C 31 46 A1 5D DE 0C 
78 D7 A1 DC 81 BE B8 43 CF 8E A6 BC 4B B9 4A E2 D8 DC EC A6 29 93 EF FE F6 9A 4F F6 F5 F9 FC 6A 
2E A7 48 49 4A AD B8 09 CA 3B 2D 5B 0C 40 9B 84 DE 3C 40 1B 62 3A 74 44 2F 3D 86 88 B8 EC B3 5F 
BF F7 E3 83 9C B2 8F 72 EF 6F FA EE AF 58 25 11 11 DF C2 DD A3 A9 D6 28 F4 99 BF 66 5E 67 DD AA 
E7 D1 D5 51 0C 40 1B 84 DE 3C 40 1B C2 77 7C FD 83 D7 7E B8 B4 33 46 49 6C D2 3F F3 7B 79 FD FE 
D2 D0 EE 2E 66 94 11 71 E7 EC C9 1B B1 85 1C 11 31 3A 9E D3 66 0D AF B4 20 97 7B F6 F3 01 BD BE 
AB 2E AD 49 A7 CB FC BF 36 4D AA E6 81 B7 45 74 7B 7D B2 E6 DD 83 23 D6 3F 95 57 3A 37 DF 7C C5 
E0 15 75 00 A5 10 F3 00 6D 09 CF 2C E0 BB 6D 1F 3E 1C BF E6 4E 36 4B 5C 61 EC 8D 23 BB 6E 54 9C 
C1 A4 FF D7 DB 16 F7 AA FC 76 7A 4E 9E F2 FC 56 0D CF 90 15 EA 26 D6 70 83 7B 31 C3 41 5F AC 9E 
F6 77 E0 F6 88 4A 2F 91 6D B6 62 10 F3 00 A5 10 F3 D0 28 0C 53 6D 97 AA ED E2 F1 9A F1 44 18 CB 
B2 F7 EE DD 8B 89 89 69 64 3B 7D 17 FC 57 71 E0 F7 23 41 F7 A2 B2 CA F5 B0 79 7A 16 AE DD 86 8E 
9F 3C D6 2B FE DC 91 23 45 D3 A4 B7 E3 95 2A 5F FC 4A 44 C4 A6 3E 3C 73 E4 88 25 8F A4 B7 E2 8A 
82 9C 4D 7F 7C F6 C8 91 47 E5 EE 82 63 FB 8E F3 3B BC F6 72 3A 4B C4 29 E2 6F 1F 3B 22 16 35 57 
31 71 4F 82 39 C7 CA 47 06 00 6D 16 62 1E 1A C5 C8 C8 48 22 91 64 66 66 AA BB 10 4D E1 E8 E8 D8 
7C 8D 9F 3E 7D 7A F7 EE DD 4D D3 96 9E AB 9F BF 73 8F C2 FC 9C 9C 9C 7C 19 27 10 EA EA 19 1A 1B 
E9 F2 29 F2 F2 DF 91 E5 E7 73 1D 3F C9 B5 D6 C6 0A 2F FE 75 80 88 88 9C 03 27 39 13 11 91 E2 F2 
C1 03 95 66 6A 37 72 C2 A4 E2 1F 93 FE 3E 50 E1 D3 26 2D 26 7E CF EA AB 83 07 D7 3A 1B 40 1B 81 
98 87 C6 1A 37 6E 5C 64 64 A4 4C 56 C3 F3 4C DB 12 7D 7D 7D 7B 7B FB E6 6B FF C1 83 07 4D DA 1E 
4F A0 6B 60 A2 6B 60 D2 A4 8D 36 54 53 16 F3 E2 C5 8B A6 68 06 40 1B 20 E6 A1 B1 04 02 81 9B 9B 
9B BA AB 68 13 38 AE D6 31 6B 20 C2 8E 02 28 07 37 D4 01 00 00 68 2D C4 3C 00 00 80 D6 C2 A0 3D 
D4 CF B4 69 D3 D4 5D 02 00 00 D4 15 7A F3 50 3B 81 40 D0 AC F7 89 B5 11 22 91 A8 F6 99 00 00 9A 
14 BE BB A1 76 FA FA FA 81 81 81 EA AE A2 D5 9B 3A 75 AA BA 4B 00 80 36 07 83 F6 50 27 7F FD F5 
D7 D5 AB 57 53 53 53 63 62 62 70 97 7C BD 58 59 59 59 5A 5A DA DB DB 77 E9 D2 45 DD B5 00 40 9B 
83 98 87 BA EA D3 A7 0F 11 85 84 84 24 25 25 A9 BB 96 D6 C4 D9 D9 B9 59 9F 99 03 00 A0 02 06 ED 
01 00 00 B4 16 62 1E 00 00 40 6B 21 E6 01 00 00 B4 16 62 1E 00 00 40 6B 21 E6 01 00 00 B4 16 62 
1E 00 00 40 6B 21 E6 01 00 00 B4 16 62 1E 00 00 40 6B 21 E6 01 00 00 B4 16 62 1E 00 00 40 6B 21 
E6 01 00 00 B4 16 62 1E 00 00 40 6B E1 D5 35 00 00 D5 1B 36 6C 98 81 81 41 6E 6E AE BA 0B A9 CC 
D0 D0 D0 D6 D6 56 DD 55 68 90 E8 E8 E8 05 0B 16 94 9F 22 16 8B 85 42 A1 BA EA 29 EF FC F9 F3 EA 
2D 00 31 0F 00 50 86 61 98 D2 9F EF DE BD AB C6 4A 54 B0 B7 B7 47 CC 97 97 9F 9F AF B1 BF AC 52 
E5 FF B4 5A 12 06 ED 01 00 CA F8 F8 F8 18 1A 1A AA BB 8A 5A A4 A5 A5 71 1C A7 EE 2A 34 48 AB 78 
3B F6 E0 C1 83 D5 B2 5E F4 E6 01 00 CA 18 18 18 DC B8 71 63 CF 9E 3D F9 F9 F9 44 54 58 58 98 92 
92 A2 EE A2 AA A1 50 28 78 3C F4 D3 88 88 38 8E 73 73 73 73 73 73 2B 3F D1 DC DC 5C 4F 4F 4F 5D 
25 55 D5 B9 73 E7 49 93 26 A9 65 D5 88 79 80 D6 21 35 35 35 3F 3F 5F 2A 95 AA BB 90 56 A0 28 A1 
1B CC D3 D3 73 C5 8A 15 45 3F 67 64 64 04 07 07 37 45 51 D0 A2 BC BD BD CD CD CD D5 5D 85 46 40 
CC 03 B4 0E B7 6E DD BA 75 EB 96 BA AB 00 80 56 06 63 3E 00 1A 4D 5F 5F 5F DD 25 B4 62 0C C3 60 
07 42 1B 87 98 07 D0 68 93 27 4F 36 35 35 55 77 15 AD D5 7F FE F3 1F 2B 2B 2B 75 57 01 A0 4E 18 
B4 07 D0 68 5D BA 74 89 89 89 79 F4 E8 51 D1 95 D5 21 21 21 8D 3C F1 AC F5 9C 9C 9C CC CC CC 88 
C8 C8 C8 C8 D3 D3 53 DD E5 00 A8 19 62 1E 40 D3 E9 EB EB F7 EC D9 B3 E8 67 3E 9F AF 81 4F 6B D1 
28 1E 1E 1E D6 D6 D6 EA AE 02 40 53 60 D0 1E 00 00 40 6B 21 E6 01 00 00 B4 16 62 1E 00 00 40 6B 
21 E6 01 00 00 B4 16 62 1E 00 00 40 6B 21 E6 01 00 00 B4 16 62 1E 00 00 40 6B 21 E6 01 00 00 B4 
16 62 1E 00 00 40 6B 21 E6 01 00 00 B4 16 62 1E 00 00 40 6B 21 E6 01 00 00 B4 16 62 1E 00 00 40 
6B 21 E6 01 00 00 B4 16 62 1E 00 00 40 6B 21 E6 01 00 00 B4 16 62 1E 00 00 40 6B 21 E6 01 00 00 
B4 16 62 1E 00 00 40 6B 09 D4 5D 00 00 B4 51 05 05 05 BB 77 EF 7E F2 E4 49 D3 36 AB AF AF 2F 14 
0A 9B AA 35 B9 5C 9E 97 97 D7 54 AD 95 12 0A 85 2F BD F4 D2 90 21 43 9A BC 65 80 4A 10 F3 00 65 
62 63 63 83 82 82 A4 52 69 13 B6 29 91 48 8C 8D 8D 9B AA B5 84 84 04 99 4C D6 54 AD 95 62 18 A6 
63 C7 8E BD 7A F5 62 18 A6 C9 1B AF C9 A9 53 A7 F6 ED DB D7 62 AB D3 34 77 EE DC F1 F1 F1 B1 B0 
B0 50 77 21 A0 E5 10 F3 00 C5 64 32 D9 DC B9 73 33 33 33 D5 5D 88 DA 2C 5E BC 78 D8 B0 61 2D B6 
BA 88 88 88 16 5B 97 06 62 59 36 26 26 06 31 0F CD 0D 31 0F 50 2C 22 22 A2 2D 67 3C 11 05 07 07 
B7 64 CC 97 E7 E1 E1 D1 E4 6D 76 EF DE DD DA DA BA 91 8D A4 A4 A4 DC B8 71 A3 49 EA 29 A2 54 2A 
5F BC 78 D1 84 0D 02 A8 86 98 07 28 C6 71 5C E9 CF 12 89 84 CF E7 37 6D FB A6 A6 A6 E6 E6 E6 8D 
6C 24 37 37 37 3D 3D BD B0 B0 B0 49 4A 2A 92 96 96 56 F4 43 F9 3D D0 92 7C 7C 7C BC BD BD 9B BC 
D9 51 A3 46 4D 9E 3C B9 91 8D 1C 3C 78 B0 69 CF 92 28 14 0A C4 3C B4 24 C4 3C 40 35 06 0D 1A A4 
A7 A7 D7 B4 6D 5A 5A 5A AE 5B B7 AE 91 8D 5C BB 76 6D E3 C6 8D 4D 52 4F A9 4B 97 2E C5 C5 C5 35 
6D 9B 9A A0 49 8E 5A D4 75 E8 03 D0 54 70 43 1D 40 0B 51 28 14 8D 6F 04 A9 03 00 F5 82 98 07 00 
00 D0 5A 88 79 00 00 00 AD 85 98 07 00 00 D0 5A 88 79 00 00 00 AD 85 98 07 00 00 D0 5A 88 79 00 
00 00 AD 85 98 07 00 00 D0 5A 88 79 00 00 00 AD 85 98 07 00 00 D0 5A 88 79 00 00 00 AD 85 98 07 
00 00 D0 5A 88 79 00 00 00 AD 85 98 07 00 00 D0 5A 88 79 00 00 00 AD 85 98 07 00 00 D0 5A 88 79 
00 00 00 AD 85 98 07 00 00 D0 5A 02 75 17 00 00 50 6F 9C 52 56 58 50 50 28 E7 F8 42 5D 7D 3D 5D 
01 3A 2C 00 35 40 CC 03 40 EB C1 16 A6 45 3E 7D 16 11 97 98 9A 2D 65 8B A7 31 3A 06 A6 56 ED 5C 
3C 3A 38 5B E8 F3 D5 5A 1D 80 06 42 CC 03 40 EB A0 CC 8D BE 77 E5 56 68 BA 8C AB 38 9D 93 E7 A6 
C5 3E 4D 8B 0B 0F 6F DF B3 4F 67 7B 03 44 3D 40 39 18 EA 02 68 66 9C 52 56 90 9B 95 91 91 96 99 
93 92 A7 54 77 35 AD 15 9B 13 7E ED FC D5 17 55 32 BE 0C 27 4B 7B 7E ED F2 83 64 59 4B 96 05 A0 
F1 D0 9B 07 68 26 D5 8C 2F EF 3F 70 CC A5 DB E0 B1 6F 7E F8 E1 1B FD EC 84 EA 2D AF 55 51 A6 87 
5C BF 1B 93 5F 1C F1 8C 8E 89 93 A7 7B 3B 4B 53 89 2E 97 9F 91 18 F1 F4 69 44 5A 21 47 44 A4 CC 
7C 76 E3 81 CD 4B DD AD 75 D4 59 2D 80 26 41 6F FE FF ED DD 79 60 13 65 DE 07 F0 DF 4C 9A 34 4D 
93 36 E9 91 9E F4 A2 85 52 0A E5 92 A3 40 A1 1C 2B 2C 2A 28 A2 2B 2B AB AB 0B 2E 8A BA 28 DE FA 
AA BB E2 BA AE EC AE 22 C7 AA AC B0 2A C7 0A 78 22 72 C8 8D 50 28 16 0A 14 28 47 5B 0A A5 77 D2 
36 25 69 8E 99 F7 8F 24 BD 48 21 6D A7 4D 3B FD 7E FE 6A D3 CC 93 E7 99 74 E6 3B CF 33 CF CC 00 
74 00 9B E1 72 E6 8E 2D 3B 32 CE 14 94 36 9C 43 26 E2 2D BA 8B 87 37 2F 79 6C FC C0 B4 3F 6D BE 
84 6E A7 9B 78 63 C1 A9 33 E5 16 FB 2F 8C 32 7A C4 E4 49 23 FB C7 45 06 AB 95 2A 8D 36 AA DF 88 
89 13 86 86 FB 30 8E F7 1A F2 73 0B 4D 2D 76 F9 01 7A 1C C4 3C 80 D0 6E 39 BE CC DB 2A 33 3E 78 
F0 9E 57 F7 56 75 6A BD BA 2B AE FA D2 85 6B CE 90 57 C4 0E 19 16 EB DF 74 18 52 E2 1F 3F 2C 49 
2B 21 56 A6 0C 08 8B 89 D3 2A 6C 88 79 00 27 0C DA 03 08 AB C5 F1 65 3F 89 65 58 60 D5 92 FF 66 
14 5B 78 22 DE 98 FD AF 3F BC 72 7B D6 87 13 95 9E AD 6F 97 C7 D7 95 96 EA 1D 23 22 8C 6F 64 6C 
A8 8B B3 1D 8C 22 66 C4 94 08 99 CA 57 CA 74 6E E5 00 BA 3C C4 3C 80 90 6E 18 5F 4E 1B EE EC 7B 
06 04 04 2C FA 70 C9 AF 87 CD 9A BC E0 FB 22 1B 4F BC E5 E2 17 4B 37 FE 5F FA C3 5A 0C AA DD 0C 
57 59 A9 77 CE 5C 94 A8 03 D4 2E 27 D2 33 32 5F BF 9B 4E 76 B0 E9 2F 1C F8 F1 87 3D C7 2F 15 97 
EB EB 58 55 60 58 EC C0 31 BF BA 7D 5C B2 B6 F9 52 E6 DC 9D EB 0F 16 DA 88 18 79 FC 84 FB C6 36 
AB 4B 6D E9 E5 12 03 47 44 8C 22 38 2A 54 25 E1 EB 2A AE 5C AD B2 DC B2 11 32 4D 64 84 46 86 63 
10 F0 00 C4 3C 80 80 6E 39 BE EC 9D 34 6F E9 4B 1B 77 3D BD DF 3B 6E D0 C8 D4 31 E3 23 EB 30 BE 
7C 0B B6 3A 93 73 16 03 23 93 FB B4 FE 72 39 73 DE 77 8B 9F 5E F4 CF 2D E7 6B B8 A6 2B 9B F1 8D 
9D 34 F7 CD F7 FE 3C 67 A0 AA E1 C5 DA 7D FF 98 37 77 6B 1D 11 1B FC BB CD 77 37 8F F9 8A 0B 99 
19 05 16 22 62 C3 86 47 84 AA 24 9C A1 F0 E4 91 33 55 B7 FA 12 19 4D B2 26 1C 31 0F 1E 81 98 07 
10 8E 3B E3 CB 6C AF 39 AB 8E 4D 0F 4C E8 A5 44 27 DE 1D BC D9 DC D0 59 96 78 B5 36 E5 EB 4E AD 
BC FF D7 4F 7D 5B 68 71 91 C4 7C 6D DE 8E F7 1F 1E 7F E8 D4 BA 6F FE 7A 7B 08 BE 0E 10 27 C4 3C 
80 70 DC 1A 5F 66 FD A3 FB FA 37 7F B1 E6 E4 96 CD 99 A5 1C 11 EB DF 7F EA 3D C3 B5 44 64 CB DB 
B3 6E 6F 9E 85 27 46 1A 35 E6 BE 89 F1 4D 0F 19 AC 55 45 85 15 26 9E 88 F1 D5 46 87 88 F9 90 C1 
4B D2 B0 1A 6D D6 D6 DD 78 A0 66 F7 CB 0F 2C 74 64 3C 23 0D 1B 3D 67 EE CC B4 21 FD B5 E6 BC 93 
47 77 AE FD 78 53 B6 9E E3 39 5D C6 7B 0F CD 8D 3F F0 D5 DC F8 36 DC 57 87 55 84 27 A6 78 D7 39 
7F E5 F4 F9 27 F3 F5 3C 11 31 EA 98 01 31 6A E7 D7 C2 C8 83 7C D0 95 07 CF 40 CC 03 08 A7 CD E3 
CB B6 A2 2D 6F CD 7D E9 B0 85 C8 AB CF C2 DD F6 98 AF 3B FC E1 FC 47 36 19 78 22 46 35 F6 DA A1 
5D CF F5 6F B4 B9 F2 75 D7 72 8E 64 95 71 44 92 C8 51 BD C4 1C F3 8C 97 4C C6 12 71 44 44 BC D9 
64 E2 88 DC 5D AF B6 82 7F 3E BF E2 B4 89 27 22 56 93 FA DC DA FF BD 35 25 C2 B1 0E A7 CE 9A B7 
E0 91 B5 0B EE 79 74 F5 19 13 CF 95 FC F0 E6 E2 EF 67 7D 3A 5D DD FA DA F9 68 E3 FA 69 EB 7F B5 
E6 97 9F CE D7 DB 88 88 55 86 F5 ED 17 83 1D 2C 78 9E 78 77 0E 00 9D AE 9D E3 CB 2D 97 5B 73 F0 
9D 45 9F 5C EA A1 77 D0 63 D5 FE 7E CE 9E B0 AD 4A 57 E5 72 35 70 FA 33 FB 76 EE CF 3C 7D A9 B8 
E1 3E 05 D6 F3 FB 3E 3B 6E E4 89 88 51 A6 BD B6 66 71 7D C6 DB 29 12 67 7F B0 F4 91 38 2F 22 22 
5B D1 A6 55 5F 95 71 CD 4B 05 10 01 C4 3C 80 70 DA 33 BE 7C 53 5C E5 8E 37 5F 58 5F D4 23 73 88 
55 6A 83 7C EB 6F 7E 73 B5 B0 D2 C5 7A B5 E9 0A F2 8A CA AE 9C CF CE D8 BD E5 9B 6D D9 E5 36 22 
E2 8A CF 9E 2F B0 12 11 31 BE E9 73 1E 74 35 24 AF 1C 3B 7B BA 3D E7 F9 DA 23 FB 0E 9B 3A AC 0D 
00 9E 83 98 07 10 8C 63 7C 99 88 EA C7 97 05 63 2B F9 FA 95 D7 BE AF E8 89 41 2F 09 8A AB 3F CB 
CD D7 5C C8 3A 55 DA EC F2 04 6B 45 CE B1 5C E7 6C 77 46 1E A4 D5 48 88 88 2F 2B 2D B7 1F 11 78 
25 0C 1E AA 71 59 B4 2C 39 A5 9F 7D FE 3B A7 BB 74 BE B4 27 AE 5D 10 3D C4 3C 80 70 DC 1A 5F B6 
9E 7C EF EE B4 99 0B DE 5E B3 F3 6C A5 D5 FD B2 79 EB E5 CF 9E FF 73 56 AD 00 D5 EC 6E 58 75 7C 
FF 28 85 63 C5 5A 2B 72 F6 6C 3F 70 BA B0 BC BA CE 46 36 B3 A1 EC E2 B1 3D FB 4E 57 38 D7 A4 24 
28 A1 AF 56 42 44 C4 1B AF 3B 0E 07 18 75 50 50 0B BB 3A 6F B5 46 69 2F 98 AF D2 E9 10 F3 20 42 
98 21 02 20 1C FB F8 72 A5 81 27 C7 F8 F2 C0 A0 E0 E6 63 C5 E6 AC F5 FF DD 72 E0 A4 65 FF E6 65 
AF FA A4 BC B2 FB C8 5F 46 DC E2 21 36 92 D0 A8 30 5D E1 95 3A DE 92 FB D1 D2 2F 92 07 BB 3F 07 
4D 2C 18 9F C8 21 C3 13 74 FB 73 ED 07 4E 36 C3 95 EC 03 57 B2 89 61 19 BE E9 A5 F0 5E 41 49 43 
FA F8 39 22 5D 22 75 AC 27 DE 6A 6E F1 C6 C3 96 FA E9 14 5E 52 29 11 11 C3 D6 0F C8 DC BC 4E 0C 
A6 CE 43 B7 80 DE 3C 80 80 6E 39 BE 5C 7B E4 AF 4F 2D 3D E5 B8 88 9B 0D 19 3D 3E E5 D6 0F AA 63 
63 7F BF 78 7E 3F 19 43 C4 9B CE FF 6F E9 39 7D 0F 9C 8B C7 78 87 0D 49 1F 9B 14 E4 DD 38 5B 9B 
66 3C 23 0B 4C 4C 1D 9D 14 E0 3C 06 62 54 4A 5F FB 57 C1 17 5D BE EC 7A 9D 71 65 57 AE D5 F2 44 
44 0C EB E8 F2 CB E4 32 89 FD 43 AC 96 1B 07 5B EA 3F 92 61 59 C4 3C 74 0B 88 79 00 21 DD 64 7C 
B9 AE BA 68 D5 53 53 67 2C CE A8 71 0C 25 FB A6 3E F1 E4 78 B9 3B A5 FA 8C 7C F5 BD DF C7 79 31 
44 64 3C 95 93 75 B1 A6 27 8E 2E 33 3E 61 29 13 A7 4E BA AD 4F B8 BA D9 A5 8A 8C CC 3F 22 69 E4 
E4 69 93 06 47 28 1A 76 69 92 C8 E8 08 39 43 44 64 BD BC 6F 57 8E CB F3 23 15 7B F7 9D B0 F2 44 
44 92 E8 41 83 02 58 22 62 E5 0A FB 52 BC D9 68 6C BE 10 67 B3 39 56 3D 2B 91 20 E6 A1 5B C0 A0 
3D 80 A0 6E 3A BE FC 49 C3 FB 58 BF 51 2F FE 73 41 A2 9B C3 EF AC FA F6 37 DF B9 FF FB DF 7C 71 
D5 C6 5B 8A 4F 1F 2F 88 18 DC 11 95 EF F2 58 9F A0 F8 A1 E3 E2 87 58 AF 57 E9 6B AE 9B CC BC 54 
A1 54 AA 94 BE 32 57 EB 51 99 94 3C 5C 71 62 77 2D 4F 96 93 AB DE DE F0 87 F5 BF ED D5 B4 5F 63 
CA 5C FE C1 8F F6 13 F2 92 F0 F4 C9 83 64 44 44 6C 78 B8 96 A5 4A 8E 78 F3 B9 93 A7 CC B7 35 5E 
80 AB 35 18 1D C7 68 72 1F B7 0E D0 00 3C 0E BD 79 00 81 DD 7A 7C 99 F1 0A 18 B1 70 ED 97 2F 0D 
6D 45 52 B0 21 F7 BC FD D6 5D 5A 86 88 C8 74 35 3B FB 5A 4F 7E 5A 3D E3 A5 50 07 85 84 47 F6 8A 
08 09 F4 77 9D F1 44 C4 6A C7 2C B8 27 4C 42 44 64 BB BA 69 FE D4 87 3F CA 6A 78 F2 AF AD 68 C7 
1B 77 CD 58 9C 79 9D 27 22 46 3E 74 DE 13 13 15 44 44 24 EB DD 2F C1 7E C7 3A EB B9 55 2F 2F 39 
DF 30 E7 D1 56 53 90 93 A7 B7 7F 8F 12 95 9F 0A 7B 4F E8 16 D0 9B 07 10 9E 7D 7C 39 E2 52 CE E9 
F3 85 25 7A 63 FD 79 61 86 91 6A FA 4F 7B EC F9 57 16 CE 1E 76 C3 DC BC 5B 61 A3 E6 BC F3 DA B2 
1D 4F 65 1A 78 FE 7A C1 C9 8B 52 3C F4 E6 56 94 33 FE FA AF D9 FB 7F FB 79 BE 85 E7 6A 4E 7F F6 
C7 D1 DB DF 1F 32 6C 50 A2 D6 5C 70 F2 97 CC 13 79 7A FB 1C 09 D6 3F F5 D5 65 0B 07 38 77 86 81 
77 CC B9 33 F4 87 B5 D7 6C C4 55 EC 7E E5 B5 5F A2 B5 FE AC 42 CA D5 55 97 17 97 19 1C A3 F8 B2 
D0 A8 70 6F 0C DA 43 B7 80 98 07 E8 18 37 8C 2F 07 84 46 2C FD 78 75 2F F5 AD E7 DC B5 44 D2 67 
DE 53 0F FE FB 91 95 A7 AC C4 37 DC 56 17 5A C6 46 CC 5A F1 6D 95 ED DE A7 D7 E5 5E E7 89 37 96 
E4 1C DC 92 73 B0 D1 1B 18 2F 6D DA 8B 5F AC 7F 69 98 A2 61 99 E0 99 EF 2E 7B F4 F8 83 1F E7 5C 
E7 89 B7 55 E5 5F AB 6A 56 A8 6F D4 E0 41 D1 B8 47 3D 74 13 18 76 02 E8 50 0D E3 CB 91 21 01 ED 
C9 78 22 22 92 F5 BE EF A9 3E 1A 6C B6 AD E0 3B E0 0F 9F 65 64 6C 78 63 76 6A 9C 5A DA 10 CD 0C 
AB 08 4B 99 F6 E4 F2 5D 59 DB FF 32 29 B4 E9 1A 65 23 EE 5E B1 E7 A7 15 8F 4F 4E 0C F2 6E F2 07 
46 AA 0C 8E 1B 3C 6E E2 A8 38 8C D8 43 B7 81 DE 3C 40 B7 E2 DD 3F 69 70 42 E1 EE 73 35 18 B2 77 
1B AB 4E 9E F5 FA 17 B3 5E B7 EA 0B CE E4 16 96 E9 8C AC 2A 28 2C AE 6F DF 50 45 CB 8B 04 8F 7C 
6C D9 F6 C7 3E A8 F9 F7 07 CB 37 6D D9 66 B1 91 44 EA AD 50 F9 29 BD 6F 7A AE C5 2B 66 EC 7D 31 
42 57 1F A0 5D 10 F3 00 DD 8C 34 A4 FF E0 D8 2B FB 2F D5 22 E8 5B C9 4B 1D 3D 60 78 74 6B 96 90 
A8 02 7B F5 0E D2 86 76 54 8D 00 3A 1E 46 9E 00 BA 1D EF F0 81 29 51 38 37 0C 00 6E 40 6F 1E A0 
0B 90 F4 7D F1 90 F9 C5 A6 AF 29 1E D8 58 F3 80 EB B7 33 3E D1 A9 33 A2 53 3B BE 5E 00 D0 DD A1 
37 0F 00 00 20 5A 88 79 00 00 00 D1 42 CC 03 00 00 88 16 62 1E 00 00 40 B4 10 F3 00 00 00 A2 85 
98 07 00 00 10 2D C4 3C 00 00 80 68 21 E6 01 00 00 44 0B 31 0F 00 00 20 5A 88 79 00 00 00 D1 42 
CC 03 00 00 88 16 62 1E 00 00 40 B4 10 F3 00 E0 61 0C D3 21 4F DB F3 F6 F6 6E 7F 21 32 99 AC FD 
85 00 78 10 9E 50 07 D0 49 58 56 80 A3 EA 0E 4A 44 CF CA CE CE 36 99 4C 56 AB 55 C0 32 25 12 09 
CF F3 5B B7 6E 6D 67 39 06 83 E1 F8 F1 E3 02 D6 CD 66 B3 09 55 14 80 3B 10 F3 00 2E 94 96 96 0A 
9B 3A 44 E4 ED ED BD 6A D5 AA 76 16 92 9F 9F 5F 56 56 56 5D 5D 2D 48 95 EC AE 5E BD 2A 60 69 6D 
93 9B 9B 2B 78 99 E7 CF 9F 17 BC 4C 80 6E 07 31 0F E0 C2 CF 3F FF 2C 78 99 47 8E 1C 59 BD 7A B5 
E0 C5 76 5F BD 7B F7 F6 74 15 3C 49 2A 95 C6 C5 C5 79 BA 16 20 7E 88 79 00 87 C0 C0 40 86 61 78 
9E F7 74 45 3C 26 24 24 A4 33 3F 6E EA D4 A9 16 8B 25 3B 3B 5B D8 75 AE 52 A9 E4 72 B9 50 A5 99 
CD E6 AA AA 2A A1 4A AB 27 93 C9 26 4F 9E AC 56 AB 05 2F 19 A0 99 1E BD 53 83 36 38 73 E6 4C 49 
49 89 A7 6B D1 51 36 6C D8 F0 F5 D7 5F 9B 4C 26 01 CB 94 48 24 12 89 44 A8 D2 2C 16 4B 47 6C B3 
0C C3 F4 EB D7 6F D1 A2 45 1A 8D 46 F0 C2 3B 59 62 62 62 68 68 A8 50 A5 E9 74 BA 13 27 4E 08 55 
1A 74 9A E4 E4 E4 A0 A0 20 4F D7 A2 4B 40 6F 1E A0 C1 FD F7 DF 7F FF FD F7 0B 5B 66 6C 6C 6C 74 
74 B4 50 A5 65 66 66 1A 0C 06 A1 4A 03 00 D1 C3 05 75 00 00 00 A2 85 98 07 00 00 10 2D C4 3C 00 
00 80 68 21 E6 01 00 00 44 0B 31 0F 00 00 20 5A 98 69 0F 00 1E B0 69 D3 A6 CD 9B 37 1B 8D 46 C1 
4B F6 F2 F2 12 F0 0A 46 8E E3 2C 16 8B 50 A5 D9 25 26 26 3E F5 D4 53 02 5E F5 07 70 13 88 79 00 
22 22 BD 5E FF E5 97 5F 5E B9 72 45 F0 92 15 0A 85 AF AF AF 50 A5 E9 74 3A C1 EF C2 EB E7 E7 37 
63 C6 8C CE BC 27 5D 55 55 D5 CA 95 2B 05 6F 48 77 71 E8 D0 A1 F0 F0 F0 05 0B 16 78 BA 22 D0 23 
20 E6 01 88 88 D6 AC 59 F3 F5 D7 5F 7B BA 16 1E 93 99 99 B9 76 ED DA 4E 7B 2E 4E 45 45 45 8F CD 
78 BB D2 D2 52 4F 57 01 7A 0A C4 3C 00 11 D1 D9 B3 67 3D 5D 05 4F 2A 2E 2E 36 18 0C 2A 95 AA F3 
3F FA B6 DB 6E 13 BC 4C AD 56 3B 7D FA F4 F6 97 B3 71 E3 46 9D 4E D7 FE 72 EA 1D 3D 7A 54 C0 D2 
00 DC 81 98 07 68 2E 36 36 56 D8 02 59 96 1D 31 62 84 54 2A 6D 67 39 A5 A5 A5 C2 1E 8E 94 95 95 
79 FC 9E 7A 11 11 11 3E 3E 3E C2 96 29 93 C9 E6 CE 9D DB CE 42 AC 56 EB EE DD BB 03 03 03 05 A9 
92 5D 79 79 79 5E 5E 9E 80 05 02 DC 12 62 1E A0 89 B4 B4 B4 88 88 08 C1 8B 7D F6 D9 67 87 0C 19 
D2 CE 42 56 AE 5C 29 EC C3 4E CA CA CA 76 EE DC 29 60 81 00 D0 D5 E0 82 3A 80 CE 20 C8 F3 66 F0 
A0 29 00 68 2D C4 3C 00 00 80 68 21 E6 01 00 00 44 0B 31 0F 00 00 20 5A 88 79 00 00 00 D1 42 CC 
03 00 00 88 16 62 1E 00 00 40 B4 10 F3 00 00 00 A2 85 98 07 00 00 10 2D C4 3C 00 00 80 68 21 E6 
01 00 00 44 0B 31 0F 00 00 20 5A 88 79 00 00 00 D1 42 CC 03 00 00 88 16 62 1E 00 00 40 B4 10 F3 
00 00 00 A2 E5 E5 E9 0A 00 00 B8 87 B7 99 4D 46 A3 C9 C2 4B 64 72 85 8F DC 0B BD 14 80 5B 43 CC 
03 40 17 C7 99 2A F2 CF 9E CB BB 5A 5C 5E 5D C7 39 5E 63 A4 CA 80 90 C8 B8 C4 BE B1 C1 0A 89 47 
6B 07 D0 B5 21 E6 01 A0 0B B3 19 2E 67 1D 3C 7A A1 D2 CC 37 7D 9D B7 18 2A AE 9C AD B8 7A E9 52 
C2 F0 D4 41 BD 94 88 7A 00 D7 30 EA 05 00 5D 15 57 73 E9 D0 4F 3F 9F BF 21 E3 1B F0 E6 8A DC 43 
FB B3 4B CD 9D 59 2D 80 EE 04 BD 79 00 E8 9A 6C 95 67 0E FF 52 78 DD 11 F1 8C 54 13 D3 AF 4F A4 
36 40 2D E7 AF EB 8A F3 CE 9E CD AB 30 F1 44 44 36 FD B9 8C EC B0 29 C3 42 A5 9E AC 2D 40 17 85 
98 07 E8 60 BC CD 6C 32 E6 E5 9E 93 A9 82 43 23 22 82 7D 31 BC EC 16 DE 58 70 EA 4C B9 C5 FE 0B 
A3 8C 1E 91 36 3C D6 DF B1 C3 52 AA 34 DA 88 F0 80 03 BB 8F 15 19 79 22 E2 0D F9 B9 85 C9 21 71 
72 C6 63 D5 05 E8 AA 10 F3 00 1D A4 C9 C4 B1 4D 5F 7F 47 44 8C 54 13 37 34 7D C6 23 0B 17 3E 34 
26 42 E6 E9 0A 76 69 5C F5 A5 0B D7 9C 21 AF 88 1D 32 AC 3E E3 1D 24 FE F1 C3 92 0A B7 1C 2B 93 
28 D5 81 41 C1 5A 85 8D 27 42 CC 03 34 87 98 07 E8 00 2D 4C 1C E3 2D BA 8B 87 37 2F C9 F8 E6 D3 
55 0B 3E 5E FB EE 3D 71 88 FA 16 F0 75 A5 A5 7A C7 AC 7A C6 37 32 36 D4 C5 9A 62 14 31 23 A6 44 
C8 54 BE 52 A4 3B 40 8B 10 F3 00 42 E3 6A 2E 1D DA 75 A4 FE A4 F2 8D 78 5B 65 C6 07 0F DE 23 DB 
BA F7 DD 71 FE 9D 59 B3 EE 83 AB AC D4 DB 1C 3F 4B D4 01 6A 97 67 3A 18 99 AF DF CD 0F 94 6C FA 
0B 07 7E FC 61 CF F1 4B C5 E5 FA 3A 56 15 18 16 3B 70 CC AF 6E 1F 97 AC 6D BE 98 39 77 E7 FA 83 
85 36 22 46 1E 3F E1 BE B1 51 4D 3E 8F AB 2D BD 5C 62 E0 88 88 51 04 47 85 AA D8 BA 8A 2B 57 AB 
2C B7 6C 84 4C 13 19 A1 91 E1 18 04 3C 0B 31 0F 20 2C D7 13 C7 9E 79 76 41 5F 8D 65 C7 7F 97 2C 
F9 6F 46 B1 85 27 E2 8D D9 FF FA C3 2B B7 67 7D 38 51 E9 D9 FA 76 4D B6 3A 93 73 F2 3C 23 93 FB 
B4 61 3E 83 39 EF BB C5 4F 2F FA E7 96 F3 35 5C D3 E3 2D C6 37 76 D2 DC 37 DF FB F3 9C 81 AA 86 
17 6B F7 FD 63 DE DC AD 75 44 6C F0 EF 36 DF DD 3C E6 2B 2E 64 66 14 58 88 88 0D 1B 1E 11 AA 22 
43 E1 C9 23 67 AA 5A 3C 8C 73 7E 90 26 59 13 8E 98 07 4F C3 05 75 00 42 BA 61 E2 D8 E4 49 23 FB 
C7 45 06 F7 8E 8C 4C 49 BB 77 D1 27 7B 7E FA E0 8E 70 09 43 44 C4 5B 2E 7E B1 74 63 29 77 B3 E2 
7A 2A DE 6C 6E E8 2C 4B BC 5A 9D F2 36 DD CA FB C6 CD FC CB 77 B9 CD 33 9E 88 F8 DA BC 1D EF 3F 
3C 7E F2 0B DB 4A B0 EE A1 07 40 6F 1E 40 40 B7 9C 38 E6 9D 34 6F E9 4B 1B 77 3D BD DF 3B 6E D0 
C8 D4 31 E3 23 EB 6E D5 27 EC 99 BC 24 0D D1 6E B3 DA 6E F2 4E 17 2C D7 7E D9 B7 F0 8C DE C2 13 
11 23 0D 1B 3D 67 EE CC B4 21 FD B5 E6 BC 93 47 77 AE FD 78 53 B6 9E E3 39 5D C6 7B 0F CD 8D 3F 
F0 D5 DC F8 D6 8F 14 B0 8A F0 C4 14 EF 3A E7 AF 9C 3E FF 64 BE 9E 27 22 46 1D 33 20 46 ED EC 3C 
31 F2 20 1F 74 E5 C1 E3 10 F3 00 C2 71 67 E2 18 DB 6B CE AA 63 D3 03 13 7A 29 1B 8F A5 D5 9C DC 
B2 39 B3 94 23 62 FD FB 4F BD 67 B8 96 88 6C 79 7B D6 ED CD B3 F0 C4 48 A3 C6 DC 37 31 BE 69 59 
D6 AA A2 C2 0A 13 4F C4 F8 6A A3 43 94 22 1B 97 63 BC 64 32 96 88 23 22 E2 CD 26 13 47 E4 76 1A 
73 BA 73 C7 CE E9 4D 3C 11 B1 9A D4 E7 D6 FE EF AD 29 11 8E 1D DD D4 59 F3 16 3C B2 76 C1 3D 8F 
AE 3E 63 E2 B9 92 1F DE 5C FC FD AC 4F A7 AB 5B 5D 3B 1F 6D 5C 3F 6D FD AF D6 FC F2 D3 F9 7A 1B 
11 B1 CA B0 BE FD 62 B0 57 85 2E 05 FF 90 D0 3A 56 AB D5 D3 55 E8 C2 DC 9A 38 C6 FA 47 F7 6D 3E 
F1 CE 56 B4 E5 AD B9 2F 1D B6 10 79 F5 59 B8 DB 1E F3 75 87 3F 9C FF C8 26 03 4F C4 A8 C6 5E 3B 
B4 EB B9 FE 8D 16 E0 EB AE E5 1C C9 2A E3 88 24 91 A3 7A 89 2E E6 89 55 FB FB 31 54 69 BF FD 4D 
95 AE CA 16 13 74 E3 CA E4 F4 67 0E 64 96 2B C2 22 22 7B 45 68 FD BC ED EB C0 56 7E 29 AF D2 46 
44 C4 28 D3 5E 5B B3 78 4A 44 93 05 15 89 B3 3F 58 7A 70 DF 94 E5 17 AD 64 2B DA B4 EA AB 77 EF 
FC 7D B0 D8 D6 1E 40 23 F8 F7 86 56 B0 D9 6C 95 95 95 9E AE 45 17 D6 FE 89 63 2E F1 35 07 DF 59 
F4 C9 A5 56 0E 5D 77 6B AC 52 1B E4 EB 18 F1 E6 0D 57 0B 2B 5D 34 DE A6 2B C8 2B 2A BB 72 3E 3B 
63 F7 96 6F B6 65 97 DB 88 88 38 43 59 79 2D 4F 44 C4 F8 A6 CF 79 D0 D5 90 BC 72 EC EC E9 71 5E 
44 44 7C ED 91 7D 87 4D 1D D5 06 80 2E 01 31 0F AD 50 53 53 C3 F3 38 97 DC A2 F6 4E 1C 6B 19 57 
B9 E3 CD 17 D6 EB 7B D0 94 31 49 50 5C FD 59 6E BE E6 42 D6 A9 D2 66 B3 18 AC 15 39 C7 72 9D B3 
DD 19 79 90 56 23 21 22 E2 0D 35 8E AB 1C BC 12 06 0F D5 B8 2C 5B 96 9C D2 CF 3E 01 9E D3 5D 3A 
8F 49 90 20 6E 88 79 00 E1 B4 6B E2 D8 CD D9 4A BE 7E E5 BB 53 86 9E 73 90 C5 AA E3 FB 47 29 1C 
1D 7A 6B 45 CE 9E ED 07 4E 17 96 57 D7 D9 C8 66 36 94 5D 3C B6 67 DF E9 0A E7 09 24 49 50 42 5F 
AD 7D D5 F3 16 8B 73 0A A4 3A 28 A8 85 1D 9C B7 5A A3 B4 97 CC 57 E9 74 88 79 10 35 9C 9B 07 10 
4C 7B 26 8E DD 12 6F BD 9C B1 F9 07 79 5A 58 4F 79 42 0B E3 13 39 64 78 82 6E 7F 6E 95 8D 88 C8 
66 B8 92 7D E0 4A 36 31 2C C3 37 BD 4A CE 2B 28 69 48 1F 3F 67 A2 B3 12 96 C8 46 44 BC D5 DC E2 
93 ED 2C F5 C3 2E 5E 52 29 11 11 C3 3A 97 BF F9 81 14 C3 60 EE 3C 74 33 E8 CD 03 08 C7 3E 71 CC 
CE 56 A5 AB 72 D9 9F B7 9E 7C EF EE B4 99 0B DE 5E B3 F3 6C A5 5B F3 19 25 A1 51 91 DE 0C 11 CF 
95 1E 38 9E 53 DE 73 4E D1 33 DE 61 43 D2 C7 26 05 79 37 8E D6 A6 19 CF C8 02 13 53 47 27 05 48 
1A 96 91 39 6E 48 C3 17 5D BE EC 7A 5D 71 65 57 AE 39 CE DF B3 8E 2E BF 4C 2E B3 DF CD 80 AC 96 
1B BF 94 FA CF 64 58 16 31 0F DD 0C 62 1E 40 38 EE 4C 1C 33 67 AD FF EF 96 03 9B 97 BD F2 F0 E4 
A4 C8 61 AF 65 DC FA 49 E9 6C EC EF 17 CF B7 9F 4C B6 EA 72 B3 CE E9 7B 4E D0 13 E3 13 96 32 71 
EA A4 DB FA 84 AB 9B CD 68 64 64 FE 11 49 23 27 4F 9B 34 38 42 D1 68 3F 26 F1 D7 F8 DB 7F B5 5E 
DE B7 2B C7 E5 71 54 C5 DE 7D 27 AC 3C 11 91 24 7A D0 A0 00 96 88 58 B9 C2 FE 78 3B DE 6C 34 36 
5F 88 B3 D9 1C 03 FB AC 44 82 98 87 6E 06 83 F6 00 02 92 04 C5 C5 A8 2F 9C D2 71 E4 98 38 16 31 
6E A0 B6 71 67 B4 F6 C8 5F 9F 5A 7A CA 62 EF 1C B2 21 A3 C7 A7 B8 F3 F4 1A 9F 91 AF BE F7 FB EF 
EE FA F7 45 2B 59 CB 73 B2 2E F6 1A 97 D0 83 8E D0 59 9F A0 F8 A1 E3 E2 87 58 AF 57 E9 6B AE 9B 
CC BC 54 A1 54 AA 94 BE 32 97 67 44 64 21 A1 81 5E 45 A5 56 22 CB C9 55 6F 6F F8 C3 FA DF F6 6A 
BA AE 4C 99 CB 3F F8 D1 7E 42 5E 12 9E 3E 79 90 8C 88 88 0D 0F D7 B2 54 C9 11 6F 3E 77 F2 94 99 
46 36 FA 5A B8 5A 83 D1 FE 85 31 72 1F 79 C7 B4 11 A0 C3 F4 A0 7D 05 40 27 68 69 E2 98 85 2C FA 
BC 03 AB 9E 9A 3A 63 71 46 8D 23 33 7C 53 9F 78 72 BC 7B B1 C1 AA 6F 7F F3 9D FB FD 19 22 22 4B 
F1 E9 E3 05 2D 3F 17 47 B4 18 2F 85 3A 28 24 3C B2 57 44 48 A0 7F 0B 19 4F 44 AC 32 A6 4F 94 FD 
EE 73 B6 AB 9B E6 4F 7D F8 A3 AC AA FA 3F DA 8A 76 BC 71 D7 8C C5 99 D7 79 22 62 E4 43 E7 3D 31 
51 41 44 44 B2 DE FD 12 EC 0B 59 CF AD 7A 79 49 86 BE 61 91 9A 82 9C 3C BD 7D 75 4B 54 7E 2A EC 
33 A1 9B 41 6F 1E 40 50 2D 4C 1C DB BD FD 5B CE 6A 6D 74 5A 99 F5 1B F5 E2 3F 17 24 BA 3D 45 8F 
0D B9 E7 ED BB 06 7E F3 D9 09 03 91 E9 6A 76 76 68 6C 07 54 5E 1C BC 23 87 0C 8F E5 7E CE CB B7 
F0 5C CD E9 CF C0 65 40 5C 00 00 13 E7 49 44 41 54 FE 38 7A FB FB 43 86 0D 4A D4 9A 0B 4E FE 92 
79 22 CF 7E 13 5C 62 FD 53 5F 5D B6 70 80 73 17 18 78 C7 9C 3B 43 7F 58 7B CD 46 5C C5 EE 57 C6 
26 FE 6F E2 B8 C1 BD 8E 1D DA 5F 5D 5E 5C 66 70 8C E2 CB 42 A3 C2 BD 31 68 0F DD 0C 8E 4C 01 04 
E6 72 E2 98 D5 D2 28 E3 19 AF 80 11 0B D7 7E F9 D2 D0 56 8D 00 B3 51 23 66 FC 3A 44 4A 44 C4 5F 
2F 38 79 F1 96 4F 48 EB B1 18 DF 98 91 DF 2E 7F A0 8F 7D 54 85 37 96 E4 1C DC B2 76 D5 A7 1B 77 
66 5E 72 DC E8 DE 4B 3B EE E5 8D 1B 5F 1A A6 A8 5F 86 0D 9E F9 EE B2 47 93 1C 8B 58 4A 8E FF F8 
BF 4F 3F BE 98 7F A5 3E E3 59 DF A8 C1 83 A2 71 93 7A E8 76 10 F3 00 C2 6B 69 E2 18 C3 48 03 92 
67 BC B4 E6 D0 D9 83 EF 4D 0B 6F F5 C5 76 6C C8 98 21 49 41 F6 FB B7 D5 99 5A BC 5A 0C 88 A4 03 
FE F0 59 46 C6 86 37 66 A7 C6 A9 A5 0D D1 CC B0 8A B0 94 69 4F 2E DF 95 B5 FD 2F 93 42 9B EE FE 
D8 88 BB 57 EC F9 69 C5 E3 93 13 83 BC 9B 5C 36 C7 48 95 C1 71 83 C7 4D 1C 15 87 11 7B E8 86 30 
68 0F D0 31 9A 4E 1C BB 7D EA F4 21 69 53 E2 FB F4 EE A5 76 67 CE 5D 4B BC D4 7D 07 F7 B9 FC 53 
0E 6E E9 72 6B AC 3A 79 D6 EB 5F CC 7A DD AA 2F 38 93 5B 58 A6 33 B2 AA A0 B0 B8 BE 7D 43 15 2D 
2F 12 3C F2 B1 65 DB 1F FB A0 E6 EA B9 DC CB C5 15 BA BF BC F3 0F A9 B7 42 E5 A7 F4 BE D9 21 99 
57 CC D8 FB 62 04 AF 3E 80 40 10 F3 00 1D 8A F1 52 A8 83 14 EA F1 E9 69 43 87 F6 13 A0 3C 49 50 
D2 E0 84 C2 DD E7 6A D0 97 77 93 97 3A 7A C0 F0 E8 D6 2C 21 51 45 24 0D 8D E8 63 B5 86 AD FE BC 
A3 6A 05 D0 59 30 06 05 D0 CD 48 43 FA 0F 8E F5 C5 39 62 00 70 07 62 1E A0 DB F1 0E 1F 98 12 85 
C9 60 00 E0 06 0C DA 03 74 01 92 BE 2F 1E 32 BF D8 F4 35 C5 03 1B 6B 1E 70 FD 76 C6 27 3A 75 46 
74 6A C7 D7 0B 00 BA 3B F4 E6 01 00 00 44 0B 31 0F 00 00 20 5A 88 79 00 00 00 D1 42 CC 03 00 00 
88 16 62 1E 00 00 40 B4 10 F3 00 00 00 A2 85 98 07 00 00 10 2D C4 3C 00 00 80 68 21 E6 01 00 00 
44 0B 31 0F 00 00 20 5A 88 79 00 00 00 D1 42 CC 03 00 00 88 16 62 1E 00 00 40 B4 10 F3 00 E0 49 
32 99 4C F0 32 95 4A 65 FB 0B 61 59 D6 C7 C7 A7 FD E5 00 78 16 1E 44 0B D0 04 C3 74 C8 73 DC BD 
BC 04 D8 D6 04 29 A4 AB D9 BE 7D BB 54 2A E5 79 5E A8 02 59 96 0D 0E 0E 1E 33 66 4C FB 8B AA AC 
AC 2C 2F 2F 17 B0 6E E5 E5 E5 42 15 05 E0 26 11 EE 35 00 DA 63 EF DE BD 49 49 49 02 EE D9 89 48 
2E 97 7F FE F9 E7 6B D7 AE 6D 67 39 65 65 65 27 4E 9C 10 B0 6E 65 65 65 42 15 D5 2A 8D 8F A5 F4 
7A BD E0 E5 97 96 96 0A 5E A6 B0 3A E8 68 12 E0 46 88 79 80 E6 72 72 72 04 2F 33 2B 2B 4B F0 32 
BB AF 5E BD 7A A9 D5 EA 8E 08 F8 EE 62 E8 D0 A1 9E AE 02 F4 14 88 79 00 22 A2 A8 A8 A8 B3 67 CF 
7A BA 16 1E 13 1C 1C AC 50 28 3A ED E3 BC BC BC 56 AC 58 B1 73 E7 CE EB D7 AF 0B 5E 78 60 60 A0 
20 E7 E6 ED 4C 26 53 49 49 89 50 A5 D9 25 24 24 A4 A5 A5 09 5B 26 40 4B 10 F3 00 44 44 8F 3C F2 
88 4C 26 2B 28 28 B8 F1 4F 72 B9 5C 2E 97 77 7E 95 5C AA A9 A9 B1 D9 6C C2 96 E9 E7 E7 77 EF BD 
F7 4A 24 12 61 8B BD B9 D0 D0 D0 07 1F 7C B0 23 4A 4E 4C 4C 0C 0D 0D 15 AA 34 9D 4E 77 E2 C4 09 
A1 4A 03 E8 7C 88 79 00 22 A2 90 90 90 67 9F 7D D6 E5 9F 62 62 62 62 62 62 3A B7 3A 2D CA CC CC 
34 18 0C 9E AE 05 00 74 1B B8 A0 0E 00 00 40 B4 10 F3 00 00 00 A2 85 98 07 00 00 10 2D C4 3C 00 
00 80 68 21 E6 01 00 00 44 0B 31 0F 00 00 20 5A 88 79 00 00 00 D1 42 CC 03 00 00 88 16 62 1E 00 
00 40 B4 10 F3 00 00 00 A2 85 98 07 00 00 10 2D C4 3C 00 00 80 68 E1 D1 35 00 6D 67 36 9B 79 9E 
EF CC 4F EC E4 8F 03 80 EE 0E 31 0F D0 16 3C CF 9F 38 71 42 AF D7 7B BA 22 D0 1C 8E 84 00 1A C3 
A0 3D 40 5B E8 F5 7A 64 7C D7 84 07 F5 02 34 86 98 07 68 0B AB D5 EA E9 2A 80 6B 1C C7 79 BA 0A 
00 5D 08 62 1E 00 00 40 B4 10 F3 00 00 00 A2 85 98 07 00 00 10 2D C4 3C 00 00 80 68 21 E6 01 00 
00 44 0B 31 0F 00 00 20 5A 88 79 00 00 00 D1 42 CC 03 00 00 88 16 62 1E 00 00 40 B4 10 F3 00 00 
00 A2 85 98 07 00 00 10 2D C4 3C 00 00 80 68 21 E6 01 00 00 44 0B 31 0F 00 00 20 5A 88 79 00 00 
00 D1 42 CC 03 00 00 88 96 97 A7 2B 00 20 76 9C C5 A0 2F 2F D7 19 A5 EA D0 D0 40 85 C4 D3 D5 69 
37 B1 B5 A7 C7 B2 19 AB 2A 2A 2B AB 8D AC 42 AD 09 D0 F8 C9 F1 55 8A 14 62 1E 3A 9B 25 67 F5 A2 
C5 3F 96 D8 88 88 58 4D FA 73 EF 3F 36 58 E6 EA 7D C6 C3 4B 9F FE E0 60 35 47 44 8C 2C F9 A1 7F 
BC 3C C5 FF EC EA E7 16 FF 58 6C 23 22 D6 6F EC 33 1F 3E 31 4C 46 44 54 B5 E3 ED 05 AB B2 2D 44 
44 92 C0 71 CF FC 6D FE 50 65 B3 A2 CC 99 CB 16 FC 63 7F 35 47 C4 AA D3 9F 5B F9 D8 E0 0E 6C 5E 
C3 87 16 67 6D DB B2 6D F7 DE 43 A7 AE 56 5B 38 FB 6B AC B7 5F 80 36 32 71 78 FA 9D 77 DF 31 3C 
42 DE 74 81 D6 B6 E3 03 C7 8A B3 15 7D FD C6 F3 EB CF 5B 9B BC 95 61 24 52 5F FF 80 80 C0 80 40 
4D 70 E4 80 71 53 C6 27 05 4A 3B B7 3D 5D BD 41 DD 4F DB 37 9D A0 46 E3 B6 E6 E2 CC EF 37 6F DD 
9F 71 EC F4 E5 2A 8B F3 45 89 6F 68 C2 80 A1 63 7E 7D EF DD 63 63 14 CD 8A 6B FC 7D 48 87 3C F6 
EF E7 D3 9B BF A3 51 C5 58 FF 71 CF 2E 9D 3F 54 C6 95 7C FF E6 A2 CF CF 59 C8 1D 12 ED ED AF 2C 
F9 7D 52 4F FB 3E 3B 0B 62 1E 3A 9B 34 7E 50 6F FA 2C BB 84 23 22 2A FB 69 D7 F1 87 06 0F 6F 1E 
10 44 64 3C B6 67 F7 85 6B 3A 9E 88 C8 7B F0 7D 83 02 58 B2 59 6A CA 8A 4B 4A 38 22 62 8C 55 26 
C7 1B 39 DB F5 8A E2 92 12 7B 28 94 7C B5 7C ED D8 65 F3 92 9B 15 68 AA 2A 29 2E A9 E6 89 18 73 
FD 72 1D C9 98 B7 6D C5 3B CB B6 E4 D6 70 4D 5F E7 EA AA CB 0B 73 0E 14 E6 1C D8 F2 4D FA FC 37 
9E BB AB B7 4F C3 DF DA DC 0E 6B 6D 45 71 89 7D E7 DF 54 51 E1 45 FB 0F DF 7D F9 F9 EA D1 8F BC 
F2 FC CC 24 55 9B CE D3 B5 A5 3D 5D BA 41 DD 53 DB 37 9D FA 3F 5D F8 E6 EF 6F AE DC 7D E5 86 8D 
C0 56 5B 7C F6 F0 96 B3 87 B7 7D 95 32 EB D9 97 1F 1D A5 6D DC B5 6F F4 7D 48 2B AF 73 CD 97 25 
E2 CD D5 F5 5B A6 49 6F 24 22 E2 6D B5 95 D7 5C 7E 89 2E B0 4C 75 1D EF D6 3B A1 0D 7A D0 26 02 
5D 85 AC 7F FA E8 70 C7 7F 1E 57 71 78 EF 71 A3 8B 37 19 8F 1D 38 A2 77 6C F9 3E 03 C7 8F D5 BA 
F9 AF 6A B9 F4 D5 F2 2F 2F B8 D7 87 E8 18 9C FE C8 F2 A7 17 FC ED BB 1B 32 B1 09 D3 95 DD EF BF 
F8 CE B6 E2 96 76 83 02 B7 83 37 15 1D 58 B1 E8 B9 4F 4F 5E 6F F5 A2 C2 B4 A7 0B 35 A8 1B 6B DF 
A6 63 BE B0 E1 95 67 DF 77 91 F1 8D 58 2B 4E AC 7B FD E9 C5 DB 8A DC CB 67 E8 0E D0 9B 87 CE 27 
4D 9A 90 16 B5 79 6D BE 8D 88 B8 CA 23 FB 8F 1B 47 8E F2 69 FA 96 C6 BB 2A DF 41 E9 63 02 DD 3F 
20 35 9D DD B0 FC DB 71 7F 9F 19 E5 91 53 8D B6 E2 6D EF FD 75 E3 F9 EB 8E BA 7B 05 26 4D 9C 36 
69 68 62 7C 7C 74 10 A3 BF 7A FE D8 F6 4D 5F ED 3A A7 B3 11 11 71 E5 FB FF FD D1 DE 11 AF 4E 50 
BB 6C 5C 9B DA 21 89 FD F5 82 DF A4 28 38 8B E9 BA C1 60 B8 6E D0 15 9E 3E 7C F8 64 B1 91 27 22 
DE 78 6E DD BB 1F 0D 58 F1 A7 E1 CD C7 CE 3B A5 3D 5D A4 41 DD 5B 3B 36 1D C3 B1 7F FF 79 55 56 
B5 E3 2F 8C 3C 62 C4 B4 3B C6 A6 24 F6 8D 0B B4 96 5C BA 70 F6 F8 9E EF B7 66 5E AB 23 22 B2 14 
EF 59 FA D7 D8 98 25 73 FA BA 3C 25 E0 1E 89 66 C4 9C 17 34 FA 86 3E 3A 57 BC 77 D5 9A 9F CB 39 
22 22 69 BF BB FF 34 23 B1 21 7D 18 9F A8 68 64 51 87 C1 AA 05 0F 90 24 A4 A7 C5 6E C8 BF 60 23 
7B A7 E4 17 E3 A8 D1 4D 76 56 C6 5F 0E 1C 75 EC 21 18 E5 D0 F4 D4 80 D6 0C 3B F1 B5 C7 3F FB 68 
FB 98 3F 4F 0D E9 FC C1 2A C3 E1 35 FF 39 E4 AC B9 6F E2 7D FF B7 78 DE F0 FA CA 87 87 C5 24 0D 
1B 37 36 F9 CF CF BD 7F B0 9C 23 22 5E 77 F0 BB 9F 4A C7 CF 0C 75 59 D1 B6 B4 83 D1 C4 8F 9D F4 
AB 26 07 45 F3 0A B6 FE FD C5 25 BB 8B 6D 44 64 BB B2 6D DD 8F F7 0D BD 37 DC DD A4 15 B2 3D 5D 
A2 41 DD 5D 5B 37 1D CB F9 8D 1F 7D 57 E8 18 4C 61 35 C3 E6 BD FD FA FD 89 CE A3 A3 88 5E 7D 87 
8C 9B 7A C7 84 CF FF EF D5 D5 27 AA 79 22 BE F6 F4 86 35 3B A6 BC 35 2D B8 ED 9B 90 4F CC F0 C9 
31 8D 7E B7 E4 5C 5A BB C6 D9 8A A0 A4 09 BF 9A E4 E2 7C 03 74 04 0C DA 83 27 48 62 D3 D3 12 1C 
C7 98 BC EE C8 BE 63 4D 87 5E 8D BF EC 3F AA 73 EC AA FC 86 A7 8F F2 6F 65 F1 7C D5 A1 FF 7C B2 
AF F2 66 83 CC 1D 82 2B DB F5 CD DE 72 C7 C7 2A 52 1E 7A 69 EE F0 1B 0E 50 64 31 D3 16 CC 1E AC 
F2 0D 8A 4A BA 2D FD CE 99 A9 91 7C CB 27 25 05 69 87 4F F4 D4 67 16 4C 72 CE C1 32 9D FC 71 5B 
9E BB 03 B2 42 B7 C7 E3 0D 12 81 B6 6D 3A A6 AC 6F 7F B8 50 1F F2 63 E7 BF 30 2B B1 F9 08 08 EB 
37 E0 B7 CF 3F 32 CC 97 B1 17 5D 7B F4 DB AD 05 3D 68 BD 8A 19 62 1E 3C 42 D2 6B C2 B8 44 C7 C4 
5A 5E 7F 74 6F 93 9D 55 E3 5D 95 66 64 7A 2B 86 64 BD A4 8E 1D 20 57 BE EB E3 D5 99 06 81 6A EB 
26 AE EA E8 A1 53 8E B3 A5 8C 26 75 C6 54 D7 C3 D3 92 D0 E9 7F FB EA DB 2F D7 2C 7B F7 FF 9E 99 
37 6B 44 98 8B 37 09 DC 0E E5 6D 93 53 9D FD 32 5B C1 89 AC 32 F7 72 56 B8 F6 74 91 06 89 42 5B 
36 1D CB 85 63 C7 2B 9C 57 47 84 4F BC 67 5C 90 CB 3D BF 24 7C E2 9D A9 6A 7B CE 93 35 EF 78 FD 
32 D0 AD 21 E6 C1 33 24 61 E3 C7 F7 F7 B6 FF CC EB 8F EE 3D DA B0 E7 6F B4 AB 62 03 47 A6 0F 6D 
7E F5 4E CB D8 F0 89 77 8F 72 EC A6 B8 A2 AD 2B 3E 3B E9 6A 8A 52 87 B1 5E 3A 77 A9 CE F1 B3 34 
7E E0 C0 16 2B CE 4A 24 37 DD F2 04 6F 87 2C A1 4F 8C F3 04 1D 77 AD F0 AA F5 A6 EF 76 12 AC 3D 
5D A5 41 E2 D0 86 4D E7 7A 7E 5E 89 23 B1 19 BF FE 83 12 5B 3C E7 AE E8 9F 1C EF 5C AF D6 8B 67 
73 7B D2 7A 15 2F C4 3C 78 08 AB 4D 4B 1F E8 38 3B C7 57 1F DD 5B DF C3 6B BC AB 0A 1E 3D 71 B0 
8F EB E5 5D 17 AA 19 FF D8 EF 86 38 C6 1D AD F9 DF 2C FB DF 79 B3 70 55 BE 15 73 69 A9 CE D9 65 
F2 D3 86 F8 36 D9 BA 8C 15 57 0B 2F DF A8 F0 9A CE C5 C4 67 C1 DB 21 F7 F7 97 3B 7A 69 5C AD C1 
E0 5E 27 4D B8 F6 74 91 06 89 44 AB 37 1D 5B 45 79 15 57 BF 70 D8 4D 26 32 B0 CA D0 60 7F C7 F7 
CC 1B F5 55 75 2D BE 13 BA 0F 4C C1 03 4F 61 03 C7 8C 1F FC D1 D1 43 B5 44 C4 D7 1C DB 7B A4 66 
FC 04 15 91 31 EB 40 FD AE 2A 6C CC 84 01 AD 9C EC CB 84 DF F1 F8 EC 5D 4F 7C 7C CA 44 44 75 B9 
5F 2E FB 66 FC 92 59 61 82 57 DE 15 AE CE 64 AE 3F 2F 2D F3 F6 66 1A FF D1 94 B1 EC B1 37 77 D7 
DE B0 10 AB BD EB DD CF 16 0E BD A1 95 42 B7 43 EE ED CD 10 F1 44 44 66 63 AD 5B 9D 34 41 DB D3 
15 1A 24 1A AD DE 74 EA CC F5 DF 24 23 57 F8 30 37 16 49 F5 7F F6 F1 76 FE CC 19 0D 06 1B A9 7A 
CA EC 46 F1 42 6F 1E 3C 86 0D 18 9D 3E 44 E9 98 F1 53 73 6C 6F 46 35 11 19 B3 F6 1F 71 EE AA 22 
C7 4E E8 D7 FA 1B 63 49 E3 EE 7D FC DE 04 FB 72 7C 6D F6 E7 2B 7F 6C F9 62 6E 41 B1 DE 72 99 73 
07 CA 1B 8D D7 6F 32 15 CD 1D C2 B6 A3 CE 6C 71 D6 47 22 95 B9 B5 E7 16 B8 3D 9E 6F 90 78 B4 76 
D3 F1 F1 A9 3F 48 E3 4D 26 D3 CD 26 49 9A 4C F5 3D 78 D6 DB C7 47 42 44 24 61 1B 82 82 A7 5B FC 
1F B0 92 1E F6 65 74 7D 88 79 F0 20 FF 91 E9 B7 A9 1C 3B 2B C3 2F FB 32 AA 38 63 D6 7E E7 E5 40 
92 D8 71 13 FA B6 E9 F6 97 B2 7E 0F 3C 7E 57 2F FB CE 86 AF 3E B2 FA E3 7D 55 9D 72 8B 2D 99 56 
AB 71 0E 78 D6 5C 2B D2 B7 77 24 59 C0 76 58 AF D7 3A 77 EE 8C 42 E5 EB DE 8E 58 E8 F6 74 52 83 
64 B2 76 5C EE DD 5D B4 6A D3 61 FC FC 94 F5 31 5F 51 7C B3 A3 AB DA D2 B2 6A E7 F7 2C 53 F9 C9 
89 88 18 99 4C 5A BF B8 CD 6A 71 F1 7F 60 B3 3A CB 64 BC BC 7A C0 EA EF 5E 10 F3 E0 49 AA 11 13 
46 AA 9D 57 F0 FC B2 EF 70 E9 2F FB 8F 3A 4E 07 4B 7A A7 A5 C7 B5 B5 5F A0 18 F4 BB F9 B7 3B AE 
DE E6 CA F7 7C B2 3E C7 DC 09 41 EF D5 BB 6F 9C 73 17 67 BB 70 E2 44 75 A3 FD A1 3C F5 99 FF 6E 
DA E8 B0 EE F9 34 DF 9B 0C 9C 36 10 AA 1D 96 FC BC 42 E7 B9 70 49 78 54 B4 7B EB 55 F8 F6 74 4A 
83 7C 7C 5A 33 9D A3 BB 6A CD A6 C3 AA E2 E2 9C F7 32 E0 74 A7 B3 2F B6 78 33 42 63 CE A9 FA 3B 
15 4A A2 E2 7B 4B 89 9A 0D EB 98 EB 5C 4D AA 30 5B EB C7 56 A4 32 2F F7 FE 17 A0 B3 20 E6 C1 A3 
14 43 D3 47 39 EE 7D C2 D7 66 ED FC CF D6 23 8E 5D 95 34 71 DC F8 F6 DC C7 CE 6F C4 A3 73 C7 07 
39 F3 E4 4A 51 67 DC C9 9E F5 1B 36 6A 80 23 62 78 C3 D1 6F 7F 68 7C DD B1 4C 19 10 10 E8 10 A0 
90 BA BB 27 14 A4 1D B6 2B 99 C7 AE 3A 27 D3 85 F5 1F 10 EC DE 8A ED 88 F6 78 B4 41 A2 D2 9A 4D 
47 1A 9F D2 DF DF 39 63 B1 70 C7 57 87 5C 8F CC 70 C5 3F 7D 7F D0 31 EE 4F 6C F8 80 94 50 7B 31 
72 7F 7F E7 F9 7C BE EC DA B5 1B 27 42 98 F5 BA 5A E7 D8 8A D2 DF 1F B1 D2 B5 E0 FB 00 CF F2 19 
32 A1 FE 0A E8 DA CC 1D 07 1D B7 4E 91 25 8D 1B 1F D1 AE 7D 37 1B 30 6E EE C3 23 FC 3A B5 63 C1 
6A D3 EF A8 BF B9 A8 E9 F4 17 EF AE CA 72 B1 3F 35 97 1C CF 29 74 BB 13 DB FE 76 D8 AE ED 58 FD 
DD 05 C7 BE 59 9A 30 F9 57 6E 9F 0A E9 88 F6 78 B4 41 A2 D2 9A 4D C7 E7 B6 69 93 22 9D FD F9 F2 
9F 96 BE B7 25 FF 86 4E B9 F1 DC 97 7F FF 24 C3 E0 F8 22 65 89 53 A6 3A 2F CF 97 84 85 3B 6F 5C 
C8 95 1E D8 72 B0 D9 E5 F4 5C F9 81 9F 73 9C 63 00 D2 E0 10 6D 0F 3C E8 EA D2 30 D3 1E 3C 4C 36 
60 C2 98 D0 EF 37 15 35 D9 73 C8 93 C7 A5 85 B6 77 67 21 09 9D F2 C7 39 3B B3 97 1F AF ED 94 33 
F3 44 44 A4 1A 3D 67 F6 A0 9F 3F FC A5 96 27 E2 AF 9F 5D F7 F2 82 FC BB 7F 73 47 EA C0 C4 3E 91 
FE E6 B2 BC DC B3 27 8F 6C DB FC ED E1 2B C6 56 54 A9 35 ED E0 8D 65 17 CE E4 94 30 44 BC D5 58 
AD D7 EB 4B F3 B3 76 6D D9 75 C6 D1 D1 63 34 A3 67 4D 8B 69 C5 8A ED 88 F6 78 B4 41 62 D2 9A 4D 
47 D6 7F E6 6F C7 EC 7A 77 5F 05 47 44 5C F9 C1 7F 2D FC D3 D9 DF DC 3F 79 78 FF BE 51 1A BE 3C 
EF EC A9 8C 6D EB D7 EF BC E0 0C 79 49 C4 ED 73 EE A8 1F 12 90 F4 1A 94 1C F2 D9 F9 22 FB B2 BB 
96 BC E8 55 31 77 F6 ED 83 A2 54 32 8B E1 6A F6 8E B5 1F AD 3A E8 BC 57 BE B4 EF A0 01 DE 37 7C 
3A 78 14 62 1E 3C 4D 96 34 61 6C E4 57 1B 2E 37 DA 59 29 52 26 A4 B5 E3 6E DA F5 24 51 D3 9F B8 
7F D7 93 FF 39 D3 19 43 F6 8E 8F EC 35 FD F9 67 CE 3F F7 EE 8F 85 75 44 44 A6 AB 87 D6 2D 39 B4 
8E 48 22 F5 E2 2D D6 1B BA C2 52 6D 52 BF 5B 1F CF B8 DF 0E DB 99 75 2F 2E 58 D7 52 31 E1 B7 2F 
7C 32 BD 55 CF 07 E8 98 F6 78 B2 41 62 D2 9A 4D 87 0D 99 BC 70 D1 99 82 D7 BF 29 30 13 11 71 FA 
33 3F AC 7C E3 87 95 24 91 4A 79 4B D3 59 75 8C 6F BF DF BE 38 7F A4 5F 43 31 B2 01 33 EF 1B FA 
D3 FB 47 AB 79 22 E2 6B 2F 6C 5B F6 C2 B6 65 AC 54 2E B1 99 9A 2C CA 06 8F BF 7B 42 48 0F 3D EA 
EA BA 7A EC 06 02 5D 87 B4 6F 7A 5A E3 0E 19 E3 3B 38 3D 55 23 CC BF A6 34 7E D6 E3 77 C7 75 EA 
98 AE 24 64 C2 73 1F FC 7D DE A8 90 26 9F 6A BB 21 13 BD 34 7D 27 CF FF FB B2 D7 A6 B8 73 72 A2 
DD ED 60 55 7D EE 7C 61 F1 D3 63 5B 9F 89 1D D2 1E 4F 36 48 44 5A B5 E9 B0 EA 91 4F 2E 59 FC 60 
8A A6 C9 F7 63 6B 96 F1 D2 90 D4 3F BE FB B7 87 92 9B 4E 63 94 44 DC F1 DC 0B 33 12 14 8D 4F B5 
70 96 A6 19 CF F8 25 CF 7E F1 F1 9E FD 85 74 4D E8 CD 83 E7 49 E2 27 A4 F5 5E 77 29 D7 3E C1 8B 
51 0D 9B 30 4A A0 94 27 22 79 F2 EC C7 A7 ED 7F E1 EB 2B 9D F8 18 0E 56 3D E0 81 B7 3E 1E 79 78 
DB 96 AD 3B 76 67 E4 56 36 9E D8 CC C8 34 D1 49 83 6F 1B 3D E9 8E 69 A3 A2 5A 31 25 BC F5 ED 60 
24 52 6F B9 52 1D 1A 15 DF 7F F8 A4 3B A7 8D 89 F7 6F 6B 37 AB 23 DA E3 D1 06 89 46 2B 37 1D 49 
E0 B0 47 97 7C 9A B6 7B E3 BA AF 77 1D 3D 5B 64 68 BC EA A5 FE 51 FD 47 4E 9E 39 FB 9E 31 51 AE 
EE 6A 2C 09 4E 7D EA FD E5 29 EB 57 6F D8 76 34 B7 A4 B6 C9 97 26 F1 0D 4F 1A FD EB 07 7E 77 CF 
A8 88 9E 70 91 43 B7 C3 F0 37 7B 9E 14 40 13 7A BD FE F8 F1 E3 9E AE 45 67 8B 89 89 89 89 89 69 
F6 62 59 59 D9 E9 D3 A7 DD 58 9A 33 D7 54 14 17 17 97 96 EA 4C 5E 7E 9A C0 C0 B0 88 88 00 9F 6E 
DC E1 E9 FA ED 49 4C 4C 0C 0D 0D 15 AA 34 9D 4E 77 E2 C4 09 A1 4A EB 42 6C 86 E2 BC FC 6B 95 FA 
6A 23 E3 E3 AF 0E 0C 8B 8D 09 51 B8 77 D8 64 A9 BA 9A 5F 58 56 55 63 A8 35 93 B7 CA 5F 13 1A 1D 
1F EE D7 E5 8E B8 92 93 93 83 82 82 3C 5D 8B 2E 01 BD 79 80 0E C5 CA 54 C1 51 AA E0 A8 04 4F 57 
44 20 62 6B 4F 8F 25 51 86 C6 27 B7 E9 68 48 EA 1F 91 E0 1F 21 74 7D A0 C3 74 A9 A3 70 00 00 00 
10 12 62 1E 00 00 40 B4 10 F3 00 00 00 A2 85 98 07 00 00 10 2D C4 3C 00 00 80 68 21 E6 01 00 00 
44 0B 31 0F 00 00 20 5A 88 79 00 00 00 D1 42 CC 03 B4 85 4A A5 F2 74 15 C0 35 3F 3F 3F 4F 57 01 
A0 0B C1 5D F0 00 DA 42 2E 97 A7 A4 A4 94 96 96 E2 76 D1 5D 07 C3 30 41 41 41 0A 85 AB 7B B2 03 
F4 54 88 79 68 05 5F 5F 5F 96 65 39 EE 86 E7 8F 8A 9A BF BF BF CB D7 35 1A 8D 46 A3 E9 E4 CA 40 
27 53 2A 95 0C 83 67 7F 74 33 0C C3 60 50 A7 1E FE 7D A1 75 6A 6A 6A 4A 4A 4A 6C B6 4E 7C DE 9B 
E7 30 0C A3 56 AB B5 5A AD A7 2B 02 9E 54 5D 5D 5D 52 52 D2 D3 8E 6E BB 2F 96 65 83 83 83 D5 6A 
B5 A7 2B D2 55 20 E6 01 00 00 44 0B 53 F0 00 00 00 44 0B 31 0F 00 00 20 5A 88 79 00 00 00 D1 42 
CC 03 00 00 88 16 62 1E 00 00 40 B4 10 F3 00 00 00 A2 85 98 07 00 00 10 2D C4 3C 00 00 80 68 21 
E6 01 00 00 44 0B 31 0F 00 00 20 5A 88 79 00 00 00 D1 FA 7F BB 3D 2E 65 19 60 2C 84 00 00 00 00 
49 45 4E 44 AE 42 60 82 
EndData
$EndBitmap
Text Notes 14025 5075 0    79   ~ 16
Layout Recommendation
Wire Wire Line
	3025 2200 3025 2250
Wire Wire Line
	3025 2250 3350 2250
Wire Wire Line
	3350 2250 3350 2200
Connection ~ 3025 2250
Text Notes 3850 2150 0    50   ~ 0
Consider input protection (what?)
$EndSCHEMATC
