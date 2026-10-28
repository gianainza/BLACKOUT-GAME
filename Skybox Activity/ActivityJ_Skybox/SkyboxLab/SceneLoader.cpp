#include "SceneLoader.h"

void SceneLoader::LoadLights(const std::string& filepath, PointLight* pointLights, unsigned int& pointLightCount, unsigned int maxLights) {
    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open scene file at " << filepath << std::endl;
        return;
    }

    try {
        json sceneData = json::parse(file);

        // Loop through the point_lights array in the JSON
        pointLightCount = 0;
        for (const auto& lightData : sceneData["lights"]["point_lights"]) {
            if (pointLightCount >= maxLights) break; // Prevent overflowing the array

            // Extract the color and start_position arrays
            float r = lightData["color"][0];
            float g = lightData["color"][1];
            float b = lightData["color"][2];

            float x = lightData["start_position"][0];
            float y = lightData["start_position"][1];
            float z = lightData["start_position"][2];

            // Initialize the light in the array (using default Activity J shadow settings for now)
            pointLights[pointLightCount] = PointLight(
                1024, 1024, // shadow map resolution
                0.01f, 100.0f, // near and far planes
                r, g, b,
                lightData["ambient_intensity"], lightData["diffuse_intensity"],
                x, y, z,
                lightData["attenuation"][0], lightData["attenuation"][1], lightData["attenuation"][2]
            );

            pointLightCount++;
        }
        std::cout << "Successfully loaded " << pointLightCount << " point lights." << std::endl;

    }
    catch (json::parse_error& e) {
        std::cerr << "Parse Error in " << filepath << " at byte " << e.byte << ": " << e.what() << std::endl;
    }
}