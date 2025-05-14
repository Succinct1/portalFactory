#ifndef CHUNK_H
#define CHUNK_H

#include "gameObject.h"
#include "common.h"
using namespace std;

class chunk : public gameObject{
public:

    int seed;
    b2Vec2 pos;
    unordered_map<int, char> surfaceData; 
    vector<gameObject*> objects;
    chunk();
    chunk(int seed, b2Vec2 pos);
    ~chunk();

private:
    void getCoord(uint coord, int* x, int* y);
    int getIndex(int x, int y);
};

#endif // CHUNK_H