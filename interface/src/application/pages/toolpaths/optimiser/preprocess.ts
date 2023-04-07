import { CylinderGeometry, Raycaster, Vector3 } from 'three'
import { SimpleColorMaterial } from './materials/Color'
import { Line, Movement } from './movements'
import { isLine, isMovementGroup } from './movement_utilities'
import { MOVEMENT_TYPE } from './movement_types'
import { Settings } from './settings'

const cylinderCenter = new Vector3(0, 0, 100)
const cylinderHeight = 170 // mm
const cylinderRadius = 190 // mm

const cylinderGeometry = new CylinderGeometry(
  cylinderRadius,
  cylinderRadius,
  cylinderHeight,
  32,
  1,
  false,
).translate(cylinderCenter.x, cylinderCenter.y, cylinderCenter.z)

const markerMaterial = new SimpleColorMaterial([1, 0, 0, 0.3])

/**
 * Takes a list of movements and pre-processes them to filter out invalid movements, etc
 */
export function preprocess(initial: Movement[], settings: Settings) {
  const movements: Movement[] = []

  for (const movement of initial) {
    if (isMovementGroup(movement) && movement.getMovements().length === 0) {
      // dump empty movement groups
      continue
    }
    const startWithin = pointWithinCylinder(
      movement.getStart(),
      cylinderCenter,
      cylinderHeight,
      cylinderRadius,
    )

    const endWithin = pointWithinCylinder(
      movement.getEnd(),
      cylinderCenter,
      cylinderHeight,
      cylinderRadius,
    )

    // If the movement is totally within the cylinder:
    if (startWithin && endWithin) {
      // Add it to the list
      movements.push(movement)
    } else if (!startWithin && !endWithin) {
      // If neither the start nor the end are within the cylinder, just dump the movement entirely
      continue
    } else {
      // The movement is at least partially within the cylinder.

      // console.log(`movement`, movement.type, `is partially within`)

      if (isLine(movement)) {
        // TODO: Make this work

        continue
        /*
        // Remap the line

        const intersection = getLineCylinderIntersection(
          startWithin ? movement.getEnd() : movement.getStart(),
          startWithin ? movement.getStart() : movement.getEnd(),
        )

        if (!intersection) {
          console.error(
            'Did maths wrong, no intersection of line',
            movement.getStart(),
            movement.getEnd(),
            `and cylinder`,
          )
          continue
        }

        let from = movement.getStart()
        let to = movement.getEnd()

        if (startWithin) {
          to = intersection
        } else {
          from = intersection
        }

        console.log(
          `remapped from `,
          movement.getStart(),
          `to`,
          from,
          `and`,
          movement.getEnd(),
          `to`,
          to,
        )

        // Replace the movement with a new line
        movements.push(
          new Line(
            from,
            to,
            markerMaterial,
            movement.objectID,
            movement.overrideKeys,
          ),
        )

        */
      } else if (isMovementGroup(movement)) {
        // recursively process any movement groups

        movements.push(...preprocess(movement.getMovements(), settings))
      } else {
        // Dump anything that's not a line
        continue
      }
    }
  }

  return movements
}

/**
 * @param point The Vector3 defining the point we're checking
 * @param center The center of the cylinder
 * @param height The height of the cylinder
 * @param radius The radius of the cylinder
 * @returns if the point is within the cylinder
 */
function pointWithinCylinder(
  point: Vector3,
  center: Vector3,
  height: number,
  radius: number,
): boolean {
  // calculate the distance between the point and the center of the cylinder on the xy plane
  const xyDistance = Math.sqrt(
    Math.pow(center.x - point.x, 2) + Math.pow(center.y - point.y, 2),
  )

  // check if the xyDistance is less than or equal to the radius
  // and if the point is between the top and bottom of the cylinder
  if (
    xyDistance <= radius &&
    point.z <= center.z + height / 2 &&
    point.z >= center.z - height / 2
  ) {
    return true
  } else {
    return false
  }
}

function getLineCylinderIntersection(
  lineStart: Vector3,
  lineEnd: Vector3,
): Vector3 | null {
  // Vector pointing from lineStart to lineEnd

  const lineDirection = lineEnd.clone().sub(lineStart)

  // Vector pointing from lineStart to cylinder center
  const lineToCylinder = cylinderCenter.clone().sub(lineStart)

  // Projection of lineToCylinder onto lineDirection
  const projection = lineToCylinder.dot(lineDirection)

  // Closest point on line to cylinder
  const closestPoint = lineStart
    .clone()
    .add(lineDirection.clone().multiplyScalar(projection))

  // Vector from closest point on line to cylinder center
  const closestToCenter = cylinderCenter.clone().sub(closestPoint)

  // Distance between closest point on line and cylinder center
  const distance = closestToCenter.length()

  // Check if intersection is within cylinder height and radius
  if (
    Math.abs(closestPoint.z - cylinderCenter.z) > cylinderHeight / 2 ||
    distance > cylinderRadius
  ) {
    return null
  }

  const intersectionVector = closestToCenter
    .clone()
    .normalize()
    .multiplyScalar(cylinderRadius)

  return intersectionVector.clone().add(closestPoint)
}
