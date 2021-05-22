#include "core/Object.hpp"

Object::Object() {}

Object::~Object() {}

void Object::onParentRemove() {
    // let the children know
    for (ObjectPtr child : children) {
        child->onParentRemove();
    }
}

void Object::onParentAdd() {
    // let the children know
    for (ObjectPtr child : children) {
        child->onParentAdd();
    }
}

// ==================================== [[ SETTERS ]] ====================================

void Object::setParent(ObjectPtr p) {
    // minor optimization
    if (p.get() == parent.get())
        return;

    ObjectPtr oldP = parent;
    parent = p;

    onParentRemove();

    root = getRoot();

    // add ourselves to our new parent
    if (parent.get() != nullptr) {
        parent->addChild(shared_from_this());
    }

    onParentAdd();

    // remove ourselves from our old parent (for real this time)
    if (oldP.get() != nullptr) {
        oldP->removeChild(shared_from_this());
    }
}

// ==================================== [[ GETTERS ]] ====================================

Object* Object::getParent() {
    return parent.get();
}

iOBJTYPE Object::getTypeFlags() {
    return typeFlags;
}

ObjectPtr Object::getRoot() {
    // return our cached copy of root if we have it :)
    if (root.get() != nullptr)
        return root;

    return (parent != nullptr) ? parent->getRoot() : nullptr;
}

// ==================================== [[ MISC. ]] ====================================

void Object::remove() {
    // remove all children
    for (auto iter = children.begin(); iter != children.end();) {
        (*iter++)->remove();
    }

    setParent(nullptr);
}

void Object::addChild(ObjectPtr c) {
    children.insert(c);
}

void Object::removeChild(ObjectPtr c) {
    children.erase(c);
}

void Object::tick(float dt) {
    for (ObjectPtr child : children) {
        child->tick(dt);
    }
}