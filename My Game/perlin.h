#ifndef PERLIN_H
#define PERLIN_H

class Perlin {
public:
    Perlin();
    ~Perlin();

    // Generates a Perlin noise value between -1 and 1 at the given 3D position.
    float noise(float x, float y, float z);

private:
    int* p; // Permutation table
    float* Gx, * Gy, * Gz; // Gradient vectors

    void initialize();  // Initializes permutation and gradient arrays
};
void generateAndSaveTerrainMap(int weight, int height, float scale);

#endif