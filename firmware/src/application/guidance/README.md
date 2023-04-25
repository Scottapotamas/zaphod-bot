# Guidance Module

Responsible for calculating the position of the end effector over time.

Operates in XYZ cartesian space using signed 32-bit integer as the positioning unit in microns.

This allows micron resolution positioning to +-2147 meters for reference, for comparison, 16-bit positioning would constrain the axis to 65 mm travel
