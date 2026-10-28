#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <json.hpp>
#include "PointLight.h"

using json = nlohmann::json;

class SceneLoader {
public:
    // Pass in the array and count by reference so the loader can modify them
    static void LoadLights(const std::string& filepath, PointLight* pointLights, unsigned int& pointLightCount, unsigned int maxLights);
};