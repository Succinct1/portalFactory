#include "gameObject.h"

gameObject::gameObject() {
}

gameObject::gameObject(b2WorldId world, b2Vec2 position, float size, float density, b2Filter* filter): size(size), density(density){
}

gameObject::~gameObject() {
}

void gameObject::update(int ticks) {
}