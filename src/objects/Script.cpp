#include "core/Root.hpp"
#include "services/ScriptService.hpp"
#include "objects/Script.hpp"

#define LIBNAME "Script"

Script::Script() {
    name = "Script";
    className = LIBNAME;
    classType = OBJ_SCRIPT;
}

void Script::onParentAdd() {
    if (root == nullptr || state != nullptr)  // if we already have a state, ignore!
        goto _passSOPAEvnt;

    scrSrvc = (ScriptService*)castObjPtr(root, Root)->getService(SCRIPTSRV);
    state = scrSrvc->newThread();

_passSOPAEvnt:
    Object::onParentAdd();
}

void Script::onParentRemove() {
    // stubbed for now
    Object::onParentRemove();
}

// temp
void Script::setSource(std::string script) {
    if (state == nullptr || scrSrvc == nullptr)
        return;

    source = script;
    luaL_loadstring(state, script.c_str());
    scrSrvc->yieldCall(state, 0);
}

void Script::serialize(pugi::xml_node &node) {
    node.prepend_attribute("source").set_value(source.c_str());

    Object::serialize(node);
}