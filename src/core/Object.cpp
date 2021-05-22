#include "core/Object.hpp"

Object::Object() {

}

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

    // remove ourselves from our old parent
    if (oldP.get() != nullptr) {
        onParentRemove();
        oldP->removeChild(shared_from_this());
    }

    root = getRoot();

    // add ourselves to our new parent
    if (parent.get() != nullptr) {
        parent->addChild(shared_from_this());
        onParentAdd();
    }
}

// ==================================== [[ GETTERS ]] ====================================

Object* Object::getParent() {
    return parent.get();
}

iOBJTYPE Object::getTypeFlags() {
    return typeFlags;
}

Root* Object::getRoot() {
    return (parent != nullptr) ? parent->getRoot() : nullptr;
}

// ==================================== [[ MISC. ]] ====================================

void Object::remove() {
    setParent(nullptr);

    // remove all children
    for (auto iter = children.begin(); iter != children.end();) {
        (*iter)->remove();
    }
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