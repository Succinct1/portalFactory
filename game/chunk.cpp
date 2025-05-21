#include "chunk.h"

// Default constructor
chunk::chunk() {
}

chunk::chunk(int seed, b2Vec2 pos) : seed(seed), pos(pos){
}

chunk::~chunk() {
}

/// @brief For storing square data in map
/// @param coord 
/// @param x 
/// @param y 
void chunk::getCoord(uint index, int* x, int* y) {
    *x = index % CHUNK_SIZE;
    *y = index / CHUNK_SIZE;
}

/// @brief Get index for storing in map
/// @param x 
/// @param y 
/// @return 
uint chunk::getIndex(int x, int y) {
    return y * CHUNK_SIZE + x;
}

b2Vec2 chunk::getBoxCoord(int x, int y) {
    b2Vec2 coord = pos;
    coord.x += x * GROUND_SIZE;
    coord.y += y * GROUND_SIZE;
    return coord;
}

void chunk::getCoord(b2Vec2 coord, int* x, int* y) {
    *x = (coord.x - pos.x) / GROUND_SIZE;
    *y = (coord.y - pos.y) / GROUND_SIZE;
}

/// @brief Hash function for determining square type
/// @param x 
/// @param y 
/// @return 
int chunk::getHash(int x, int y) {
    hash<int> hasher;
    return hasher(seed) ^ (hasher(x) << 1) ^ (hasher(y) << 2);
}