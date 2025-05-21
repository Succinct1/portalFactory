#ifndef GAME_H
#define GAME_H

#include "common.h"
#include "chunk.h"
#include "player.h"

class game {
public:
    b2WorldId worldId;
    int worldSeed; // Seed for the entire world
    slopeTypes lastSlopeType;
    float timeStep = DEFAULT_TIME_STEP;
    b2Vec2 leftTerminus, rightTerminus; 
    ulong leftIndex, rightIndex;
    b2Vec2 leftEdge, rightEdge;
    unordered_map<ulong, chunk*> chunks;
    vector<player*> players;
    unordered_set<gameObject*> updateList;
    unordered_set<gameObject*> removeList;

    unordered_set<b2ShapeId, b2ShapeIdHash, b2ShapeIdEqual> queryReturn; 

    game();
    game(string file);
    ~game();
    
    void gameTick();
    void loadChunk(b2Vec2 coord);
    void buildChunk(b2Vec2 coord);
    void buildSurfaceChunk(bool left);
    bool addChunk(uint index);

    void getIds(b2Vec2 tl, b2Vec2 br, unordered_set<b2ShapeId>* collection);
    static bool getID(b2ShapeId shapeId, void* context);

    static b2Vec2 getChunkCoord(ulong index);
    static b2Vec2 getChunkCoord(b2Vec2 coord);
    static ulong getChunkIndex(b2Vec2 coord);
    int generateChunkSeed(b2Vec2 coord); 
    b2Vec2 addLine(b2Vec2 start, int dx, int dy);
};

#endif // GAME_H