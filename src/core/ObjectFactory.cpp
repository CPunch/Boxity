#include "core/ObjectFactory.hpp"

#include "objects/Box.hpp"
#include "objects/Ball.hpp"
#include "objects/Script.hpp"

struct Factory {
    FactoryMethod construct;
    std::string name;

    Factory(): construct(nullptr), name("") {}
    Factory(FactoryMethod meth, std::string n): construct(meth), name(n) {}
};

Factory factories[OBJ_INCOMPMAX] {};

ObjectPtr ObjectFactory::create(OBJCLASS obj) {
    Factory fact = factories[obj];

    // fact isn't setup/can't be constructed, so return a nullptr
    if (fact.construct == nullptr)
        return ObjectPtr(nullptr);

    return fact.construct();
}

std::string ObjectFactory::getName(OBJCLASS obj) {
    Factory fact = factories[obj];

    // fact isn't setup, so return generic "Object" string
    if (fact.construct == nullptr)
        return "Object";

    return fact.name;
}

OBJCLASS ObjectFactory::getClass(std::string name) {
    // find the OBJCLASS
    for (int i = 0; i < OBJ_INCOMPMAX; i++) {
        if (factories[i].name.compare(name) == 0)
            return (OBJCLASS)i;
    }

    // wasn't found, return incomp type
    return OBJ_INCOMPMAX;
}

void ObjectFactory::addObjClass(OBJCLASS obj, std::string name, FactoryMethod meth) {
    factories[obj] = Factory(meth, name);
}

// ==================================== [[ LUA ]] ====================================

static int cfNew(lua_State *L) {
    ObjectPtr obj;
    const char *name = luaL_checkstring(L, 1);

    OBJCLASS objClass = ObjectFactory::getClass(std::string(name));
    // check if object class exists, then try to create one. if either fail, throw an error
    if (objClass == OBJ_INCOMPMAX || (obj = ObjectFactory::create(objClass)).get() == nullptr)
        return luaL_error(L, "Object type '%s' is not creatable!", name);

    // wrap it onto the lua state & return it
    obj->pushLua(L);
    return 1;
}

static const luaL_Reg libMethods[] = {
    {"new", cfNew},
    {NULL, NULL}
};

void ObjectFactory::addBindings(lua_State *L) {
    luaL_newlib(L, libMethods);
    lua_setglobal(L, "Factory");
}