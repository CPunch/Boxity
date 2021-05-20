#include "core/Object.hpp"

Object::Object(Object *p) {
    setParent(p);
}

Object::~Object() {
    setParent(nullptr);

    // delete all children
    auto _children = children; // make a copy of children since it'll change as we iterate over it
    for (Object *child : _children) {
        delete child;
    }
}

void Object::onParentAdd() {}
void Object::onParentRemove() {}

// ==================================== [[ SETTERS ]] ====================================

void Object::setParent(Object *p) {
    // minor optimization
    if (p == parent)
        return;

    // remove ourselves from our old parent
    if (parent != nullptr) {
        onParentRemove();
        parent->removeChild(this);
    }

    parent = p;

    // add ourselves to our new parent
    if (p != nullptr) {
        p->addChild(this);
        onParentAdd();
    }
}

// ==================================== [[ GETTERS ]] ====================================

Object* Object::getParent() {
    return parent;
}

iOBJTYPE Object::getTypeFlags() {
    return typeFlags;
}

b2World* Object::getWorld() {
    if (parent != nullptr && isPhysical(parent))
        return parent->getWorld();
    // else if the parent isn't a physical object, return a nullptr
    return nullptr;
}

// ==================================== [[ MISC. ]] ====================================

void Object::addChild(Object *c) {
    children.insert(c);
}

void Object::removeChild(Object *c) {
    children.erase(c);
}

void Object::tick() {
    for (Object *child : children) {
        child->tick();
    }
}

void Object::render(sf::RenderWindow &win) {
    // if any of our children are renderable, render them
    for (Object *child : children) {
        if (isRenderable(child))
            child->render(win);
    }
}