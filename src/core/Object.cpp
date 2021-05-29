#include "core/Object.hpp"

#include <iostream>

#define OBJLIBNAME "Object"

Object::Object() {
    name = "Object";
}

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

void Object::setName(std::string n) {
    name = n;
}

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

std::string Object::getName() {
    return name;
}

ObjectPtr Object::getParent() {
    return parent;
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

void Object::tick(uint64_t dt) {
    for (ObjectPtr child : children) {
        child->tick(dt);
    }
}

// ==================================== [[ LUA ]] ====================================

void Object::pushObj(lua_State *L, ObjectPtr obj) {
    // sanity check
    if (obj.get() == nullptr) {
        lua_pushnil(L);
        return;
    }

    // create a shared_ptr userdata on the lua heap
    void *oPtr = lua_newuserdata(L, sizeof(ObjectPtr));

    // use the new operator to increase our reference count of this shared_ptr
    new(oPtr) ObjectPtr(obj);

    // set the udata's metatable
    luaL_getmetatable(L, OBJLIBNAME);
    lua_setmetatable(L, -2);
}

ObjectPtr* Object::grabObj(lua_State* L, int indx) {
    void *oPtr = luaL_checkudata(L, indx, OBJLIBNAME);

    return oPtr == NULL ? nullptr : (ObjectPtr*)oPtr;
}

static int objIndex(lua_State *L) {
    ObjectPtr *oPtr = Object::grabObj(L, 1);
    // sanity check
    if (oPtr == nullptr)
        return 0;

    const char *indx = luaL_checkstring(L, 2);

    // check if the index is in the getters table
    lua_getmetatable(L, 1);
    lua_getfield(L, -1, "__getters");
    lua_getfield(L, -1, indx);
    if (!lua_isnil(L, -1)) {
        // we got our getter function, call it
        lua_remove(L, -2); // removes __getters
        lua_remove(L, -2); // removes meta

        // push Obj & call getter
        lua_pushvalue(L, 1);
        lua_call(L, 1, 1);
        return 1;
    } else {
        lua_pop(L, 2); // pops result && __getters

        lua_getfield(L, -1, "__methods");
        lua_getfield(L, -1, indx);
        if (!lua_isnil(L, -1)) {
            lua_remove(L, -2); // removes __methods
            lua_remove(L, -2); // removes meta

            return 1; // returns the method
        }
    }

    std::cout << "indexed with \'" << indx << "\'" << std::endl;
    return 0;
}

// when lua decides to clean up it's reference, this'll be called
static int objGC(lua_State *L) {
    ObjectPtr *oPtr = Object::grabObj(L, 1);

    // reset the shared_ptr, removing this reference
    if (oPtr != nullptr)
        oPtr->reset();

    return 0;
}

// ==================================== [[ LUA GETTERS ]] ====================================

static int objGetParent(lua_State *L) {
    ObjectPtr *oPtr = Object::grabObj(L, 1);

    // sanity check
    if (oPtr != nullptr) {
        Object::pushObj(L, (*oPtr)->getParent());
        return 1;
    }

    return 0;
}

static int objGetName(lua_State *L) {
    ObjectPtr *oPtr = Object::grabObj(L, 1);

    // sanity check
    if (oPtr != nullptr) {
        lua_pushstring(L, (*oPtr)->getName().c_str());
        return 1;
    }

    return 0;
}

static const luaL_Reg ObjGetters[] {
    {"parent", objGetParent},
    {"name", objGetName},
    {NULL, NULL}
};

// ==================================== [[ LUA SETTERS ]] ====================================

static int objSetParent(lua_State *L) {
    ObjectPtr *oPtr = Object::grabObj(L, 1);
    ObjectPtr *newParent = Object::grabObj(L, 1);

    // sanity check
    if (oPtr != nullptr) {
        if (newParent != nullptr)
            (*oPtr)->setParent(*newParent);
        else
            (*oPtr)->setParent(nullptr);
    }

    return 0;
}

static const luaL_Reg ObjSetters[] {
    {"parent", objSetParent},
    {NULL, NULL}
};

static const luaL_Reg ObjMethods[] {
    {NULL, NULL}
};

static const luaL_Reg ObjLib[] {
    {NULL, NULL}
};

void Object::registerClass(lua_State* L, const luaL_Reg *setters, const luaL_Reg *getters, const luaL_Reg *methods, const char *name) {
    luaL_newmetatable(L, name);

    // don't let the user set the metatable for these
    lua_pushstring(L, "__metatable");
    lua_pushstring(L, "This metatable is locked");
    lua_rawset(L, -3); // meta.__metatable = "This metatable is locked"

    // set setters
    lua_pushstring(L, "__setters");
    lua_newtable(L);
    luaL_setfuncs(L, setters, 0);
    lua_rawset(L, -3); // meta.__setters = setters

    // set getters
    lua_pushstring(L, "__getters");
    lua_newtable(L);
    luaL_setfuncs(L, getters, 0);
    lua_rawset(L, -3); // meta.__getters = getters

    // set methods
    lua_pushstring(L, "__methods");
    lua_newtable(L);
    luaL_setfuncs(L, methods, 0);
    lua_rawset(L, -3); // meta.__methods = methods

    // set __index
    lua_pushstring(L, "__index");
    lua_pushcfunction(L, objIndex);
    lua_rawset(L, -3); // meta.__index = objIndex

    // set __newindex

    // set __gc
    lua_pushstring(L, "__gc");
    lua_pushcfunction(L, objGC);
    lua_rawset(L, -3); // meta.__gc = objGC
}

void Object::addBindings(lua_State *L) {
    Object::registerClass(L, ObjSetters, ObjGetters, ObjMethods, "Object");
}