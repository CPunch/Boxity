#pragma once

#include <string>
#include "lua.hpp"
#include "core/Object.hpp"

typedef ObjectPtr (*FactoryMethod)();

namespace ObjectFactory {
    ObjectPtr create(OBJCLASS);

    std::string getName(OBJCLASS);
    OBJCLASS getClass(std::string);

    void addObjClass(OBJCLASS, std::string, FactoryMethod);
    void addBindings(lua_State*);
}