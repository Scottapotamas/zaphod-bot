# Delta Mechanism Design

Goal is for a high speed, high resolution, low load actuator in 3-dof space.

- Ideally the platform has minimal visual occlusion from the end affector over the widest range of angles possible.
- Ideally the end effector is capable of moving at the same/similar velocities for any direction.
- The expected load on the end effector is not much heavier than some small electronics (sensor, LEDs, etc).
- The mechanism should operate while pointing up, and while hanging in a 'downwards' facing orientation.

## Mechanism Options

- SCARA (single arm)
- SCARA (parallel)
- Delta (linear)
- Delta (angular)
- Cartesian XYZ style approaches
- Multi-dof arm approach

Key metrics for selection are personal attraction (motivation is important), low effector mass, high velocities and resonably high positioning resolution/repeatability.

Cost and size of actuators is an important factor, mechanical simplicity of arms is somewhat important.

A delta was considered one of the best options, beating out SCARA and various COREXY/XZ designs on speed, form factor, and personal attraction to the design.

## Delta Design Notes

### General Notes

- Need to work out base size to package motors, electronics, power supply and other ancillaries. Ideally an IEC cable enters the base 'box' and little to no other lines are in/out if possible.
- Should be portable by a single person if possible (if not, add handles).
- Leave room for a higher end processor (Odroid or Raspberry Pi etc) for future expansion.
- Mounting points on sides and base for mounting to external motion control systems, support structures or feet for table use.
- Ideally the moving mechanism is vibration damped from any extenal mounts to reduce transmission of HF movements.

### Base, Arm Sizing


### End Effector


### External Mounting


