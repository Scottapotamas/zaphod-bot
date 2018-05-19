# Architecture Overview

There are a few layers of abstraction which make up the overall structure of the firmware.

tasks 		HSM structured higher level logical tasks. Subscribe and emit events. Only allowed to interact with drivers. 
drivers		Simpler self-contained modules, some contain a simple-state-machine implementation. Typically manage hardware, convert sensors to quantities etc.
hal 		Hardware abstraction to interact with specific peripherals or microcontroller functions.
utility		Helper code which forms the backbone of the statemachines, or some other generic behaviour (fifo, queues, state machine macros).

The hal layer provides convenience wrappers for other lower level functionality, be it direct register calls or interfacing with the STM32 HAL layers.

The intent of 'my' hal layer sitting above the STM32 HAL is to further decouple the code from the STM ecosystem, which improves testability and reuse of drivers.

## Tasks
 
Based on a hierarchical state machine structure (HSM), which provides event-driven state machines called 'tasks'.  
Found in /src/app_state_machines

Event timers are evaluated on a tick timer (along with ADC or other timing specific modules).
The main loop sleeps wherever possible, waking on interrupt. At this point, the background processors are visited, then a task from the queue is run.
For a given 'task', an arbitary number of states can be defined (as functions), which contain a macro driven pattern for handling entries and exits from the state, and handling for external events.


## Drivers
 
Simpler blocks designed to operate statelessly or with a simple state machine and processing loop. These aren't handled by the task systems, and aren't intended for complex logic.
Found in /src/drivers

Drivers should expose public facing functions for init, getters and setters, and processing 'loops' if required.
A driver will typically perform a role where interaction with the HAL layer is required, such as sampling a ADC, i2c sensor, or drive a PWM output, and typically provides human-friendly interactions with the underlying hardware. ADC or sensor readings should be converted into a SI unit for example, and averaging/bounds/error detection should be done at this layer.


## hal

Interacts with the hardware on a more generic level. This may be direct hardware register manipulation, or calls to a vendor HAL codebase.  
Found in /src/hal   

hal code should strive to abstract the operation of the bare metal to simplistic actions such as start/stop/action, and should attempt to consolidate settings for peripherals.
This is designed to allow the driver layers to be unit tested independantly of hardware, as the hal layer can be mocked. Also promotes portability as the hal layer can be changed subtly for new vendor IC's as needed.


## utilities

Provides standard functionality used across driver or task layers, includes functionality like queuing, state handling or buffer style functions.
Found in /src/utilities  

Many modules consist of defines or macros to simplify the higher layer.  
Where possible, these are as generic as possible.


# Delta Notes

General overview/thoughts regarding firmware development for the controller. These might be as simple as listing the responsibility of certain modules.


## Fan Control

The fan driver presents a simpler loop run in the background at a lower-rate.  
Interacts with the hal PWM to set fan speeds, and monitor the fan hall sensor and temperatures.  

Handles starting/stopping the fan with a blip to higher speed to ensure safe starting.  
Includes stall detection and underspeed detection, attempts to restart.  
Monitors the temperature and tracks a fan curve for auto-speed.  

## Kinematics 

The kinematics driver provides transforms between the cartesian co-ordinate space, and angular roations for each motor.

Forward Kinematics (angles to position) and Inverse Kinematics (position to angles) are available.
Functions take a structure for either position or angles as input, with a pointer to a writable structrure (output) of the other type.

Bypassing this module would allow use of the overall motion planners etc for different mechanisms (SCARA, cartesian, etc)

## Path Interpolator

Driver operates in the cartesian domain only, and is designed to accept a 'motion request' and then execute the motion over some duration of time. 
As movement to a destination isn't as simple as driving motors to the final destination instantly (co-ordination is required for correct temporal profiles, kinematics considerations etc), we need to calculate a target position for the end effector at a given time, then calculating the intermediary movements to provide speed/duration control etc.

To ensure the effector follows the target profile over time, the motion is broken up into smaller chunks. Greater number of chunks provides greater adherance to the intended profile.
As the delta is intended for use against temporal problems, the movement engine was designed around the concept of 'time domain' movement execution, not feed rate based moves like many CNC machines.

As such, each movement has a corresponding 'duration', and the interpolator driver strives to complete the movement within this time window. By adjusting the rate of the interpolator's loop, the fidelity of the movements is then traded against movement speed. A large and small movement which share execution durations will recieve a similar number of chunks, and therefore positional adherance will vary.

Tool-positioning calculations depend on the style of motion requested, and several interpolation functions are included to assist with this:

### Transit movements

Transit movements are used where the 'where' matters more than the 'how', and will execute without any guarantee of the tool path between points.

For the delta, the end effector position along this transit move will be influenced by the non-linear relationship of the mechanics and motor angles.

### Line Movements

Linear interpolations are used to perform line following between 2 3D points.

### Spline Movements

Catmull-Rom Spline interpolations are used to follow a segment of the curve defined by 4 points.

https://en.wikipedia.org/wiki/Centripetal_Catmull-Rom_spline

The higher level task, or the movement generation tool can generate large chains of points to ensure all waypoints are hit.

### Arbitary Equation Moves

Not currently implemented.

The ability to accept an arbitary mathematical function is a wanted feature? 
Could be cool to provide arbitary spirographic functions for demos.

## Clearpath Driver

Handles the homing and control of a single servo.
Monitors current and feedback pins to approximate some failure detection/prevention.
