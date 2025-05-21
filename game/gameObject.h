#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "common.h"

class game;

// Interface for all game objects
class gameObject {
public:
    game* instance;
    b2BodyDef* bodyDef;
    b2BodyId body;
    b2ShapeDef* shapeDef;
    b2ShapeId shape;

    gameObject();
    gameObject(b2WorldId world, b2Vec2 position, float size, float density, b2Filter* filter);
    ~gameObject();
    virtual void update(int ticks = 1);
    virtual void sensorBeginTouch(b2SensorBeginTouchEvent* event);
    virtual void sensorEndTouch(b2SensorEndTouchEvent* event);
    virtual void beginContact(b2ContactBeginTouchEvent* event);
    virtual void endContact(b2ContactEndTouchEvent* event);
    virtual void destroyThis();

private:
};

#endif // GAME_OBJECT_H