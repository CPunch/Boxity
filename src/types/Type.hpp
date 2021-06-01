#pragma once

#include <memory>
#include <string>
#include "lua.hpp"

#define castTypePtr(x, type) ((type*)(x).get())

class Type;
typedef std::shared_ptr<Type> TypePtr;

class Type : public std::enable_shared_from_this<Type> {
protected:
    std::string classname;
    bool updated;

    static void pushRawLua(lua_State *L, TypePtr tPtr, const char *meta);

public:
    Type();

    std::string getTypeName();
    bool getUpdated();

    void setUpdated(bool);

    static void addBindings(lua_State*);
    virtual void pushLua(lua_State *L);
    static TypePtr* grabLua(lua_State* L, int indx, const char *classname);

    static void registerLuaGetters(lua_State*);
    static void registerLuaSetters(lua_State*);
    static void registerLuaMethods(lua_State*);
    static void registerClass(lua_State* L, registerLuaTable settertbl, registerLuaTable gettertbl, registerLuaTable methodtbl, const char *name);
};