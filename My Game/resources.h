#ifndef RESOURCES_H
#define RESOURCES_H
#include <vector>
#include <unordered_map>
#include <string>

// Structure to represent a resource
struct Resource {
    std::string type;
    int quantity;
};

// Mapping from terrain types to resource types and base quantities
extern std::unordered_map<char, Resource> TERRAIN_TO_RESOURCE;

// Mapping from terrain types to resource quantity ranges
extern std::unordered_map<char, std::pair<int, int>> RESOURCE_QUANTITY_RANGE;

// Function to generate a random resource based on terrain type
Resource GenerateRandomResource(char terrainType);

// Function to generate a 2D resource map with random quantities
std::vector<std::vector<Resource>> GenerateResourcesWithRandomQuantities(const std::vector<std::vector<char>>& terrainMap);

// Function to load a terrain map from a text file
std::vector<std::vector<char>> LoadTerrainMap(const std::string& fileName);

// Function to print the resource map to the console
void PrintResources(const std::vector<std::vector<Resource>>& resourceMap);

// Function to save the resource map to a text file
void SaveResourcesToFile(const std::vector<std::vector<Resource>>& resourceMap, const std::string& fileName);

// Function to check the alignment of terrain and resource maps
void CheckMapAlignment(const std::vector<std::vector<char>>& terrainMap, const std::vector<std::vector<Resource>>& resourceMap);


#endif