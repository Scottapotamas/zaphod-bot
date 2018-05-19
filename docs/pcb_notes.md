# Main Interface PCB

Purpose: Accept inputs, provide output connectors, maintain wiring and provide high level control over system.

Side Goals: Look awesome.

# Requirements

## Input

- Accept 75V DC from IPC-3 PSU with 2-pin Molex SABRE connector.
- Minimalistic input protection and filtering as supply is trustworthy.
- Polyfuse, a few caps for stability?
- Input voltage test points

## Processing

- Accept input position data at high rate (USB, Ethernet, Wifi, SD/USB are all acceptable).
- Drive 3 stepper motors at high speed, i.e. maximum achievable square waves for ultimate positioning control
- Drive 1-n RGB LEDs for output
- External optocoupled IO for inputs/soft stop switch, and camera trigger events.
- Should be able to handle reasonably involved trig/floating point maths for kinematics calculations.
- External outputs for end affector upgrades in the future
- Inputs for homing switches
- Buzzer output for safety alerts?

## General nice-to-haves

- Onboard NTC for temperature monitoring
- Expansion NTC's to monitor IPC-3 power supply temperature, etc.
- PWM Fan output for cooling
- Consider input voltage, and current sensing on each servo output rail.
- SWD Connection with ARM CORTEX 5x2 pin female 1.25mm pitch.
- Ensure high reliability, robust IO and isolate where practical.
- Expansion PWM outputs for end affector upgrades/changes in the future (pumps/servos etc)
- Expansion UART and/or CANBUS for external interfacing.
- Terminal over UART is nicer than USB for debugging.
- Testpoints for scope/function generator
- TTL level lines could use 2.54mm 4x2 connectors for Saleae logic analyser connection (step+dir+enable+hlfb).
- Consider onboard RTC battery connection.
- Connection for ESP8266/32 for wireless expansion?

- Matte-black PCB with ENIG finish. Gold text etc.
- Use only Orange or Blue LEDS for indication
- Leds should be low brightness or include disable functionality.

## Outputs

- 3x Motor Connectors (power + control connectors)
- Status/state LEDS on important pins for visual debugging.
- External UART
- External USB
- External IO

- Bonus for 4th motor output if 4th axis expansion is considered