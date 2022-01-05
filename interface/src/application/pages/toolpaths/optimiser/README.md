# Materials

Materials can't have any effect on the ordering of the movements.

# TODO:

Refactor materials so we can modify them on the UI side without re-optimising
the frames.

- Material Overrides shouldn't be in the Settings structure

- Need a way to produce the final toolpath.

- A RampedColorMaterial that produces a gradient between two points, used for
  vertex colouring on the GPencil
- GPencil needs to use RampedColorMaterial
- A velocity colour material that produces a gradient that uses the line segment
  velocity. This means Materials need a way to grab the current settings object?
