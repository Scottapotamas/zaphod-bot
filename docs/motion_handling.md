# Motion Handling

Summary of the motion planning setup.

## Work Area

The main co-ordinate system is represented as a cartesian space, measured in microns. This allows for use of int32 x, y, z numbers with sufficient resolution and spare room for rails etc.

The XY plane is defined as the plane parallel to the main hexagonal plates of the delta, with the 0-point of XY being co-linear with the central vertical axis of the bot (centre of plate, motor axis etc). The Z axis is defined as the vertical distance from the delta, where the '0' position is the closest reachable point to the delta's base electronics assembly (i.e. arms are about to hit endstops). Positive Z positions are away from the robot, moving towards the fully extended range of motion.

## Motion Processing Pipeline

Movements can be specified as one of several types, a transit, line, or one of several spline choices (catmull rom, quadratic bezier, cubic bezier). 
Points are passed to the pathing engine to generate the lines or splines, in micron-resolution x,y,z cartesian format.
A 'target duration' is specified for the movement, where the delta will attempt to complete the move by the elapsed duration.

While executing a movement, the pathing engine will calculate the time elapsed since the start, and calculate the percentage of completion required.
The end-effector's position is calculated using the appropriate line or spline interpolation function, with the percentage as the dependant variable.
The output co-ordinates are then fed into the kinematics driver, which performs the inverse kinematics required to translate the cartesian co-ordinates into motor angles.
On successful IK solve, the motor angles are sent to the clearpath handlers, which convert the target position into steps, and manage the actuation of the servos.

By repeating this process often, movement along a path can be achieved with some level of accuracy (assuming the servo motors can maintain control to meet the setpoint).
As the processing stages require a non-zero amount of compute time, the number of 'subdivisions' achieved by the pathing engine is dependant on the time allowed by the movement.
Therefore, large fast moves have lower resolution than either smaller fast moves, or large slow moves.





## Servo Angles

When the servo homes, it moves to an endstop which correlates to a bicep angle of -65° (where 0° is parallel to the plate). 
The motor then moves 150 steps from this point as a 'buffer', or to -56.5°.

