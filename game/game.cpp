#include "game.h"

game::game() {

}


game::game(string file) {
    // Load the world from the file
}

game::~game() {
}

void game::getIds(b2Vec2 tl, b2Vec2 br, unordered_set<b2ShapeId>* collection) {
    b2AABB aabb;
    aabb.lowerBound = tl;
    aabb.upperBound = br;
    b2QueryFilter filter = b2DefaultQueryFilter();
    b2World_OverlapAABB(worldId, aabb, filter, getID, collection);
}

bool game::getID(b2ShapeId shapeId, void* context) {
    unordered_set<b2ShapeId>* collection = (unordered_set<b2ShapeId>*)context;
    collection->insert(shapeId);
    return true;
}