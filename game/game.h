#ifndef GAME_H
#define GAME_H

#include "common.h"
#include "chunk.h"

class game {
public:
    b2WorldId worldId;
    unordered_map<int, chunk*> chunks;
    unordered_set<b2ShapeId, b2ShapeIdHash, b2ShapeIdEqual> queryReturn; // Use custom hash and equality
    game();
    game(string file);
    ~game();

    void loadChunk(b2Vec2 coord);
    void buildChunk(b2Vec2 coord);
    void getIds(b2Vec2 tl, b2Vec2 br, unordered_set<b2ShapeId>* collection);
    static bool getID(b2ShapeId shapeId, void* context);

private:
};

#endif // GAME_H