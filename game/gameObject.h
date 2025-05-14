#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "common.h"

// Interface for all game objects
class gameObject {
public:
    b2BodyDef bodyDef;
    b2BodyId body;
    b2ShapeDef shapeDef;
    b2ShapeId shape;
    b2Vec2 pos;
    float size;
    float density;

    gameObject();
    gameObject(b2WorldId world, b2Vec2 position, float size, float density, b2Filter* filter);
    ~gameObject();
    virtual void update(int ticks);
    

private:
};

#endif // GAME_OBJECT_H