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
Text Notes 7900 775  0    79   ~ 16
Servo Power Connectors
Text Notes 2450 775  0    79   ~ 16
Servo Data Connectors
$Comp
L appli_capacitor:100NF_X7R_1005M C?
U 1 1 5AB91AFB
P 8050 2025
F 0 "C?" H 7973 1890 50  0000 R CNN
F 1 "100NF_X7R_1005M" H 7973 1970 35  0000 R CNN
F 2 "Applidyne_Capacitor:CAPC1005X60N" V 8165 1925 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1936565.pdf" V 8190 1925 20  0001 C CNN
F 4 "TDK" V 8240 1925 20  0001 C CNN "manf"
F 5 "CGA2B3X7R1H104K050BB" V 8265 1925 20  0001 C CNN "manf#"
F 6 "Element14" V 8290 1925 20  0001 C CNN "Supplier"
F 7 "2210822" V 8315 1925 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/tdk/cga2b3x7r1h104k050bb/cap-mlcc-x7r-100nf-50v-0402/dp/2210822" V 8340 1925 20  0001 C CNN "Supplier URL"
F 9 "0.087" V 8365 1925 20  0001 C CNN "Supplier Price"
F 10 "100" V 8390 1925 20  0001 C CNN "Supplier Price Break"
	1    8050 2025
	1    0    0    1   
$EndComp
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5AB91CBE
P 8050 1325
F 0 "#PWR?" H 8050 1285 30  0001 C CNN
F 1 "+3.3V" H 8059 1463 30  0000 C CNN
F 2 "" H 8050 1325 60  0000 C CNN
F 3 "" H 8050 1325 60  0000 C CNN
	1    8050 1325
	1    0    0    -1  
$EndComp
Wire Wire Line
	8050 1825 8050 1475
Wire Wire Line
	8050 1475 8200 1475
Connection ~ 8050 1475
Wire Wire Line
	8050 1475 8050 1325
Wire Wire Line
	8700 2175 8700 2225
Wire Wire Line
	8050 2025 8050 2275
$Comp
L appli_power:GND #PWR?
U 1 1 5AB91D62
P 8700 2275
F 0 "#PWR?" H 8700 2025 50  0001 C CNN
F 1 "GND" H 8705 2102 50  0000 C CNN
F 2 "" H 8700 2275 60  0000 C CNN
F 3 "" H 8700 2275 60  0000 C CNN
	1    8700 2275
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5AB91D78
P 8050 2275
F 0 "#PWR?" H 8050 2025 50  0001 C CNN
F 1 "GND" H 8055 2102 50  0000 C CNN
F 2 "" H 8050 2275 60  0000 C CNN
F 3 "" H 8050 2275 60  0000 C CNN
	1    8050 2275
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+POWER #PWR?
U 1 1 5AB91E53
P 9350 1325
F 0 "#PWR?" H 9350 1275 20  0001 C CNN
F 1 "+POWER" H 9353 1463 30  0000 C CNN
F 2 "" H 9350 1325 60  0000 C CNN
F 3 "" H 9350 1325 60  0000 C CNN
	1    9350 1325
	1    0    0    -1  
$EndComp
Wire Wire Line
	9350 1325 9350 1475
Wire Wire Line
	9350 1475 9200 1475
$Comp
L appli_special:ACS711KEXLT_15AB U?
U 1 1 5AB955A4
P 8700 1675
F 0 "U?" H 8700 2140 50  0000 C CNN
F 1 "ACS711KEXLT_15AB" H 8700 2049 50  0000 C CNN
F 2 "Applidyne_QFN:ACS-EX-12" H 8700 875 50  0001 C CNN
F 3 "http://www.allegromicro.com/~/media/Files/Datasheets/ACS711-Datasheet.ashx" H 8700 575 50  0001 C CNN
F 4 "Allegro MicroSystems" H 8700 775 50  0001 C CNN "manf"
F 5 "ACS711KEXLT-15AB-T" H 8700 675 50  0001 C CNN "manf#"
F 6 "DigiKey" H 8700 475 50  0001 C CNN "Supplier"
F 7 "620-1483-1-ND" H 8700 375 50  0001 C CNN "Suppler Part No"
F 8 "https://www.digikey.com.au/product-detail/en/allegro-microsystems-llc/ACS711KEXLT-15AB-T/620-1483-1-ND/3868192" H 8700 275 50  0001 C CNN "Supplier URL"
F 9 "1.90" H 8700 175 50  0001 C CNN "Supplier Price"
F 10 "1" H 8700 75  50  0001 C CNN "Supplier Price Break"
	1    8700 1675
	1    0    0    -1  
$EndComp
Wire Wire Line
	8500 2175 8500 2225
Wire Wire Line
	8500 2225 8600 2225
Connection ~ 8700 2225
Wire Wire Line
	8700 2225 8700 2275
Wire Wire Line
	8900 2175 8900 2225
Wire Wire Line
	8900 2225 8800 2225
Wire Wire Line
	8800 2175 8800 2225
Connection ~ 8800 2225
Wire Wire Line
	8800 2225 8700 2225
Wire Wire Line
	8600 2175 8600 2225
Connection ~ 8600 2225
Wire Wire Line
	8600 2225 8700 2225
Text Notes 8950 2200 0    50   ~ 0
NC->GND for \nbetter EMI per DS
Text Notes 6550 1400 0    50   ~ 0
90mA/A sensitivity.\n3.3V/2 = 0A
Wire Wire Line
	8200 1575 7350 1575
Wire Wire Line
	8200 1675 7800 1675
$Comp
L appli_resistor:10K_1005M R?
U 1 1 5AB96600
P 7800 1550
F 0 "R?" H 7727 1365 50  0000 R CNN
F 1 "10K_1005M" H 7727 1445 35  0000 R CNN
F 2 "Applidyne_Resistor:RESC1005X40N" V 7910 1400 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2111203.pdf" V 7935 1400 20  0001 C CNN
F 4 "VISHAY" V 7985 1400 20  0001 C CNN "manf"
F 5 "CRCW040210K0FKED" V 8010 1400 20  0001 C CNN "manf#"
F 6 "Element14" V 8035 1400 20  0001 C CNN "Supplier"
F 7 "1469669" V 8060 1400 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/vishay/crcw040210k0fked/res-thick-film-10k-1-0-063w-0402/dp/1469669" V 8085 1400 20  0001 C CNN "Supplier URL"
F 9 "0.009" V 8110 1400 20  0001 C CNN "Supplier Price"
F 10 "10" V 8135 1400 20  0001 C CNN "Supplier Price Break"
	1    7800 1550
	1    0    0    1   
$EndComp
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5AB96FA3
P 7800 1225
F 0 "#PWR?" H 7800 1185 30  0001 C CNN
F 1 "+3.3V" H 7809 1363 30  0000 C CNN
F 2 "" H 7800 1225 60  0000 C CNN
F 3 "" H 7800 1225 60  0000 C CNN
	1    7800 1225
	1    0    0    -1  
$EndComp
Wire Wire Line
	7800 1225 7800 1250
Wire Wire Line
	7800 1550 7800 1675
Connection ~ 7800 1675
Wire Wire Line
	7800 1675 7250 1675
Text GLabel 7250 1525 0    50   Output ~ 0
MOTOR_1_CURRENT
Text GLabel 7250 1675 0    50   Output ~ 0
~MOTOR_1_OC
Text Notes 6475 1950 0    50   ~ 0
FAULT latches low when\ncurrent > 15A rating
Wire Wire Line
	7350 1575 7350 1525
Wire Wire Line
	7350 1525 7250 1525
$Comp
L appli_power:GND #PWR?
U 1 1 5AB9A0AB
P 9850 2275
F 0 "#PWR?" H 9850 2025 50  0001 C CNN
F 1 "GND" H 9855 2102 50  0000 C CNN
F 2 "" H 9850 2275 60  0000 C CNN
F 3 "" H 9850 2275 60  0000 C CNN
	1    9850 2275
	1    0    0    -1  
$EndComp
$Comp
L appli_connector_molex:MOLEX_MINIFIT_JR_2x2_WAY P?
U 1 1 5AB9CD10
P 10200 1625
F 0 "P?" H 10278 1655 50  0000 L CNN
F 1 "MOLEX_MINIFIT_JR_2x2_WAY" H 10278 1575 35  0000 L CNN
F 2 "Applidyne_Connector_Molex:Molex_Mini-Fit_Jr_5566-04A_2x02_P4.20mm_Vertical" V 10375 1625 20  0001 C CNN
F 3 "https://www.molex.com/pdm_docs/sd/1724470002_sd.pdf" V 10400 1625 20  0001 C CNN
F 4 "MOLEX" V 10450 1625 20  0001 C CNN "manf"
F 5 "172447-0204" V 10475 1625 20  0001 C CNN "manf#"
F 6 "DigiKey" V 10500 1625 20  0001 C CNN "Supplier"
F 7 "WM14942-ND" V 10525 1625 20  0001 C CNN "Supplier Part No"
F 8 "https://www.digikey.com.au/product-detail/en/1724470204/WM14942-ND/5116909?utm_campaign=buynow&WT.z_cid=ref_octopart_dkc_buynow&utm_medium=aggregator&curr=usd&site=us&utm_source=octopart" V 10550 1625 20  0001 C CNN "Supplier URL"
F 9 "1.66" V 10575 1625 20  0001 C CNN "Supplier Price"
F 10 "1" V 10600 1625 20  0001 C CNN "Supplier Price Break"
	1    10200 1625
	1    0    0    -1  
$EndComp
$Comp
L appli_connector_molex:MOLEX_MINIFIT_JR_2x4_WAY P?
U 1 1 5AB9CF32
P 5325 2350
F 0 "P?" H 5300 2950 50  0000 L CNN
F 1 "MOLEX_MINIFIT_JR_2x4_WAY" H 5300 2850 35  0000 L CNN
F 2 "Applidyne_Connector_Molex:Molex_MiniFit-JR-5569-08A2_2x04x4.20mm_Angled" V 5500 2350 20  0001 C CNN
F 3 "https://www.molex.com/pdm_docs/sd/1724470002_sd.pdf" V 5525 2350 20  0001 C CNN
F 4 "MOLEX" V 5575 2350 20  0001 C CNN "manf"
F 5 "172447-0208" V 5600 2350 20  0001 C CNN "manf#"
F 6 "Mouser" V 5625 2350 20  0001 C CNN "Supplier"
F 7 "538-172447-0208" V 5650 2350 20  0001 C CNN "Supplier Part No"
F 8 "https://au.mouser.com/ProductDetail/Molex/172447-0208?qs=sGAEpiMZZMvc81WFyF5Edkd3jpEQlJBQt6f989MIPuM%3d" V 5675 2350 20  0001 C CNN "Supplier URL"
F 9 "1.78" V 5700 2350 20  0001 C CNN "Supplier Price"
F 10 "1" V 5725 2350 20  0001 C CNN "Supplier Price Break"
	1    5325 2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	9850 1775 10000 1775
Wire Wire Line
	9850 1775 9850 2275
$Comp
L appli_capacitor:100NF_X7R_1005M C?
U 1 1 5AB9FC14
P 8050 3600
F 0 "C?" H 7973 3465 50  0000 R CNN
F 1 "100NF_X7R_1005M" H 7973 3545 35  0000 R CNN
F 2 "Applidyne_Capacitor:CAPC1005X60N" V 8165 3500 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1936565.pdf" V 8190 3500 20  0001 C CNN
F 4 "TDK" V 8240 3500 20  0001 C CNN "manf"
F 5 "CGA2B3X7R1H104K050BB" V 8265 3500 20  0001 C CNN "manf#"
F 6 "Element14" V 8290 3500 20  0001 C CNN "Supplier"
F 7 "2210822" V 8315 3500 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/tdk/cga2b3x7r1h104k050bb/cap-mlcc-x7r-100nf-50v-0402/dp/2210822" V 8340 3500 20  0001 C CNN "Supplier URL"
F 9 "0.087" V 8365 3500 20  0001 C CNN "Supplier Price"
F 10 "100" V 8390 3500 20  0001 C CNN "Supplier Price Break"
	1    8050 3600
	1    0    0    1   
$EndComp
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5AB9FC1B
P 8050 2900
F 0 "#PWR?" H 8050 2860 30  0001 C CNN
F 1 "+3.3V" H 8059 3038 30  0000 C CNN
F 2 "" H 8050 2900 60  0000 C CNN
F 3 "" H 8050 2900 60  0000 C CNN
	1    8050 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	8050 3400 8050 3050
Wire Wire Line
	8050 3050 8200 3050
Connection ~ 8050 3050
Wire Wire Line
	8050 3050 8050 2900
Wire Wire Line
	8700 3750 8700 3800
Wire Wire Line
	8050 3600 8050 3850
$Comp
L appli_power:GND #PWR?
U 1 1 5AB9FC27
P 8700 3850
F 0 "#PWR?" H 8700 3600 50  0001 C CNN
F 1 "GND" H 8705 3677 50  0000 C CNN
F 2 "" H 8700 3850 60  0000 C CNN
F 3 "" H 8700 3850 60  0000 C CNN
	1    8700 3850
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5AB9FC2D
P 8050 3850
F 0 "#PWR?" H 8050 3600 50  0001 C CNN
F 1 "GND" H 8055 3677 50  0000 C CNN
F 2 "" H 8050 3850 60  0000 C CNN
F 3 "" H 8050 3850 60  0000 C CNN
	1    8050 3850
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+POWER #PWR?
U 1 1 5AB9FC33
P 9350 2900
F 0 "#PWR?" H 9350 2850 20  0001 C CNN
F 1 "+POWER" H 9353 3038 30  0000 C CNN
F 2 "" H 9350 2900 60  0000 C CNN
F 3 "" H 9350 2900 60  0000 C CNN
	1    9350 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	9350 2900 9350 3050
Wire Wire Line
	9350 3050 9200 3050
$Comp
L appli_special:ACS711KEXLT_15AB U?
U 1 1 5AB9FC42
P 8700 3250
F 0 "U?" H 8700 3715 50  0000 C CNN
F 1 "ACS711KEXLT_15AB" H 8700 3624 50  0000 C CNN
F 2 "Applidyne_QFN:ACS-EX-12" H 8700 2450 50  0001 C CNN
F 3 "http://www.allegromicro.com/~/media/Files/Datasheets/ACS711-Datasheet.ashx" H 8700 2150 50  0001 C CNN
F 4 "Allegro MicroSystems" H 8700 2350 50  0001 C CNN "manf"
F 5 "ACS711KEXLT-15AB-T" H 8700 2250 50  0001 C CNN "manf#"
F 6 "DigiKey" H 8700 2050 50  0001 C CNN "Supplier"
F 7 "620-1483-1-ND" H 8700 1950 50  0001 C CNN "Suppler Part No"
F 8 "https://www.digikey.com.au/product-detail/en/allegro-microsystems-llc/ACS711KEXLT-15AB-T/620-1483-1-ND/3868192" H 8700 1850 50  0001 C CNN "Supplier URL"
F 9 "1.90" H 8700 1750 50  0001 C CNN "Supplier Price"
F 10 "1" H 8700 1650 50  0001 C CNN "Supplier Price Break"
	1    8700 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	8500 3750 8500 3800
Wire Wire Line
	8500 3800 8600 3800
Connection ~ 8700 3800
Wire Wire Line
	8700 3800 8700 3850
Wire Wire Line
	8900 3750 8900 3800
Wire Wire Line
	8900 3800 8800 3800
Wire Wire Line
	8800 3750 8800 3800
Connection ~ 8800 3800
Wire Wire Line
	8800 3800 8700 3800
Wire Wire Line
	8600 3750 8600 3800
Connection ~ 8600 3800
Wire Wire Line
	8600 3800 8700 3800
Wire Wire Line
	8200 3150 7350 3150
Wire Wire Line
	8200 3250 7800 3250
$Comp
L appli_resistor:10K_1005M R?
U 1 1 5AB9FC60
P 7800 3125
F 0 "R?" H 7727 2940 50  0000 R CNN
F 1 "10K_1005M" H 7727 3020 35  0000 R CNN
F 2 "Applidyne_Resistor:RESC1005X40N" V 7910 2975 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2111203.pdf" V 7935 2975 20  0001 C CNN
F 4 "VISHAY" V 7985 2975 20  0001 C CNN "manf"
F 5 "CRCW040210K0FKED" V 8010 2975 20  0001 C CNN "manf#"
F 6 "Element14" V 8035 2975 20  0001 C CNN "Supplier"
F 7 "1469669" V 8060 2975 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/vishay/crcw040210k0fked/res-thick-film-10k-1-0-063w-0402/dp/1469669" V 8085 2975 20  0001 C CNN "Supplier URL"
F 9 "0.009" V 8110 2975 20  0001 C CNN "Supplier Price"
F 10 "10" V 8135 2975 20  0001 C CNN "Supplier Price Break"
	1    7800 3125
	1    0    0    1   
$EndComp
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5AB9FC67
P 7800 2800
F 0 "#PWR?" H 7800 2760 30  0001 C CNN
F 1 "+3.3V" H 7809 2938 30  0000 C CNN
F 2 "" H 7800 2800 60  0000 C CNN
F 3 "" H 7800 2800 60  0000 C CNN
	1    7800 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	7800 2800 7800 2825
Wire Wire Line
	7800 3125 7800 3250
Connection ~ 7800 3250
Wire Wire Line
	7800 3250 7250 3250
Text GLabel 7250 3100 0    50   Output ~ 0
MOTOR_2_CURRENT
Text GLabel 7250 3250 0    50   Output ~ 0
~MOTOR_2_OC
Wire Wire Line
	7350 3150 7350 3100
Wire Wire Line
	7350 3100 7250 3100
$Comp
L appli_power:GND #PWR?
U 1 1 5AB9FC76
P 9850 3850
F 0 "#PWR?" H 9850 3600 50  0001 C CNN
F 1 "GND" H 9855 3677 50  0000 C CNN
F 2 "" H 9850 3850 60  0000 C CNN
F 3 "" H 9850 3850 60  0000 C CNN
	1    9850 3850
	1    0    0    -1  
$EndComp
$Comp
L appli_connector_molex:MOLEX_MINIFIT_JR_2x2_WAY P?
U 1 1 5AB9FC83
P 10200 3200
F 0 "P?" H 10278 3230 50  0000 L CNN
F 1 "MOLEX_MINIFIT_JR_2x2_WAY" H 10278 3150 35  0000 L CNN
F 2 "Applidyne_Connector_Molex:Molex_Mini-Fit_Jr_5566-04A_2x02_P4.20mm_Vertical" V 10375 3200 20  0001 C CNN
F 3 "https://www.molex.com/pdm_docs/sd/1724470002_sd.pdf" V 10400 3200 20  0001 C CNN
F 4 "MOLEX" V 10450 3200 20  0001 C CNN "manf"
F 5 "172447-0204" V 10475 3200 20  0001 C CNN "manf#"
F 6 "DigiKey" V 10500 3200 20  0001 C CNN "Supplier"
F 7 "WM14942-ND" V 10525 3200 20  0001 C CNN "Supplier Part No"
F 8 "https://www.digikey.com.au/product-detail/en/1724470204/WM14942-ND/5116909?utm_campaign=buynow&WT.z_cid=ref_octopart_dkc_buynow&utm_medium=aggregator&curr=usd&site=us&utm_source=octopart" V 10550 3200 20  0001 C CNN "Supplier URL"
F 9 "1.66" V 10575 3200 20  0001 C CNN "Supplier Price"
F 10 "1" V 10600 3200 20  0001 C CNN "Supplier Price Break"
	1    10200 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	9850 3350 10000 3350
Wire Wire Line
	9850 3350 9850 3850
$Comp
L appli_capacitor:100NF_X7R_1005M C?
U 1 1 5ABA0C87
P 8050 5200
F 0 "C?" H 7973 5065 50  0000 R CNN
F 1 "100NF_X7R_1005M" H 7973 5145 35  0000 R CNN
F 2 "Applidyne_Capacitor:CAPC1005X60N" V 8165 5100 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1936565.pdf" V 8190 5100 20  0001 C CNN
F 4 "TDK" V 8240 5100 20  0001 C CNN "manf"
F 5 "CGA2B3X7R1H104K050BB" V 8265 5100 20  0001 C CNN "manf#"
F 6 "Element14" V 8290 5100 20  0001 C CNN "Supplier"
F 7 "2210822" V 8315 5100 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/tdk/cga2b3x7r1h104k050bb/cap-mlcc-x7r-100nf-50v-0402/dp/2210822" V 8340 5100 20  0001 C CNN "Supplier URL"
F 9 "0.087" V 8365 5100 20  0001 C CNN "Supplier Price"
F 10 "100" V 8390 5100 20  0001 C CNN "Supplier Price Break"
	1    8050 5200
	1    0    0    1   
$EndComp
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5ABA0C8E
P 8050 4500
F 0 "#PWR?" H 8050 4460 30  0001 C CNN
F 1 "+3.3V" H 8059 4638 30  0000 C CNN
F 2 "" H 8050 4500 60  0000 C CNN
F 3 "" H 8050 4500 60  0000 C CNN
	1    8050 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	8050 5000 8050 4650
Wire Wire Line
	8050 4650 8200 4650
Connection ~ 8050 4650
Wire Wire Line
	8050 4650 8050 4500
Wire Wire Line
	8700 5350 8700 5400
Wire Wire Line
	8050 5200 8050 5450
$Comp
L appli_power:GND #PWR?
U 1 1 5ABA0C9A
P 8700 5450
F 0 "#PWR?" H 8700 5200 50  0001 C CNN
F 1 "GND" H 8705 5277 50  0000 C CNN
F 2 "" H 8700 5450 60  0000 C CNN
F 3 "" H 8700 5450 60  0000 C CNN
	1    8700 5450
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5ABA0CA0
P 8050 5450
F 0 "#PWR?" H 8050 5200 50  0001 C CNN
F 1 "GND" H 8055 5277 50  0000 C CNN
F 2 "" H 8050 5450 60  0000 C CNN
F 3 "" H 8050 5450 60  0000 C CNN
	1    8050 5450
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+POWER #PWR?
U 1 1 5ABA0CA6
P 9350 4500
F 0 "#PWR?" H 9350 4450 20  0001 C CNN
F 1 "+POWER" H 9353 4638 30  0000 C CNN
F 2 "" H 9350 4500 60  0000 C CNN
F 3 "" H 9350 4500 60  0000 C CNN
	1    9350 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	9350 4500 9350 4650
Wire Wire Line
	9350 4650 9200 4650
$Comp
L appli_special:ACS711KEXLT_15AB U?
U 1 1 5ABA0CB5
P 8700 4850
F 0 "U?" H 8700 5315 50  0000 C CNN
F 1 "ACS711KEXLT_15AB" H 8700 5224 50  0000 C CNN
F 2 "Applidyne_QFN:ACS-EX-12" H 8700 4050 50  0001 C CNN
F 3 "http://www.allegromicro.com/~/media/Files/Datasheets/ACS711-Datasheet.ashx" H 8700 3750 50  0001 C CNN
F 4 "Allegro MicroSystems" H 8700 3950 50  0001 C CNN "manf"
F 5 "ACS711KEXLT-15AB-T" H 8700 3850 50  0001 C CNN "manf#"
F 6 "DigiKey" H 8700 3650 50  0001 C CNN "Supplier"
F 7 "620-1483-1-ND" H 8700 3550 50  0001 C CNN "Suppler Part No"
F 8 "https://www.digikey.com.au/product-detail/en/allegro-microsystems-llc/ACS711KEXLT-15AB-T/620-1483-1-ND/3868192" H 8700 3450 50  0001 C CNN "Supplier URL"
F 9 "1.90" H 8700 3350 50  0001 C CNN "Supplier Price"
F 10 "1" H 8700 3250 50  0001 C CNN "Supplier Price Break"
	1    8700 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	8500 5350 8500 5400
Wire Wire Line
	8500 5400 8600 5400
Connection ~ 8700 5400
Wire Wire Line
	8700 5400 8700 5450
Wire Wire Line
	8900 5350 8900 5400
Wire Wire Line
	8900 5400 8800 5400
Wire Wire Line
	8800 5350 8800 5400
Connection ~ 8800 5400
Wire Wire Line
	8800 5400 8700 5400
Wire Wire Line
	8600 5350 8600 5400
Connection ~ 8600 5400
Wire Wire Line
	8600 5400 8700 5400
Wire Wire Line
	8200 4750 7350 4750
Wire Wire Line
	8200 4850 7800 4850
$Comp
L appli_resistor:10K_1005M R?
U 1 1 5ABA0CD3
P 7800 4725
F 0 "R?" H 7727 4540 50  0000 R CNN
F 1 "10K_1005M" H 7727 4620 35  0000 R CNN
F 2 "Applidyne_Resistor:RESC1005X40N" V 7910 4575 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2111203.pdf" V 7935 4575 20  0001 C CNN
F 4 "VISHAY" V 7985 4575 20  0001 C CNN "manf"
F 5 "CRCW040210K0FKED" V 8010 4575 20  0001 C CNN "manf#"
F 6 "Element14" V 8035 4575 20  0001 C CNN "Supplier"
F 7 "1469669" V 8060 4575 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/vishay/crcw040210k0fked/res-thick-film-10k-1-0-063w-0402/dp/1469669" V 8085 4575 20  0001 C CNN "Supplier URL"
F 9 "0.009" V 8110 4575 20  0001 C CNN "Supplier Price"
F 10 "10" V 8135 4575 20  0001 C CNN "Supplier Price Break"
	1    7800 4725
	1    0    0    1   
$EndComp
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5ABA0CDA
P 7800 4400
F 0 "#PWR?" H 7800 4360 30  0001 C CNN
F 1 "+3.3V" H 7809 4538 30  0000 C CNN
F 2 "" H 7800 4400 60  0000 C CNN
F 3 "" H 7800 4400 60  0000 C CNN
	1    7800 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	7800 4400 7800 4425
Wire Wire Line
	7800 4725 7800 4850
Connection ~ 7800 4850
Wire Wire Line
	7800 4850 7250 4850
Text GLabel 7250 4700 0    50   Output ~ 0
MOTOR_3_CURRENT
Text GLabel 7250 4850 0    50   Output ~ 0
~MOTOR_3_OC
Wire Wire Line
	7350 4750 7350 4700
Wire Wire Line
	7350 4700 7250 4700
$Comp
L appli_power:GND #PWR?
U 1 1 5ABA0CE9
P 9850 5450
F 0 "#PWR?" H 9850 5200 50  0001 C CNN
F 1 "GND" H 9855 5277 50  0000 C CNN
F 2 "" H 9850 5450 60  0000 C CNN
F 3 "" H 9850 5450 60  0000 C CNN
	1    9850 5450
	1    0    0    -1  
$EndComp
$Comp
L appli_connector_molex:MOLEX_MINIFIT_JR_2x2_WAY P?
U 1 1 5ABA0CF6
P 10200 4800
F 0 "P?" H 10278 4830 50  0000 L CNN
F 1 "MOLEX_MINIFIT_JR_2x2_WAY" H 10278 4750 35  0000 L CNN
F 2 "Applidyne_Connector_Molex:Molex_Mini-Fit_Jr_5566-04A_2x02_P4.20mm_Vertical" V 10375 4800 20  0001 C CNN
F 3 "https://www.molex.com/pdm_docs/sd/1724470002_sd.pdf" V 10400 4800 20  0001 C CNN
F 4 "MOLEX" V 10450 4800 20  0001 C CNN "manf"
F 5 "172447-0204" V 10475 4800 20  0001 C CNN "manf#"
F 6 "DigiKey" V 10500 4800 20  0001 C CNN "Supplier"
F 7 "WM14942-ND" V 10525 4800 20  0001 C CNN "Supplier Part No"
F 8 "https://www.digikey.com.au/product-detail/en/1724470204/WM14942-ND/5116909?utm_campaign=buynow&WT.z_cid=ref_octopart_dkc_buynow&utm_medium=aggregator&curr=usd&site=us&utm_source=octopart" V 10550 4800 20  0001 C CNN "Supplier URL"
F 9 "1.66" V 10575 4800 20  0001 C CNN "Supplier Price"
F 10 "1" V 10600 4800 20  0001 C CNN "Supplier Price Break"
	1    10200 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	9850 4950 10000 4950
Wire Wire Line
	9850 4950 9850 5450
Text Label 4050 1400 0    50   ~ 0
HLFB-
Text Label 4775 2500 0    50   ~ 0
INPUTB-
Text Label 4750 2600 0    50   ~ 0
INPUTA-
Text Label 4750 2700 0    50   ~ 0
ENABLE-
$Comp
L appli_power:+12V #PWR?
U 1 1 5ABA4403
P 4575 1850
F 0 "#PWR?" H 4575 1700 50  0001 C CNN
F 1 "+12V" H 4590 2023 50  0000 C CNN
F 2 "" H 4575 1850 60  0000 C CNN
F 3 "" H 4575 1850 60  0000 C CNN
	1    4575 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5125 2100 4575 2100
Wire Wire Line
	5125 2200 4575 2200
Wire Wire Line
	4575 2200 4575 2100
Wire Wire Line
	5125 2300 4575 2300
Wire Wire Line
	4575 2300 4575 2200
Connection ~ 4575 2200
Wire Wire Line
	4575 2100 4575 2000
Connection ~ 4575 2100
Wire Wire Line
	5125 2400 4475 2400
Wire Wire Line
	4475 2400 4475 1400
Wire Wire Line
	4475 1400 3975 1400
$Comp
L Transistor_Array:ULN2803A U?
U 1 1 5AC9AE17
P 2975 3350
F 0 "U?" H 2975 3917 50  0000 C CNN
F 1 "ULN2803A" H 2975 3826 50  0000 C CNN
F 2 "" H 3025 2700 50  0001 L CNN
F 3 "http://www.ti.com/lit/ds/symlink/uln2803a.pdf" H 3075 3150 50  0001 C CNN
	1    2975 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	3975 2700 5125 2700
Wire Wire Line
	3875 2600 5125 2600
Wire Wire Line
	3775 2500 5125 2500
$Comp
L appli_connector_molex:MOLEX_MINIFIT_JR_2x4_WAY P?
U 1 1 5ACB982C
P 5325 3300
F 0 "P?" H 5403 3330 50  0000 L CNN
F 1 "MOLEX_MINIFIT_JR_2x4_WAY" H 5403 3250 35  0000 L CNN
F 2 "Applidyne_Connector_Molex:Molex_MiniFit-JR-5569-08A2_2x04x4.20mm_Angled" V 5500 3300 20  0001 C CNN
F 3 "https://www.molex.com/pdm_docs/sd/1724470002_sd.pdf" V 5525 3300 20  0001 C CNN
F 4 "MOLEX" V 5575 3300 20  0001 C CNN "manf"
F 5 "172447-0208" V 5600 3300 20  0001 C CNN "manf#"
F 6 "Mouser" V 5625 3300 20  0001 C CNN "Supplier"
F 7 "538-172447-0208" V 5650 3300 20  0001 C CNN "Supplier Part No"
F 8 "https://au.mouser.com/ProductDetail/Molex/172447-0208?qs=sGAEpiMZZMvc81WFyF5Edkd3jpEQlJBQt6f989MIPuM%3d" V 5675 3300 20  0001 C CNN "Supplier URL"
F 9 "1.78" V 5700 3300 20  0001 C CNN "Supplier Price"
F 10 "1" V 5725 3300 20  0001 C CNN "Supplier Price Break"
	1    5325 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5125 3050 4575 3050
Wire Wire Line
	5125 3150 4575 3150
Wire Wire Line
	4575 3150 4575 3050
Wire Wire Line
	5125 3250 4575 3250
Wire Wire Line
	4575 3250 4575 3150
Connection ~ 4575 3150
Wire Wire Line
	4575 3050 4575 2950
Connection ~ 4575 3050
Connection ~ 4575 2300
Wire Wire Line
	5125 2950 4575 2950
Wire Wire Line
	5125 3350 4275 3350
Wire Wire Line
	5125 3450 3375 3450
Wire Wire Line
	3375 3550 5125 3550
Wire Wire Line
	5125 3650 3375 3650
Wire Wire Line
	3975 2700 3975 3350
Wire Wire Line
	3975 3350 3375 3350
Wire Wire Line
	3375 3250 3875 3250
Wire Wire Line
	3875 3250 3875 2600
Wire Wire Line
	3375 3150 3775 3150
Wire Wire Line
	3775 3150 3775 2500
Wire Wire Line
	3375 3050 3575 3050
Wire Wire Line
	3575 3050 3575 2875
$Comp
L appli_power:+12V #PWR?
U 1 1 5AD08F43
P 3575 2875
F 0 "#PWR?" H 3575 2725 50  0001 C CNN
F 1 "+12V" H 3590 3048 50  0000 C CNN
F 2 "" H 3575 2875 60  0000 C CNN
F 3 "" H 3575 2875 60  0000 C CNN
	1    3575 2875
	1    0    0    -1  
$EndComp
Wire Wire Line
	2575 3150 2225 3150
Wire Wire Line
	2575 3250 2225 3250
Wire Wire Line
	2575 3350 2225 3350
Wire Wire Line
	2575 3450 2225 3450
Wire Wire Line
	2575 3550 2225 3550
Wire Wire Line
	2575 3650 2225 3650
Wire Wire Line
	2975 4050 2975 4100
$Comp
L appli_power:GND #PWR?
U 1 1 5AD43F33
P 2975 4100
F 0 "#PWR?" H 2975 3850 50  0001 C CNN
F 1 "GND" H 2980 3927 50  0000 C CNN
F 2 "" H 2975 4100 60  0000 C CNN
F 3 "" H 2975 4100 60  0000 C CNN
	1    2975 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	4275 2200 3975 2200
Wire Wire Line
	4275 2200 4275 3350
Text Label 4025 2200 0    50   ~ 0
HLFB-
Text Label 4775 3450 0    50   ~ 0
INPUTB-
Text Label 4775 3550 0    50   ~ 0
INPUTA-
Text Label 4775 3650 0    50   ~ 0
ENABLE-
Text Notes 5425 2750 0    63   ~ 0
HLFB+\nINPUTB+\nINPUTA+\nENABLE+\nHLFB-\nINPUTB-\nINPUTA-\nENABLE-\n
Text GLabel 2225 3250 0    50   Input ~ 0
SERVO1_A
Text GLabel 2225 3150 0    50   Input ~ 0
SERVO1_B
Text GLabel 2225 3350 0    50   Input ~ 0
SERVO1_EN
Text GLabel 2225 3550 0    50   Input ~ 0
SERVO2_A
Text GLabel 2225 3450 0    50   Input ~ 0
SERVO2_B
Text GLabel 2225 3650 0    50   Input ~ 0
SERVO2_EN
Text GLabel 1350 1500 0    50   Output ~ 0
SERVO1_HLFB
Text GLabel 1300 2425 0    50   Output ~ 0
SERVO2_HLFB
$Comp
L Comparator:LM393 U?
U 1 1 5AD964FB
P 2150 1500
F 0 "U?" H 2325 1875 50  0000 C CNN
F 1 "LM393" H 2325 1800 50  0000 C CNN
F 2 "" H 2150 1500 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm393-n.pdf" H 2150 1500 50  0001 C CNN
	1    2150 1500
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1850 1500 1700 1500
$Comp
L appli_power:+3.3V #PWR?
U 1 1 5ADD064C
P 1700 1075
F 0 "#PWR?" H 1700 1035 30  0001 C CNN
F 1 "+3.3V" H 1709 1213 30  0000 C CNN
F 2 "" H 1700 1075 60  0000 C CNN
F 3 "" H 1700 1075 60  0000 C CNN
	1    1700 1075
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+12V #PWR?
U 1 1 5AE4BE30
P 2250 1100
F 0 "#PWR?" H 2250 950 50  0001 C CNN
F 1 "+12V" H 2265 1273 50  0000 C CNN
F 2 "" H 2250 1100 60  0000 C CNN
F 3 "" H 2250 1100 60  0000 C CNN
	1    2250 1100
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5AE4C157
P 1700 1850
F 0 "#PWR?" H 1700 1600 50  0001 C CNN
F 1 "GND" H 1705 1677 50  0000 C CNN
F 2 "" H 1700 1850 60  0000 C CNN
F 3 "" H 1700 1850 60  0000 C CNN
	1    1700 1850
	1    0    0    -1  
$EndComp
$Comp
L appli_power:GND #PWR?
U 1 1 5AE4C2E8
P 2250 1850
F 0 "#PWR?" H 2250 1600 50  0001 C CNN
F 1 "GND" H 2255 1677 50  0000 C CNN
F 2 "" H 2250 1850 60  0000 C CNN
F 3 "" H 2250 1850 60  0000 C CNN
	1    2250 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 1850 2250 1800
Wire Wire Line
	1700 1850 1700 1800
Wire Wire Line
	1700 1600 1700 1500
Connection ~ 1700 1500
Wire Wire Line
	1700 1500 1350 1500
Wire Wire Line
	1700 1500 1700 1425
Wire Wire Line
	1700 1125 1700 1075
Wire Wire Line
	750  1025 750  1050
Wire Wire Line
	2450 1400 3150 1400
Wire Wire Line
	5125 2000 4575 2000
Connection ~ 4575 2000
Wire Wire Line
	4575 2000 4575 1850
Connection ~ 4575 2950
Wire Wire Line
	4575 2950 4575 2300
Wire Wire Line
	9200 3250 10000 3250
Wire Wire Line
	9200 1675 10000 1675
Wire Wire Line
	9200 4850 10000 4850
$Comp
L appli_resistor:1K00_1608M R?
U 1 1 5ABC9A87
P 1700 1425
F 0 "R?" H 1628 1240 50  0000 R CNN
F 1 "1K00_1608M" H 1628 1320 35  0000 R CNN
F 2 "RESC1608*" V 1810 1275 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 1835 1275 20  0001 C CNN
F 4 "MULTICOMP" V 1885 1275 20  0001 C CNN "manf"
F 5 "MCHP03W8F1001T5E" V 1910 1275 20  0001 C CNN "manf#"
F 6 "Element14" V 1935 1275 20  0001 C CNN "Supplier"
F 7 "1576283" V 1960 1275 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mchp03w8f1001t5e/resistor-thick-film-1k-0-125w/dp/1576283" V 1985 1275 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 2010 1275 20  0001 C CNN "Supplier Price"
F 10 "50+" V 2035 1275 20  0001 C CNN "Supplier Price Break"
	1    1700 1425
	1    0    0    1   
$EndComp
$Comp
L appli_capacitor:1NF_X7R_1608M C?
U 1 1 5ABDA8BB
P 1700 1800
F 0 "C?" H 1622 1665 50  0000 R CNN
F 1 "1NF_X7R_1608M" H 1622 1745 35  0000 R CNN
F 2 "CAPC1608*" V 1815 1700 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1679506.pdf" V 1840 1700 20  0001 C CNN
F 4 "AVX" V 1890 1700 20  0001 C CNN "manf"
F 5 "06031C102KAT2A" V 1915 1700 20  0001 C CNN "manf#"
F 6 "Element14" V 1940 1700 20  0001 C CNN "Supplier"
F 7 "1833874" V 1965 1700 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/avx/06031c102kat2a/cap-mlcc-x7r-1nf-100v-0603/dp/1833874?ost=1833874" V 1990 1700 20  0001 C CNN "Supplier URL"
F 9 "0.004" V 2015 1700 20  0001 C CNN "Supplier Price"
F 10 "10" V 2040 1700 20  0001 C CNN "Supplier Price Break"
	1    1700 1800
	1    0    0    1   
$EndComp
$Comp
L appli_capacitor:100NF_X7R_1608M C?
U 1 1 5ABF1105
P 750 825
F 0 "C?" H 828 760 50  0000 L CNN
F 1 "100NF_X7R_1608M" H 828 680 35  0000 L CNN
F 2 "CAPC1608*" V 865 725 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1732728.pdf" V 890 725 20  0001 C CNN
F 4 "KEMET" V 940 725 20  0001 C CNN "manf"
F 5 "C0603C104K5RACAUTO" V 965 725 20  0001 C CNN "manf#"
F 6 "Element14" V 990 725 20  0001 C CNN "Supplier"
F 7 "2070398" V 1015 725 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/kemet/c0603c104k5racauto/cap-mlcc-x7r-100nf-50v-0603/dp/2070398" V 1040 725 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 1065 725 20  0001 C CNN "Supplier Price"
F 10 "1" V 1090 725 20  0001 C CNN "Supplier Price Break"
	1    750  825 
	1    0    0    -1  
$EndComp
$Comp
L appli_resistor:1K00_1608M R?
U 1 1 5ABF1233
P 3150 1800
F 0 "R?" H 3077 1615 50  0000 R CNN
F 1 "1K00_1608M" H 3077 1695 35  0000 R CNN
F 2 "RESC1608*" V 3260 1650 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1723233.pdf" V 3285 1650 20  0001 C CNN
F 4 "MULTICOMP" V 3335 1650 20  0001 C CNN "manf"
F 5 "MCHP03W8F1001T5E" V 3360 1650 20  0001 C CNN "manf#"
F 6 "Element14" V 3385 1650 20  0001 C CNN "Supplier"
F 7 "1576283" V 3410 1650 20  0001 C CNN "Supplier Part No"
F 8 "http://au.element14.com/multicomp/mchp03w8f1001t5e/resistor-thick-film-1k-0-125w/dp/1576283" V 3435 1650 20  0001 C CNN "Supplier URL"
F 9 "0.011" V 3460 1650 20  0001 C CNN "Supplier Price"
F 10 "50+" V 3485 1650 20  0001 C CNN "Supplier Price Break"
	1    3150 1800
	-1   0    0    1   
$EndComp
Wire Wire Line
	2250 1100 2250 1200
$Comp
L appli_power:GND #PWR?
U 1 1 5AC0C492
P 750 1050
F 0 "#PWR?" H 750 800 50  0001 C CNN
F 1 "GND" H 755 877 50  0000 C CNN
F 2 "" H 750 1050 60  0000 C CNN
F 3 "" H 750 1050 60  0000 C CNN
	1    750  1050
	1    0    0    -1  
$EndComp
$Comp
L appli_power:+12V #PWR?
U 1 1 5AC11C0D
P 750 775
F 0 "#PWR?" H 750 625 50  0001 C CNN
F 1 "+12V" H 765 948 50  0000 C CNN
F 2 "" H 750 775 60  0000 C CNN
F 3 "" H 750 775 60  0000 C CNN
	1    750  775 
	1    0    0    -1  
$EndComp
Wire Wire Line
	750  775  750  825 
$EndSCHEMATC
