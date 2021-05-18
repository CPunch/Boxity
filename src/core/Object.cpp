#include "core/Object.hpp"

Object::Object(World *w) {
    wrld = w;
    wrld->addObject(this);
}

Object::~Object() {
    wrld->removeObject(this);
}

void Object::tick() {
    
}