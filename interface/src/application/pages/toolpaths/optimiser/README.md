# Materials

Materials can't have any effect on the ordering of the movements.

## Blender Materials

Materials support arbitrary data transfer. Any `Object Data` custom properties
with the prefix `material_` will be transfered into the `material` entry without
the prefix. We use the `Object Data` custom properties because that usually
makes more sense with the object. GPencil and Lights are supported

Eg: to use the rainbow colour:

`material_type`: `rainbow`

Or a regular colour material

```
material_type: color
material_color: [1.0, 1.0, 1.0, 1.0]
```

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
