# Guidance Module

Responsible for calculating the position of the end effector over time.

Operates in XYZ cartesian space using signed 32-bit integer as the positioning unit in microns.

This allows micron resolution positioning to +-2147 meters for reference, for comparison, 16-bit positioning would constrain the axis to 65 mm travel

# Input Requests

Movement requests can originate from several different sources:

- supervisor task(s) may request the effector to move somewhere/home,
- user-interface requested moves when the user manually jogs, or from a planned sequence of moves, 
- generated moves as part of the demonstration mode(s).

As a result, we need to handle an inbound queue of motion requests that is deeper than what we can currently execute.
There's also the possibility that inbound requests arrive out-of-order.

Therefore, the move request pipeline:

- Provides an inbound request queue,
- Pending moves are then copied into a reasonably large pool,
- In the background, or as needed, search the pool for the next move,
- Pass that movement request to the path interpolator module for execution 'very soon'.

This behaviour is implemented by `request_handler`.

# Executing Motion Requests

The path interpolator maintains a (small) inbound move queue and waits for movements to be added, 
then it runs through a sequence of processes to execute the move.

## Pre-move processing

Before executing a move, there are a series of transforms and checks applied:

- If the move is a relative position move, transform it to be an absolute move using the current effector position,
- Check if the move violates any effector velocity limits
- Check if the move will violate the boundary of the movement volume

## Following requested curves

For a given movement request, we need to calculate the desired effector position over the move's duration.

- Wait until the move is meant to start,
- Calculate our 'current completion percentage' from the start timestamp and the current "elapsed" time,
- Find the desired position that matches the percentage completion of the move (interpolate position by time percentage)
- Send the desired position out for execution,
- Cleanup and wait for a new movement.

This functionality is provided by `path_interpolator`.

## Output

Output positions in cartesian space are sent to be handled by the `kinematics` module.
