#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <unordered_set>
#include <inttypes.h>
#include <memory>

enum OBJTYPE {
    RENDEROBJ  = 1,
    PHYSICOBJ  = 2,
    SERVICEOBJ = 4
};

class Object;
typedef std::shared_ptr<Object> ObjectPtr;
typedef uint8_t iOBJTYPE;

#define isRenderable(x) (x->getTypeFlags() & RENDEROBJ)
#define isPhysical(x)   (x->getTypeFlags() & PHYSICOBJ)
#define addFlag(x,flag) x |= flag
#define castObjPtr(x, type) ((type*)(x).get())

class Root;
class Object : public std::enable_shared_from_this<Object> {
protected:
    std::unordered_set<ObjectPtr> children;
    ObjectPtr parent = nullptr;
    ObjectPtr root = nullptr;
    iOBJTYPE typeFlags = 0;

    void addChild(ObjectPtr);
    void removeChild(ObjectPtr);

    // events
    virtual void onParentRemove(); // called before child is removed from parent
    virtual void onParentAdd(); // called after child is added to parent

public:
    Object();
    ~Object();

    void setParent(ObjectPtr);

    Object* getParent();
    iOBJTYPE getTypeFlags();
    virtual ObjectPtr getRoot();

    void remove();
    virtual void tick(float);
};