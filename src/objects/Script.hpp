#pragma once

#include "core/Object.hpp"
#include "services/ScriptService.hpp"

class Script : public Object {
private:
    lua_State *state = nullptr;
    ScriptService *scrSrvc = nullptr;
    std::string source;

    // custom events
    void onParentRemove();
    void onParentAdd();

    static ObjectPtr createScript();

public:
    Script();

    void setSource(std::string script);
    virtual void serialize(pugi::xml_node &node);
    virtual void deserialize(pugi::xml_node &node);

    static void addBindings(lua_State*);
};