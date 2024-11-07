#include "perlin.h" 
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

void generateAndSaveTerrainMap(int width, int height, float scale) {
    // Create a Perlin noise object
    Perlin p;

    // 2D vector to store noise values
    std::vector<std::vector<float>> noiseGrid(height, std::vector<float>(width));

    // Open file for writing
    std::ofstream outFile("terrain_map.txt");
    if (!outFile) {
        std::cerr << "Error: Could not open file for writing.\n";
        return;
    }

    // Fill the noiseGrid with Perlin noise values and save the terrain symbols to file
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Generate Perlin noise value
            float noiseValue = p.noise(x * scale, y * scale, 0.0f);

            // Store noise value in the grid
            noiseGrid[y][x] = noiseValue;

            // Convert noise value to a terrain symbol
            char terrain;
            if (noiseValue < -0.2f) terrain = 'W';          // Water
            else if (noiseValue < 0.15f) terrain = 'P';     // Plains
            else if (noiseValue < 0.35f) terrain = 'F';      // Forest
            else terrain = 'M';                             // Mountains

            // Write symbol to file
            outFile << terrain;
        }
        outFile << '\n';  // Newline after each row
    }

    // Close file
    outFile.close();
    std::cout << "Terrain map saved to terrain_map.txt\n";
}

int main() {
    int width = 50;      // Width of the map
    int height = 50;     // Height of the map
    float scale = 0.1f;  // Scale for the Perlin noise

    // Generate and save the terrain map
    generateAndSaveTerrainMap(width, height, scale);

    return 0;
}
