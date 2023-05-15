# Guidance Module

Responsible for calculating the position of the end effector over time.

Operates in XYZ cartesian space using signed 32-bit integer as the positioning unit in microns.

This allows micron resolution positioning to +-2147 meters for reference, for comparison, 16-bit positioning would constrain the axis to 65 mm travel

# Principle of operation

End-effector guidance is achieved through two distinct approaches:

- Executing planned moves which describe a line/spline in 3D space along with a duration, or
- Passing in a target effector position and 'let the robot work out how to get there'

# Executing Movements

The movement handling pipeline implemented in `path_interpolator` can be summarised as:

- Provide an inbound request queue,
- Mutate/check the request as needed,
- Calculate the effector position matching the expected 'completion at this time',
- Pass the position off for kinematics handling.

## Request Sources

Movement requests can originate from several different sources:

- supervisor task(s) may request the effector to move somewhere/home,
- user-interface requested moves when the user manually jogs, or from a planned sequence of moves,
- generated moves as part of the demonstration mode(s).

As a result, we need to handle an inbound queue of motion requests that is deeper than what we can currently execute.
There's also the possibility that inbound requests arrive out-of-order.

For simpler/internal behaviours such as manual jog requests or moves generated internally, the 'planning' contract is pretty easy to uphold.
In situations where deep pools and ordering is required (i.e. event mode), the supervisor will proxy mvoement requests through a `request_handler` task which contains bulk storage and will emit events in order.

## Inbound Request Queue

The path interpolator maintains a (small) inbound move queue and waits for movements to be added, 
then unblocks to run through the execution sequences until another move is needed.

## Pre-move processing

Before executing a move, there are a series of transforms and checks applied:

- If the move is a relative position move, transform it to be an absolute move using the current effector position,
- Check if the move violates any effector velocity limits
- Check if the move will violate the boundary of the movement volume

## Following requested curves

For a given movement request, we need to calculate the desired effector position over the move's duration.

- Wait until the move is meant to start,
  - If the sync event hasn't occured, or is in the future, sleep the task until then.
  - Start a timer once the move has begun.
- Calculate our 'current completion percentage' from the start timestamp and the current "elapsed" time,
- Find the desired position that matches the percentage completion of the move (interpolate position by time percentage)
- Send the desired position out for execution,
- Cleanup and wait for a new movement.

This functionality is provided by `path_interpolator`.

## Output

Output positions in cartesian space are sent to be handled by the `kinematics` module.

Due to the 'modular' nature of the possible pathing implementations, this is done via callback function. 

# Tracking position targets

The `point_follower` implementation accepts an input position in Cartesian space, along with state about the previous position/velocity, and generates a simple linear effector ramp for X/Y/Z axis independently.

This results in the effector 'following' the target position smoothly, without requiring any forward knowledge of the movement behaviour.