#ifndef CHUNK_H
#define CHUNK_H

#include "gameObject.h"
#include "common.h"

class chunk : public gameObject{
public:

    int seed;
    b2Vec2 pos; // Bottom Left corner in box2d space
    long index; // Index in the chunk map
    unordered_map<uint, char> surfaceData; 
    vector<gameObject*> objects;
    
    chunk();
    chunk(int seed, b2Vec2 coord);
    ~chunk();

    static void getCoord(uint index, int* x, int* y);
    static uint getIndex(int x, int y);

    b2Vec2 getBoxCoord(int x, int y);
    void getCoord(b2Vec2 coord, int* x, int* y);
    int getHash(int x, int y);
};

#endif // CHUNK_H