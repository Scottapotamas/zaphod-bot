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
Text Notes 12600 900  0    79   ~ 16
Input Voltage Monitoring
Text Notes 6350 3250 0    79   ~ 16
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
P 12450 1600
F 0 "R?" H 12378 1415 50  0000 R CNN
F 1 "130K_1608M" H 12378 1495 35  0000 R CNN
F 2 "RESC1608*" V 12560 1450 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2059623.pdf" V 12585 1450 20  0001 C CNN
F 4 "PANASONIC" V 12635 1450 20  0001 C CNN "manf"
F 5 "ERJ3GEYJ134V" V 12660 1450 20  0001 C CNN "manf#"
F 6 "Element14" V 12685 1450 20  0001 C CNN "Supplier"
F 7 "2059658" V 12710 1450 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/panasonic-electronic-components/erj3geyj134v/res-thick-film-130k-5-0-1w-0603/dp/2059658" V 12735 1450 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 12760 1450 20  0001 C CNN "Supplier Price"
F 10 "50" V 12785 1450 20  0001 C CNN "Supplier Price Break"
	1    12450 1600
	1    0    0    1   
$EndComp
$Comp
L appli_resistor:130K_1608M R?
U 1 1 5ABC5A8A
P 12450 2000
F 0 "R?" H 12378 1815 50  0000 R CNN
F 1 "130K_1608M" H 12378 1895 35  0000 R CNN
F 2 "RESC1608*" V 12560 1850 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2059623.pdf" V 12585 1850 20  0001 C CNN
F 4 "PANASONIC" V 12635 1850 20  0001 C CNN "manf"
F 5 "ERJ3GEYJ134V" V 12660 1850 20  0001 C CNN "manf#"
F 6 "Element14" V 12685 1850 20  0001 C CNN "Supplier"
F 7 "2059658" V 12710 1850 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/panasonic-electronic-components/erj3geyj134v/res-thick-film-130k-5-0-1w-0603/dp/2059658" V 12735 1850 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 12760 1850 20  0001 C CNN "Supplier Price"
F 10 "50" V 12785 1850 20  0001 C CNN "Supplier Price Break"
	1    12450 2000
	1    0    0    1   
$EndComp
$Comp
L appli_resistor:10K0_1608M R?
U 1 1 5ABC5BF5
P 12450 2400
F 0 "R?" H 12377 2285 50  0000 R CNN
F 1 "10K0_1608M" H 12377 2205 35  0000 R CNN
F 2 "RESC1608*" V 12560 2250 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 12585 2250 20  0001 C CNN
F 4 "TE CONNECTIVITY" V 12635 2250 20  0001 C CNN "manf"
F 5 "CRGH0603F10K" V 12660 2250 20  0001 C CNN "manf#"
F 6 "Element14" V 12685 2250 20  0001 C CNN "Supplier"
F 7 "2332016" V 12710 2250 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/te-connectivity/crgh0603f10k/resistor-power-10k-0-2w-1-0603/dp/2332016" V 12735 2250 20  0001 C CNN "Supplier URL"
F 9 "0.02" V 12760 2250 20  0001 C CNN "Supplier Price"
F 10 "10" V 12785 2250 20  0001 C CNN "Supplier Price Break"
	1    12450 2400
	1    0    0    1   
$EndComp
$Comp
L appli_power:+POWER #PWR0108
U 1 1 5ABC5F0E
P 12450 1250
F 0 "#PWR0108" H 12450 1200 20  0001 C CNN
F 1 "+POWER" H 12453 1388 30  0000 C CNN
F 2 "" H 12450 1250 60  0000 C CNN
F 3 "" H 12450 1250 60  0000 C CNN
	1    12450 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	12450 1250 12450 1300
Wire Wire Line
	12450 1600 12450 1700
Wire Wire Line
	12450 2000 12450 2050
Wire Wire Line
	12800 2150 12800 2050
Wire Wire Line
	12800 2050 12450 2050
Connection ~ 12450 2050
Wire Wire Line
	12450 2050 12450 2100
Wire Wire Line
	12450 2400 12450 2450
$Comp
L appli_power:GND #PWR0109
U 1 1 5ABC61AD
P 12450 2500
F 0 "#PWR0109" H 12450 2250 50  0001 C CNN
F 1 "GND" H 12455 2327 50  0000 C CNN
F 2 "" H 12450 2500 60  0000 C CNN
F 3 "" H 12450 2500 60  0000 C CNN
	1    12450 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	12800 2350 12800 2450
Wire Wire Line
	12800 2450 12450 2450
Connection ~ 12450 2450
Wire Wire Line
	12450 2450 12450 2500
Wire Wire Line
	12800 2050 13450 2050
Connection ~ 12800 2050
$Comp
L appli_device:BAV99 D?
U 1 1 5ABC7E1F
P 13450 2050
F 0 "D?" H 13558 2096 50  0000 L CNN
F 1 "BAV99" H 13558 2005 50  0000 L CNN
F 2 "SOT23" H 13650 1900 20  0001 C CNN
F 3 "http://datasheet.octopart.com/BAV99-/T3-NXP-datasheet-5318834.pdf" H 13650 1875 20  0001 C CNN
F 4 "NXP" H 13650 1825 20  0001 C CNN "manf"
F 5 "RS Online" H 13650 1775 20  0001 C CNN "Supplier"
F 6 " 544-4534" H 13650 1750 20  0001 C CNN "Supplier Part No"
F 7 "http://au.rs-online.com/web/p/rectifier-schottky-diodes/5444534/" H 13650 1725 20  0001 C CNN "Supplier URL"
F 8 "0.045" H 13650 1700 20  0001 C CNN "Supplier Price"
F 9 "1" H 13650 1675 20  0001 C CNN "Supplier Price Break"
F 10 "BAV99" H 13650 1800 20  0001 C CNN "manf#"
	1    13450 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	13450 2300 13450 2500
Wire Wire Line
	13450 1800 13450 1725
$Comp
L appli_power:GND #PWR0110
U 1 1 5ABC81D7
P 13450 2500
F 0 "#PWR0110" H 13450 2250 50  0001 C CNN
F 1 "GND" H 13455 2327 50  0000 C CNN
F 2 "" H 13450 2500 60  0000 C CNN
F 3 "" H 13450 2500 60  0000 C CNN
	1    13450 2500
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+3.3V #PWR0111
U 1 1 5ABC8212
P 13450 1725
F 0 "#PWR0111" H 13450 1685 30  0001 C CNN
F 1 "+3.3V" H 13459 1863 30  0000 C CNN
F 2 "" H 13450 1725 60  0000 C CNN
F 3 "" H 13450 1725 60  0000 C CNN
	1    13450 1725
	1    0    0    -1  
$EndComp
Wire Wire Line
	13450 2050 13875 2050
Connection ~ 13450 2050
$Comp
L appli_resistor:1K00_1608M R?
U 1 1 5ABC8785
P 14175 2050
F 0 "R?" V 13987 1900 50  0000 C CNN
F 1 "1K00_1608M" V 14067 1900 35  0000 C CNN
F 2 "RESC1608*" V 14285 1900 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 14310 1900 20  0001 C CNN
F 4 "MULTICOMP" V 14360 1900 20  0001 C CNN "manf"
F 5 "MCHP03W8F1001T5E" V 14385 1900 20  0001 C CNN "manf#"
F 6 "Element14" V 14410 1900 20  0001 C CNN "Supplier"
F 7 "1576283" V 14435 1900 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mchp03w8f1001t5e/resistor-thick-film-1k-0-125w/dp/1576283" V 14460 1900 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 14485 1900 20  0001 C CNN "Supplier Price"
F 10 "50+" V 14510 1900 20  0001 C CNN "Supplier Price Break"
	1    14175 2050
	0    1    1    0   
$EndComp
Wire Wire Line
	14175 2050 14325 2050
Text GLabel 14325 2050 2    50   Output ~ 0
VOLTAGE_INPUT
Text Notes 12950 1400 0    50   ~ 0
Teknic IPC-3 outputs 75VDC nominal.\nDivider is 27:1, so 75V = 2.78.\nProvides enough overhead for sensing 90VDC.
Text Notes 14325 2250 0    50   ~ 0
27:1 ratio
$Comp
L appli_power:+12V #PWR0112
U 1 1 5ABE329D
P 12800 5050
F 0 "#PWR0112" H 12800 4900 50  0001 C CNN
F 1 "+12V" H 12815 5223 50  0000 C CNN
F 2 "" H 12800 5050 60  0000 C CNN
F 3 "" H 12800 5050 60  0000 C CNN
	1    12800 5050
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
L appli_resistor:0R_1608M R?
U 1 1 5B040E00
P 9150 5800
F 0 "R?" V 8962 5650 50  0000 C CNN
F 1 "0R_1608M" V 9042 5650 35  0000 C CNN
F 2 "RESC1608*" V 9260 5650 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1739504.pdf" V 9285 5650 20  0001 C CNN
F 4 "MULTICOMP" V 9335 5650 20  0001 C CNN "manf"
F 5 "MC0063W06030R" V 9360 5650 20  0001 C CNN "manf#"
F 6 "Element14" V 9385 5650 20  0001 C CNN "Supplier"
F 7 "9331662" V 9410 5650 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mc0063w06030r/resistor-thick-film-0r-0-063w/dp/9331662" V 9435 5650 20  0001 C CNN "Supplier URL"
F 9 "0.023" V 9460 5650 20  0001 C CNN "Supplier Price"
F 10 "50" V 9485 5650 20  0001 C CNN "Supplier Price Break"
	1    9150 5800
	0    1    1    0   
$EndComp
$Comp
L appli_resistor:10K0_1608M R?
U 1 1 5B040F7F
P 9150 6400
F 0 "R?" V 8962 6250 50  0000 C CNN
F 1 "10K0_1608M" V 9042 6250 35  0000 C CNN
F 2 "RESC1608*" V 9260 6250 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 9285 6250 20  0001 C CNN
F 4 "TE CONNECTIVITY" V 9335 6250 20  0001 C CNN "manf"
F 5 "CRGH0603F10K" V 9360 6250 20  0001 C CNN "manf#"
F 6 "Element14" V 9385 6250 20  0001 C CNN "Supplier"
F 7 "2332016" V 9410 6250 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/te-connectivity/crgh0603f10k/resistor-power-10k-0-2w-1-0603/dp/2332016" V 9435 6250 20  0001 C CNN "Supplier URL"
F 9 "0.02" V 9460 6250 20  0001 C CNN "Supplier Price"
F 10 "10" V 9485 6250 20  0001 C CNN "Supplier Price Break"
	1    9150 6400
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
	8700 5050 9600 5050
Connection ~ 8700 5050
Wire Wire Line
	8100 4750 8700 4750
Wire Wire Line
	10450 5050 10800 5050
Wire Wire Line
	9600 5500 9600 5800
Wire Wire Line
	9600 5800 9150 5800
Wire Wire Line
	9600 6100 9150 6100
$Comp
L appli_resistor:0R_1608M R?
U 1 1 5B0D507E
P 9150 6100
F 0 "R?" V 8962 5950 50  0000 C CNN
F 1 "0R_1608M" V 9042 5950 35  0000 C CNN
F 2 "RESC1608*" V 9260 5950 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1739504.pdf" V 9285 5950 20  0001 C CNN
F 4 "MULTICOMP" V 9335 5950 20  0001 C CNN "manf"
F 5 "MC0063W06030R" V 9360 5950 20  0001 C CNN "manf#"
F 6 "Element14" V 9385 5950 20  0001 C CNN "Supplier"
F 7 "9331662" V 9410 5950 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mc0063w06030r/resistor-thick-film-0r-0-063w/dp/9331662" V 9435 5950 20  0001 C CNN "Supplier URL"
F 9 "0.023" V 9460 5950 20  0001 C CNN "Supplier Price"
F 10 "50" V 9485 5950 20  0001 C CNN "Supplier Price Break"
	1    9150 6100
	0    1    1    0   
$EndComp
Wire Wire Line
	8850 6100 8600 6100
Wire Wire Line
	7500 5350 8700 5350
Wire Wire Line
	8700 5350 8700 5800
Wire Wire Line
	8700 5800 8850 5800
Wire Wire Line
	7500 5450 8600 5450
Wire Wire Line
	8600 5450 8600 6100
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
L appli_power:+POWER #PWR?
U 1 1 5B0E5387
P 9600 4425
F 0 "#PWR?" H 9600 4375 20  0001 C CNN
F 1 "+POWER" H 9603 4563 30  0000 C CNN
F 2 "" H 9600 4425 60  0000 C CNN
F 3 "" H 9600 4425 60  0000 C CNN
	1    9600 4425
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
	8500 5550 8500 6400
Wire Wire Line
	8500 6400 8850 6400
Wire Wire Line
	9150 6400 9600 6400
Wire Wire Line
	9600 6100 9600 6400
Wire Wire Line
	9600 6625 9600 6400
Connection ~ 9600 6400
Wire Wire Line
	8800 5250 8800 5350
Wire Wire Line
	8800 5350 9300 5350
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
	9600 5050 10150 5050
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
Wire Wire Line
	8700 4450 8975 4450
Connection ~ 8700 4450
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
Text Notes 9725 6250 0    50   ~ 0
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
Text Notes 4775 4075 0    157  ~ 31
check for 100V rated part requirements
$Comp
L appli_power:PWR_FLAG #FLG?
U 1 1 5AD551CC
P 4075 8450
F 0 "#FLG?" H 4075 8500 30  0001 C CNN
F 1 "PWR_FLAG" H 4075 8583 30  0000 C CNN
F 2 "" H 4075 8450 60  0000 C CNN
F 3 "" H 4075 8450 60  0000 C CNN
	1    4075 8450
	1    0    0    -1  
$EndComp
$Comp
L appli_power:PWR_FLAG #FLG?
U 1 1 5AD55228
P 4325 1550
F 0 "#FLG?" H 4325 1600 30  0001 C CNN
F 1 "PWR_FLAG" H 4325 1683 30  0000 C CNN
F 2 "" H 4325 1550 60  0000 C CNN
F 3 "" H 4325 1550 60  0000 C CNN
	1    4325 1550
	1    0    0    -1  
$EndComp
$Comp
L appli_power:PWR_FLAG #FLG?
U 1 1 5AD5527D
P 12675 4525
F 0 "#FLG?" H 12675 4575 30  0001 C CNN
F 1 "PWR_FLAG" H 12675 4658 30  0000 C CNN
F 2 "" H 12675 4525 60  0000 C CNN
F 3 "" H 12675 4525 60  0000 C CNN
	1    12675 4525
	1    0    0    -1  
$EndComp
$Comp
L appli_capacitor:100n_X7R_1608M C?
U 1 1 5ADBDD4B
P 12800 2150
F 0 "C?" H 12878 2085 50  0000 L CNN
F 1 "100n_X7R_1608M" H 12878 2005 35  0000 L CNN
F 2 "CAPC1608*" V 12915 2050 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1732728.pdf" V 12940 2050 20  0001 C CNN
F 4 "KEMET" V 12990 2050 20  0001 C CNN "manf"
F 5 "C0603C104K5RACAUTO" V 13015 2050 20  0001 C CNN "manf#"
F 6 "Element14" V 13040 2050 20  0001 C CNN "Supplier"
F 7 "2070398" V 13065 2050 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c0603c104k5racauto/cap-mlcc-x7r-100nf-50v-0603/dp/2070398" V 13090 2050 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 13115 2050 20  0001 C CNN "Supplier Price"
F 10 "1" V 13140 2050 20  0001 C CNN "Supplier Price Break"
	1    12800 2150
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
P 9600 5800
F 0 "R?" H 9673 5685 50  0000 L CNN
F 1 "R01_3216M" H 9673 5605 35  0000 L CNN
F 2 "" V 9710 5650 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2049155.pdf?" V 9735 5650 20  0001 C CNN
F 4 "Vishay" V 9785 5650 20  0001 C CNN "manf"
F 5 "WSLP1206R0100FEA" V 9810 5650 20  0001 C CNN "manf#"
F 6 "Element14" V 9835 5650 20  0001 C CNN "Supplier"
F 7 "683-6215" V 9860 5650 20  0001 C CNN "Supplier Part No"
F 8 "http://au.rs-online.com/web/p/surface-mount-fixed-resistors/6836215" V 9885 5650 20  0001 C CNN "Supplier URL"
F 9 "1.15" V 9910 5650 20  0001 C CNN "Supplier Price"
F 10 "1" V 9935 5650 20  0001 C CNN "Supplier Price Break"
	1    9600 5800
	1    0    0    -1  
$EndComp
Connection ~ 9600 5800
Connection ~ 9600 6100
$Comp
L appli_inductor:10u2_PA2050 L?
U 1 1 5ADD7CF3
P 10450 5050
F 0 "L?" V 10263 4900 55  0000 C CNN
F 1 "10u2_PA2050" V 10347 4900 35  0000 C CNN
F 2 "Applidyne_Inductor:PULSE_PA2050" V 10565 4900 20  0001 C CNN
F 3 "https://au.mouser.com/datasheet/2/336/-369081.pdf" V 10590 4900 20  0001 C CNN
F 4 "PULSE ELECTRONICS" V 10640 4900 20  0001 C CNN "manf"
F 5 "PA2050.103NL" V 10950 4900 20  0001 C CNN "manf#"
F 6 "Mouser" V 10690 4900 20  0001 C CNN "Supplier"
F 7 "673-PA2050.103NL" V 10850 4900 20  0001 C CNN "Supplier Part No"
F 8 "https://au.mouser.com/ProductDetail/Pulse-Electronics/PA2050103NL" V 10900 4900 20  0001 C CNN "Supplier URL"
F 9 "5.59" V 10765 4900 20  0001 C CNN "Supplier Price"
F 10 "1" V 10790 4900 20  0001 C CNN "Supplier Price Break"
	1    10450 5050
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
	12400 5050 12800 5050
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
$EndSCHEMATC
