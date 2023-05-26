# Overwatch Task

The 'highest level' task for application logic, responsible for managing connections between subsystems, high level robot behaviour, etc.

- Doesn't init/start tasks, handle IPC or hardware, 
- Does generate high-level commands, modify routing of commands between tasks,
- Whilst not explicitly designed, I'm attempting to achieve many of the same goals as those of the Mediator pattern,
- This task shouldn't need to interact/communicate with anything below the 'application' level tasks,
- Fully event driven,

# Main States

There are 4 main states that describe the delta-robot:

- DISARMED
- ARMING
- ARMED
- DISARMING

but there's only two states we can request, two transition states, and an edge-case.
What happens when we're in either of the main states?

- When DISARMED, it only needs to wait for the arming signal.
- When ARMED, wait for the disarm signal and monitor the system's health

The edge case is handling emergency stop events where we skip a graceful disarm process.

# Operational States

Several user-selectable modes of operation are available for selection:

- MANUAL - one-off user jog commands and manual control of effector LED colour,
- TRACK - end effector tries to chase a target position,
- DEMO - generates long-running movement sequences that 'look cool',
- EVENT - user provided movement sequences

To keep behaviour of any given mode clean, the entry to these mode states are responsible for configuring any subsystems and connecting callback chains.
When exiting a mode, the cleanup should leave the system in a 'null state', after which the new state's entry process will configure as needed.

Just changing modes with the minimal setup isn't entirely desirable though, if we're armed then we really want to gracefully change state. 
Start by storing the 'requested mode', run through transition state CHANGE_MODE:

- Setup for a simple event driven mode,
- Home the end effector,
- Once homed, transition into the mode that was asked for originally.

A user requesting to re-home the mechanism is just a special case of the change mode behaviour.

## State telemetry 

We 'hide' the underlying behaviour of the CHANGE_MODE transition state from the UI/telemetry