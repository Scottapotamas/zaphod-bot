# General Build Notes

Steps taken during build, issues and musings for potential future versions.

## Wiring
 
ModDIY Mini-Fit JR wires are probably 100mm too long for optimium wiring inside the enclosure.
The wires also needed to have the heatshrink 'thinned' around the connector where they enter the plug, due to heatshrink stopping full insertion of the pin.

Mains wiring could have been sleeved. Probably could have made it slightly shorter, and used more flexible wiring.

LEMO connectors were a pain to wire by hand. Spend little bit more to get the pre-loomed versions.


## Mechanics
 
Thicker carbon side plates might be a good upgrade to reduce flex in the PSU connector, and improve the weight of the lower section.

Tolerances can be tightened on the carbon plates against the main frame.

The CNC house should have blasted or finished the CNC aluminium before anodizing, the natural grain shows through too much.

The Bicep-Motor shaft connection needs tighter tolerances, the slip on the shaft requires very forceful tightening.

Wiring to the motors could have been improved if the holes were large enough to accept the Mini-fit connector. Alternatively, put a panel mount connector on the top plate, and use a 'external loom' and 'internal loom'.

I probably could have packaged the stack differently, going for a tall shape with 90deg gearboxes on the motors, which would thin out the stack, add some height (and possibly make the PCB a more sane geometry), and allow for longer biceps in the same bench-space. While the gearboxes cost more, that could be offset with smaller clearpath servos for lower power, and would result in higher precision.

## PCB

The PCB power connector and motor connectors should be spaced out slightly more, as the Sabre connectors from the IPC-5 PSU conflict with the motor looms, and the outermost PSU connector actually fouls with the PCB input connector if the locking tabs are installed on both connectors.

For this version, I'm abusing the mating friction/force for the actual spades, and have removed the locking tabs on the Sabre to make things fit.

More mounting holes for better rigidity would have been great, but would be difficult to fit with current layout. Thicker PCB would also help flex.


## Other Thoughts

Fan is quite quiet and could possibly be upgraded to something with a higher flow rate. Positioning could improve to reduce wiring loom clutter internally.