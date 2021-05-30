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

void Object::pushLua(lua_State *L) {
    // create a shared_ptr userdata on the lua heap
    void *oPtr = lua_newuserdata(L, sizeof(ObjectPtr));

    // handle error
    if (oPtr == NULL)
        return;

    // use the new operator to increase our reference count of this shared_ptr
    new(oPtr) ObjectPtr(shared_from_this());

    // set the udata's metatable
    luaL_getmetatable(L, OBJLIBNAME);
    lua_setmetatable(L, -2);
}

ObjectPtr* Object::grabLua(lua_State *L, int indx, const char *classname) {
    void *oPtr = luaL_checkudata(L, indx, OBJLIBNAME);
    if (oPtr == NULL)
        return nullptr;

    // check if the classname is in the __childof table
    lua_getmetatable(L, 1);
    lua_getfield(L, -1, "__childof");
    lua_getfield(L, -1, classname);
    if (lua_isnil(L, -1)) { // the classname wasn't in __childof!
        lua_pop(L, 3); // pop nil, __childof & metatable

        return nullptr;
    }

    lua_pop(L, 3);
    return (ObjectPtr*)oPtr;
}

static int objIndex(lua_State *L) {
    ObjectPtr *oPtr = Object::grabLua(L, 1, "Object");
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

    return luaL_error(L, "attempt to index nil value '%s'", indx);
}

static int objNewIndex(lua_State *L) {
    ObjectPtr *oPtr = Object::grabLua(L, 1, "Object");
    // sanity check
    if (oPtr == nullptr)
        return 0;

    const char *indx = luaL_checkstring(L, 2);
    // check if the index is in the getters table
    lua_getmetatable(L, 1);
    lua_getfield(L, -1, "__setters");
    lua_getfield(L, -1, indx);
    if (!lua_isnil(L, -1)) {
        // we got our setter function, call it
        lua_remove(L, -2); // removes __setters
        lua_remove(L, -2); // removes meta

        // push Obj, new value & call getter
        lua_pushvalue(L, 1);
        lua_pushvalue(L, 3);
        lua_call(L, 2, 0);
        return 0;
    }

    return luaL_error(L, "attempt to index nil value '%s'", indx);
}

// when lua decides to clean up it's reference, this'll be called
static int objGC(lua_State *L) {
    ObjectPtr *oPtr = Object::grabLua(L, 1, "Object");

    // reset the shared_ptr, removing this reference
    if (oPtr != nullptr)
        oPtr->reset();

    return 0;
}

// ==================================== [[ LUA GETTERS ]] ====================================

static int objGetParent(lua_State *L) {
    ObjectPtr *oPtr = Object::grabLua(L, 1, "Object");

    // sanity check
    if (oPtr != nullptr && (*oPtr)->getParent() != nullptr) {
        (*oPtr)->getParent()->pushLua(L);
        return 1;
    }

    return 0;
}

static int objGetName(lua_State *L) {
    ObjectPtr *oPtr = Object::grabLua(L, 1, "Object");

    // sanity check
    if (oPtr != nullptr) {
        lua_pushstring(L, (*oPtr)->getName().c_str());
        return 1;
    }

    return 0;
}

void Object::registerLuaGetters(lua_State *L) {
    static const luaL_Reg getters[] {
        {"parent", objGetParent},
        {"name", objGetName},
        {NULL, NULL}
    };

    luaL_setfuncs(L, getters, 0);
}

// ==================================== [[ LUA SETTERS ]] ====================================

static int objSetParent(lua_State *L) {
    ObjectPtr *oPtr = Object::grabLua(L, 1, "Object");
    ObjectPtr *newParent = Object::grabLua(L, 1, "Object");

    // sanity check
    if (oPtr != nullptr) {
        if (newParent != nullptr)
            (*oPtr)->setParent(*newParent);
        else
            (*oPtr)->setParent(nullptr);
    }

    return 0;
}

static int objSetName(lua_State *L) {
    ObjectPtr *oPtr = Object::grabLua(L, 1, "Object");
    // sanity check
    if (oPtr == nullptr)
        return 0;

    const char *newName = luaL_checkstring(L, 2);

    (*oPtr)->setName(std::string(newName));
    return 0;
}

void Object::registerLuaSetters(lua_State *L) {
    static const luaL_Reg setters[] {
        {"parent", objSetParent},
        {"name", objSetName},
        {NULL, NULL}
    };

    luaL_setfuncs(L, setters, 0);
}

void Object::registerClass(lua_State* L, registerLuaTable setterTbl, registerLuaTable getterTbl, registerLuaTable methodTbl, registerLuaTable childTbl, const char *name) {
    luaL_newmetatable(L, name);

    // don't let the user set the metatable for these
    lua_pushstring(L, "__metatable");
    lua_pushstring(L, "This metatable is locked");
    lua_rawset(L, -3); // meta.__metatable = "This metatable is locked"

    // set setters
    lua_pushstring(L, "__setters");
    lua_newtable(L);
    setterTbl(L);
    lua_rawset(L, -3); // meta.__setters = setters

    // set getters
    lua_pushstring(L, "__getters");
    lua_newtable(L);
    getterTbl(L);
    lua_rawset(L, -3); // meta.__getters = getters

    // set type info
    lua_pushstring(L, "__childof");
    lua_newtable(L);
    childTbl(L);
    lua_rawset(L, -3); // meta.__childof = childtypes

    // set methods
    lua_pushstring(L, "__methods");
    lua_newtable(L);
    methodTbl(L);
    lua_rawset(L, -3); // meta.__methods = methods

    // set __index
    lua_pushstring(L, "__index");
    lua_pushcfunction(L, objIndex);
    lua_rawset(L, -3); // meta.__index = objIndex

    // set __newindex
    lua_pushstring(L, "__newindex");
    lua_pushcfunction(L, objNewIndex);
    lua_rawset(L, -3); // meta.__newindex = objNewIndex

    // set __gc
    lua_pushstring(L, "__gc");
    lua_pushcfunction(L, objGC);
    lua_rawset(L, -3); // meta.__gc = objGC

    lua_pop(L, 1);
}

void Object::registerLuaMethods(lua_State *L) {
    static const luaL_Reg methods[] {
        {NULL, NULL}
    };

    luaL_setfuncs(L, methods, 0);
}

void Object::registerLuaChild(lua_State *L) {
    lua_pushstring(L, "Object");
    lua_pushboolean(L, 1);
    lua_rawset(L, -3); // adds "Object" to the child table
}

void Object::addBindings(lua_State *L) {
    registerClass(L, registerLuaSetters, registerLuaGetters, registerLuaMethods, registerLuaChild, "Object");
}