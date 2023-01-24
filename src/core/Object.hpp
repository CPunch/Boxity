#pragma once

#include "lua.hpp"
#include "pugixml.hpp"

#include <string>
#include <unordered_set>
#include <map>
#include <inttypes.h>
#include <memory>

enum OBJTYPE {
    RENDEROBJ  = 1,
    ENTITYOBJ  = 2
};

enum OBJCLASS {
    OBJ_ROOT,
    OBJ_BALL,
    OBJ_BOX,
    OBJ_SCRIPT,
    OBJ_INCOMPMAX // incomplete class type
};

class Object;
typedef std::shared_ptr<Object> ObjectPtr;
typedef uint8_t iOBJTYPE;

#define isRenderable(x) (x->getTypeFlags() & RENDEROBJ)
#define isPhysical(x)   (x->getTypeFlags() & ENTITYOBJ)
#define addFlag(x,flag) x |= flag
#define castObjPtr(x, type) ((type*)(x).get())

class Root;
class Object : public std::enable_shared_from_this<Object> {
protected:
    std::unordered_set<ObjectPtr> children;
    std::string name;
    std::string className;
    ObjectPtr parent = nullptr;
    iOBJTYPE typeFlags = 0;
    OBJCLASS classType = OBJ_INCOMPMAX;

    void addChild(ObjectPtr);
    void removeChild(ObjectPtr);

    void pushRawLua(lua_State *L, const char *libname);

    // events
    virtual void onParentRemove(); // called before child is removed from parent
    virtual void onParentAdd(); // called after child is added to parent

public:
    Object();
    ~Object();

    // setters
    void setName(std::string);
    void setParent(ObjectPtr);

    // getters
    std::string getName();
    std::string getClassName();
    ObjectPtr getParent();
    std::unordered_set<ObjectPtr> getChildren();
    ObjectPtr findChild(std::string name);
    iOBJTYPE getTypeFlags();
    OBJCLASS getClass();

    void remove();
    virtual void tick(uint64_t);
    virtual void serialize(pugi::xml_node &node);
    virtual void deserialize(pugi::xml_node &node);

    // lua stuff
    static void addBindings(lua_State*);
    virtual void pushLua(lua_State*);
    static ObjectPtr* grabLua(lua_State *L, int indx, const char *classname);

    static void registerLuaGetters(lua_State*);
    static void registerLuaSetters(lua_State*);
    static void registerLuaMethods(lua_State*);
    static void registerLuaChild(lua_State*);
    static void registerClass(lua_State* L, registerLuaTable settertbl, registerLuaTable gettertbl, registerLuaTable methodtbl, registerLuaTable childTbl, const char *name);
};