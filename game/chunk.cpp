#include "chunk.h"

// Default constructor
chunk::chunk() {
}

chunk::chunk(int seed, b2Vec2 pos) : seed(seed) {

}

chunk::~chunk() {
}

void chunk::getCoord(uint coord, int* x, int* y) {
    *x = coord % CHUNK_SIZE;
    *y = coord / CHUNK_SIZE;
}

int chunk::getIndex(int x, int y) {
    return y * CHUNK_SIZE + x;
}