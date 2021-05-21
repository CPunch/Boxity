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

void Object::onParentRemove() {
    // let the children know
    for (Object *child : children) {
        child->onParentRemove();
    }
}

void Object::onParentAdd() {
    // let the children know
    for (Object *child : children) {
        child->onParentAdd();
    }
}

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
    root = getRoot();

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

Root* Object::getRoot() {
    return (parent != nullptr) ? parent->getRoot() : nullptr;
}

// ==================================== [[ MISC. ]] ====================================

void Object::addChild(Object *c) {
    children.insert(c);
}

void Object::removeChild(Object *c) {
    children.erase(c);
}

void Object::tick(float dt) {
    for (Object *child : children) {
        child->tick(dt);
    }
}

void Object::render(sf::RenderWindow &win) {
    // if any of our children are renderable, render them
    for (Object *child : children) {
        if (isRenderable(child))
            child->render(win);
    }
}