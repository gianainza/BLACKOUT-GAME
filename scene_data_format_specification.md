# Scene Data Format Specification (BLACKOUT)

## 1. Introduction
This document defines the JSON structure used to load levels for our game, **BLACKOUT**. The scene loader parses this single JSON file at runtime. This allows the Technical Artist (R3) to assemble the level and the Gameplay Programmer (R4) to define collision boundaries and patrol paths without needing to recompile the C++ project.

## 2. General Formatting Rules
*   **Vectors/Coordinates:** All 3D positions, rotations, scales, and colors (RGB) must be formatted as JSON arrays containing exactly three floats. Example: `[1.0, 0.5, 0.0]`.
*   **File Paths:** All asset paths (models, textures) must be strings relative to the executable's working directory. Example: `"Models/wall.obj"`.
*   **IDs:** Materials and Meshes are assigned unique string `"id"` tags so that objects can reference them easily.
*   **Angles:** Rotations are defined in degrees (Euler angles) for ease of human editing.

## 3. Required Sections Breakdown

### 3.1 Materials
Defines the physical look of surfaces. R3 must define at least 5 distinct materials here.
*   `id`: Unique string identifier used by objects.
*   `texture_path`: Path to the diffuse texture map.
*   `specular_intensity`: Float representing how bright specular highlights are.
*   `shininess`: Float representing how tight/focused the specular highlight is.

### 3.2 Lights
Defines the illumination of the scene.
*   `directional_light`: Requires `direction` [x,y,z], `color` [r,g,b], `ambient_intensity`, and `diffuse_intensity`.
*   `point_lights`: Array of bulbs. **Blackout Specific:** Requires a `patrol_path` (an array of `[x,y,z]` waypoints) and a `patrol_speed`. Also requires `color`, `ambient_intensity`, `diffuse_intensity`, and `attenuation` (formatted as `[constant, linear, exponent]`).
*   `spot_lights`: Array of spotlights. Requires `position`, `direction`, `color`, `edge` (float, cutoff angle), and `fade` parameters.

### 3.3 Objects (Geometry)
Defines the static layout of the yard. **Blackout Specific:** R4 will use these definitions to generate collision boxes. 
*   `id`: String identifier for debugging/collision.
*   `mesh_path`: Path to the 3D model file (or primitive name if loading is disabled).
*   `material_id`: Must match an `id` defined in the materials section.
*   `position`: `[x, y, z]` world coordinates.
*   `rotation`: `[x, y, z]` rotation in degrees.
*   `scale`: `[x, y, z]` scale multipliers.

### 3.4 Game Logic Markers
*   `player_start`: `[x,y,z]` position where the player spawns.
*   `exit_zone`: `[x,y,z]` position and `radius` defining the win condition trigger.

### 3.5 Skybox
Defines the textures used to create the background sky environment. 
*   `faces`: An array of six string file paths representing the cube map faces. The order must match the loading sequence expected by the C++ code (typically right, left, up, down, back, front).

---

## 4. Concrete Example (Valid Scene JSON)

```json
{
  "materials": [
    {
      "id": "mat_concrete",
      "texture_path": "Textures/concrete.png",
      "specular_intensity": 0.2,
      "shininess": 8.0
    },
    {
      "id": "mat_metal_crate",
      "texture_path": "Textures/metal.png",
      "specular_intensity": 1.0,
      "shininess": 64.0
    }
  ],
  "lights": {
    "directional_light": {
      "direction": [-0.2, -1.0, -0.3],
      "color": [0.1, 0.1, 0.15],
      "ambient_intensity": 0.05,
      "diffuse_intensity": 0.1
    },
    "point_lights": [
      {
        "id": "patrol_guard_1",
        "color": [1.0, 0.9, 0.6],
        "ambient_intensity": 0.1,
        "diffuse_intensity": 1.0,
        "attenuation": [0.3, 0.2, 0.1],
        "patrol_speed": 2.5,
        "patrol_path": [
          [5.0, 2.0, 0.0],
          [5.0, 2.0, 15.0],
          [-5.0, 2.0, 15.0]
        ]
      },
      {
        "id": "patrol_guard_2",
        "color": [1.0, 0.9, 0.6],
        "ambient_intensity": 0.1,
        "diffuse_intensity": 1.0,
        "attenuation": [0.3, 0.2, 0.1],
        "patrol_speed": 3.0,
        "patrol_path": [
          [-10.0, 2.0, -10.0],
          [10.0, 2.0, -10.0]
        ]
      }
    ],
    "spot_lights": [
      {
        "position": [0.0, 10.0, 0.0],
        "direction": [0.0, -1.0, 0.0],
        "color": [1.0, 1.0, 1.0],
        "ambient_intensity": 0.0,
        "diffuse_intensity": 1.0,
        "edge": 20.0
      }
    ]
  },
  "objects": [
    {
      "id": "floor",
      "mesh_path": "Models/plane.obj",
      "material_id": "mat_concrete",
      "position": [0.0, 0.0, 0.0],
      "rotation": [0.0, 0.0, 0.0],
      "scale": [50.0, 1.0, 50.0]
    },
    {
      "id": "hiding_crate_1",
      "mesh_path": "Models/cube.obj",
      "material_id": "mat_metal_crate",
      "position": [2.0, 1.0, 5.0],
      "rotation": [0.0, 45.0, 0.0],
      "scale": [2.0, 2.0, 2.0]
    }
  ],
  "skybox": {
    "faces": [
      "Textures/Skybox/cupertin-lake_rt.tga",
      "Textures/Skybox/cupertin-lake_lf.tga",
      "Textures/Skybox/cupertin-lake_up.tga",
      "Textures/Skybox/cupertin-lake_dn.tga",
      "Textures/Skybox/cupertin-lake_bk.tga",
      "Textures/Skybox/cupertin-lake_ft.tga"
    ]
  },
  "game_logic": {
    "player_start": [0.0, 1.0, 0.0],
    "exit_zone": {
      "position": [0.0, 1.0, 40.0],
      "radius": 3.0
    }
  }
}
```