# Kinematics Module

Responsible for taking a desired end-effector position and getting the servo's to execute it.

- Acts as the 'source of truth' for the end effector's current position in cartesian space
- Maintain end-effector movement statistics i.e. cartesian speed
- Handle inbound cartesian position requests
  - Check the request validity (speed violations, boundary guards),
  - Run inverse-kinematics calculations for the delta-robot mechanics
  - Emit desired servo angles