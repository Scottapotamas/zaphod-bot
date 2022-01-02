import { LightMove, LightMoveType } from "./hardware";
import { Movement } from "./movements";

export function rgbToHsl(
  red: number,
  green: number,
  blue: number
): [h: number, s: number, v: number] {
  const redPercent = red;
  const greenPercent = green;
  const bluePercent = blue;

  const max = Math.max(redPercent, greenPercent, bluePercent);
  const min = Math.min(redPercent, greenPercent, bluePercent);
  const lightness = (max + min) / 2;

  let hue = 0;
  let saturation = 0;

  if (red === green && red === blue) {
    // Achromatic - no color
  } else {
    const diff = max - min;
    saturation = lightness > 0.5 ? diff / (2 - max - min) : diff / (max + min);
    switch (max) {
      case redPercent: {
        hue =
          (greenPercent - bluePercent) / diff +
          (greenPercent < bluePercent ? 6 : 0);
        break;
      }
      case greenPercent: {
        hue = (bluePercent - redPercent) / diff + 2;
        break;
      }
      case bluePercent: {
        hue = (redPercent - greenPercent) / diff + 4;
        break;
      }
      default: {
        throw new Error("Color error?");
        break;
      }
    }

    hue /= 6;
    hue *= 360;
  }

  return [hue, saturation, lightness];
}

export interface ColorMaterialJSON {
  type: "color";
  color: [number, number, number, number];
}

export type MaterialJSON = ColorMaterialJSON;

export abstract class Material {
  abstract type: string;

  /**
   * Given a movement ID, generate the light moves for this material, (defer to materials).
   */
  abstract generateLightpath: (id: number, movement: Movement) => LightMove[];
}


export function isSimpleColorMaterial(material: Material): material is SimpleColorMaterial {
  return material.type === 'color'
}

/**
 * A group of movements that must be executed in order.
 */
export class SimpleColorMaterial extends Material {
  readonly type = "color";

  constructor(public color: [number, number, number, number]) {
    super();
  }

  public generateLightpath = (id: number, movement: Movement) => {
    const fade: LightMove = {
      id,
      duration: movement.getDuration(),
      type: LightMoveType.IMMEDIATE,
      points: [rgbToHsl(this.color[0], this.color[1], this.color[2])],
      num_points: 1,
    };

    return [fade];
  };
}

export const blankMaterial: ColorMaterialJSON = {
  type: "color",
  color: [24/255, 74/255, 144/255, 0],
};

export function importColorMaterial(json: ColorMaterialJSON) {
  const mat = new SimpleColorMaterial(json.color);

  return mat;
}

export function importMaterial(json: MaterialJSON) {
  switch (json.type) {
    case "color":
      return importColorMaterial(json);

    default:
      throw new Error(`Error importing material, unknown type ${json["type"]}`);
  }
}
