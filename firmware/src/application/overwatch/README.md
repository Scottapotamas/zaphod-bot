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

but there's actually two states we can request, two transition states, and an edge-case.
What happens when we're in either of the main states?

- When DISARMED, it only needs to wait for the arming signal.
- When ARMED, run a sub-state-machine which handles the main operating modes

The edge case is handling emergency stop events where we skip a graceful disarm process.

# Operational States

Several user-selectable modes of operation are available while armed:

- MANUAL - one-off user jog commands and manual control of effector LED colour,
- TRACK - end effector tries to chase a target position,
- DEMO - generates long-running movement sequences that 'look cool',
- EVENT - user provided movement sequences

In the same manner as the top level state machine, attempting to enter any of the operation sub-modes requires a transition process.
Start by storing the 'requested mode', run a transition state:

- Cleanup any queues/subscriptions/timers/etc,
- Home the end effector,
- Manage the new queue connections/subscriptions needed for the new mode,

Then transition into the mode that was asked for originally.