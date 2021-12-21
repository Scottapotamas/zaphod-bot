import {
  GPencil,
  GPencilJSON,
  GPencilToMovementsSettings,
  importGPencil,
} from "./gpencil";
import { Material, MaterialJSON } from "./material";
import {
  importParticles,
  Particles,
  ParticlesJSON,
  ParticlesToMovementsSettings,
} from "./particles";

export type MovementJSON = GPencilJSON | ParticlesJSON;
export type Renderable = GPencil | Particles;

export function importJson(json: MovementJSON): Renderable {
  switch (json.type) {
    case "gpencil":
      return importGPencil(json);

    case "particles":
      return importParticles(json);

    default:
      throw new Error(`Error importing movement, unknown type ${json["type"]}`);
  }
}
