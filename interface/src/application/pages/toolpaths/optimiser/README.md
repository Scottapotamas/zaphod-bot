# Colors

All colors are unit colours internally, RGB 0-1 values.

# Materials

Materials can't have any effect on the ordering of the movements.

## Blender Materials

Materials support arbitrary data transfer. Any `Object Data` custom properties
with the prefix `material.` will be transfered into the `material` entry without
the prefix. We use the `Object Data` custom properties because that usually
makes more sense with the object. GPencil and Lights are supported

Eg: to use the rainbow colour:

`material.type`: `rainbow`

Or a regular colour material

```
material.type: color
material.color: [1.0, 1.0, 1.0, 1.0]
```

Python syntax dicts can be entered for complex data structures

```
{'type': 'color', 'color': [1.0, 0.087, 0.05]}
```

Dot notation string keys can be used to override specific members (for use with
drivers)

```
material.foreground = {'type': 'z_depth', 'color_from': [1.0, 1.0, 1.0], 'color_to': [0.0, 0.0, 0.0], 'depth_from': 950, 'depth_to': 1000}
material.foreground.color_from = [0,0,1]
```

{'type': 'z_gradient', 'z_from': 105, 'z_to': 180, 'color_from': [0.360784,
0.145098, 0.360784], 'color_to': [0.760784, 0.454902, 0.760784]}

## Empty special data export

Custom properties of empty objects with the prefix `frame.` are exported, minus
the prefix, into the frame metadata store.

`frame.exp_angle` for example becomes `exp_angle` which is used to control the
4th axis.

## Drivers

Any property can be 'driven' by any combination of math, python scripting, and
other properties.

Right click -> `Add Driver` to add a driver.

# TODO:

- DRY the threejs representation into a simple colour interpolator
- BlendMaterial that can operate over two materials, multiply / min / max
- Z Fade material that fades 0 - 1 based on distance to camera near and far
- Fuzz material that can vary lighting intensity
- Some way to multiply light intensity with speed

- Motion fuzziness?
