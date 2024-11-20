#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include "resources.h"


std::unordered_map<char, Resource> TERRAIN_TO_RESOURCE = {
    { 'M', { "St", 10 } },
    { 'F', { "Wd", 20 } },
    { 'P', { "Fd", 15 } },
    { 'W', { "No", 0 } }
};


std::unordered_map<char, std::pair<int, int>> RESOURCE_QUANTITY_RANGE = {
    { 'M', { 5, 20 } },  // Stone: 5 to 20 units
    { 'F', { 10, 30 } }, // Wood: 10 to 30 units
    { 'P', { 8, 25 } },  // Food: 8 to 25 units
    { 'W', { 0, 0 } }    // Water: 0 units
};


Resource GenerateRandomResource(char terrainType) {
    const auto& range = RESOURCE_QUANTITY_RANGE[terrainType];
    int quantity = rand() % (range.second - range.first + 1) + range.first;
    return { TERRAIN_TO_RESOURCE[terrainType].type, quantity };
}


std::vector<std::vector<Resource>> GenerateResourcesWithRandomQuantities(const std::vector<std::vector<char>>& terrainMap) {
    std::vector<std::vector<Resource>> resourceMap(terrainMap.size());
    for (size_t y = 0; y < terrainMap.size(); ++y) {
        resourceMap[y].resize(terrainMap[y].size());
        for (size_t x = 0; x < terrainMap[y].size(); ++x) {
            char terrainType = terrainMap[y][x];
            resourceMap[y][x] = GenerateRandomResource(terrainType);
        }
    }
    return resourceMap;
}


// Function to load terrain map from a text file
std::vector<std::vector<char>> LoadTerrainMap(const std::string& fileName) {
    std::vector<std::vector<char>> terrainMap;
    std::ifstream inFile(fileName);

    if (inFile.is_open()) {
        std::string line;
        while (std::getline(inFile, line)) {
            std::vector<char> row(line.begin(), line.end());
            terrainMap.push_back(row);
        }
        inFile.close();
    } else {
        std::cerr << "Error: Unable to open file " << fileName << std::endl;
    }

    return terrainMap;
}


// Function to display resources
void PrintResources(const std::vector<std::vector<Resource>>& resourceMap) {
    for (const auto& row : resourceMap) {
        for (const auto& resource : row) {
            std::cout << resource.type << "(" << resource.quantity << ") ";
        }
        std::cout << std::endl;
    }
}


void SaveResourcesToFile(const std::vector<std::vector<Resource>>& resourceMap, const std::string& fileName) {
    std::ofstream outFile(fileName);

    if (outFile.is_open()) {
        for (const auto& row : resourceMap) {
            for (const auto& resource : row) {
                outFile << resource.type << "(" << resource.quantity << ") ";
            }
            outFile << '\n';
        }
        outFile.close();
        std::cout << "Resources saved to " << fileName << std::endl;
    } else {
        std::cerr << "Error: Could not open file " << fileName << " for writing." << std::endl;
    }
}


void CheckMapAlignment(const std::vector<std::vector<char>>& terrainMap, const std::vector<std::vector<Resource>>& resourceMap) {
    if (terrainMap.size() != resourceMap.size()) {
        std::cerr << "Row count mismatch: Terrain Map = " << terrainMap.size()
                  << ", Resource Map = " << resourceMap.size() << std::endl;
        return;
    }

    for (size_t y = 0; y < terrainMap.size(); ++y) {
        if (terrainMap[y].size() != resourceMap[y].size()) {
            std::cerr << "Column count mismatch at row " << y << ": Terrain Map = " 
                      << terrainMap[y].size() << ", Resource Map = " 
                      << resourceMap[y].size() << std::endl;
        }
    }
}

