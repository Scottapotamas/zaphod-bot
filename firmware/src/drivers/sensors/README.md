# Sensors Module

Responsible for managing the sensor data being used on the system.

- Accepts raw sensor data such as ADC values, timer counts, etc
- Maintains a buffer of values, with ability to provide simple averages etc,
- Handles unit conversion to SI units (celsius, volts, amps, rpm etc)
- Decoupled from task usage via a centralised Observer pattern


# TODO

- Work out how to provide servo power events in watts when they require two events
- Cleanup notification type handling etc
- Track timestamp information
- Calculate min/max/'noise' stats alongside signal
- Able to calculate the sensor update rate