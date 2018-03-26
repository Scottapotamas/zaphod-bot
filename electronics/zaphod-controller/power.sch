EESchema Schematic File Version 4
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
$EndSCHEMATC
