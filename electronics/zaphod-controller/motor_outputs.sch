EESchema Schematic File Version 4
LIBS:zaphod-controller-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 5
Title "Servo Interfaces"
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 2300 850  0    79   ~ 16
Servo Power Connectors
Text Notes 7450 875  0    79   ~ 16
Servo Data Connectors
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5AB91CBE
P 2450 1400
F 0 "#PWR?" H 2450 1360 30  0001 C CNN
F 1 "+3.3V" H 2459 1538 30  0000 C CNN
F 2 "" H 2450 1400 60  0000 C CNN
F 3 "" H 2450 1400 60  0000 C CNN
	1    2450 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 1900 2450 1550
Wire Wire Line
	2450 1550 2600 1550
Connection ~ 2450 1550
Wire Wire Line
	2450 1550 2450 1400
Wire Wire Line
	3100 2250 3100 2300
Wire Wire Line
	2450 2100 2450 2350
$Comp
L appli_power:GND #PWR?
U 1 1 5AB91D62
P 3100 2350
F 0 "#PWR?" H 3100 2100 50  0001 C CNN
F 1 "GND" H 3105 2177 50  0000 C CNN
F 2 "" H 3100 2350 60  0000 C CNN
F 3 "" H 3100 2350 60  0000 C CNN
	1    3100 2350
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5AB91D78
P 2450 2350
F 0 "#PWR?" H 2450 2100 50  0001 C CNN
F 1 "GND" H 2455 2177 50  0000 C CNN
F 2 "" H 2450 2350 60  0000 C CNN
F 3 "" H 2450 2350 60  0000 C CNN
	1    2450 2350
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+POWER #PWR?
U 1 1 5AB91E53
P 3750 1400
F 0 "#PWR?" H 3750 1350 20  0001 C CNN
F 1 "+POWER" H 3753 1538 30  0000 C CNN
F 2 "" H 3750 1400 60  0000 C CNN
F 3 "" H 3750 1400 60  0000 C CNN
	1    3750 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 1400 3750 1550
Wire Wire Line
	3750 1550 3600 1550
$Comp
L appli_special:ACS711KEXLT_15AB U?
U 1 1 5AB955A4
P 3100 1750
F 0 "U?" H 3100 2215 50  0000 C CNN
F 1 "ACS711KEXLT_15AB" H 3100 2124 50  0000 C CNN
F 2 "Applidyne_QFN:ACS-EX-12" H 3100 950 50  0001 C CNN
F 3 "http://www.allegromicro.com/~/media/Files/Datasheets/ACS711-Datasheet.ashx" H 3100 650 50  0001 C CNN
F 4 "Allegro MicroSystems" H 3100 850 50  0001 C CNN "manf"
F 5 "ACS711KEXLT-15AB-T" H 3100 750 50  0001 C CNN "manf#"
F 6 "DigiKey" H 3100 550 50  0001 C CNN "Supplier"
F 7 "620-1483-1-ND" H 3100 450 50  0001 C CNN "Suppler Part No"
F 8 "https://www.digikey.com.au/product-detail/en/allegro-microsystems-llc/ACS711KEXLT-15AB-T/620-1483-1-ND/3868192" H 3100 350 50  0001 C CNN "Supplier URL"
F 9 "1.90" H 3100 250 50  0001 C CNN "Supplier Price"
F 10 "1" H 3100 150 50  0001 C CNN "Supplier Price Break"
	1    3100 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 2250 2900 2300
Wire Wire Line
	2900 2300 3000 2300
Connection ~ 3100 2300
Wire Wire Line
	3100 2300 3100 2350
Wire Wire Line
	3300 2250 3300 2300
Wire Wire Line
	3300 2300 3200 2300
Wire Wire Line
	3200 2250 3200 2300
Connection ~ 3200 2300
Wire Wire Line
	3200 2300 3100 2300
Wire Wire Line
	3000 2250 3000 2300
Connection ~ 3000 2300
Wire Wire Line
	3000 2300 3100 2300
Text Notes 3350 2275 0    50   ~ 0
NC->GND for \nbetter EMI per DS
Text Notes 650  1275 0    50   ~ 0
90mV/A sensitivity.\n3.3V/2 = 0A
Wire Wire Line
	2600 1650 1700 1650
Wire Wire Line
	2600 1750 2200 1750
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5AB96FA3
P 2200 1300
F 0 "#PWR?" H 2200 1260 30  0001 C CNN
F 1 "+3.3V" H 2209 1438 30  0000 C CNN
F 2 "" H 2200 1300 60  0000 C CNN
F 3 "" H 2200 1300 60  0000 C CNN
	1    2200 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	2200 1300 2200 1325
Wire Wire Line
	2200 1625 2200 1750
Connection ~ 2200 1750
Wire Wire Line
	2200 1750 1300 1750
Text GLabel 1300 1525 0    50   Output ~ 0
MOTOR_1_CURRENT
Text GLabel 1300 1750 0    50   Output ~ 0
~MOTOR_1_OC
Text Notes 650  2075 0    50   ~ 0
FAULT latches low when\ncurrent > 15A rating
Wire Wire Line
	1700 1650 1700 1525
Wire Wire Line
	1700 1525 1650 1525
$Comp
L appli_power:GND #PWR?
U 1 1 5AB9A0AB
P 4250 2350
F 0 "#PWR?" H 4250 2100 50  0001 C CNN
F 1 "GND" H 4255 2177 50  0000 C CNN
F 2 "" H 4250 2350 60  0000 C CNN
F 3 "" H 4250 2350 60  0000 C CNN
	1    4250 2350
	1    0    0    -1  
$EndComp
$Comp
L appli_connector_molex:MOLEX_MINIFIT_JR_2x2_WAY P?
U 1 1 5AB9CD10
P 4600 1700
F 0 "P?" H 4678 1730 50  0000 L CNN
F 1 "MOLEX_MINIFIT_JR_2x2_WAY" H 4678 1650 35  0000 L CNN
F 2 "Applidyne_Connector_Molex:Molex_Mini-Fit_Jr_5566-04A_2x02_P4.20mm_Vertical" V 4775 1700 20  0001 C CNN
F 3 "https://www.molex.com/pdm_docs/sd/1724470002_sd.pdf" V 4800 1700 20  0001 C CNN
F 4 "MOLEX" V 4850 1700 20  0001 C CNN "manf"
F 5 "172447-0204" V 4875 1700 20  0001 C CNN "manf#"
F 6 "DigiKey" V 4900 1700 20  0001 C CNN "Supplier"
F 7 "WM14942-ND" V 4925 1700 20  0001 C CNN "Supplier Part No"
F 8 "https://www.digikey.com.au/product-detail/en/1724470204/WM14942-ND/5116909?utm_campaign=buynow&WT.z_cid=ref_octopart_dkc_buynow&utm_medium=aggregator&curr=usd&site=us&utm_source=octopart" V 4950 1700 20  0001 C CNN "Supplier URL"
F 9 "1.66" V 4975 1700 20  0001 C CNN "Supplier Price"
F 10 "1" V 5000 1700 20  0001 C CNN "Supplier Price Break"
	1    4600 1700
	1    0    0    -1  
$EndComp
$Comp
L appli_connector_molex:MOLEX_MINIFIT_JR_2x4_WAY P?
U 1 1 5AB9CF32
P 10225 2050
F 0 "P?" H 10200 2650 50  0000 L CNN
F 1 "MOLEX_MINIFIT_JR_2x4_WAY" H 10200 2550 35  0000 L CNN
F 2 "Applidyne_Connector_Molex:Molex_MiniFit-JR-5569-08A2_2x04x4.20mm_Angled" V 10400 2050 20  0001 C CNN
F 3 "https://www.molex.com/pdm_docs/sd/1724470002_sd.pdf" V 10425 2050 20  0001 C CNN
F 4 "MOLEX" V 10475 2050 20  0001 C CNN "manf"
F 5 "172447-0208" V 10500 2050 20  0001 C CNN "manf#"
F 6 "Mouser" V 10525 2050 20  0001 C CNN "Supplier"
F 7 "538-172447-0208" V 10550 2050 20  0001 C CNN "Supplier Part No"
F 8 "https://au.mouser.com/ProductDetail/Molex/172447-0208?qs=sGAEpiMZZMvc81WFyF5Edkd3jpEQlJBQt6f989MIPuM%3d" V 10575 2050 20  0001 C CNN "Supplier URL"
F 9 "1.78" V 10600 2050 20  0001 C CNN "Supplier Price"
F 10 "1" V 10625 2050 20  0001 C CNN "Supplier Price Break"
	1    10225 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 1850 4400 1850
Wire Wire Line
	4250 1850 4250 2350
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5AB9FC1B
P 2450 2975
F 0 "#PWR?" H 2450 2935 30  0001 C CNN
F 1 "+3.3V" H 2459 3113 30  0000 C CNN
F 2 "" H 2450 2975 60  0000 C CNN
F 3 "" H 2450 2975 60  0000 C CNN
	1    2450 2975
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 3475 2450 3125
Wire Wire Line
	2450 3125 2600 3125
Connection ~ 2450 3125
Wire Wire Line
	2450 3125 2450 2975
Wire Wire Line
	3100 3825 3100 3875
Wire Wire Line
	2450 3675 2450 3925
$Comp
L appli_power:GND #PWR?
U 1 1 5AB9FC27
P 3100 3925
F 0 "#PWR?" H 3100 3675 50  0001 C CNN
F 1 "GND" H 3105 3752 50  0000 C CNN
F 2 "" H 3100 3925 60  0000 C CNN
F 3 "" H 3100 3925 60  0000 C CNN
	1    3100 3925
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5AB9FC2D
P 2450 3925
F 0 "#PWR?" H 2450 3675 50  0001 C CNN
F 1 "GND" H 2455 3752 50  0000 C CNN
F 2 "" H 2450 3925 60  0000 C CNN
F 3 "" H 2450 3925 60  0000 C CNN
	1    2450 3925
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+POWER #PWR?
U 1 1 5AB9FC33
P 3750 2975
F 0 "#PWR?" H 3750 2925 20  0001 C CNN
F 1 "+POWER" H 3753 3113 30  0000 C CNN
F 2 "" H 3750 2975 60  0000 C CNN
F 3 "" H 3750 2975 60  0000 C CNN
	1    3750 2975
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 2975 3750 3125
Wire Wire Line
	3750 3125 3600 3125
$Comp
L appli_special:ACS711KEXLT_15AB U?
U 1 1 5AB9FC42
P 3100 3325
F 0 "U?" H 3100 3790 50  0000 C CNN
F 1 "ACS711KEXLT_15AB" H 3100 3699 50  0000 C CNN
F 2 "Applidyne_QFN:ACS-EX-12" H 3100 2525 50  0001 C CNN
F 3 "http://www.allegromicro.com/~/media/Files/Datasheets/ACS711-Datasheet.ashx" H 3100 2225 50  0001 C CNN
F 4 "Allegro MicroSystems" H 3100 2425 50  0001 C CNN "manf"
F 5 "ACS711KEXLT-15AB-T" H 3100 2325 50  0001 C CNN "manf#"
F 6 "DigiKey" H 3100 2125 50  0001 C CNN "Supplier"
F 7 "620-1483-1-ND" H 3100 2025 50  0001 C CNN "Suppler Part No"
F 8 "https://www.digikey.com.au/product-detail/en/allegro-microsystems-llc/ACS711KEXLT-15AB-T/620-1483-1-ND/3868192" H 3100 1925 50  0001 C CNN "Supplier URL"
F 9 "1.90" H 3100 1825 50  0001 C CNN "Supplier Price"
F 10 "1" H 3100 1725 50  0001 C CNN "Supplier Price Break"
	1    3100 3325
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 3825 2900 3875
Wire Wire Line
	2900 3875 3000 3875
Connection ~ 3100 3875
Wire Wire Line
	3100 3875 3100 3925
Wire Wire Line
	3300 3825 3300 3875
Wire Wire Line
	3300 3875 3200 3875
Wire Wire Line
	3200 3825 3200 3875
Connection ~ 3200 3875
Wire Wire Line
	3200 3875 3100 3875
Wire Wire Line
	3000 3825 3000 3875
Connection ~ 3000 3875
Wire Wire Line
	3000 3875 3100 3875
Wire Wire Line
	2600 3225 1750 3225
Wire Wire Line
	2600 3325 2200 3325
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5AB9FC67
P 2200 2875
F 0 "#PWR?" H 2200 2835 30  0001 C CNN
F 1 "+3.3V" H 2209 3013 30  0000 C CNN
F 2 "" H 2200 2875 60  0000 C CNN
F 3 "" H 2200 2875 60  0000 C CNN
	1    2200 2875
	1    0    0    -1  
$EndComp
Wire Wire Line
	2200 2875 2200 2900
Wire Wire Line
	2200 3200 2200 3325
Connection ~ 2200 3325
Wire Wire Line
	2200 3325 1300 3325
Text GLabel 1300 3075 0    50   Output ~ 0
MOTOR_2_CURRENT
Text GLabel 1300 3325 0    50   Output ~ 0
~MOTOR_2_OC
Wire Wire Line
	1750 3225 1750 3075
Wire Wire Line
	1750 3075 1700 3075
$Comp
L appli_power:GND #PWR?
U 1 1 5AB9FC76
P 4250 3925
F 0 "#PWR?" H 4250 3675 50  0001 C CNN
F 1 "GND" H 4255 3752 50  0000 C CNN
F 2 "" H 4250 3925 60  0000 C CNN
F 3 "" H 4250 3925 60  0000 C CNN
	1    4250 3925
	1    0    0    -1  
$EndComp
$Comp
L appli_connector_molex:MOLEX_MINIFIT_JR_2x2_WAY P?
U 1 1 5AB9FC83
P 4600 3275
F 0 "P?" H 4678 3305 50  0000 L CNN
F 1 "MOLEX_MINIFIT_JR_2x2_WAY" H 4678 3225 35  0000 L CNN
F 2 "Applidyne_Connector_Molex:Molex_Mini-Fit_Jr_5566-04A_2x02_P4.20mm_Vertical" V 4775 3275 20  0001 C CNN
F 3 "https://www.molex.com/pdm_docs/sd/1724470002_sd.pdf" V 4800 3275 20  0001 C CNN
F 4 "MOLEX" V 4850 3275 20  0001 C CNN "manf"
F 5 "172447-0204" V 4875 3275 20  0001 C CNN "manf#"
F 6 "DigiKey" V 4900 3275 20  0001 C CNN "Supplier"
F 7 "WM14942-ND" V 4925 3275 20  0001 C CNN "Supplier Part No"
F 8 "https://www.digikey.com.au/product-detail/en/1724470204/WM14942-ND/5116909?utm_campaign=buynow&WT.z_cid=ref_octopart_dkc_buynow&utm_medium=aggregator&curr=usd&site=us&utm_source=octopart" V 4950 3275 20  0001 C CNN "Supplier URL"
F 9 "1.66" V 4975 3275 20  0001 C CNN "Supplier Price"
F 10 "1" V 5000 3275 20  0001 C CNN "Supplier Price Break"
	1    4600 3275
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 3425 4400 3425
Wire Wire Line
	4250 3425 4250 3925
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5ABA0C8E
P 2450 4575
F 0 "#PWR?" H 2450 4535 30  0001 C CNN
F 1 "+3.3V" H 2459 4713 30  0000 C CNN
F 2 "" H 2450 4575 60  0000 C CNN
F 3 "" H 2450 4575 60  0000 C CNN
	1    2450 4575
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 5075 2450 4725
Wire Wire Line
	2450 4725 2600 4725
Connection ~ 2450 4725
Wire Wire Line
	2450 4725 2450 4575
Wire Wire Line
	3100 5425 3100 5475
Wire Wire Line
	2450 5275 2450 5525
$Comp
L appli_power:GND #PWR?
U 1 1 5ABA0C9A
P 3100 5525
F 0 "#PWR?" H 3100 5275 50  0001 C CNN
F 1 "GND" H 3105 5352 50  0000 C CNN
F 2 "" H 3100 5525 60  0000 C CNN
F 3 "" H 3100 5525 60  0000 C CNN
	1    3100 5525
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5ABA0CA0
P 2450 5525
F 0 "#PWR?" H 2450 5275 50  0001 C CNN
F 1 "GND" H 2455 5352 50  0000 C CNN
F 2 "" H 2450 5525 60  0000 C CNN
F 3 "" H 2450 5525 60  0000 C CNN
	1    2450 5525
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+POWER #PWR?
U 1 1 5ABA0CA6
P 3750 4575
F 0 "#PWR?" H 3750 4525 20  0001 C CNN
F 1 "+POWER" H 3753 4713 30  0000 C CNN
F 2 "" H 3750 4575 60  0000 C CNN
F 3 "" H 3750 4575 60  0000 C CNN
	1    3750 4575
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 4575 3750 4725
Wire Wire Line
	3750 4725 3600 4725
$Comp
L appli_special:ACS711KEXLT_15AB U?
U 1 1 5ABA0CB5
P 3100 4925
F 0 "U?" H 3100 5390 50  0000 C CNN
F 1 "ACS711KEXLT_15AB" H 3100 5299 50  0000 C CNN
F 2 "Applidyne_QFN:ACS-EX-12" H 3100 4125 50  0001 C CNN
F 3 "http://www.allegromicro.com/~/media/Files/Datasheets/ACS711-Datasheet.ashx" H 3100 3825 50  0001 C CNN
F 4 "Allegro MicroSystems" H 3100 4025 50  0001 C CNN "manf"
F 5 "ACS711KEXLT-15AB-T" H 3100 3925 50  0001 C CNN "manf#"
F 6 "DigiKey" H 3100 3725 50  0001 C CNN "Supplier"
F 7 "620-1483-1-ND" H 3100 3625 50  0001 C CNN "Suppler Part No"
F 8 "https://www.digikey.com.au/product-detail/en/allegro-microsystems-llc/ACS711KEXLT-15AB-T/620-1483-1-ND/3868192" H 3100 3525 50  0001 C CNN "Supplier URL"
F 9 "1.90" H 3100 3425 50  0001 C CNN "Supplier Price"
F 10 "1" H 3100 3325 50  0001 C CNN "Supplier Price Break"
	1    3100 4925
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 5425 2900 5475
Wire Wire Line
	2900 5475 3000 5475
Connection ~ 3100 5475
Wire Wire Line
	3100 5475 3100 5525
Wire Wire Line
	3300 5425 3300 5475
Wire Wire Line
	3300 5475 3200 5475
Wire Wire Line
	3200 5425 3200 5475
Connection ~ 3200 5475
Wire Wire Line
	3200 5475 3100 5475
Wire Wire Line
	3000 5425 3000 5475
Connection ~ 3000 5475
Wire Wire Line
	3000 5475 3100 5475
Wire Wire Line
	2600 4925 2200 4925
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5ABA0CDA
P 2200 4475
F 0 "#PWR?" H 2200 4435 30  0001 C CNN
F 1 "+3.3V" H 2209 4613 30  0000 C CNN
F 2 "" H 2200 4475 60  0000 C CNN
F 3 "" H 2200 4475 60  0000 C CNN
	1    2200 4475
	1    0    0    -1  
$EndComp
Wire Wire Line
	2200 4775 2200 4925
$Comp
L appli_power:GND #PWR?
U 1 1 5ABA0CE9
P 4250 5525
F 0 "#PWR?" H 4250 5275 50  0001 C CNN
F 1 "GND" H 4255 5352 50  0000 C CNN
F 2 "" H 4250 5525 60  0000 C CNN
F 3 "" H 4250 5525 60  0000 C CNN
	1    4250 5525
	1    0    0    -1  
$EndComp
$Comp
L appli_connector_molex:MOLEX_MINIFIT_JR_2x2_WAY P?
U 1 1 5ABA0CF6
P 4600 4875
F 0 "P?" H 4678 4905 50  0000 L CNN
F 1 "MOLEX_MINIFIT_JR_2x2_WAY" H 4678 4825 35  0000 L CNN
F 2 "Applidyne_Connector_Molex:Molex_Mini-Fit_Jr_5566-04A_2x02_P4.20mm_Vertical" V 4775 4875 20  0001 C CNN
F 3 "https://www.molex.com/pdm_docs/sd/1724470002_sd.pdf" V 4800 4875 20  0001 C CNN
F 4 "MOLEX" V 4850 4875 20  0001 C CNN "manf"
F 5 "172447-0204" V 4875 4875 20  0001 C CNN "manf#"
F 6 "DigiKey" V 4900 4875 20  0001 C CNN "Supplier"
F 7 "WM14942-ND" V 4925 4875 20  0001 C CNN "Supplier Part No"
F 8 "https://www.digikey.com.au/product-detail/en/1724470204/WM14942-ND/5116909?utm_campaign=buynow&WT.z_cid=ref_octopart_dkc_buynow&utm_medium=aggregator&curr=usd&site=us&utm_source=octopart" V 4950 4875 20  0001 C CNN "Supplier URL"
F 9 "1.66" V 4975 4875 20  0001 C CNN "Supplier Price"
F 10 "1" V 5000 4875 20  0001 C CNN "Supplier Price Break"
	1    4600 4875
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 5025 4400 5025
Wire Wire Line
	4250 5025 4250 5525
Text Label 8950 1100 0    50   ~ 0
HLFB-
Text Label 9675 2200 0    50   ~ 0
INPUTB-
Text Label 9650 2300 0    50   ~ 0
INPUTA-
Text Label 9650 2400 0    50   ~ 0
ENABLE-
$Comp
L appli_power:+12V #PWR?
U 1 1 5ABA4403
P 9475 1550
F 0 "#PWR?" H 9475 1400 50  0001 C CNN
F 1 "+12V" H 9490 1723 50  0000 C CNN
F 2 "" H 9475 1550 60  0000 C CNN
F 3 "" H 9475 1550 60  0000 C CNN
	1    9475 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	10025 1800 9475 1800
Wire Wire Line
	10025 1900 9475 1900
Wire Wire Line
	9475 1900 9475 1800
Wire Wire Line
	10025 2000 9475 2000
Wire Wire Line
	9475 2000 9475 1900
Connection ~ 9475 1900
Wire Wire Line
	9475 1800 9475 1700
Connection ~ 9475 1800
Wire Wire Line
	10025 2100 9375 2100
Wire Wire Line
	9375 2100 9375 1100
Wire Wire Line
	9375 1100 8875 1100
$Comp
L Transistor_Array:ULN2803A U?
U 1 1 5AC9AE17
P 7875 3050
F 0 "U?" H 7875 3617 50  0000 C CNN
F 1 "ULN2803A" H 7875 3526 50  0000 C CNN
F 2 "" H 7925 2400 50  0001 L CNN
F 3 "http://www.ti.com/lit/ds/symlink/uln2803a.pdf" H 7975 2850 50  0001 C CNN
	1    7875 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	8875 2400 10025 2400
Wire Wire Line
	8775 2300 10025 2300
Wire Wire Line
	8675 2200 10025 2200
$Comp
L appli_connector_molex:MOLEX_MINIFIT_JR_2x4_WAY P?
U 1 1 5ACB982C
P 10225 3000
F 0 "P?" H 10303 3030 50  0000 L CNN
F 1 "MOLEX_MINIFIT_JR_2x4_WAY" H 10303 2950 35  0000 L CNN
F 2 "Applidyne_Connector_Molex:Molex_MiniFit-JR-5569-08A2_2x04x4.20mm_Angled" V 10400 3000 20  0001 C CNN
F 3 "https://www.molex.com/pdm_docs/sd/1724470002_sd.pdf" V 10425 3000 20  0001 C CNN
F 4 "MOLEX" V 10475 3000 20  0001 C CNN "manf"
F 5 "172447-0208" V 10500 3000 20  0001 C CNN "manf#"
F 6 "Mouser" V 10525 3000 20  0001 C CNN "Supplier"
F 7 "538-172447-0208" V 10550 3000 20  0001 C CNN "Supplier Part No"
F 8 "https://au.mouser.com/ProductDetail/Molex/172447-0208?qs=sGAEpiMZZMvc81WFyF5Edkd3jpEQlJBQt6f989MIPuM%3d" V 10575 3000 20  0001 C CNN "Supplier URL"
F 9 "1.78" V 10600 3000 20  0001 C CNN "Supplier Price"
F 10 "1" V 10625 3000 20  0001 C CNN "Supplier Price Break"
	1    10225 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	10025 2750 9475 2750
Wire Wire Line
	10025 2850 9475 2850
Wire Wire Line
	9475 2850 9475 2750
Wire Wire Line
	10025 2950 9475 2950
Wire Wire Line
	9475 2950 9475 2850
Connection ~ 9475 2850
Wire Wire Line
	9475 2750 9475 2650
Connection ~ 9475 2750
Connection ~ 9475 2000
Wire Wire Line
	10025 2650 9475 2650
Wire Wire Line
	10025 3050 9175 3050
Wire Wire Line
	10025 3150 8275 3150
Wire Wire Line
	8275 3250 10025 3250
Wire Wire Line
	10025 3350 8275 3350
Wire Wire Line
	8875 2400 8875 3050
Wire Wire Line
	8875 3050 8275 3050
Wire Wire Line
	8275 2950 8775 2950
Wire Wire Line
	8775 2950 8775 2300
Wire Wire Line
	8275 2850 8675 2850
Wire Wire Line
	8675 2850 8675 2200
Wire Wire Line
	8275 2750 8475 2750
Wire Wire Line
	8475 2750 8475 2575
$Comp
L appli_power:+12V #PWR?
U 1 1 5AD08F43
P 8475 2575
F 0 "#PWR?" H 8475 2425 50  0001 C CNN
F 1 "+12V" H 8490 2748 50  0000 C CNN
F 2 "" H 8475 2575 60  0000 C CNN
F 3 "" H 8475 2575 60  0000 C CNN
	1    8475 2575
	1    0    0    -1  
$EndComp
Wire Wire Line
	7475 2850 7125 2850
Wire Wire Line
	7475 2950 7125 2950
Wire Wire Line
	7475 3050 7125 3050
Wire Wire Line
	7475 3150 7125 3150
Wire Wire Line
	7475 3250 7125 3250
Wire Wire Line
	7475 3350 7125 3350
Wire Wire Line
	7875 3750 7875 3800
$Comp
L appli_power:GND #PWR?
U 1 1 5AD43F33
P 7875 3800
F 0 "#PWR?" H 7875 3550 50  0001 C CNN
F 1 "GND" H 7880 3627 50  0000 C CNN
F 2 "" H 7875 3800 60  0000 C CNN
F 3 "" H 7875 3800 60  0000 C CNN
	1    7875 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	9175 1900 8875 1900
Wire Wire Line
	9175 1900 9175 3050
Text Label 8925 1900 0    50   ~ 0
HLFB-
Text Label 9675 3150 0    50   ~ 0
INPUTB-
Text Label 9675 3250 0    50   ~ 0
INPUTA-
Text Label 9675 3350 0    50   ~ 0
ENABLE-
Text Notes 10325 2450 0    63   ~ 0
HLFB+\nINPUTB+\nINPUTA+\nENABLE+\nHLFB-\nINPUTB-\nINPUTA-\nENABLE-\n
Text GLabel 7125 2950 0    50   Input ~ 0
SERVO1_A
Text GLabel 7125 2850 0    50   Input ~ 0
SERVO1_B
Text GLabel 7125 3050 0    50   Input ~ 0
SERVO1_EN
Text GLabel 7125 3250 0    50   Input ~ 0
SERVO2_A
Text GLabel 7125 3150 0    50   Input ~ 0
SERVO2_B
Text GLabel 7125 3350 0    50   Input ~ 0
SERVO2_EN
Text GLabel 6775 2225 0    50   Output ~ 0
SERVO2_HLFB
Wire Wire Line
	5850 1125 5850 1150
Wire Wire Line
	10025 1700 9475 1700
Connection ~ 9475 1700
Wire Wire Line
	9475 1700 9475 1550
Connection ~ 9475 2650
Wire Wire Line
	9475 2650 9475 2000
Wire Wire Line
	3600 3325 4400 3325
Wire Wire Line
	3600 1750 4400 1750
Wire Wire Line
	3600 4925 4400 4925
$Comp
L appli_capacitor:100NF_X7R_1608M C?
U 1 1 5ABF1105
P 5850 925
F 0 "C?" H 5928 860 50  0000 L CNN
F 1 "100NF_X7R_1608M" H 5928 780 35  0000 L CNN
F 2 "CAPC1608*" V 5965 825 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1732728.pdf" V 5990 825 20  0001 C CNN
F 4 "KEMET" V 6040 825 20  0001 C CNN "manf"
F 5 "C0603C104K5RACAUTO" V 6065 825 20  0001 C CNN "manf#"
F 6 "Element14" V 6090 825 20  0001 C CNN "Supplier"
F 7 "2070398" V 6115 825 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c0603c104k5racauto/cap-mlcc-x7r-100nf-50v-0603/dp/2070398" V 6140 825 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 6165 825 20  0001 C CNN "Supplier Price"
F 10 "1" V 6190 825 20  0001 C CNN "Supplier Price Break"
	1    5850 925 
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5AC0C492
P 5850 1150
F 0 "#PWR?" H 5850 900 50  0001 C CNN
F 1 "GND" H 5855 977 50  0000 C CNN
F 2 "" H 5850 1150 60  0000 C CNN
F 3 "" H 5850 1150 60  0000 C CNN
	1    5850 1150
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+12V #PWR?
U 1 1 5AC11C0D
P 5850 875
F 0 "#PWR?" H 5850 725 50  0001 C CNN
F 1 "+12V" H 5865 1048 50  0000 C CNN
F 2 "" H 5850 875 60  0000 C CNN
F 3 "" H 5850 875 60  0000 C CNN
	1    5850 875 
	1    0    0    -1  
$EndComp
Wire Wire Line
	5850 875  5850 925 
$Comp
L appli_capacitor:100NF_X7R_1608M C?
U 1 1 5ABCECAE
P 2450 3675
F 0 "C?" H 2373 3540 50  0000 R CNN
F 1 "100NF_X7R_1608M" H 2373 3620 35  0000 R CNN
F 2 "CAPC1608*" V 2565 3575 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1732728.pdf" V 2590 3575 20  0001 C CNN
F 4 "KEMET" V 2640 3575 20  0001 C CNN "manf"
F 5 "C0603C104K5RACAUTO" V 2665 3575 20  0001 C CNN "manf#"
F 6 "Element14" V 2690 3575 20  0001 C CNN "Supplier"
F 7 "2070398" V 2715 3575 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c0603c104k5racauto/cap-mlcc-x7r-100nf-50v-0603/dp/2070398" V 2740 3575 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 2765 3575 20  0001 C CNN "Supplier Price"
F 10 "1" V 2790 3575 20  0001 C CNN "Supplier Price Break"
	1    2450 3675
	1    0    0    1   
$EndComp
$Comp
L appli_resistor:10K0_1608M R?
U 1 1 5ABCEE82
P 2200 3200
F 0 "R?" H 2128 3015 50  0000 R CNN
F 1 "10K0_1608M" H 2128 3095 35  0000 R CNN
F 2 "RESC1608*" V 2310 3050 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 2335 3050 20  0001 C CNN
F 4 "TE CONNECTIVITY" V 2385 3050 20  0001 C CNN "manf"
F 5 "CRGH0603F10K" V 2410 3050 20  0001 C CNN "manf#"
F 6 "Element14" V 2435 3050 20  0001 C CNN "Supplier"
F 7 "2332016" V 2460 3050 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/te-connectivity/crgh0603f10k/resistor-power-10k-0-2w-1-0603/dp/2332016" V 2485 3050 20  0001 C CNN "Supplier URL"
F 9 "0.02" V 2510 3050 20  0001 C CNN "Supplier Price"
F 10 "10" V 2535 3050 20  0001 C CNN "Supplier Price Break"
	1    2200 3200
	1    0    0    1   
$EndComp
$Comp
L appli_capacitor:100NF_X7R_1608M C?
U 1 1 5ABCF127
P 2450 2100
F 0 "C?" H 2373 1965 50  0000 R CNN
F 1 "100NF_X7R_1608M" H 2373 2045 35  0000 R CNN
F 2 "CAPC1608*" V 2565 2000 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1732728.pdf" V 2590 2000 20  0001 C CNN
F 4 "KEMET" V 2640 2000 20  0001 C CNN "manf"
F 5 "C0603C104K5RACAUTO" V 2665 2000 20  0001 C CNN "manf#"
F 6 "Element14" V 2690 2000 20  0001 C CNN "Supplier"
F 7 "2070398" V 2715 2000 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c0603c104k5racauto/cap-mlcc-x7r-100nf-50v-0603/dp/2070398" V 2740 2000 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 2765 2000 20  0001 C CNN "Supplier Price"
F 10 "1" V 2790 2000 20  0001 C CNN "Supplier Price Break"
	1    2450 2100
	1    0    0    1   
$EndComp
$Comp
L appli_resistor:10K0_1608M R?
U 1 1 5ABCF17B
P 2200 1625
F 0 "R?" H 2128 1440 50  0000 R CNN
F 1 "10K0_1608M" H 2128 1520 35  0000 R CNN
F 2 "RESC1608*" V 2310 1475 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 2335 1475 20  0001 C CNN
F 4 "TE CONNECTIVITY" V 2385 1475 20  0001 C CNN "manf"
F 5 "CRGH0603F10K" V 2410 1475 20  0001 C CNN "manf#"
F 6 "Element14" V 2435 1475 20  0001 C CNN "Supplier"
F 7 "2332016" V 2460 1475 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/te-connectivity/crgh0603f10k/resistor-power-10k-0-2w-1-0603/dp/2332016" V 2485 1475 20  0001 C CNN "Supplier URL"
F 9 "0.02" V 2510 1475 20  0001 C CNN "Supplier Price"
F 10 "10" V 2535 1475 20  0001 C CNN "Supplier Price Break"
	1    2200 1625
	1    0    0    1   
$EndComp
$Comp
L appli_resistor:10K0_1608M R?
U 1 1 5ABCF1CD
P 2200 4775
F 0 "R?" H 2128 4590 50  0000 R CNN
F 1 "10K0_1608M" H 2128 4670 35  0000 R CNN
F 2 "RESC1608*" V 2310 4625 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 2335 4625 20  0001 C CNN
F 4 "TE CONNECTIVITY" V 2385 4625 20  0001 C CNN "manf"
F 5 "CRGH0603F10K" V 2410 4625 20  0001 C CNN "manf#"
F 6 "Element14" V 2435 4625 20  0001 C CNN "Supplier"
F 7 "2332016" V 2460 4625 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/te-connectivity/crgh0603f10k/resistor-power-10k-0-2w-1-0603/dp/2332016" V 2485 4625 20  0001 C CNN "Supplier URL"
F 9 "0.02" V 2510 4625 20  0001 C CNN "Supplier Price"
F 10 "10" V 2535 4625 20  0001 C CNN "Supplier Price Break"
	1    2200 4775
	1    0    0    1   
$EndComp
$Comp
L appli_capacitor:100NF_X7R_1608M C?
U 1 1 5ABCF21D
P 2450 5275
F 0 "C?" H 2373 5140 50  0000 R CNN
F 1 "100NF_X7R_1608M" H 2373 5220 35  0000 R CNN
F 2 "CAPC1608*" V 2565 5175 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1732728.pdf" V 2590 5175 20  0001 C CNN
F 4 "KEMET" V 2640 5175 20  0001 C CNN "manf"
F 5 "C0603C104K5RACAUTO" V 2665 5175 20  0001 C CNN "manf#"
F 6 "Element14" V 2690 5175 20  0001 C CNN "Supplier"
F 7 "2070398" V 2715 5175 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c0603c104k5racauto/cap-mlcc-x7r-100nf-50v-0603/dp/2070398" V 2740 5175 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 2765 5175 20  0001 C CNN "Supplier Price"
F 10 "1" V 2790 5175 20  0001 C CNN "Supplier Price Break"
	1    2450 5275
	1    0    0    1   
$EndComp
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5ABEC55A
P 2450 6250
F 0 "#PWR?" H 2450 6210 30  0001 C CNN
F 1 "+3.3V" H 2459 6388 30  0000 C CNN
F 2 "" H 2450 6250 60  0000 C CNN
F 3 "" H 2450 6250 60  0000 C CNN
	1    2450 6250
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 6750 2450 6400
Wire Wire Line
	2450 6400 2600 6400
Connection ~ 2450 6400
Wire Wire Line
	2450 6400 2450 6250
Wire Wire Line
	3100 7100 3100 7150
Wire Wire Line
	2450 6950 2450 7200
$Comp
L appli_power:GND #PWR?
U 1 1 5ABEC566
P 3100 7200
F 0 "#PWR?" H 3100 6950 50  0001 C CNN
F 1 "GND" H 3105 7027 50  0000 C CNN
F 2 "" H 3100 7200 60  0000 C CNN
F 3 "" H 3100 7200 60  0000 C CNN
	1    3100 7200
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5ABEC56C
P 2450 7200
F 0 "#PWR?" H 2450 6950 50  0001 C CNN
F 1 "GND" H 2455 7027 50  0000 C CNN
F 2 "" H 2450 7200 60  0000 C CNN
F 3 "" H 2450 7200 60  0000 C CNN
	1    2450 7200
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+POWER #PWR?
U 1 1 5ABEC572
P 3750 6250
F 0 "#PWR?" H 3750 6200 20  0001 C CNN
F 1 "+POWER" H 3753 6388 30  0000 C CNN
F 2 "" H 3750 6250 60  0000 C CNN
F 3 "" H 3750 6250 60  0000 C CNN
	1    3750 6250
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 6250 3750 6400
Wire Wire Line
	3750 6400 3600 6400
$Comp
L appli_special:ACS711KEXLT_15AB U?
U 1 1 5ABEC581
P 3100 6600
F 0 "U?" H 3100 7065 50  0000 C CNN
F 1 "ACS711KEXLT_15AB" H 3100 6974 50  0000 C CNN
F 2 "Applidyne_QFN:ACS-EX-12" H 3100 5800 50  0001 C CNN
F 3 "http://www.allegromicro.com/~/media/Files/Datasheets/ACS711-Datasheet.ashx" H 3100 5500 50  0001 C CNN
F 4 "Allegro MicroSystems" H 3100 5700 50  0001 C CNN "manf"
F 5 "ACS711KEXLT-15AB-T" H 3100 5600 50  0001 C CNN "manf#"
F 6 "DigiKey" H 3100 5400 50  0001 C CNN "Supplier"
F 7 "620-1483-1-ND" H 3100 5300 50  0001 C CNN "Suppler Part No"
F 8 "https://www.digikey.com.au/product-detail/en/allegro-microsystems-llc/ACS711KEXLT-15AB-T/620-1483-1-ND/3868192" H 3100 5200 50  0001 C CNN "Supplier URL"
F 9 "1.90" H 3100 5100 50  0001 C CNN "Supplier Price"
F 10 "1" H 3100 5000 50  0001 C CNN "Supplier Price Break"
	1    3100 6600
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 7100 2900 7150
Wire Wire Line
	2900 7150 3000 7150
Connection ~ 3100 7150
Wire Wire Line
	3100 7150 3100 7200
Wire Wire Line
	3300 7100 3300 7150
Wire Wire Line
	3300 7150 3200 7150
Wire Wire Line
	3200 7100 3200 7150
Connection ~ 3200 7150
Wire Wire Line
	3200 7150 3100 7150
Wire Wire Line
	3000 7100 3000 7150
Connection ~ 3000 7150
Wire Wire Line
	3000 7150 3100 7150
Wire Wire Line
	2600 6600 2200 6600
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5ABEC596
P 2200 6150
F 0 "#PWR?" H 2200 6110 30  0001 C CNN
F 1 "+3.3V" H 2209 6288 30  0000 C CNN
F 2 "" H 2200 6150 60  0000 C CNN
F 3 "" H 2200 6150 60  0000 C CNN
	1    2200 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2200 6475 2200 6600
$Comp
L appli_power:GND #PWR?
U 1 1 5ABEC5A4
P 4250 7200
F 0 "#PWR?" H 4250 6950 50  0001 C CNN
F 1 "GND" H 4255 7027 50  0000 C CNN
F 2 "" H 4250 7200 60  0000 C CNN
F 3 "" H 4250 7200 60  0000 C CNN
	1    4250 7200
	1    0    0    -1  
$EndComp
$Comp
L appli_connector_molex:MOLEX_MINIFIT_JR_2x2_WAY P?
U 1 1 5ABEC5B1
P 4600 6550
F 0 "P?" H 4678 6580 50  0000 L CNN
F 1 "MOLEX_MINIFIT_JR_2x2_WAY" H 4678 6500 35  0000 L CNN
F 2 "Applidyne_Connector_Molex:Molex_Mini-Fit_Jr_5566-04A_2x02_P4.20mm_Vertical" V 4775 6550 20  0001 C CNN
F 3 "https://www.molex.com/pdm_docs/sd/1724470002_sd.pdf" V 4800 6550 20  0001 C CNN
F 4 "MOLEX" V 4850 6550 20  0001 C CNN "manf"
F 5 "172447-0204" V 4875 6550 20  0001 C CNN "manf#"
F 6 "DigiKey" V 4900 6550 20  0001 C CNN "Supplier"
F 7 "WM14942-ND" V 4925 6550 20  0001 C CNN "Supplier Part No"
F 8 "https://www.digikey.com.au/product-detail/en/1724470204/WM14942-ND/5116909?utm_campaign=buynow&WT.z_cid=ref_octopart_dkc_buynow&utm_medium=aggregator&curr=usd&site=us&utm_source=octopart" V 4950 6550 20  0001 C CNN "Supplier URL"
F 9 "1.66" V 4975 6550 20  0001 C CNN "Supplier Price"
F 10 "1" V 5000 6550 20  0001 C CNN "Supplier Price Break"
	1    4600 6550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 6700 4400 6700
Wire Wire Line
	4250 6700 4250 7200
Wire Wire Line
	3600 6600 4400 6600
$Comp
L appli_resistor:10K0_1608M R?
U 1 1 5ABEC5C2
P 2200 6475
F 0 "R?" H 2128 6290 50  0000 R CNN
F 1 "10K0_1608M" H 2128 6370 35  0000 R CNN
F 2 "RESC1608*" V 2310 6325 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 2335 6325 20  0001 C CNN
F 4 "TE CONNECTIVITY" V 2385 6325 20  0001 C CNN "manf"
F 5 "CRGH0603F10K" V 2410 6325 20  0001 C CNN "manf#"
F 6 "Element14" V 2435 6325 20  0001 C CNN "Supplier"
F 7 "2332016" V 2460 6325 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/te-connectivity/crgh0603f10k/resistor-power-10k-0-2w-1-0603/dp/2332016" V 2485 6325 20  0001 C CNN "Supplier URL"
F 9 "0.02" V 2510 6325 20  0001 C CNN "Supplier Price"
F 10 "10" V 2535 6325 20  0001 C CNN "Supplier Price Break"
	1    2200 6475
	1    0    0    1   
$EndComp
$Comp
L appli_capacitor:100NF_X7R_1608M C?
U 1 1 5ABEC5D0
P 2450 6950
F 0 "C?" H 2373 6815 50  0000 R CNN
F 1 "100NF_X7R_1608M" H 2373 6895 35  0000 R CNN
F 2 "CAPC1608*" V 2565 6850 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1732728.pdf" V 2590 6850 20  0001 C CNN
F 4 "KEMET" V 2640 6850 20  0001 C CNN "manf"
F 5 "C0603C104K5RACAUTO" V 2665 6850 20  0001 C CNN "manf#"
F 6 "Element14" V 2690 6850 20  0001 C CNN "Supplier"
F 7 "2070398" V 2715 6850 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c0603c104k5racauto/cap-mlcc-x7r-100nf-50v-0603/dp/2070398" V 2740 6850 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 2765 6850 20  0001 C CNN "Supplier Price"
F 10 "1" V 2790 6850 20  0001 C CNN "Supplier Price Break"
	1    2450 6950
	1    0    0    1   
$EndComp
$Comp
L appli_resistor:1K_QUAD_ARRAY_1608M R?
U 1 1 5ABFA3D0
P 1650 1525
F 0 "R?" V 1450 1375 50  0000 C CNN
F 1 "1K_QUAD_ARRAY_1608M" V 1530 1375 35  0000 C CNN
F 2 "Applidyne_Resistor:R_Array_Concave_4x1608M" V 1865 1375 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1885635.pdf" V 1890 1375 20  0001 C CNN
F 4 "BOURNS" V 1940 1375 20  0001 C CNN "manf"
F 5 "CAY16-102J4LF" V 1965 1375 20  0001 C CNN "manf#"
F 6 "Element14" V 1990 1375 20  0001 C CNN "Supplier"
F 7 "1770133" V 2015 1375 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/bourns/cay16-102j4lf/resistor-array-4-1k-1206-5-smd/dp/1770133?CMP=GRHB-OCTOPART" V 2040 1375 20  0001 C CNN "Supplier URL"
F 9 "0.073" V 2065 1375 20  0001 C CNN "Supplier Price"
F 10 "50" V 2090 1375 20  0001 C CNN "Supplier Price Break"
	1    1650 1525
	0    1    1    0   
$EndComp
Wire Wire Line
	1350 1525 1300 1525
$Comp
L appli_resistor:1K_QUAD_ARRAY_1608M R?
U 2 1 5AC34563
P 1700 3075
F 0 "R?" V 1500 2925 50  0000 C CNN
F 1 "1K_QUAD_ARRAY_1608M" V 1580 2925 35  0000 C CNN
F 2 "Applidyne_Resistor:R_Array_Concave_4x1608M" V 1915 2925 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1885635.pdf" V 1940 2925 20  0001 C CNN
F 4 "BOURNS" V 1990 2925 20  0001 C CNN "manf"
F 5 "CAY16-102J4LF" V 2015 2925 20  0001 C CNN "manf#"
F 6 "Element14" V 2040 2925 20  0001 C CNN "Supplier"
F 7 "1770133" V 2065 2925 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/bourns/cay16-102j4lf/resistor-array-4-1k-1206-5-smd/dp/1770133?CMP=GRHB-OCTOPART" V 2090 2925 20  0001 C CNN "Supplier URL"
F 9 "0.073" V 2115 2925 20  0001 C CNN "Supplier Price"
F 10 "50" V 2140 2925 20  0001 C CNN "Supplier Price Break"
	2    1700 3075
	0    1    1    0   
$EndComp
Wire Wire Line
	1400 3075 1300 3075
Wire Wire Line
	2600 4825 1750 4825
Wire Wire Line
	2200 4925 1300 4925
Text GLabel 1300 4675 0    50   Output ~ 0
MOTOR_3_CURRENT
Text GLabel 1300 4925 0    50   Output ~ 0
~MOTOR_3_OC
Wire Wire Line
	1750 4825 1750 4675
Wire Wire Line
	1750 4675 1700 4675
$Comp
L appli_resistor:1K_QUAD_ARRAY_1608M R?
U 3 1 5AC4DF5E
P 1700 4675
F 0 "R?" V 1500 4525 50  0000 C CNN
F 1 "1K_QUAD_ARRAY_1608M" V 1580 4525 35  0000 C CNN
F 2 "Applidyne_Resistor:R_Array_Concave_4x1608M" V 1915 4525 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1885635.pdf" V 1940 4525 20  0001 C CNN
F 4 "BOURNS" V 1990 4525 20  0001 C CNN "manf"
F 5 "CAY16-102J4LF" V 2015 4525 20  0001 C CNN "manf#"
F 6 "Element14" V 2040 4525 20  0001 C CNN "Supplier"
F 7 "1770133" V 2065 4525 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/bourns/cay16-102j4lf/resistor-array-4-1k-1206-5-smd/dp/1770133?CMP=GRHB-OCTOPART" V 2090 4525 20  0001 C CNN "Supplier URL"
F 9 "0.073" V 2115 4525 20  0001 C CNN "Supplier Price"
F 10 "50" V 2140 4525 20  0001 C CNN "Supplier Price Break"
	3    1700 4675
	0    1    1    0   
$EndComp
Wire Wire Line
	1400 4675 1300 4675
Wire Wire Line
	2600 6500 1750 6500
Wire Wire Line
	2200 6600 1300 6600
Text GLabel 1300 6350 0    50   Output ~ 0
MOTOR_4_CURRENT
Text GLabel 1300 6600 0    50   Output ~ 0
~MOTOR_4_OC
Wire Wire Line
	1750 6500 1750 6350
Wire Wire Line
	1750 6350 1700 6350
$Comp
L appli_resistor:1K_QUAD_ARRAY_1608M R?
U 4 1 5AC54726
P 1700 6350
F 0 "R?" V 1500 6200 50  0000 C CNN
F 1 "1K_QUAD_ARRAY_1608M" V 1580 6200 35  0000 C CNN
F 2 "Applidyne_Resistor:R_Array_Concave_4x1608M" V 1915 6200 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1885635.pdf" V 1940 6200 20  0001 C CNN
F 4 "BOURNS" V 1990 6200 20  0001 C CNN "manf"
F 5 "CAY16-102J4LF" V 2015 6200 20  0001 C CNN "manf#"
F 6 "Element14" V 2040 6200 20  0001 C CNN "Supplier"
F 7 "1770133" V 2065 6200 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/bourns/cay16-102j4lf/resistor-array-4-1k-1206-5-smd/dp/1770133?CMP=GRHB-OCTOPART" V 2090 6200 20  0001 C CNN "Supplier URL"
F 9 "0.073" V 2115 6200 20  0001 C CNN "Supplier Price"
F 10 "50" V 2140 6200 20  0001 C CNN "Supplier Price Break"
	4    1700 6350
	0    1    1    0   
$EndComp
Wire Wire Line
	1400 6350 1300 6350
Connection ~ 2200 4925
Connection ~ 2200 6600
Text GLabel 6725 1875 0    50   Output ~ 0
SERVO1_HLFB
Text Notes 5700 1550 0    50   ~ 0
todo delete
$EndSCHEMATC
