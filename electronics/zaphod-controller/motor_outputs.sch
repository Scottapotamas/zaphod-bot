EESchema Schematic File Version 4
LIBS:zaphod-controller-cache
EELAYER 26 0
EELAYER END
$Descr A3 16535 11693
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
Text Notes 3525 1050 0    79   ~ 16
Servo Power Connectors
Text Notes 11050 1050 0    79   ~ 16
Servo Data Connectors
$Comp
L appli_power:+3.3V #PWR0135
U 1 1 5AB91CBE
P 3675 1600
F 0 "#PWR0135" H 3675 1560 30  0001 C CNN
F 1 "+3.3V" H 3684 1738 30  0000 C CNN
F 2 "" H 3675 1600 60  0000 C CNN
F 3 "" H 3675 1600 60  0000 C CNN
	1    3675 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	3675 2100 3675 1750
Wire Wire Line
	3675 1750 3825 1750
Connection ~ 3675 1750
Wire Wire Line
	3675 1750 3675 1600
Wire Wire Line
	4325 2450 4325 2500
Wire Wire Line
	3675 2300 3675 2550
$Comp
L appli_power:GND #PWR0136
U 1 1 5AB91D62
P 4325 2550
F 0 "#PWR0136" H 4325 2300 50  0001 C CNN
F 1 "GND" H 4330 2377 50  0000 C CNN
F 2 "" H 4325 2550 60  0000 C CNN
F 3 "" H 4325 2550 60  0000 C CNN
	1    4325 2550
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR0137
U 1 1 5AB91D78
P 3675 2550
F 0 "#PWR0137" H 3675 2300 50  0001 C CNN
F 1 "GND" H 3680 2377 50  0000 C CNN
F 2 "" H 3675 2550 60  0000 C CNN
F 3 "" H 3675 2550 60  0000 C CNN
	1    3675 2550
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+POWER #PWR0138
U 1 1 5AB91E53
P 4975 1600
F 0 "#PWR0138" H 4975 1550 20  0001 C CNN
F 1 "+POWER" H 4978 1738 30  0000 C CNN
F 2 "" H 4975 1600 60  0000 C CNN
F 3 "" H 4975 1600 60  0000 C CNN
	1    4975 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	4975 1600 4975 1750
Wire Wire Line
	4975 1750 4825 1750
$Comp
L appli_special:ACS711KEXLT_15AB U?
U 1 1 5AB955A4
P 4325 1950
F 0 "U?" H 4325 2415 50  0000 C CNN
F 1 "ACS711KEXLT_15AB" H 4325 2324 50  0000 C CNN
F 2 "Applidyne_QFN:ACS-EX-12" H 4325 1150 50  0001 C CNN
F 3 "http://www.allegromicro.com/~/media/Files/Datasheets/ACS711-Datasheet.ashx" H 4325 850 50  0001 C CNN
F 4 "Allegro MicroSystems" H 4325 1050 50  0001 C CNN "manf"
F 5 "ACS711KEXLT-15AB-T" H 4325 950 50  0001 C CNN "manf#"
F 6 "DigiKey" H 4325 750 50  0001 C CNN "Supplier"
F 7 "620-1483-1-ND" H 4325 650 50  0001 C CNN "Suppler Part No"
F 8 "https://www.digikey.com.au/product-detail/en/allegro-microsystems-llc/ACS711KEXLT-15AB-T/620-1483-1-ND/3868192" H 4325 550 50  0001 C CNN "Supplier URL"
F 9 "1.90" H 4325 450 50  0001 C CNN "Supplier Price"
F 10 "1" H 4325 350 50  0001 C CNN "Supplier Price Break"
	1    4325 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	4125 2450 4125 2500
Wire Wire Line
	4125 2500 4225 2500
Connection ~ 4325 2500
Wire Wire Line
	4325 2500 4325 2550
Wire Wire Line
	4525 2450 4525 2500
Wire Wire Line
	4525 2500 4425 2500
Wire Wire Line
	4425 2450 4425 2500
Connection ~ 4425 2500
Wire Wire Line
	4425 2500 4325 2500
Wire Wire Line
	4225 2450 4225 2500
Connection ~ 4225 2500
Wire Wire Line
	4225 2500 4325 2500
Text Notes 4575 2475 0    50   ~ 0
NC->GND for \nbetter EMI per DS
Text Notes 1400 1500 0    50   ~ 0
90mV/A sensitivity.\n3.3V/2 = 0A
Wire Wire Line
	3825 1850 2950 1850
Wire Wire Line
	3825 1950 3425 1950
$Comp
L appli_power:+3.3V #PWR0139
U 1 1 5AB96FA3
P 3425 1500
F 0 "#PWR0139" H 3425 1460 30  0001 C CNN
F 1 "+3.3V" H 3434 1638 30  0000 C CNN
F 2 "" H 3425 1500 60  0000 C CNN
F 3 "" H 3425 1500 60  0000 C CNN
	1    3425 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	3425 1500 3425 1525
Wire Wire Line
	3425 1825 3425 1950
Connection ~ 3425 1950
Wire Wire Line
	3425 1950 2150 1950
Text GLabel 2150 1725 0    50   Output ~ 0
MOTOR_1_CURRENT
Text GLabel 2150 1950 0    50   Output ~ 0
~MOTOR_1_OC
Text Notes 1300 2300 0    50   ~ 0
FAULT latches low when\ncurrent > 15A rating
Wire Wire Line
	2950 1850 2950 1725
Wire Wire Line
	2950 1725 2900 1725
$Comp
L appli_power:GND #PWR0140
U 1 1 5AB9A0AB
P 5475 2550
F 0 "#PWR0140" H 5475 2300 50  0001 C CNN
F 1 "GND" H 5480 2377 50  0000 C CNN
F 2 "" H 5475 2550 60  0000 C CNN
F 3 "" H 5475 2550 60  0000 C CNN
	1    5475 2550
	1    0    0    -1  
$EndComp
$Comp
L appli_connector_molex:MOLEX_MINIFIT_JR_2x2_WAY P?
U 1 1 5AB9CD10
P 5825 1900
F 0 "P?" H 5775 2250 50  0000 L CNN
F 1 "MOLEX_MINIFIT_JR_2x2_WAY" H 5775 2175 35  0000 L CNN
F 2 "Applidyne_Connector_Molex:Molex_Mini-Fit_Jr_5566-04A_2x02_P4.20mm_Vertical" V 6000 1900 20  0001 C CNN
F 3 "https://www.molex.com/pdm_docs/sd/1724470002_sd.pdf" V 6025 1900 20  0001 C CNN
F 4 "MOLEX" V 6075 1900 20  0001 C CNN "manf"
F 5 "172447-0204" V 6100 1900 20  0001 C CNN "manf#"
F 6 "DigiKey" V 6125 1900 20  0001 C CNN "Supplier"
F 7 "WM14942-ND" V 6150 1900 20  0001 C CNN "Supplier Part No"
F 8 "https://www.digikey.com.au/product-detail/en/1724470204/WM14942-ND/5116909?utm_campaign=buynow&WT.z_cid=ref_octopart_dkc_buynow&utm_medium=aggregator&curr=usd&site=us&utm_source=octopart" V 6175 1900 20  0001 C CNN "Supplier URL"
F 9 "1.66" V 6200 1900 20  0001 C CNN "Supplier Price"
F 10 "1" V 6225 1900 20  0001 C CNN "Supplier Price Break"
	1    5825 1900
	1    0    0    -1  
$EndComp
$Comp
L appli_connector_molex:MOLEX_MINIFIT_JR_2x4_WAY P?
U 1 1 5AB9CF32
P 13225 2650
F 0 "P?" H 13200 3250 50  0000 L CNN
F 1 "MOLEX_MINIFIT_JR_2x4_WAY" H 13200 3150 35  0000 L CNN
F 2 "Applidyne_Connector_Molex:Molex_MiniFit-JR-5569-08A2_2x04x4.20mm_Angled" V 13400 2650 20  0001 C CNN
F 3 "https://www.molex.com/pdm_docs/sd/1724470002_sd.pdf" V 13425 2650 20  0001 C CNN
F 4 "MOLEX" V 13475 2650 20  0001 C CNN "manf"
F 5 "172447-0208" V 13500 2650 20  0001 C CNN "manf#"
F 6 "Mouser" V 13525 2650 20  0001 C CNN "Supplier"
F 7 "538-172447-0208" V 13550 2650 20  0001 C CNN "Supplier Part No"
F 8 "https://au.mouser.com/ProductDetail/Molex/172447-0208?qs=sGAEpiMZZMvc81WFyF5Edkd3jpEQlJBQt6f989MIPuM%3d" V 13575 2650 20  0001 C CNN "Supplier URL"
F 9 "1.78" V 13600 2650 20  0001 C CNN "Supplier Price"
F 10 "1" V 13625 2650 20  0001 C CNN "Supplier Price Break"
	1    13225 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5475 2050 5625 2050
Wire Wire Line
	5475 2050 5475 2550
$Comp
L appli_power:+3.3V #PWR0141
U 1 1 5AB9FC1B
P 3675 3275
F 0 "#PWR0141" H 3675 3235 30  0001 C CNN
F 1 "+3.3V" H 3684 3413 30  0000 C CNN
F 2 "" H 3675 3275 60  0000 C CNN
F 3 "" H 3675 3275 60  0000 C CNN
	1    3675 3275
	1    0    0    -1  
$EndComp
Wire Wire Line
	3675 3775 3675 3425
Wire Wire Line
	3675 3425 3825 3425
Connection ~ 3675 3425
Wire Wire Line
	3675 3425 3675 3275
Wire Wire Line
	4325 4125 4325 4175
Wire Wire Line
	3675 3975 3675 4225
$Comp
L appli_power:GND #PWR0142
U 1 1 5AB9FC27
P 4325 4225
F 0 "#PWR0142" H 4325 3975 50  0001 C CNN
F 1 "GND" H 4330 4052 50  0000 C CNN
F 2 "" H 4325 4225 60  0000 C CNN
F 3 "" H 4325 4225 60  0000 C CNN
	1    4325 4225
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR0143
U 1 1 5AB9FC2D
P 3675 4225
F 0 "#PWR0143" H 3675 3975 50  0001 C CNN
F 1 "GND" H 3680 4052 50  0000 C CNN
F 2 "" H 3675 4225 60  0000 C CNN
F 3 "" H 3675 4225 60  0000 C CNN
	1    3675 4225
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+POWER #PWR0144
U 1 1 5AB9FC33
P 4975 3275
F 0 "#PWR0144" H 4975 3225 20  0001 C CNN
F 1 "+POWER" H 4978 3413 30  0000 C CNN
F 2 "" H 4975 3275 60  0000 C CNN
F 3 "" H 4975 3275 60  0000 C CNN
	1    4975 3275
	1    0    0    -1  
$EndComp
Wire Wire Line
	4975 3275 4975 3425
Wire Wire Line
	4975 3425 4825 3425
$Comp
L appli_special:ACS711KEXLT_15AB U?
U 1 1 5AB9FC42
P 4325 3625
F 0 "U?" H 4325 4090 50  0000 C CNN
F 1 "ACS711KEXLT_15AB" H 4325 3999 50  0000 C CNN
F 2 "Applidyne_QFN:ACS-EX-12" H 4325 2825 50  0001 C CNN
F 3 "http://www.allegromicro.com/~/media/Files/Datasheets/ACS711-Datasheet.ashx" H 4325 2525 50  0001 C CNN
F 4 "Allegro MicroSystems" H 4325 2725 50  0001 C CNN "manf"
F 5 "ACS711KEXLT-15AB-T" H 4325 2625 50  0001 C CNN "manf#"
F 6 "DigiKey" H 4325 2425 50  0001 C CNN "Supplier"
F 7 "620-1483-1-ND" H 4325 2325 50  0001 C CNN "Suppler Part No"
F 8 "https://www.digikey.com.au/product-detail/en/allegro-microsystems-llc/ACS711KEXLT-15AB-T/620-1483-1-ND/3868192" H 4325 2225 50  0001 C CNN "Supplier URL"
F 9 "1.90" H 4325 2125 50  0001 C CNN "Supplier Price"
F 10 "1" H 4325 2025 50  0001 C CNN "Supplier Price Break"
	1    4325 3625
	1    0    0    -1  
$EndComp
Wire Wire Line
	4125 4125 4125 4175
Wire Wire Line
	4125 4175 4225 4175
Connection ~ 4325 4175
Wire Wire Line
	4325 4175 4325 4225
Wire Wire Line
	4525 4125 4525 4175
Wire Wire Line
	4525 4175 4425 4175
Wire Wire Line
	4425 4125 4425 4175
Connection ~ 4425 4175
Wire Wire Line
	4425 4175 4325 4175
Wire Wire Line
	4225 4125 4225 4175
Connection ~ 4225 4175
Wire Wire Line
	4225 4175 4325 4175
Wire Wire Line
	3825 3525 2975 3525
Wire Wire Line
	3825 3625 3425 3625
$Comp
L appli_power:+3.3V #PWR0145
U 1 1 5AB9FC67
P 3425 3175
F 0 "#PWR0145" H 3425 3135 30  0001 C CNN
F 1 "+3.3V" H 3434 3313 30  0000 C CNN
F 2 "" H 3425 3175 60  0000 C CNN
F 3 "" H 3425 3175 60  0000 C CNN
	1    3425 3175
	1    0    0    -1  
$EndComp
Wire Wire Line
	3425 3175 3425 3200
Wire Wire Line
	3425 3500 3425 3625
Connection ~ 3425 3625
Wire Wire Line
	3425 3625 2150 3625
Text GLabel 2150 3375 0    50   Output ~ 0
MOTOR_2_CURRENT
Text GLabel 2150 3625 0    50   Output ~ 0
~MOTOR_2_OC
Wire Wire Line
	2975 3525 2975 3375
Wire Wire Line
	2975 3375 2925 3375
$Comp
L appli_power:GND #PWR0146
U 1 1 5AB9FC76
P 5475 4225
F 0 "#PWR0146" H 5475 3975 50  0001 C CNN
F 1 "GND" H 5480 4052 50  0000 C CNN
F 2 "" H 5475 4225 60  0000 C CNN
F 3 "" H 5475 4225 60  0000 C CNN
	1    5475 4225
	1    0    0    -1  
$EndComp
$Comp
L appli_connector_molex:MOLEX_MINIFIT_JR_2x2_WAY P?
U 1 1 5AB9FC83
P 5825 3575
F 0 "P?" H 5903 3605 50  0000 L CNN
F 1 "MOLEX_MINIFIT_JR_2x2_WAY" H 5903 3525 35  0000 L CNN
F 2 "Applidyne_Connector_Molex:Molex_Mini-Fit_Jr_5566-04A_2x02_P4.20mm_Vertical" V 6000 3575 20  0001 C CNN
F 3 "https://www.molex.com/pdm_docs/sd/1724470002_sd.pdf" V 6025 3575 20  0001 C CNN
F 4 "MOLEX" V 6075 3575 20  0001 C CNN "manf"
F 5 "172447-0204" V 6100 3575 20  0001 C CNN "manf#"
F 6 "DigiKey" V 6125 3575 20  0001 C CNN "Supplier"
F 7 "WM14942-ND" V 6150 3575 20  0001 C CNN "Supplier Part No"
F 8 "https://www.digikey.com.au/product-detail/en/1724470204/WM14942-ND/5116909?utm_campaign=buynow&WT.z_cid=ref_octopart_dkc_buynow&utm_medium=aggregator&curr=usd&site=us&utm_source=octopart" V 6175 3575 20  0001 C CNN "Supplier URL"
F 9 "1.66" V 6200 3575 20  0001 C CNN "Supplier Price"
F 10 "1" V 6225 3575 20  0001 C CNN "Supplier Price Break"
	1    5825 3575
	1    0    0    -1  
$EndComp
Wire Wire Line
	5475 3725 5625 3725
Wire Wire Line
	5475 3725 5475 4225
$Comp
L appli_power:+3.3V #PWR0147
U 1 1 5ABA0C8E
P 3675 4975
F 0 "#PWR0147" H 3675 4935 30  0001 C CNN
F 1 "+3.3V" H 3684 5113 30  0000 C CNN
F 2 "" H 3675 4975 60  0000 C CNN
F 3 "" H 3675 4975 60  0000 C CNN
	1    3675 4975
	1    0    0    -1  
$EndComp
Wire Wire Line
	3675 5475 3675 5125
Wire Wire Line
	3675 5125 3825 5125
Connection ~ 3675 5125
Wire Wire Line
	3675 5125 3675 4975
Wire Wire Line
	4325 5825 4325 5875
Wire Wire Line
	3675 5675 3675 5925
$Comp
L appli_power:GND #PWR0148
U 1 1 5ABA0C9A
P 4325 5925
F 0 "#PWR0148" H 4325 5675 50  0001 C CNN
F 1 "GND" H 4330 5752 50  0000 C CNN
F 2 "" H 4325 5925 60  0000 C CNN
F 3 "" H 4325 5925 60  0000 C CNN
	1    4325 5925
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR0149
U 1 1 5ABA0CA0
P 3675 5925
F 0 "#PWR0149" H 3675 5675 50  0001 C CNN
F 1 "GND" H 3680 5752 50  0000 C CNN
F 2 "" H 3675 5925 60  0000 C CNN
F 3 "" H 3675 5925 60  0000 C CNN
	1    3675 5925
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+POWER #PWR0150
U 1 1 5ABA0CA6
P 4975 4975
F 0 "#PWR0150" H 4975 4925 20  0001 C CNN
F 1 "+POWER" H 4978 5113 30  0000 C CNN
F 2 "" H 4975 4975 60  0000 C CNN
F 3 "" H 4975 4975 60  0000 C CNN
	1    4975 4975
	1    0    0    -1  
$EndComp
Wire Wire Line
	4975 4975 4975 5125
Wire Wire Line
	4975 5125 4825 5125
$Comp
L appli_special:ACS711KEXLT_15AB U?
U 1 1 5ABA0CB5
P 4325 5325
F 0 "U?" H 4325 5790 50  0000 C CNN
F 1 "ACS711KEXLT_15AB" H 4325 5699 50  0000 C CNN
F 2 "Applidyne_QFN:ACS-EX-12" H 4325 4525 50  0001 C CNN
F 3 "http://www.allegromicro.com/~/media/Files/Datasheets/ACS711-Datasheet.ashx" H 4325 4225 50  0001 C CNN
F 4 "Allegro MicroSystems" H 4325 4425 50  0001 C CNN "manf"
F 5 "ACS711KEXLT-15AB-T" H 4325 4325 50  0001 C CNN "manf#"
F 6 "DigiKey" H 4325 4125 50  0001 C CNN "Supplier"
F 7 "620-1483-1-ND" H 4325 4025 50  0001 C CNN "Suppler Part No"
F 8 "https://www.digikey.com.au/product-detail/en/allegro-microsystems-llc/ACS711KEXLT-15AB-T/620-1483-1-ND/3868192" H 4325 3925 50  0001 C CNN "Supplier URL"
F 9 "1.90" H 4325 3825 50  0001 C CNN "Supplier Price"
F 10 "1" H 4325 3725 50  0001 C CNN "Supplier Price Break"
	1    4325 5325
	1    0    0    -1  
$EndComp
Wire Wire Line
	4125 5825 4125 5875
Wire Wire Line
	4125 5875 4225 5875
Connection ~ 4325 5875
Wire Wire Line
	4325 5875 4325 5925
Wire Wire Line
	4525 5825 4525 5875
Wire Wire Line
	4525 5875 4425 5875
Wire Wire Line
	4425 5825 4425 5875
Connection ~ 4425 5875
Wire Wire Line
	4425 5875 4325 5875
Wire Wire Line
	4225 5825 4225 5875
Connection ~ 4225 5875
Wire Wire Line
	4225 5875 4325 5875
Wire Wire Line
	3825 5325 3425 5325
$Comp
L appli_power:+3.3V #PWR0151
U 1 1 5ABA0CDA
P 3425 4875
F 0 "#PWR0151" H 3425 4835 30  0001 C CNN
F 1 "+3.3V" H 3434 5013 30  0000 C CNN
F 2 "" H 3425 4875 60  0000 C CNN
F 3 "" H 3425 4875 60  0000 C CNN
	1    3425 4875
	1    0    0    -1  
$EndComp
Wire Wire Line
	3425 5175 3425 5325
$Comp
L appli_power:GND #PWR0152
U 1 1 5ABA0CE9
P 5475 5925
F 0 "#PWR0152" H 5475 5675 50  0001 C CNN
F 1 "GND" H 5480 5752 50  0000 C CNN
F 2 "" H 5475 5925 60  0000 C CNN
F 3 "" H 5475 5925 60  0000 C CNN
	1    5475 5925
	1    0    0    -1  
$EndComp
$Comp
L appli_connector_molex:MOLEX_MINIFIT_JR_2x2_WAY P?
U 1 1 5ABA0CF6
P 5825 5275
F 0 "P?" H 5903 5305 50  0000 L CNN
F 1 "MOLEX_MINIFIT_JR_2x2_WAY" H 5903 5225 35  0000 L CNN
F 2 "Applidyne_Connector_Molex:Molex_Mini-Fit_Jr_5566-04A_2x02_P4.20mm_Vertical" V 6000 5275 20  0001 C CNN
F 3 "https://www.molex.com/pdm_docs/sd/1724470002_sd.pdf" V 6025 5275 20  0001 C CNN
F 4 "MOLEX" V 6075 5275 20  0001 C CNN "manf"
F 5 "172447-0204" V 6100 5275 20  0001 C CNN "manf#"
F 6 "DigiKey" V 6125 5275 20  0001 C CNN "Supplier"
F 7 "WM14942-ND" V 6150 5275 20  0001 C CNN "Supplier Part No"
F 8 "https://www.digikey.com.au/product-detail/en/1724470204/WM14942-ND/5116909?utm_campaign=buynow&WT.z_cid=ref_octopart_dkc_buynow&utm_medium=aggregator&curr=usd&site=us&utm_source=octopart" V 6175 5275 20  0001 C CNN "Supplier URL"
F 9 "1.66" V 6200 5275 20  0001 C CNN "Supplier Price"
F 10 "1" V 6225 5275 20  0001 C CNN "Supplier Price Break"
	1    5825 5275
	1    0    0    -1  
$EndComp
Wire Wire Line
	5475 5425 5625 5425
Wire Wire Line
	5475 5425 5475 5925
$Comp
L appli_power:+12V #PWR0153
U 1 1 5ABA4403
P 12475 2150
F 0 "#PWR0153" H 12475 2000 50  0001 C CNN
F 1 "+12V" H 12490 2323 50  0000 C CNN
F 2 "" H 12475 2150 60  0000 C CNN
F 3 "" H 12475 2150 60  0000 C CNN
	1    12475 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	13025 2400 12475 2400
Wire Wire Line
	13025 2500 12475 2500
Wire Wire Line
	12475 2500 12475 2400
Wire Wire Line
	13025 2600 12475 2600
Wire Wire Line
	12475 2600 12475 2500
Connection ~ 12475 2500
Wire Wire Line
	12475 2400 12475 2300
Connection ~ 12475 2400
Wire Wire Line
	13025 2700 12250 2700
Wire Wire Line
	12250 2700 12250 1700
Wire Wire Line
	12250 1700 11875 1700
$Comp
L Transistor_Array:ULN2803A U?
U 1 1 5AC9AE17
P 10850 3650
F 0 "U?" H 10850 4217 50  0000 C CNN
F 1 "ULN2803A" H 10850 4126 50  0000 C CNN
F 2 "" H 10900 3000 50  0001 L CNN
F 3 "http://www.ti.com/lit/ds/symlink/uln2803a.pdf" H 10950 3450 50  0001 C CNN
	1    10850 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	12050 3000 13025 3000
Wire Wire Line
	11950 2900 13025 2900
Wire Wire Line
	11850 2800 13025 2800
$Comp
L appli_connector_molex:MOLEX_MINIFIT_JR_2x4_WAY P?
U 1 1 5ACB982C
P 13225 3600
F 0 "P?" H 13303 3630 50  0000 L CNN
F 1 "MOLEX_MINIFIT_JR_2x4_WAY" H 13303 3550 35  0000 L CNN
F 2 "Applidyne_Connector_Molex:Molex_MiniFit-JR-5569-08A2_2x04x4.20mm_Angled" V 13400 3600 20  0001 C CNN
F 3 "https://www.molex.com/pdm_docs/sd/1724470002_sd.pdf" V 13425 3600 20  0001 C CNN
F 4 "MOLEX" V 13475 3600 20  0001 C CNN "manf"
F 5 "172447-0208" V 13500 3600 20  0001 C CNN "manf#"
F 6 "Mouser" V 13525 3600 20  0001 C CNN "Supplier"
F 7 "538-172447-0208" V 13550 3600 20  0001 C CNN "Supplier Part No"
F 8 "https://au.mouser.com/ProductDetail/Molex/172447-0208?qs=sGAEpiMZZMvc81WFyF5Edkd3jpEQlJBQt6f989MIPuM%3d" V 13575 3600 20  0001 C CNN "Supplier URL"
F 9 "1.78" V 13600 3600 20  0001 C CNN "Supplier Price"
F 10 "1" V 13625 3600 20  0001 C CNN "Supplier Price Break"
	1    13225 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	13025 3350 12475 3350
Wire Wire Line
	13025 3450 12475 3450
Wire Wire Line
	12475 3450 12475 3350
Wire Wire Line
	13025 3550 12475 3550
Wire Wire Line
	12475 3550 12475 3450
Connection ~ 12475 3450
Wire Wire Line
	12475 3350 12475 3250
Connection ~ 12475 3350
Connection ~ 12475 2600
Wire Wire Line
	13025 3250 12475 3250
Wire Wire Line
	13025 3650 12150 3650
Wire Wire Line
	13025 3750 11250 3750
Wire Wire Line
	11250 3850 13025 3850
Wire Wire Line
	13025 3950 11250 3950
Wire Wire Line
	12050 3000 12050 3650
Wire Wire Line
	12050 3650 11250 3650
Wire Wire Line
	11250 3550 11950 3550
Wire Wire Line
	11950 3550 11950 2900
Wire Wire Line
	11250 3450 11850 3450
Wire Wire Line
	11850 3450 11850 2800
Wire Wire Line
	11250 3350 11650 3350
Wire Wire Line
	11650 3350 11650 3300
$Comp
L appli_power:+12V #PWR0154
U 1 1 5AD08F43
P 11650 3300
F 0 "#PWR0154" H 11650 3150 50  0001 C CNN
F 1 "+12V" H 11665 3473 50  0000 C CNN
F 2 "" H 11650 3300 60  0000 C CNN
F 3 "" H 11650 3300 60  0000 C CNN
	1    11650 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	10450 3450 10100 3450
Wire Wire Line
	10450 3550 10100 3550
Wire Wire Line
	10450 3650 10100 3650
Wire Wire Line
	10450 3750 10100 3750
Wire Wire Line
	10450 3850 10100 3850
Wire Wire Line
	10450 3950 10100 3950
Wire Wire Line
	10850 4350 10850 4400
$Comp
L appli_power:GND #PWR0155
U 1 1 5AD43F33
P 10850 4400
F 0 "#PWR0155" H 10850 4150 50  0001 C CNN
F 1 "GND" H 10855 4227 50  0000 C CNN
F 2 "" H 10850 4400 60  0000 C CNN
F 3 "" H 10850 4400 60  0000 C CNN
	1    10850 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	12150 2450 11875 2450
Wire Wire Line
	12150 2450 12150 3650
Text Notes 13325 3050 0    63   ~ 0
HLFB+\nINPUTB+\nINPUTA+\nENABLE+\nHLFB-\nINPUTB-\nINPUTA-\nENABLE-\n
Text GLabel 10100 3550 0    50   Input ~ 0
SERVO1_A
Text GLabel 10100 3450 0    50   Input ~ 0
SERVO1_B
Text GLabel 10100 3650 0    50   Input ~ 0
SERVO1_EN
Text GLabel 10100 3850 0    50   Input ~ 0
SERVO2_A
Text GLabel 10100 3750 0    50   Input ~ 0
SERVO2_B
Text GLabel 10100 3950 0    50   Input ~ 0
SERVO2_EN
Text GLabel 10100 2450 0    50   Output ~ 0
SERVO2_HLFB
Wire Wire Line
	9950 3075 9950 3100
Wire Wire Line
	13025 2300 12475 2300
Connection ~ 12475 2300
Wire Wire Line
	12475 2300 12475 2150
Connection ~ 12475 3250
Wire Wire Line
	12475 3250 12475 2600
Wire Wire Line
	4825 3625 5375 3625
Wire Wire Line
	4825 1950 5375 1950
Wire Wire Line
	4825 5325 5375 5325
$Comp
L appli_capacitor:100NF_X7R_1608M C?
U 1 1 5ABF1105
P 9950 2875
F 0 "C?" H 10028 2810 50  0000 L CNN
F 1 "100NF_X7R_1608M" H 10028 2730 35  0000 L CNN
F 2 "CAPC1608*" V 10065 2775 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1732728.pdf" V 10090 2775 20  0001 C CNN
F 4 "KEMET" V 10140 2775 20  0001 C CNN "manf"
F 5 "C0603C104K5RACAUTO" V 10165 2775 20  0001 C CNN "manf#"
F 6 "Element14" V 10190 2775 20  0001 C CNN "Supplier"
F 7 "2070398" V 10215 2775 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c0603c104k5racauto/cap-mlcc-x7r-100nf-50v-0603/dp/2070398" V 10240 2775 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 10265 2775 20  0001 C CNN "Supplier Price"
F 10 "1" V 10290 2775 20  0001 C CNN "Supplier Price Break"
	1    9950 2875
	-1   0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR0156
U 1 1 5AC0C492
P 9950 3100
F 0 "#PWR0156" H 9950 2850 50  0001 C CNN
F 1 "GND" H 9955 2927 50  0000 C CNN
F 2 "" H 9950 3100 60  0000 C CNN
F 3 "" H 9950 3100 60  0000 C CNN
	1    9950 3100
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+12V #PWR0157
U 1 1 5AC11C0D
P 9950 2825
F 0 "#PWR0157" H 9950 2675 50  0001 C CNN
F 1 "+12V" H 9965 2998 50  0000 C CNN
F 2 "" H 9950 2825 60  0000 C CNN
F 3 "" H 9950 2825 60  0000 C CNN
	1    9950 2825
	1    0    0    -1  
$EndComp
Wire Wire Line
	9950 2825 9950 2875
$Comp
L appli_capacitor:100NF_X7R_1608M C?
U 1 1 5ABCECAE
P 3675 3975
F 0 "C?" H 3598 3840 50  0000 R CNN
F 1 "100NF_X7R_1608M" H 3598 3920 35  0000 R CNN
F 2 "CAPC1608*" V 3790 3875 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1732728.pdf" V 3815 3875 20  0001 C CNN
F 4 "KEMET" V 3865 3875 20  0001 C CNN "manf"
F 5 "C0603C104K5RACAUTO" V 3890 3875 20  0001 C CNN "manf#"
F 6 "Element14" V 3915 3875 20  0001 C CNN "Supplier"
F 7 "2070398" V 3940 3875 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c0603c104k5racauto/cap-mlcc-x7r-100nf-50v-0603/dp/2070398" V 3965 3875 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 3990 3875 20  0001 C CNN "Supplier Price"
F 10 "1" V 4015 3875 20  0001 C CNN "Supplier Price Break"
	1    3675 3975
	1    0    0    1   
$EndComp
$Comp
L appli_resistor:10K0_1608M R?
U 1 1 5ABCEE82
P 3425 3500
F 0 "R?" H 3353 3315 50  0000 R CNN
F 1 "10K0_1608M" H 3353 3395 35  0000 R CNN
F 2 "RESC1608*" V 3535 3350 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 3560 3350 20  0001 C CNN
F 4 "TE CONNECTIVITY" V 3610 3350 20  0001 C CNN "manf"
F 5 "CRGH0603F10K" V 3635 3350 20  0001 C CNN "manf#"
F 6 "Element14" V 3660 3350 20  0001 C CNN "Supplier"
F 7 "2332016" V 3685 3350 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/te-connectivity/crgh0603f10k/resistor-power-10k-0-2w-1-0603/dp/2332016" V 3710 3350 20  0001 C CNN "Supplier URL"
F 9 "0.02" V 3735 3350 20  0001 C CNN "Supplier Price"
F 10 "10" V 3760 3350 20  0001 C CNN "Supplier Price Break"
	1    3425 3500
	1    0    0    1   
$EndComp
$Comp
L appli_capacitor:100NF_X7R_1608M C?
U 1 1 5ABCF127
P 3675 2300
F 0 "C?" H 3598 2165 50  0000 R CNN
F 1 "100NF_X7R_1608M" H 3598 2245 35  0000 R CNN
F 2 "CAPC1608*" V 3790 2200 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1732728.pdf" V 3815 2200 20  0001 C CNN
F 4 "KEMET" V 3865 2200 20  0001 C CNN "manf"
F 5 "C0603C104K5RACAUTO" V 3890 2200 20  0001 C CNN "manf#"
F 6 "Element14" V 3915 2200 20  0001 C CNN "Supplier"
F 7 "2070398" V 3940 2200 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c0603c104k5racauto/cap-mlcc-x7r-100nf-50v-0603/dp/2070398" V 3965 2200 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 3990 2200 20  0001 C CNN "Supplier Price"
F 10 "1" V 4015 2200 20  0001 C CNN "Supplier Price Break"
	1    3675 2300
	1    0    0    1   
$EndComp
$Comp
L appli_resistor:10K0_1608M R?
U 1 1 5ABCF17B
P 3425 1825
F 0 "R?" H 3353 1640 50  0000 R CNN
F 1 "10K0_1608M" H 3353 1720 35  0000 R CNN
F 2 "RESC1608*" V 3535 1675 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 3560 1675 20  0001 C CNN
F 4 "TE CONNECTIVITY" V 3610 1675 20  0001 C CNN "manf"
F 5 "CRGH0603F10K" V 3635 1675 20  0001 C CNN "manf#"
F 6 "Element14" V 3660 1675 20  0001 C CNN "Supplier"
F 7 "2332016" V 3685 1675 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/te-connectivity/crgh0603f10k/resistor-power-10k-0-2w-1-0603/dp/2332016" V 3710 1675 20  0001 C CNN "Supplier URL"
F 9 "0.02" V 3735 1675 20  0001 C CNN "Supplier Price"
F 10 "10" V 3760 1675 20  0001 C CNN "Supplier Price Break"
	1    3425 1825
	1    0    0    1   
$EndComp
$Comp
L appli_resistor:10K0_1608M R?
U 1 1 5ABCF1CD
P 3425 5175
F 0 "R?" H 3353 4990 50  0000 R CNN
F 1 "10K0_1608M" H 3353 5070 35  0000 R CNN
F 2 "RESC1608*" V 3535 5025 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 3560 5025 20  0001 C CNN
F 4 "TE CONNECTIVITY" V 3610 5025 20  0001 C CNN "manf"
F 5 "CRGH0603F10K" V 3635 5025 20  0001 C CNN "manf#"
F 6 "Element14" V 3660 5025 20  0001 C CNN "Supplier"
F 7 "2332016" V 3685 5025 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/te-connectivity/crgh0603f10k/resistor-power-10k-0-2w-1-0603/dp/2332016" V 3710 5025 20  0001 C CNN "Supplier URL"
F 9 "0.02" V 3735 5025 20  0001 C CNN "Supplier Price"
F 10 "10" V 3760 5025 20  0001 C CNN "Supplier Price Break"
	1    3425 5175
	1    0    0    1   
$EndComp
$Comp
L appli_capacitor:100NF_X7R_1608M C?
U 1 1 5ABCF21D
P 3675 5675
F 0 "C?" H 3598 5540 50  0000 R CNN
F 1 "100NF_X7R_1608M" H 3598 5620 35  0000 R CNN
F 2 "CAPC1608*" V 3790 5575 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1732728.pdf" V 3815 5575 20  0001 C CNN
F 4 "KEMET" V 3865 5575 20  0001 C CNN "manf"
F 5 "C0603C104K5RACAUTO" V 3890 5575 20  0001 C CNN "manf#"
F 6 "Element14" V 3915 5575 20  0001 C CNN "Supplier"
F 7 "2070398" V 3940 5575 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c0603c104k5racauto/cap-mlcc-x7r-100nf-50v-0603/dp/2070398" V 3965 5575 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 3990 5575 20  0001 C CNN "Supplier Price"
F 10 "1" V 4015 5575 20  0001 C CNN "Supplier Price Break"
	1    3675 5675
	1    0    0    1   
$EndComp
$Comp
L appli_power:+3.3V #PWR0158
U 1 1 5ABEC55A
P 3675 6750
F 0 "#PWR0158" H 3675 6710 30  0001 C CNN
F 1 "+3.3V" H 3684 6888 30  0000 C CNN
F 2 "" H 3675 6750 60  0000 C CNN
F 3 "" H 3675 6750 60  0000 C CNN
	1    3675 6750
	1    0    0    -1  
$EndComp
Wire Wire Line
	3675 7250 3675 6900
Wire Wire Line
	3675 6900 3825 6900
Connection ~ 3675 6900
Wire Wire Line
	3675 6900 3675 6750
Wire Wire Line
	4325 7600 4325 7650
Wire Wire Line
	3675 7450 3675 7700
$Comp
L appli_power:GND #PWR0159
U 1 1 5ABEC566
P 4325 7700
F 0 "#PWR0159" H 4325 7450 50  0001 C CNN
F 1 "GND" H 4330 7527 50  0000 C CNN
F 2 "" H 4325 7700 60  0000 C CNN
F 3 "" H 4325 7700 60  0000 C CNN
	1    4325 7700
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR0160
U 1 1 5ABEC56C
P 3675 7700
F 0 "#PWR0160" H 3675 7450 50  0001 C CNN
F 1 "GND" H 3680 7527 50  0000 C CNN
F 2 "" H 3675 7700 60  0000 C CNN
F 3 "" H 3675 7700 60  0000 C CNN
	1    3675 7700
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+POWER #PWR0161
U 1 1 5ABEC572
P 4975 6750
F 0 "#PWR0161" H 4975 6700 20  0001 C CNN
F 1 "+POWER" H 4978 6888 30  0000 C CNN
F 2 "" H 4975 6750 60  0000 C CNN
F 3 "" H 4975 6750 60  0000 C CNN
	1    4975 6750
	1    0    0    -1  
$EndComp
Wire Wire Line
	4975 6750 4975 6900
Wire Wire Line
	4975 6900 4825 6900
$Comp
L appli_special:ACS711KEXLT_15AB U?
U 1 1 5ABEC581
P 4325 7100
F 0 "U?" H 4325 7565 50  0000 C CNN
F 1 "ACS711KEXLT_15AB" H 4325 7474 50  0000 C CNN
F 2 "Applidyne_QFN:ACS-EX-12" H 4325 6300 50  0001 C CNN
F 3 "http://www.allegromicro.com/~/media/Files/Datasheets/ACS711-Datasheet.ashx" H 4325 6000 50  0001 C CNN
F 4 "Allegro MicroSystems" H 4325 6200 50  0001 C CNN "manf"
F 5 "ACS711KEXLT-15AB-T" H 4325 6100 50  0001 C CNN "manf#"
F 6 "DigiKey" H 4325 5900 50  0001 C CNN "Supplier"
F 7 "620-1483-1-ND" H 4325 5800 50  0001 C CNN "Suppler Part No"
F 8 "https://www.digikey.com.au/product-detail/en/allegro-microsystems-llc/ACS711KEXLT-15AB-T/620-1483-1-ND/3868192" H 4325 5700 50  0001 C CNN "Supplier URL"
F 9 "1.90" H 4325 5600 50  0001 C CNN "Supplier Price"
F 10 "1" H 4325 5500 50  0001 C CNN "Supplier Price Break"
	1    4325 7100
	1    0    0    -1  
$EndComp
Wire Wire Line
	4125 7600 4125 7650
Wire Wire Line
	4125 7650 4225 7650
Connection ~ 4325 7650
Wire Wire Line
	4325 7650 4325 7700
Wire Wire Line
	4525 7600 4525 7650
Wire Wire Line
	4525 7650 4425 7650
Wire Wire Line
	4425 7600 4425 7650
Connection ~ 4425 7650
Wire Wire Line
	4425 7650 4325 7650
Wire Wire Line
	4225 7600 4225 7650
Connection ~ 4225 7650
Wire Wire Line
	4225 7650 4325 7650
Wire Wire Line
	3825 7100 3425 7100
$Comp
L appli_power:+3.3V #PWR0162
U 1 1 5ABEC596
P 3425 6650
F 0 "#PWR0162" H 3425 6610 30  0001 C CNN
F 1 "+3.3V" H 3434 6788 30  0000 C CNN
F 2 "" H 3425 6650 60  0000 C CNN
F 3 "" H 3425 6650 60  0000 C CNN
	1    3425 6650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3425 6975 3425 7100
$Comp
L appli_power:GND #PWR0163
U 1 1 5ABEC5A4
P 5475 7700
F 0 "#PWR0163" H 5475 7450 50  0001 C CNN
F 1 "GND" H 5480 7527 50  0000 C CNN
F 2 "" H 5475 7700 60  0000 C CNN
F 3 "" H 5475 7700 60  0000 C CNN
	1    5475 7700
	1    0    0    -1  
$EndComp
$Comp
L appli_connector_molex:MOLEX_MINIFIT_JR_2x2_WAY P?
U 1 1 5ABEC5B1
P 5825 7050
F 0 "P?" H 5903 7080 50  0000 L CNN
F 1 "MOLEX_MINIFIT_JR_2x2_WAY" H 5903 7000 35  0000 L CNN
F 2 "Applidyne_Connector_Molex:Molex_Mini-Fit_Jr_5566-04A_2x02_P4.20mm_Vertical" V 6000 7050 20  0001 C CNN
F 3 "https://www.molex.com/pdm_docs/sd/1724470002_sd.pdf" V 6025 7050 20  0001 C CNN
F 4 "MOLEX" V 6075 7050 20  0001 C CNN "manf"
F 5 "172447-0204" V 6100 7050 20  0001 C CNN "manf#"
F 6 "DigiKey" V 6125 7050 20  0001 C CNN "Supplier"
F 7 "WM14942-ND" V 6150 7050 20  0001 C CNN "Supplier Part No"
F 8 "https://www.digikey.com.au/product-detail/en/1724470204/WM14942-ND/5116909?utm_campaign=buynow&WT.z_cid=ref_octopart_dkc_buynow&utm_medium=aggregator&curr=usd&site=us&utm_source=octopart" V 6175 7050 20  0001 C CNN "Supplier URL"
F 9 "1.66" V 6200 7050 20  0001 C CNN "Supplier Price"
F 10 "1" V 6225 7050 20  0001 C CNN "Supplier Price Break"
	1    5825 7050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5475 7200 5625 7200
Wire Wire Line
	5475 7200 5475 7700
Wire Wire Line
	4825 7100 5375 7100
$Comp
L appli_resistor:10K0_1608M R?
U 1 1 5ABEC5C2
P 3425 6975
F 0 "R?" H 3353 6790 50  0000 R CNN
F 1 "10K0_1608M" H 3353 6870 35  0000 R CNN
F 2 "RESC1608*" V 3535 6825 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 3560 6825 20  0001 C CNN
F 4 "TE CONNECTIVITY" V 3610 6825 20  0001 C CNN "manf"
F 5 "CRGH0603F10K" V 3635 6825 20  0001 C CNN "manf#"
F 6 "Element14" V 3660 6825 20  0001 C CNN "Supplier"
F 7 "2332016" V 3685 6825 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/te-connectivity/crgh0603f10k/resistor-power-10k-0-2w-1-0603/dp/2332016" V 3710 6825 20  0001 C CNN "Supplier URL"
F 9 "0.02" V 3735 6825 20  0001 C CNN "Supplier Price"
F 10 "10" V 3760 6825 20  0001 C CNN "Supplier Price Break"
	1    3425 6975
	1    0    0    1   
$EndComp
$Comp
L appli_capacitor:100NF_X7R_1608M C?
U 1 1 5ABEC5D0
P 3675 7450
F 0 "C?" H 3598 7315 50  0000 R CNN
F 1 "100NF_X7R_1608M" H 3598 7395 35  0000 R CNN
F 2 "CAPC1608*" V 3790 7350 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1732728.pdf" V 3815 7350 20  0001 C CNN
F 4 "KEMET" V 3865 7350 20  0001 C CNN "manf"
F 5 "C0603C104K5RACAUTO" V 3890 7350 20  0001 C CNN "manf#"
F 6 "Element14" V 3915 7350 20  0001 C CNN "Supplier"
F 7 "2070398" V 3940 7350 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c0603c104k5racauto/cap-mlcc-x7r-100nf-50v-0603/dp/2070398" V 3965 7350 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 3990 7350 20  0001 C CNN "Supplier Price"
F 10 "1" V 4015 7350 20  0001 C CNN "Supplier Price Break"
	1    3675 7450
	1    0    0    1   
$EndComp
$Comp
L appli_resistor:1K_QUAD_ARRAY_1608M R?
U 1 1 5ABFA3D0
P 2900 1725
F 0 "R?" V 2700 1575 50  0000 C CNN
F 1 "1K_QUAD_ARRAY_1608M" V 2780 1575 35  0000 C CNN
F 2 "Applidyne_Resistor:R_Array_Concave_4x1608M" V 3115 1575 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1885635.pdf" V 3140 1575 20  0001 C CNN
F 4 "BOURNS" V 3190 1575 20  0001 C CNN "manf"
F 5 "CAY16-102J4LF" V 3215 1575 20  0001 C CNN "manf#"
F 6 "Element14" V 3240 1575 20  0001 C CNN "Supplier"
F 7 "1770133" V 3265 1575 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/bourns/cay16-102j4lf/resistor-array-4-1k-1206-5-smd/dp/1770133?CMP=GRHB-OCTOPART" V 3290 1575 20  0001 C CNN "Supplier URL"
F 9 "0.073" V 3315 1575 20  0001 C CNN "Supplier Price"
F 10 "50" V 3340 1575 20  0001 C CNN "Supplier Price Break"
	1    2900 1725
	0    1    1    0   
$EndComp
Wire Wire Line
	2600 1725 2300 1725
$Comp
L appli_resistor:1K_QUAD_ARRAY_1608M R?
U 2 1 5AC34563
P 2925 3375
F 0 "R?" V 2725 3225 50  0000 C CNN
F 1 "1K_QUAD_ARRAY_1608M" V 2805 3225 35  0000 C CNN
F 2 "Applidyne_Resistor:R_Array_Concave_4x1608M" V 3140 3225 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1885635.pdf" V 3165 3225 20  0001 C CNN
F 4 "BOURNS" V 3215 3225 20  0001 C CNN "manf"
F 5 "CAY16-102J4LF" V 3240 3225 20  0001 C CNN "manf#"
F 6 "Element14" V 3265 3225 20  0001 C CNN "Supplier"
F 7 "1770133" V 3290 3225 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/bourns/cay16-102j4lf/resistor-array-4-1k-1206-5-smd/dp/1770133?CMP=GRHB-OCTOPART" V 3315 3225 20  0001 C CNN "Supplier URL"
F 9 "0.073" V 3340 3225 20  0001 C CNN "Supplier Price"
F 10 "50" V 3365 3225 20  0001 C CNN "Supplier Price Break"
	2    2925 3375
	0    1    1    0   
$EndComp
Wire Wire Line
	2625 3375 2300 3375
Wire Wire Line
	3825 5225 2975 5225
Wire Wire Line
	3425 5325 2150 5325
Text GLabel 2150 5075 0    50   Output ~ 0
MOTOR_3_CURRENT
Text GLabel 2150 5325 0    50   Output ~ 0
~MOTOR_3_OC
Wire Wire Line
	2975 5225 2975 5075
Wire Wire Line
	2975 5075 2925 5075
$Comp
L appli_resistor:1K_QUAD_ARRAY_1608M R?
U 3 1 5AC4DF5E
P 2925 5075
F 0 "R?" V 2725 4925 50  0000 C CNN
F 1 "1K_QUAD_ARRAY_1608M" V 2805 4925 35  0000 C CNN
F 2 "Applidyne_Resistor:R_Array_Concave_4x1608M" V 3140 4925 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1885635.pdf" V 3165 4925 20  0001 C CNN
F 4 "BOURNS" V 3215 4925 20  0001 C CNN "manf"
F 5 "CAY16-102J4LF" V 3240 4925 20  0001 C CNN "manf#"
F 6 "Element14" V 3265 4925 20  0001 C CNN "Supplier"
F 7 "1770133" V 3290 4925 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/bourns/cay16-102j4lf/resistor-array-4-1k-1206-5-smd/dp/1770133?CMP=GRHB-OCTOPART" V 3315 4925 20  0001 C CNN "Supplier URL"
F 9 "0.073" V 3340 4925 20  0001 C CNN "Supplier Price"
F 10 "50" V 3365 4925 20  0001 C CNN "Supplier Price Break"
	3    2925 5075
	0    1    1    0   
$EndComp
Wire Wire Line
	2625 5075 2300 5075
Wire Wire Line
	3825 7000 2975 7000
Wire Wire Line
	3425 7100 2150 7100
Text GLabel 2150 6850 0    50   Output ~ 0
MOTOR_4_CURRENT
Text GLabel 2150 7100 0    50   Output ~ 0
~MOTOR_4_OC
Wire Wire Line
	2975 7000 2975 6850
Wire Wire Line
	2975 6850 2925 6850
$Comp
L appli_resistor:1K_QUAD_ARRAY_1608M R?
U 4 1 5AC54726
P 2925 6850
F 0 "R?" V 2725 6700 50  0000 C CNN
F 1 "1K_QUAD_ARRAY_1608M" V 2805 6700 35  0000 C CNN
F 2 "Applidyne_Resistor:R_Array_Concave_4x1608M" V 3140 6700 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1885635.pdf" V 3165 6700 20  0001 C CNN
F 4 "BOURNS" V 3215 6700 20  0001 C CNN "manf"
F 5 "CAY16-102J4LF" V 3240 6700 20  0001 C CNN "manf#"
F 6 "Element14" V 3265 6700 20  0001 C CNN "Supplier"
F 7 "1770133" V 3290 6700 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/bourns/cay16-102j4lf/resistor-array-4-1k-1206-5-smd/dp/1770133?CMP=GRHB-OCTOPART" V 3315 6700 20  0001 C CNN "Supplier URL"
F 9 "0.073" V 3340 6700 20  0001 C CNN "Supplier Price"
F 10 "50" V 3365 6700 20  0001 C CNN "Supplier Price Break"
	4    2925 6850
	0    1    1    0   
$EndComp
Wire Wire Line
	2625 6850 2300 6850
Connection ~ 3425 5325
Connection ~ 3425 7100
Text GLabel 10100 1700 0    50   Output ~ 0
SERVO1_HLFB
Wire Wire Line
	5625 5125 5375 5125
Wire Wire Line
	5375 5125 5375 5325
Connection ~ 5375 5325
Wire Wire Line
	5375 5325 5625 5325
Wire Wire Line
	5625 5225 5475 5225
Wire Wire Line
	5475 5225 5475 5425
Connection ~ 5475 5425
Wire Wire Line
	5625 3525 5475 3525
Wire Wire Line
	5475 3525 5475 3725
Connection ~ 5475 3725
Wire Wire Line
	5625 7000 5475 7000
Wire Wire Line
	5475 7000 5475 7200
Connection ~ 5475 7200
Wire Wire Line
	5625 6900 5375 6900
Wire Wire Line
	5375 6900 5375 7100
Connection ~ 5375 7100
Wire Wire Line
	5375 7100 5625 7100
Wire Wire Line
	5625 3425 5375 3425
Wire Wire Line
	5375 3425 5375 3625
Connection ~ 5375 3625
Wire Wire Line
	5375 3625 5625 3625
Wire Wire Line
	5625 1850 5475 1850
Wire Wire Line
	5475 1850 5475 2050
Connection ~ 5475 2050
Wire Wire Line
	5625 1750 5375 1750
Wire Wire Line
	5375 1750 5375 1950
Connection ~ 5375 1950
Wire Wire Line
	5375 1950 5625 1950
Wire Wire Line
	11400 1950 11400 2000
$Comp
L appli_power:GND #PWR0164
U 1 1 5AC4B40C
P 11400 2000
F 0 "#PWR0164" H 11400 1750 50  0001 C CNN
F 1 "GND" H 11405 1827 50  0000 C CNN
F 2 "" H 11400 2000 60  0000 C CNN
F 3 "" H 11400 2000 60  0000 C CNN
	1    11400 2000
	1    0    0    -1  
$EndComp
$Comp
L appli_device:BAV99 D?
U 1 1 5AC535C5
P 10200 1700
F 0 "D?" H 10308 1746 50  0000 L CNN
F 1 "BAV99" H 10308 1655 50  0000 L CNN
F 2 "SOT23" H 10400 1550 20  0001 C CNN
F 3 "http://datasheet.octopart.com/BAV99-/T3-NXP-datasheet-5318834.pdf" H 10400 1525 20  0001 C CNN
F 4 "NXP" H 10400 1475 20  0001 C CNN "manf"
F 5 "RS Online" H 10400 1425 20  0001 C CNN "Supplier"
F 6 " 544-4534" H 10400 1400 20  0001 C CNN "Supplier Part No"
F 7 "http://au.rs-online.com/web/p/rectifier-schottky-diodes/5444534/" H 10400 1375 20  0001 C CNN "Supplier URL"
F 8 "0.045" H 10400 1350 20  0001 C CNN "Supplier Price"
F 9 "1" H 10400 1325 20  0001 C CNN "Supplier Price Break"
F 10 "BAV99" H 10400 1450 20  0001 C CNN "manf#"
	1    10200 1700
	1    0    0    -1  
$EndComp
$Comp
L appli_capacitor:10NF_X7R_1608M C?
U 1 1 5AC53ABE
P 11150 1950
F 0 "C?" H 11072 1815 50  0000 R CNN
F 1 "10NF_X7R_1608M" H 11072 1895 35  0000 R CNN
F 2 "CAPC1608*" V 11265 1850 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1679506.pdf" V 11290 1850 20  0001 C CNN
F 4 "AVX" V 11340 1850 20  0001 C CNN "manf"
F 5 "06033C103KAT2A" V 11365 1850 20  0001 C CNN "manf#"
F 6 "Element14" V 11390 1850 20  0001 C CNN "Supplier"
F 7 "2280646" V 11415 1850 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/avx/06033c103kat2a/cap-mlcc-x7r-10nf-25v-0603/dp/2280646" V 11440 1850 20  0001 C CNN "Supplier URL"
F 9 "0.002" V 11465 1850 20  0001 C CNN "Supplier Price"
F 10 "10" V 11490 1850 20  0001 C CNN "Supplier Price Break"
	1    11150 1950
	1    0    0    1   
$EndComp
$Comp
L appli_resistor:100R_1608M R?
U 1 1 5AC63736
P 11050 1700
F 0 "R?" V 10862 1550 50  0000 C CNN
F 1 "100R_1608M" V 10942 1550 35  0000 C CNN
F 2 "RESC1608*" V 11160 1550 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1788326.pdf" V 11185 1550 20  0001 C CNN
F 4 "MULTICOMP" V 11235 1550 20  0001 C CNN "manf"
F 5 "MC0063W06035100R" V 11260 1550 20  0001 C CNN "manf#"
F 6 "Element14" V 11285 1550 20  0001 C CNN "Supplier"
F 7 "9331689" V 11310 1550 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mc0063w06035100r/product-range-mc-series/dp/9331689" V 11335 1550 20  0001 C CNN "Supplier URL"
F 9 "0.022" V 11360 1550 20  0001 C CNN "Supplier Price"
F 10 "50" V 11385 1550 20  0001 C CNN "Supplier Price Break"
	1    11050 1700
	0    1    1    0   
$EndComp
Wire Wire Line
	11575 1700 11400 1700
Wire Wire Line
	11400 1750 11400 1700
Connection ~ 11400 1700
Wire Wire Line
	10100 1700 10200 1700
$Comp
L appli_power:+3.3V #PWR0165
U 1 1 5ACC2B2D
P 10200 1400
F 0 "#PWR0165" H 10200 1360 30  0001 C CNN
F 1 "+3.3V" H 10209 1538 30  0000 C CNN
F 2 "" H 10200 1400 60  0000 C CNN
F 3 "" H 10200 1400 60  0000 C CNN
	1    10200 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	10200 1400 10200 1450
$Comp
L appli_power:GND #PWR0166
U 1 1 5ACCAEF9
P 10200 2000
F 0 "#PWR0166" H 10200 1750 50  0001 C CNN
F 1 "GND" H 10205 1827 50  0000 C CNN
F 2 "" H 10200 2000 60  0000 C CNN
F 3 "" H 10200 2000 60  0000 C CNN
	1    10200 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	10200 2000 10200 1950
Wire Wire Line
	11050 1700 11150 1700
Wire Wire Line
	11150 1750 11150 1700
Connection ~ 11150 1700
Wire Wire Line
	11150 1700 11400 1700
Wire Wire Line
	11150 1950 11150 2000
$Comp
L appli_power:GND #PWR0167
U 1 1 5ACEC7F7
P 11150 2000
F 0 "#PWR0167" H 11150 1750 50  0001 C CNN
F 1 "GND" H 11155 1827 50  0000 C CNN
F 2 "" H 11150 2000 60  0000 C CNN
F 3 "" H 11150 2000 60  0000 C CNN
	1    11150 2000
	1    0    0    -1  
$EndComp
$Comp
L appli_device:3V3_500mW_SOD123 D?
U 1 1 5AD170A5
P 11400 2625
F 0 "D?" V 11354 2693 50  0000 L CNN
F 1 "3V3" V 11445 2693 50  0000 L CNN
F 2 "SOD3715*" H 11400 2475 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2244233.pdf?_ga=1.127173262.268044246.1492042373" H 11400 2450 20  0001 C CNN
F 4 "VISAY SEMICONDUCTOR" H 11400 2400 20  0001 C CNN "manf"
F 5 "MMSZ5226" H 11400 2375 20  0001 C CNN "manf#"
F 6 "Element14" H 11400 2350 20  0001 C CNN "Supplier"
F 7 "1900035" H 11400 2325 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/vishay-semiconductor/mmsz5257b-v-gs08/zener-diode/dp/1890860" H 11400 2300 20  0001 C CNN "Supplier URL"
F 9 "0.095" H 11400 2275 20  0001 C CNN "Supplier Price"
F 10 "1" H 11400 2250 20  0001 C CNN "Supplier Price Break"
	1    11400 2625
	0    1    1    0   
$EndComp
Wire Wire Line
	11400 2725 11400 2750
$Comp
L appli_power:GND #PWR0168
U 1 1 5AD170AD
P 11400 2750
F 0 "#PWR0168" H 11400 2500 50  0001 C CNN
F 1 "GND" H 11405 2577 50  0000 C CNN
F 2 "" H 11400 2750 60  0000 C CNN
F 3 "" H 11400 2750 60  0000 C CNN
	1    11400 2750
	1    0    0    -1  
$EndComp
$Comp
L appli_device:BAV99 D?
U 1 1 5AD170C8
P 10400 2450
F 0 "D?" H 10508 2496 50  0000 L CNN
F 1 "BAV99" H 10508 2405 50  0000 L CNN
F 2 "SOT23" H 10600 2300 20  0001 C CNN
F 3 "http://datasheet.octopart.com/BAV99-/T3-NXP-datasheet-5318834.pdf" H 10600 2275 20  0001 C CNN
F 4 "NXP" H 10600 2225 20  0001 C CNN "manf"
F 5 "RS Online" H 10600 2175 20  0001 C CNN "Supplier"
F 6 " 544-4534" H 10600 2150 20  0001 C CNN "Supplier Part No"
F 7 "http://au.rs-online.com/web/p/rectifier-schottky-diodes/5444534/" H 10600 2125 20  0001 C CNN "Supplier URL"
F 8 "0.045" H 10600 2100 20  0001 C CNN "Supplier Price"
F 9 "1" H 10600 2075 20  0001 C CNN "Supplier Price Break"
F 10 "BAV99" H 10600 2200 20  0001 C CNN "manf#"
	1    10400 2450
	1    0    0    -1  
$EndComp
$Comp
L appli_capacitor:10NF_X7R_1608M C?
U 1 1 5AD170D6
P 11175 2700
F 0 "C?" H 11097 2565 50  0000 R CNN
F 1 "10NF_X7R_1608M" H 11097 2645 35  0000 R CNN
F 2 "CAPC1608*" V 11290 2600 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1679506.pdf" V 11315 2600 20  0001 C CNN
F 4 "AVX" V 11365 2600 20  0001 C CNN "manf"
F 5 "06033C103KAT2A" V 11390 2600 20  0001 C CNN "manf#"
F 6 "Element14" V 11415 2600 20  0001 C CNN "Supplier"
F 7 "2280646" V 11440 2600 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/avx/06033c103kat2a/cap-mlcc-x7r-10nf-25v-0603/dp/2280646" V 11465 2600 20  0001 C CNN "Supplier URL"
F 9 "0.002" V 11490 2600 20  0001 C CNN "Supplier Price"
F 10 "10" V 11515 2600 20  0001 C CNN "Supplier Price Break"
	1    11175 2700
	1    0    0    1   
$EndComp
$Comp
L appli_resistor:100R_1608M R?
U 1 1 5AD170E4
P 11050 2450
F 0 "R?" V 10862 2300 50  0000 C CNN
F 1 "100R_1608M" V 10942 2300 35  0000 C CNN
F 2 "RESC1608*" V 11160 2300 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1788326.pdf" V 11185 2300 20  0001 C CNN
F 4 "MULTICOMP" V 11235 2300 20  0001 C CNN "manf"
F 5 "MC0063W06035100R" V 11260 2300 20  0001 C CNN "manf#"
F 6 "Element14" V 11285 2300 20  0001 C CNN "Supplier"
F 7 "9331689" V 11310 2300 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mc0063w06035100r/product-range-mc-series/dp/9331689" V 11335 2300 20  0001 C CNN "Supplier URL"
F 9 "0.022" V 11360 2300 20  0001 C CNN "Supplier Price"
F 10 "50" V 11385 2300 20  0001 C CNN "Supplier Price Break"
	1    11050 2450
	0    1    1    0   
$EndComp
Wire Wire Line
	11575 2450 11400 2450
Wire Wire Line
	11400 2525 11400 2450
Connection ~ 11400 2450
$Comp
L appli_power:+3.3V #PWR0169
U 1 1 5AD170F1
P 10400 2150
F 0 "#PWR0169" H 10400 2110 30  0001 C CNN
F 1 "+3.3V" H 10409 2288 30  0000 C CNN
F 2 "" H 10400 2150 60  0000 C CNN
F 3 "" H 10400 2150 60  0000 C CNN
	1    10400 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	10400 2150 10400 2200
$Comp
L appli_power:GND #PWR0170
U 1 1 5AD170F8
P 10400 2750
F 0 "#PWR0170" H 10400 2500 50  0001 C CNN
F 1 "GND" H 10405 2577 50  0000 C CNN
F 2 "" H 10400 2750 60  0000 C CNN
F 3 "" H 10400 2750 60  0000 C CNN
	1    10400 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	10400 2750 10400 2700
Wire Wire Line
	11050 2450 11175 2450
Wire Wire Line
	11175 2500 11175 2450
Connection ~ 11175 2450
Wire Wire Line
	11175 2450 11400 2450
Wire Wire Line
	11175 2700 11175 2775
$Comp
L appli_power:GND #PWR0171
U 1 1 5AD17104
P 11175 2775
F 0 "#PWR0171" H 11175 2525 50  0001 C CNN
F 1 "GND" H 11180 2602 50  0000 C CNN
F 2 "" H 11175 2775 60  0000 C CNN
F 3 "" H 11175 2775 60  0000 C CNN
	1    11175 2775
	1    0    0    -1  
$EndComp
$Comp
L appli_device:3V3_500mW_SOD123 D?
U 1 1 5ADB7608
P 11400 1850
F 0 "D?" V 11354 1918 50  0000 L CNN
F 1 "3V3" V 11445 1918 50  0000 L CNN
F 2 "SOD3715*" H 11400 1700 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2244233.pdf?_ga=1.127173262.268044246.1492042373" H 11400 1675 20  0001 C CNN
F 4 "VISAY SEMICONDUCTOR" H 11400 1625 20  0001 C CNN "manf"
F 5 "MMSZ5226" H 11400 1600 20  0001 C CNN "manf#"
F 6 "Element14" H 11400 1575 20  0001 C CNN "Supplier"
F 7 "1900035" H 11400 1550 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/vishay-semiconductor/mmsz5257b-v-gs08/zener-diode/dp/1890860" H 11400 1525 20  0001 C CNN "Supplier URL"
F 9 "0.095" H 11400 1500 20  0001 C CNN "Supplier Price"
F 10 "1" H 11400 1475 20  0001 C CNN "Supplier Price Break"
	1    11400 1850
	0    1    1    0   
$EndComp
Wire Wire Line
	10100 2450 10400 2450
Connection ~ 10200 1700
Wire Wire Line
	10200 1700 10750 1700
Connection ~ 10400 2450
Wire Wire Line
	10400 2450 10750 2450
$Comp
L appli_resistor:22K_1608M R?
U 1 1 5AEEF03A
P 11875 1700
F 0 "R?" V 11687 1550 50  0000 C CNN
F 1 "22K_1608M" V 11767 1550 35  0000 C CNN
F 2 "RESC1608*" V 11985 1550 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1716707.pdf" V 12010 1550 20  0001 C CNN
F 4 "MULTICOMP" V 12060 1550 20  0001 C CNN "manf"
F 5 "MCMR06X2202FTL" V 12085 1550 20  0001 C CNN "manf#"
F 6 "Element14" V 12110 1550 20  0001 C CNN "Supplier"
F 7 "2073430" V 12135 1550 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mcmr06x2202ftl/resistor-0603-22k-1-anti-sulfur/dp/2073430" V 12160 1550 20  0001 C CNN "Supplier URL"
F 9 "0.013" V 12185 1550 20  0001 C CNN "Supplier Price"
F 10 "100" V 12210 1550 20  0001 C CNN "Supplier Price Break"
	1    11875 1700
	0    1    1    0   
$EndComp
$Comp
L appli_resistor:22K_1608M R?
U 1 1 5AEEF126
P 11875 2450
F 0 "R?" V 11687 2300 50  0000 C CNN
F 1 "22K_1608M" V 11767 2300 35  0000 C CNN
F 2 "RESC1608*" V 11985 2300 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1716707.pdf" V 12010 2300 20  0001 C CNN
F 4 "MULTICOMP" V 12060 2300 20  0001 C CNN "manf"
F 5 "MCMR06X2202FTL" V 12085 2300 20  0001 C CNN "manf#"
F 6 "Element14" V 12110 2300 20  0001 C CNN "Supplier"
F 7 "2073430" V 12135 2300 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mcmr06x2202ftl/resistor-0603-22k-1-anti-sulfur/dp/2073430" V 12160 2300 20  0001 C CNN "Supplier URL"
F 9 "0.013" V 12185 2300 20  0001 C CNN "Supplier Price"
F 10 "100" V 12210 2300 20  0001 C CNN "Supplier Price Break"
	1    11875 2450
	0    1    1    0   
$EndComp
NoConn ~ 10450 4050
NoConn ~ 10450 4150
$Comp
L appli_connector_molex:MOLEX_MINIFIT_JR_2x4_WAY P?
U 1 1 5AF59803
P 13225 6200
F 0 "P?" H 13200 6800 50  0000 L CNN
F 1 "MOLEX_MINIFIT_JR_2x4_WAY" H 13200 6700 35  0000 L CNN
F 2 "Applidyne_Connector_Molex:Molex_MiniFit-JR-5569-08A2_2x04x4.20mm_Angled" V 13400 6200 20  0001 C CNN
F 3 "https://www.molex.com/pdm_docs/sd/1724470002_sd.pdf" V 13425 6200 20  0001 C CNN
F 4 "MOLEX" V 13475 6200 20  0001 C CNN "manf"
F 5 "172447-0208" V 13500 6200 20  0001 C CNN "manf#"
F 6 "Mouser" V 13525 6200 20  0001 C CNN "Supplier"
F 7 "538-172447-0208" V 13550 6200 20  0001 C CNN "Supplier Part No"
F 8 "https://au.mouser.com/ProductDetail/Molex/172447-0208?qs=sGAEpiMZZMvc81WFyF5Edkd3jpEQlJBQt6f989MIPuM%3d" V 13575 6200 20  0001 C CNN "Supplier URL"
F 9 "1.78" V 13600 6200 20  0001 C CNN "Supplier Price"
F 10 "1" V 13625 6200 20  0001 C CNN "Supplier Price Break"
	1    13225 6200
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+12V #PWR0172
U 1 1 5AF5980A
P 12475 5700
F 0 "#PWR0172" H 12475 5550 50  0001 C CNN
F 1 "+12V" H 12490 5873 50  0000 C CNN
F 2 "" H 12475 5700 60  0000 C CNN
F 3 "" H 12475 5700 60  0000 C CNN
	1    12475 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	13025 5950 12475 5950
Wire Wire Line
	13025 6050 12475 6050
Wire Wire Line
	12475 6050 12475 5950
Wire Wire Line
	13025 6150 12475 6150
Wire Wire Line
	12475 6150 12475 6050
Connection ~ 12475 6050
Wire Wire Line
	12475 5950 12475 5850
Connection ~ 12475 5950
Wire Wire Line
	13025 6250 12250 6250
Wire Wire Line
	12250 6250 12250 5250
Wire Wire Line
	12250 5250 11875 5250
$Comp
L Transistor_Array:ULN2803A U?
U 1 1 5AF5981B
P 10850 7200
F 0 "U?" H 10850 7767 50  0000 C CNN
F 1 "ULN2803A" H 10850 7676 50  0000 C CNN
F 2 "" H 10900 6550 50  0001 L CNN
F 3 "http://www.ti.com/lit/ds/symlink/uln2803a.pdf" H 10950 7000 50  0001 C CNN
	1    10850 7200
	1    0    0    -1  
$EndComp
Wire Wire Line
	12050 6550 13025 6550
Wire Wire Line
	11950 6450 13025 6450
Wire Wire Line
	11850 6350 13025 6350
$Comp
L appli_connector_molex:MOLEX_MINIFIT_JR_2x4_WAY P?
U 1 1 5AF5982C
P 13225 7150
F 0 "P?" H 13303 7180 50  0000 L CNN
F 1 "MOLEX_MINIFIT_JR_2x4_WAY" H 13303 7100 35  0000 L CNN
F 2 "Applidyne_Connector_Molex:Molex_MiniFit-JR-5569-08A2_2x04x4.20mm_Angled" V 13400 7150 20  0001 C CNN
F 3 "https://www.molex.com/pdm_docs/sd/1724470002_sd.pdf" V 13425 7150 20  0001 C CNN
F 4 "MOLEX" V 13475 7150 20  0001 C CNN "manf"
F 5 "172447-0208" V 13500 7150 20  0001 C CNN "manf#"
F 6 "Mouser" V 13525 7150 20  0001 C CNN "Supplier"
F 7 "538-172447-0208" V 13550 7150 20  0001 C CNN "Supplier Part No"
F 8 "https://au.mouser.com/ProductDetail/Molex/172447-0208?qs=sGAEpiMZZMvc81WFyF5Edkd3jpEQlJBQt6f989MIPuM%3d" V 13575 7150 20  0001 C CNN "Supplier URL"
F 9 "1.78" V 13600 7150 20  0001 C CNN "Supplier Price"
F 10 "1" V 13625 7150 20  0001 C CNN "Supplier Price Break"
	1    13225 7150
	1    0    0    -1  
$EndComp
Wire Wire Line
	13025 6900 12475 6900
Wire Wire Line
	13025 7000 12475 7000
Wire Wire Line
	12475 7000 12475 6900
Wire Wire Line
	13025 7100 12475 7100
Wire Wire Line
	12475 7100 12475 7000
Connection ~ 12475 7000
Wire Wire Line
	12475 6900 12475 6800
Connection ~ 12475 6900
Connection ~ 12475 6150
Wire Wire Line
	13025 6800 12475 6800
Wire Wire Line
	13025 7200 12150 7200
Wire Wire Line
	13025 7300 11250 7300
Wire Wire Line
	11250 7400 13025 7400
Wire Wire Line
	13025 7500 11250 7500
Wire Wire Line
	12050 6550 12050 7200
Wire Wire Line
	12050 7200 11250 7200
Wire Wire Line
	11250 7100 11950 7100
Wire Wire Line
	11950 7100 11950 6450
Wire Wire Line
	11250 7000 11850 7000
Wire Wire Line
	11850 7000 11850 6350
Wire Wire Line
	11250 6900 11650 6900
Wire Wire Line
	11650 6900 11650 6850
$Comp
L appli_power:+12V #PWR0173
U 1 1 5AF59849
P 11650 6850
F 0 "#PWR0173" H 11650 6700 50  0001 C CNN
F 1 "+12V" H 11665 7023 50  0000 C CNN
F 2 "" H 11650 6850 60  0000 C CNN
F 3 "" H 11650 6850 60  0000 C CNN
	1    11650 6850
	1    0    0    -1  
$EndComp
Wire Wire Line
	10450 7000 10100 7000
Wire Wire Line
	10450 7100 10100 7100
Wire Wire Line
	10450 7200 10100 7200
Wire Wire Line
	10450 7300 10100 7300
Wire Wire Line
	10450 7400 10100 7400
Wire Wire Line
	10450 7500 10100 7500
Wire Wire Line
	10850 7900 10850 7950
$Comp
L appli_power:GND #PWR0174
U 1 1 5AF59856
P 10850 7950
F 0 "#PWR0174" H 10850 7700 50  0001 C CNN
F 1 "GND" H 10855 7777 50  0000 C CNN
F 2 "" H 10850 7950 60  0000 C CNN
F 3 "" H 10850 7950 60  0000 C CNN
	1    10850 7950
	1    0    0    -1  
$EndComp
Wire Wire Line
	12150 6000 11875 6000
Wire Wire Line
	12150 6000 12150 7200
Text Notes 13325 6600 0    63   ~ 0
HLFB+\nINPUTB+\nINPUTA+\nENABLE+\nHLFB-\nINPUTB-\nINPUTA-\nENABLE-\n
Text GLabel 10100 7100 0    50   Input ~ 0
SERVO3_A
Text GLabel 10100 7000 0    50   Input ~ 0
SERVO3_B
Text GLabel 10100 7200 0    50   Input ~ 0
SERVO3_EN
Text GLabel 10100 7400 0    50   Input ~ 0
SERVO4_A
Text GLabel 10100 7300 0    50   Input ~ 0
SERVO4_B
Text GLabel 10100 7500 0    50   Input ~ 0
SERVO4_EN
Text GLabel 10100 6000 0    50   Output ~ 0
SERVO4_HLFB
Wire Wire Line
	9950 6625 9950 6650
Wire Wire Line
	13025 5850 12475 5850
Connection ~ 12475 5850
Wire Wire Line
	12475 5850 12475 5700
Connection ~ 12475 6800
Wire Wire Line
	12475 6800 12475 6150
$Comp
L appli_capacitor:100NF_X7R_1608M C?
U 1 1 5AF59873
P 9950 6425
F 0 "C?" H 10028 6360 50  0000 L CNN
F 1 "100NF_X7R_1608M" H 10028 6280 35  0000 L CNN
F 2 "CAPC1608*" V 10065 6325 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1732728.pdf" V 10090 6325 20  0001 C CNN
F 4 "KEMET" V 10140 6325 20  0001 C CNN "manf"
F 5 "C0603C104K5RACAUTO" V 10165 6325 20  0001 C CNN "manf#"
F 6 "Element14" V 10190 6325 20  0001 C CNN "Supplier"
F 7 "2070398" V 10215 6325 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c0603c104k5racauto/cap-mlcc-x7r-100nf-50v-0603/dp/2070398" V 10240 6325 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 10265 6325 20  0001 C CNN "Supplier Price"
F 10 "1" V 10290 6325 20  0001 C CNN "Supplier Price Break"
	1    9950 6425
	-1   0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR0175
U 1 1 5AF5987A
P 9950 6650
F 0 "#PWR0175" H 9950 6400 50  0001 C CNN
F 1 "GND" H 9955 6477 50  0000 C CNN
F 2 "" H 9950 6650 60  0000 C CNN
F 3 "" H 9950 6650 60  0000 C CNN
	1    9950 6650
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+12V #PWR0176
U 1 1 5AF59880
P 9950 6375
F 0 "#PWR0176" H 9950 6225 50  0001 C CNN
F 1 "+12V" H 9965 6548 50  0000 C CNN
F 2 "" H 9950 6375 60  0000 C CNN
F 3 "" H 9950 6375 60  0000 C CNN
	1    9950 6375
	1    0    0    -1  
$EndComp
Wire Wire Line
	9950 6375 9950 6425
Text GLabel 10100 5250 0    50   Output ~ 0
SERVO3_HLFB
Wire Wire Line
	11400 5500 11400 5550
$Comp
L appli_power:GND #PWR0177
U 1 1 5AF59889
P 11400 5550
F 0 "#PWR0177" H 11400 5300 50  0001 C CNN
F 1 "GND" H 11405 5377 50  0000 C CNN
F 2 "" H 11400 5550 60  0000 C CNN
F 3 "" H 11400 5550 60  0000 C CNN
	1    11400 5550
	1    0    0    -1  
$EndComp
$Comp
L appli_device:BAV99 D?
U 1 1 5AF59896
P 10200 5250
F 0 "D?" H 10308 5296 50  0000 L CNN
F 1 "BAV99" H 10308 5205 50  0000 L CNN
F 2 "SOT23" H 10400 5100 20  0001 C CNN
F 3 "http://datasheet.octopart.com/BAV99-/T3-NXP-datasheet-5318834.pdf" H 10400 5075 20  0001 C CNN
F 4 "NXP" H 10400 5025 20  0001 C CNN "manf"
F 5 "RS Online" H 10400 4975 20  0001 C CNN "Supplier"
F 6 " 544-4534" H 10400 4950 20  0001 C CNN "Supplier Part No"
F 7 "http://au.rs-online.com/web/p/rectifier-schottky-diodes/5444534/" H 10400 4925 20  0001 C CNN "Supplier URL"
F 8 "0.045" H 10400 4900 20  0001 C CNN "Supplier Price"
F 9 "1" H 10400 4875 20  0001 C CNN "Supplier Price Break"
F 10 "BAV99" H 10400 5000 20  0001 C CNN "manf#"
	1    10200 5250
	1    0    0    -1  
$EndComp
$Comp
L appli_capacitor:10NF_X7R_1608M C?
U 1 1 5AF598A4
P 11150 5500
F 0 "C?" H 11072 5365 50  0000 R CNN
F 1 "10NF_X7R_1608M" H 11072 5445 35  0000 R CNN
F 2 "CAPC1608*" V 11265 5400 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1679506.pdf" V 11290 5400 20  0001 C CNN
F 4 "AVX" V 11340 5400 20  0001 C CNN "manf"
F 5 "06033C103KAT2A" V 11365 5400 20  0001 C CNN "manf#"
F 6 "Element14" V 11390 5400 20  0001 C CNN "Supplier"
F 7 "2280646" V 11415 5400 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/avx/06033c103kat2a/cap-mlcc-x7r-10nf-25v-0603/dp/2280646" V 11440 5400 20  0001 C CNN "Supplier URL"
F 9 "0.002" V 11465 5400 20  0001 C CNN "Supplier Price"
F 10 "10" V 11490 5400 20  0001 C CNN "Supplier Price Break"
	1    11150 5500
	1    0    0    1   
$EndComp
$Comp
L appli_resistor:100R_1608M R?
U 1 1 5AF598B2
P 11050 5250
F 0 "R?" V 10862 5100 50  0000 C CNN
F 1 "100R_1608M" V 10942 5100 35  0000 C CNN
F 2 "RESC1608*" V 11160 5100 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1788326.pdf" V 11185 5100 20  0001 C CNN
F 4 "MULTICOMP" V 11235 5100 20  0001 C CNN "manf"
F 5 "MC0063W06035100R" V 11260 5100 20  0001 C CNN "manf#"
F 6 "Element14" V 11285 5100 20  0001 C CNN "Supplier"
F 7 "9331689" V 11310 5100 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mc0063w06035100r/product-range-mc-series/dp/9331689" V 11335 5100 20  0001 C CNN "Supplier URL"
F 9 "0.022" V 11360 5100 20  0001 C CNN "Supplier Price"
F 10 "50" V 11385 5100 20  0001 C CNN "Supplier Price Break"
	1    11050 5250
	0    1    1    0   
$EndComp
Wire Wire Line
	11575 5250 11400 5250
Wire Wire Line
	11400 5300 11400 5250
Connection ~ 11400 5250
Wire Wire Line
	10100 5250 10200 5250
$Comp
L appli_power:+3.3V #PWR0178
U 1 1 5AF598BD
P 10200 4950
F 0 "#PWR0178" H 10200 4910 30  0001 C CNN
F 1 "+3.3V" H 10209 5088 30  0000 C CNN
F 2 "" H 10200 4950 60  0000 C CNN
F 3 "" H 10200 4950 60  0000 C CNN
	1    10200 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	10200 4950 10200 5000
$Comp
L appli_power:GND #PWR0179
U 1 1 5AF598C4
P 10200 5550
F 0 "#PWR0179" H 10200 5300 50  0001 C CNN
F 1 "GND" H 10205 5377 50  0000 C CNN
F 2 "" H 10200 5550 60  0000 C CNN
F 3 "" H 10200 5550 60  0000 C CNN
	1    10200 5550
	1    0    0    -1  
$EndComp
Wire Wire Line
	10200 5550 10200 5500
Wire Wire Line
	11050 5250 11150 5250
Wire Wire Line
	11150 5300 11150 5250
Connection ~ 11150 5250
Wire Wire Line
	11150 5250 11400 5250
Wire Wire Line
	11150 5500 11150 5550
$Comp
L appli_power:GND #PWR0180
U 1 1 5AF598D0
P 11150 5550
F 0 "#PWR0180" H 11150 5300 50  0001 C CNN
F 1 "GND" H 11155 5377 50  0000 C CNN
F 2 "" H 11150 5550 60  0000 C CNN
F 3 "" H 11150 5550 60  0000 C CNN
	1    11150 5550
	1    0    0    -1  
$EndComp
$Comp
L appli_device:3V3_500mW_SOD123 D?
U 1 1 5AF598DD
P 11400 6175
F 0 "D?" V 11354 6243 50  0000 L CNN
F 1 "3V3" V 11445 6243 50  0000 L CNN
F 2 "SOD3715*" H 11400 6025 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2244233.pdf?_ga=1.127173262.268044246.1492042373" H 11400 6000 20  0001 C CNN
F 4 "VISAY SEMICONDUCTOR" H 11400 5950 20  0001 C CNN "manf"
F 5 "MMSZ5226" H 11400 5925 20  0001 C CNN "manf#"
F 6 "Element14" H 11400 5900 20  0001 C CNN "Supplier"
F 7 "1900035" H 11400 5875 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/vishay-semiconductor/mmsz5257b-v-gs08/zener-diode/dp/1890860" H 11400 5850 20  0001 C CNN "Supplier URL"
F 9 "0.095" H 11400 5825 20  0001 C CNN "Supplier Price"
F 10 "1" H 11400 5800 20  0001 C CNN "Supplier Price Break"
	1    11400 6175
	0    1    1    0   
$EndComp
Wire Wire Line
	11400 6275 11400 6300
$Comp
L appli_power:GND #PWR0181
U 1 1 5AF598E5
P 11400 6300
F 0 "#PWR0181" H 11400 6050 50  0001 C CNN
F 1 "GND" H 11405 6127 50  0000 C CNN
F 2 "" H 11400 6300 60  0000 C CNN
F 3 "" H 11400 6300 60  0000 C CNN
	1    11400 6300
	1    0    0    -1  
$EndComp
$Comp
L appli_device:BAV99 D?
U 1 1 5AF598F2
P 10400 6000
F 0 "D?" H 10508 6046 50  0000 L CNN
F 1 "BAV99" H 10508 5955 50  0000 L CNN
F 2 "SOT23" H 10600 5850 20  0001 C CNN
F 3 "http://datasheet.octopart.com/BAV99-/T3-NXP-datasheet-5318834.pdf" H 10600 5825 20  0001 C CNN
F 4 "NXP" H 10600 5775 20  0001 C CNN "manf"
F 5 "RS Online" H 10600 5725 20  0001 C CNN "Supplier"
F 6 " 544-4534" H 10600 5700 20  0001 C CNN "Supplier Part No"
F 7 "http://au.rs-online.com/web/p/rectifier-schottky-diodes/5444534/" H 10600 5675 20  0001 C CNN "Supplier URL"
F 8 "0.045" H 10600 5650 20  0001 C CNN "Supplier Price"
F 9 "1" H 10600 5625 20  0001 C CNN "Supplier Price Break"
F 10 "BAV99" H 10600 5750 20  0001 C CNN "manf#"
	1    10400 6000
	1    0    0    -1  
$EndComp
$Comp
L appli_capacitor:10NF_X7R_1608M C?
U 1 1 5AF59900
P 11175 6250
F 0 "C?" H 11097 6115 50  0000 R CNN
F 1 "10NF_X7R_1608M" H 11097 6195 35  0000 R CNN
F 2 "CAPC1608*" V 11290 6150 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1679506.pdf" V 11315 6150 20  0001 C CNN
F 4 "AVX" V 11365 6150 20  0001 C CNN "manf"
F 5 "06033C103KAT2A" V 11390 6150 20  0001 C CNN "manf#"
F 6 "Element14" V 11415 6150 20  0001 C CNN "Supplier"
F 7 "2280646" V 11440 6150 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/avx/06033c103kat2a/cap-mlcc-x7r-10nf-25v-0603/dp/2280646" V 11465 6150 20  0001 C CNN "Supplier URL"
F 9 "0.002" V 11490 6150 20  0001 C CNN "Supplier Price"
F 10 "10" V 11515 6150 20  0001 C CNN "Supplier Price Break"
	1    11175 6250
	1    0    0    1   
$EndComp
$Comp
L appli_resistor:100R_1608M R?
U 1 1 5AF5990E
P 11050 6000
F 0 "R?" V 10862 5850 50  0000 C CNN
F 1 "100R_1608M" V 10942 5850 35  0000 C CNN
F 2 "RESC1608*" V 11160 5850 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1788326.pdf" V 11185 5850 20  0001 C CNN
F 4 "MULTICOMP" V 11235 5850 20  0001 C CNN "manf"
F 5 "MC0063W06035100R" V 11260 5850 20  0001 C CNN "manf#"
F 6 "Element14" V 11285 5850 20  0001 C CNN "Supplier"
F 7 "9331689" V 11310 5850 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mc0063w06035100r/product-range-mc-series/dp/9331689" V 11335 5850 20  0001 C CNN "Supplier URL"
F 9 "0.022" V 11360 5850 20  0001 C CNN "Supplier Price"
F 10 "50" V 11385 5850 20  0001 C CNN "Supplier Price Break"
	1    11050 6000
	0    1    1    0   
$EndComp
Wire Wire Line
	11575 6000 11400 6000
Wire Wire Line
	11400 6075 11400 6000
Connection ~ 11400 6000
$Comp
L appli_power:+3.3V #PWR0182
U 1 1 5AF59918
P 10400 5700
F 0 "#PWR0182" H 10400 5660 30  0001 C CNN
F 1 "+3.3V" H 10409 5838 30  0000 C CNN
F 2 "" H 10400 5700 60  0000 C CNN
F 3 "" H 10400 5700 60  0000 C CNN
	1    10400 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	10400 5700 10400 5750
$Comp
L appli_power:GND #PWR0183
U 1 1 5AF5991F
P 10400 6300
F 0 "#PWR0183" H 10400 6050 50  0001 C CNN
F 1 "GND" H 10405 6127 50  0000 C CNN
F 2 "" H 10400 6300 60  0000 C CNN
F 3 "" H 10400 6300 60  0000 C CNN
	1    10400 6300
	1    0    0    -1  
$EndComp
Wire Wire Line
	10400 6300 10400 6250
Wire Wire Line
	11050 6000 11175 6000
Wire Wire Line
	11175 6050 11175 6000
Connection ~ 11175 6000
Wire Wire Line
	11175 6000 11400 6000
Wire Wire Line
	11175 6250 11175 6325
$Comp
L appli_power:GND #PWR0184
U 1 1 5AF5992B
P 11175 6325
F 0 "#PWR0184" H 11175 6075 50  0001 C CNN
F 1 "GND" H 11180 6152 50  0000 C CNN
F 2 "" H 11175 6325 60  0000 C CNN
F 3 "" H 11175 6325 60  0000 C CNN
	1    11175 6325
	1    0    0    -1  
$EndComp
$Comp
L appli_device:3V3_500mW_SOD123 D?
U 1 1 5AF59938
P 11400 5400
F 0 "D?" V 11354 5468 50  0000 L CNN
F 1 "3V3" V 11445 5468 50  0000 L CNN
F 2 "SOD3715*" H 11400 5250 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2244233.pdf?_ga=1.127173262.268044246.1492042373" H 11400 5225 20  0001 C CNN
F 4 "VISAY SEMICONDUCTOR" H 11400 5175 20  0001 C CNN "manf"
F 5 "MMSZ5226" H 11400 5150 20  0001 C CNN "manf#"
F 6 "Element14" H 11400 5125 20  0001 C CNN "Supplier"
F 7 "1900035" H 11400 5100 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/vishay-semiconductor/mmsz5257b-v-gs08/zener-diode/dp/1890860" H 11400 5075 20  0001 C CNN "Supplier URL"
F 9 "0.095" H 11400 5050 20  0001 C CNN "Supplier Price"
F 10 "1" H 11400 5025 20  0001 C CNN "Supplier Price Break"
	1    11400 5400
	0    1    1    0   
$EndComp
Wire Wire Line
	10100 6000 10400 6000
Connection ~ 10200 5250
Wire Wire Line
	10200 5250 10750 5250
Connection ~ 10400 6000
Wire Wire Line
	10400 6000 10750 6000
$Comp
L appli_resistor:22K_1608M R?
U 1 1 5AF5994B
P 11875 5250
F 0 "R?" V 11687 5100 50  0000 C CNN
F 1 "22K_1608M" V 11767 5100 35  0000 C CNN
F 2 "RESC1608*" V 11985 5100 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1716707.pdf" V 12010 5100 20  0001 C CNN
F 4 "MULTICOMP" V 12060 5100 20  0001 C CNN "manf"
F 5 "MCMR06X2202FTL" V 12085 5100 20  0001 C CNN "manf#"
F 6 "Element14" V 12110 5100 20  0001 C CNN "Supplier"
F 7 "2073430" V 12135 5100 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mcmr06x2202ftl/resistor-0603-22k-1-anti-sulfur/dp/2073430" V 12160 5100 20  0001 C CNN "Supplier URL"
F 9 "0.013" V 12185 5100 20  0001 C CNN "Supplier Price"
F 10 "100" V 12210 5100 20  0001 C CNN "Supplier Price Break"
	1    11875 5250
	0    1    1    0   
$EndComp
$Comp
L appli_resistor:22K_1608M R?
U 1 1 5AF59959
P 11875 6000
F 0 "R?" V 11687 5850 50  0000 C CNN
F 1 "22K_1608M" V 11767 5850 35  0000 C CNN
F 2 "RESC1608*" V 11985 5850 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1716707.pdf" V 12010 5850 20  0001 C CNN
F 4 "MULTICOMP" V 12060 5850 20  0001 C CNN "manf"
F 5 "MCMR06X2202FTL" V 12085 5850 20  0001 C CNN "manf#"
F 6 "Element14" V 12110 5850 20  0001 C CNN "Supplier"
F 7 "2073430" V 12135 5850 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mcmr06x2202ftl/resistor-0603-22k-1-anti-sulfur/dp/2073430" V 12160 5850 20  0001 C CNN "Supplier URL"
F 9 "0.013" V 12185 5850 20  0001 C CNN "Supplier Price"
F 10 "100" V 12210 5850 20  0001 C CNN "Supplier Price Break"
	1    11875 6000
	0    1    1    0   
$EndComp
NoConn ~ 10450 7600
NoConn ~ 10450 7700
Text Notes 12825 1900 0    50   Italic 0
Servo IO are optocouplers suitable for 5-24V switched logic.\n12V logic is used. Servo IO self-limits current.\n\nInputs (and Enable) require min 9mA.\nULN2803 is a octal NPN (darlington) driver.\n\nOutput is user-configurable.\nBehaves like a NPN (also has internal current limit).\nIn ASG or EN modes, acts as a HIGH = GOOD status flag.\nIn velocity or torque mode, outputs 45Hz squarewave.\n50% duty = 0, 5% = -MAX, 95% = +MAX
Text Notes 9175 4500 0    50   Italic 0
ULN2803 includes series resistors.\nAccepts 3.3V logic direct from micro.
Text GLabel 10100 9150 0    50   Input ~ 0
SERVO1_A
Text GLabel 10100 9050 0    50   Input ~ 0
SERVO1_B
Text GLabel 10100 9250 0    50   Input ~ 0
SERVO1_EN
Text GLabel 10100 9600 0    50   Input ~ 0
SERVO2_A
Text GLabel 10100 9500 0    50   Input ~ 0
SERVO2_B
Text GLabel 10100 9700 0    50   Input ~ 0
SERVO2_EN
Text GLabel 10100 10050 0    50   Input ~ 0
SERVO3_A
Text GLabel 10100 9950 0    50   Input ~ 0
SERVO3_B
Text GLabel 10100 10150 0    50   Input ~ 0
SERVO3_EN
Text GLabel 10100 10500 0    50   Input ~ 0
SERVO4_A
Text GLabel 10100 10400 0    50   Input ~ 0
SERVO4_B
Text GLabel 10100 10600 0    50   Input ~ 0
SERVO4_EN
Text Notes 9550 8650 0    79   ~ 16
Servo Data Logic Debug
Wire Wire Line
	10100 9050 10400 9050
Wire Wire Line
	10400 9150 10100 9150
Wire Wire Line
	10100 9250 10400 9250
Wire Wire Line
	10400 9500 10100 9500
Wire Wire Line
	10400 9600 10100 9600
Wire Wire Line
	10100 9700 10400 9700
Wire Wire Line
	10100 9950 10400 9950
Wire Wire Line
	10400 10050 10100 10050
Wire Wire Line
	10100 10150 10400 10150
Wire Wire Line
	10400 10400 10100 10400
Wire Wire Line
	10100 10500 10400 10500
Wire Wire Line
	10400 10600 10100 10600
$Comp
L appli_connector_wurth:CONN_01X04 P?
U 1 1 5B115C4D
P 10600 9100
F 0 "P?" H 10678 9141 50  0000 L CNN
F 1 "CONN_01X04" H 10678 9050 50  0000 L CNN
F 2 "Applidyne_Connector_Wurth:61300411121" V 10775 9100 50  0001 C CNN
F 3 "http://www.farnell.com/cad/2124416.pdf" H 10600 8775 50  0001 C CNN
F 4 "WURTH" H 10600 8650 60  0001 C CNN "manf"
F 5 "61300411121" H 10600 8550 60  0001 C CNN "manf#"
F 6 "Element 14" H 10600 8450 60  0001 C CNN "Supplier"
F 7 "2356155" H 10600 8375 60  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/wurth-elektronik/61300411121/header-2-54mm-pin-tht-vertical/dp/2356155" H 10600 8275 60  0001 C CNN "Supplier URL"
F 9 "0.125" H 10600 8175 60  0001 C CNN "Supplier Price"
F 10 "1" H 10600 8075 60  0001 C CNN "Supplier Price Break"
	1    10600 9100
	1    0    0    -1  
$EndComp
Text GLabel 10100 8950 0    50   Output ~ 0
SERVO1_HLFB
Text GLabel 10100 9400 0    50   Output ~ 0
SERVO2_HLFB
Text GLabel 10100 9850 0    50   Output ~ 0
SERVO3_HLFB
Text GLabel 10100 10300 0    50   Output ~ 0
SERVO4_HLFB
Wire Wire Line
	10100 8950 10400 8950
Wire Wire Line
	10100 9400 10400 9400
Wire Wire Line
	10100 9850 10400 9850
Wire Wire Line
	10100 10300 10400 10300
$Comp
L appli_connector_wurth:CONN_01X04 P?
U 1 1 5B161F6C
P 10600 9550
F 0 "P?" H 10678 9591 50  0000 L CNN
F 1 "CONN_01X04" H 10678 9500 50  0000 L CNN
F 2 "Applidyne_Connector_Wurth:61300411121" V 10775 9550 50  0001 C CNN
F 3 "http://www.farnell.com/cad/2124416.pdf" H 10600 9225 50  0001 C CNN
F 4 "WURTH" H 10600 9100 60  0001 C CNN "manf"
F 5 "61300411121" H 10600 9000 60  0001 C CNN "manf#"
F 6 "Element 14" H 10600 8900 60  0001 C CNN "Supplier"
F 7 "2356155" H 10600 8825 60  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/wurth-elektronik/61300411121/header-2-54mm-pin-tht-vertical/dp/2356155" H 10600 8725 60  0001 C CNN "Supplier URL"
F 9 "0.125" H 10600 8625 60  0001 C CNN "Supplier Price"
F 10 "1" H 10600 8525 60  0001 C CNN "Supplier Price Break"
	1    10600 9550
	1    0    0    -1  
$EndComp
$Comp
L appli_connector_wurth:CONN_01X04 P?
U 1 1 5B161FF4
P 10600 10000
F 0 "P?" H 10678 10041 50  0000 L CNN
F 1 "CONN_01X04" H 10678 9950 50  0000 L CNN
F 2 "Applidyne_Connector_Wurth:61300411121" V 10775 10000 50  0001 C CNN
F 3 "http://www.farnell.com/cad/2124416.pdf" H 10600 9675 50  0001 C CNN
F 4 "WURTH" H 10600 9550 60  0001 C CNN "manf"
F 5 "61300411121" H 10600 9450 60  0001 C CNN "manf#"
F 6 "Element 14" H 10600 9350 60  0001 C CNN "Supplier"
F 7 "2356155" H 10600 9275 60  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/wurth-elektronik/61300411121/header-2-54mm-pin-tht-vertical/dp/2356155" H 10600 9175 60  0001 C CNN "Supplier URL"
F 9 "0.125" H 10600 9075 60  0001 C CNN "Supplier Price"
F 10 "1" H 10600 8975 60  0001 C CNN "Supplier Price Break"
	1    10600 10000
	1    0    0    -1  
$EndComp
$Comp
L appli_connector_wurth:CONN_01X04 P?
U 1 1 5B162086
P 10600 10450
F 0 "P?" H 10678 10491 50  0000 L CNN
F 1 "CONN_01X04" H 10678 10400 50  0000 L CNN
F 2 "Applidyne_Connector_Wurth:61300411121" V 10775 10450 50  0001 C CNN
F 3 "http://www.farnell.com/cad/2124416.pdf" H 10600 10125 50  0001 C CNN
F 4 "WURTH" H 10600 10000 60  0001 C CNN "manf"
F 5 "61300411121" H 10600 9900 60  0001 C CNN "manf#"
F 6 "Element 14" H 10600 9800 60  0001 C CNN "Supplier"
F 7 "2356155" H 10600 9725 60  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/wurth-elektronik/61300411121/header-2-54mm-pin-tht-vertical/dp/2356155" H 10600 9625 60  0001 C CNN "Supplier URL"
F 9 "0.125" H 10600 9525 60  0001 C CNN "Supplier Price"
F 10 "1" H 10600 9425 60  0001 C CNN "Supplier Price Break"
	1    10600 10450
	1    0    0    -1  
$EndComp
Text Notes 9500 8775 0    50   ~ 0
For use with scopes or logic analysers.
Text Notes 5925 2100 0    63   ~ 0
+75V\nGND\n+75V\nGND\n
$Comp
L appli_test_points:CLIP_HOOK_PURPLE TEST?
U 1 1 5ACEAC98
P 2300 3150
F 0 "TEST?" V 2231 3328 39  0000 L CNN
F 1 "CLIP_HOOK_PURPLE" H 2350 3150 39  0001 L TNN
F 2 "Applidyne_Test:KEYSTONE_5119" H 2350 3100 39  0001 L TNN
F 3 "http://www.farnell.com/datasheets/1703983.pdf" H 2350 2950 39  0001 L BNN
F 4 "KEYSTONE" H 2350 3050 39  0001 L TNN "manf"
F 5 "5119" H 2350 2900 39  0001 L BNN "manf#"
F 6 "Element14" H 2350 2850 39  0001 L BNN "Supplier"
F 7 "2501305" H 2350 2800 39  0001 L BNN "Supplier Part No"
F 8 "http://au.element14.com/keystone/5119/test-point-pcb-purple-through/dp/2501305" H 2350 2750 39  0001 L BNN "Supplier URL"
F 9 "0.3" H 2350 2700 39  0001 L BNN "Supplier Price"
F 10 "1" H 2350 2650 39  0001 L BNN "Suppier Price Break"
	1    2300 3150
	0    1    1    0   
$EndComp
Wire Wire Line
	2300 3150 2300 3375
Connection ~ 2300 3375
Wire Wire Line
	2300 3375 2150 3375
$Comp
L appli_test_points:CLIP_HOOK_PURPLE TEST?
U 1 1 5ACFDF96
P 2300 4850
F 0 "TEST?" V 2231 5028 39  0000 L CNN
F 1 "CLIP_HOOK_PURPLE" H 2350 4850 39  0001 L TNN
F 2 "Applidyne_Test:KEYSTONE_5119" H 2350 4800 39  0001 L TNN
F 3 "http://www.farnell.com/datasheets/1703983.pdf" H 2350 4650 39  0001 L BNN
F 4 "KEYSTONE" H 2350 4750 39  0001 L TNN "manf"
F 5 "5119" H 2350 4600 39  0001 L BNN "manf#"
F 6 "Element14" H 2350 4550 39  0001 L BNN "Supplier"
F 7 "2501305" H 2350 4500 39  0001 L BNN "Supplier Part No"
F 8 "http://au.element14.com/keystone/5119/test-point-pcb-purple-through/dp/2501305" H 2350 4450 39  0001 L BNN "Supplier URL"
F 9 "0.3" H 2350 4400 39  0001 L BNN "Supplier Price"
F 10 "1" H 2350 4350 39  0001 L BNN "Suppier Price Break"
	1    2300 4850
	0    1    1    0   
$EndComp
$Comp
L appli_test_points:CLIP_HOOK_PURPLE TEST?
U 1 1 5AD11933
P 2300 6625
F 0 "TEST?" V 2231 6803 39  0000 L CNN
F 1 "CLIP_HOOK_PURPLE" H 2350 6625 39  0001 L TNN
F 2 "Applidyne_Test:KEYSTONE_5119" H 2350 6575 39  0001 L TNN
F 3 "http://www.farnell.com/datasheets/1703983.pdf" H 2350 6425 39  0001 L BNN
F 4 "KEYSTONE" H 2350 6525 39  0001 L TNN "manf"
F 5 "5119" H 2350 6375 39  0001 L BNN "manf#"
F 6 "Element14" H 2350 6325 39  0001 L BNN "Supplier"
F 7 "2501305" H 2350 6275 39  0001 L BNN "Supplier Part No"
F 8 "http://au.element14.com/keystone/5119/test-point-pcb-purple-through/dp/2501305" H 2350 6225 39  0001 L BNN "Supplier URL"
F 9 "0.3" H 2350 6175 39  0001 L BNN "Supplier Price"
F 10 "1" H 2350 6125 39  0001 L BNN "Suppier Price Break"
	1    2300 6625
	0    1    1    0   
$EndComp
Wire Wire Line
	2300 6625 2300 6850
Connection ~ 2300 6850
Wire Wire Line
	2300 6850 2150 6850
Wire Wire Line
	2300 4850 2300 5075
Connection ~ 2300 5075
Wire Wire Line
	2300 5075 2150 5075
$Comp
L appli_test_points:CLIP_HOOK_PURPLE TEST?
U 1 1 5AD4DDF1
P 2300 1500
F 0 "TEST?" V 2231 1678 39  0000 L CNN
F 1 "CLIP_HOOK_PURPLE" H 2350 1500 39  0001 L TNN
F 2 "Applidyne_Test:KEYSTONE_5119" H 2350 1450 39  0001 L TNN
F 3 "http://www.farnell.com/datasheets/1703983.pdf" H 2350 1300 39  0001 L BNN
F 4 "KEYSTONE" H 2350 1400 39  0001 L TNN "manf"
F 5 "5119" H 2350 1250 39  0001 L BNN "manf#"
F 6 "Element14" H 2350 1200 39  0001 L BNN "Supplier"
F 7 "2501305" H 2350 1150 39  0001 L BNN "Supplier Part No"
F 8 "http://au.element14.com/keystone/5119/test-point-pcb-purple-through/dp/2501305" H 2350 1100 39  0001 L BNN "Supplier URL"
F 9 "0.3" H 2350 1050 39  0001 L BNN "Supplier Price"
F 10 "1" H 2350 1000 39  0001 L BNN "Suppier Price Break"
	1    2300 1500
	0    1    1    0   
$EndComp
Wire Wire Line
	2300 1500 2300 1725
Connection ~ 2300 1725
Wire Wire Line
	2300 1725 2150 1725
$EndSCHEMATC
