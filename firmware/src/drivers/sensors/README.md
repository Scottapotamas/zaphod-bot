# Sensors Module

Responsible for managing the sensor data being used on the system.

- Accepts raw sensor data such as ADC values, timer counts, etc
- Handles unit conversion
- Performs min/max/filtering of data
- Decoupled from task usage via a centralised Observer pattern