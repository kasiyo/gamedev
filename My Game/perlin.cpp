#include "perlin.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <utility>


Perlin::Perlin() {
    // Initialize random seed and permutation and gradient arrays
    initialize();
}

Perlin::~Perlin() {
    delete[] p;
    delete[] Gx;
    delete[] Gy;
    delete[] Gz;
}

void Perlin::initialize() {
    srand(static_cast<unsigned>(time(nullptr)));

    p = new int[256];
    Gx = new float[256];
    Gy = new float[256];
    Gz = new float[256];

    for (int i = 0; i < 256; ++i) {
        p[i] = i;
        Gx[i] = (float(rand()) / RAND_MAX) * 2 - 1;
        Gy[i] = (float(rand()) / RAND_MAX) * 2 - 1;
        Gz[i] = (float(rand()) / RAND_MAX) * 2 - 1;
    }

    // Shuffle permutation table
    for (int i = 255; i > 0; --i) {
        int j = rand() % (i + 1);
        std::swap(p[i], p[j]);
    }
}

float Perlin::noise(float x, float y, float z) {
    // Calculate unit cube coordinates surrounding the sample point
    int X = static_cast<int>(floor(x)) & 255;
    int Y = static_cast<int>(floor(y)) & 255;
    int Z = static_cast<int>(floor(z)) & 255;

    // Relative coordinates within the cube
    x -= floor(x);
    y -= floor(y);
    z -= floor(z);

    // Fade curves for each coordinate
    auto fade = [](float t) { return t * t * t * (t * (t * 6 - 15) + 10); };
    float u = fade(x);
    float v = fade(y);
    float w = fade(z);

    // Hash coordinates of the 8 cube corners
    auto hash = [this](int x, int y, int z) {
        return p[(p[(p[x] + y) & 255] + z) & 255];
        };

    // Linearly interpolate between the gradients of the 8 corners
    auto grad = [this](int hash, float x, float y, float z) {
        int h = hash & 15;
        float u = h < 8 ? x : y;
        float v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
        return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
        };

    int A = hash(X, Y, Z);
    int B = hash(X + 1, Y, Z);
    int C = hash(X, Y + 1, Z);
    int D = hash(X + 1, Y + 1, Z);
    int E = hash(X, Y, Z + 1);
    int F = hash(X + 1, Y, Z + 1);
    int G = hash(X, Y + 1, Z + 1);
    int H = hash(X + 1, Y + 1, Z + 1);

	auto lerp = [](float a, float b, float t) { return a + t * (b - a); };

    float X1 = lerp(grad(A, x, y, z), grad(B, x - 1, y, z), u);
    float X2 = lerp(grad(C, x, y - 1, z), grad(D, x - 1, y - 1, z), u);
    float Y1 = lerp(X1, X2, v);

    X1 = lerp(grad(E, x, y, z - 1), grad(F, x - 1, y, z - 1), u);
    X2 = lerp(grad(G, x, y - 1, z - 1), grad(H, x - 1, y - 1, z - 1), u);
    float Y2 = lerp(X1, X2, v);

    return lerp(Y1, Y2, w);
}
