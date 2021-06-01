#include "types/Type.hpp"

#define LIBNAME "Type"

Type::Type() {
    classname = "Type";
}

std::string Type::getTypeName() {
    return classname;
}

bool Type::getUpdated() {
    return updated;
}

void Type::setUpdated(bool u) {
    updated = u;
}

// ==================================== [[ LUA ]] ====================================

void Type::pushLua(lua_State *L) {
    // create a shared_ptr userdata on the lua heap
    void *tPtr = lua_newuserdata(L, sizeof(TypePtr));

    // handle error
    if (tPtr == NULL)
        return;

    // use the new operator to increase our reference count of this shared_ptr
    new(tPtr) TypePtr(shared_from_this());

    // set the udata's metatable
    luaL_getmetatable(L, LIBNAME);
    lua_setmetatable(L, -2);
}

TypePtr* Type::grabLua(lua_State* L, int indx, const char *classname) {
    void *tPtr = luaL_checkudata(L, indx, LIBNAME);
    if (tPtr == NULL)
        return nullptr;

    // check if the classname is in the __typeclass table
    lua_getmetatable(L, 1);
    lua_getfield(L, -1, "__typeclass");
    lua_getfield(L, -1, classname);
    if (lua_isnil(L, -1)) { // the classname wasn't in __typeclass!
        lua_pop(L, 3); // pop nil, __typeclass & metatable

        return nullptr;
    }

    lua_pop(L, 3);
    return (TypePtr*)tPtr;
}

static int typeIndex(lua_State *L) {
    TypePtr *tPtr = Type::grabLua(L, 1, "Type");
    // sanity check
    if (tPtr == nullptr)
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

static int typeNewIndex(lua_State *L) {
    TypePtr *tPtr = Type::grabLua(L, 1, "Type");
    // sanity check
    if (tPtr == nullptr)
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
static int typeGC(lua_State *L) {
    TypePtr *tPtr = Type::grabLua(L, 1, "Type");

    // reset the shared_ptr, removing this reference
    if (tPtr != nullptr)
        tPtr->reset();

    return 0;
}

// ==================================== [[ LUA GETTERS ]] ====================================

static int typeGetTypeName(lua_State *L) {
    TypePtr *tPtr = Type::grabLua(L, 1, LIBNAME);

    if (tPtr != nullptr) {
        lua_pushstring(L, (*tPtr)->getTypeName().c_str());
        return 1;
    }

    return 0;
}

void Type::registerLuaGetters(lua_State *L) {
    static const luaL_Reg getters[] {
        {"typename", typeGetTypeName},
        {NULL, NULL}
    };

    luaL_setfuncs(L, getters, 0);
}

// ==================================== [[ LUA SETTERS ]] ====================================

void Type::registerLuaSetters(lua_State *L) {
    static const luaL_Reg setters[] {
        {NULL, NULL}
    };

    luaL_setfuncs(L, setters, 0);
}

// ==================================== [[ LUA METHODS ]] ====================================

void Type::registerLuaMethods(lua_State *L) {
    static const luaL_Reg methods[] {
        {NULL, NULL}
    };

    luaL_setfuncs(L, methods, 0);
}

void Type::registerClass(lua_State* L, registerLuaTable setterTbl, registerLuaTable getterTbl, registerLuaTable methodTbl, const char *name) {
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

    // set methods
    lua_pushstring(L, "__methods");
    lua_newtable(L);
    methodTbl(L);
    lua_rawset(L, -3); // meta.__methods = methods

    // set __index
    lua_pushstring(L, "__index");
    lua_pushcfunction(L, typeIndex);
    lua_rawset(L, -3); // meta.__index = objIndex

    // set __newindex
    lua_pushstring(L, "__newindex");
    lua_pushcfunction(L, typeNewIndex);
    lua_rawset(L, -3); // meta.__newindex = objNewIndex

    // set __typeclass
    lua_pushstring(L, "__typeclass");
    lua_newtable(L);
    lua_pushstring(L, LIBNAME);
    lua_pushboolean(L, true);
    lua_rawset(L, -3); // __typeclass.Type = true
    lua_pushstring(L, name);
    lua_pushboolean(L, true);
    lua_rawset(L, -3); // __typeclass.${name} = true
    lua_rawset(L, -3); // meta.__typeclass = tbl

    // set __gc
    lua_pushstring(L, "__gc");
    lua_pushcfunction(L, typeGC);
    lua_rawset(L, -3); // meta.__gc = objGC

    lua_pop(L, 1);
}

void Type::addBindings(lua_State *L) {
    registerClass(L, registerLuaSetters, registerLuaGetters, registerLuaMethods, LIBNAME);
}