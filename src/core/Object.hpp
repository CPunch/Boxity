#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <unordered_set>
#include <inttypes.h>

enum OBJTYPE {
    RENDEROBJ  = 1,
    PHYSICOBJ  = 2,
    SERVICEOBJ = 4
};

typedef uint8_t iOBJTYPE;

#define isRenderable(x) (((Object*)x)->getTypeFlags() & RENDEROBJ)
#define isPhysical(x)   (((Object*)x)->getTypeFlags() & PHYSICOBJ)
#define addFlag(x,flag) x |= flag

class Root;
class Object {
protected:
    std::unordered_set<Object*> children;
    Object *parent = nullptr;
    Root *root = nullptr;
    iOBJTYPE typeFlags = 0;

    void addChild(Object*);
    void removeChild(Object*);

    // events
    virtual void onParentRemove(); // called before child is removed from parent
    virtual void onParentAdd(); // called after child is added to parent

public:
    Object(Object *p);
    ~Object();

    void setParent(Object*);

    Object* getParent();
    iOBJTYPE getTypeFlags();
    virtual Root* getRoot();

    virtual void tick(float);
    virtual void render(sf::RenderWindow&);
};