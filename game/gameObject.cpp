#include "gameObject.h"

gameObject::gameObject() {
}

gameObject::gameObject(b2WorldId world, b2Vec2 position, float size, float density, b2Filter* filter){
}

gameObject::~gameObject() {
}

void gameObject::update(int ticks) {
}

void gameObject::sensorBeginTouch(b2SensorBeginTouchEvent* event) {
    // Empty implementation
}

void gameObject::sensorEndTouch(b2SensorEndTouchEvent* event) {
    // Empty implementation
}

void gameObject::beginContact(b2ContactBeginTouchEvent* event) {
    // Empty implementation
}

void gameObject::endContact(b2ContactEndTouchEvent* event) {
    // Empty implementation
}

void gameObject::destroyThis() {
    // Empty implementation
}