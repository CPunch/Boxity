#include "core/Root.hpp"
#include "services/ScriptService.hpp"
#include "objects/Script.hpp"

Script::Script() {
    name = "Script";
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
void Script::run(std::string script) {
    if (state == nullptr || scrSrvc == nullptr)
        return;

    luaL_loadstring(state, script.c_str());
    scrSrvc->yieldCall(state, 0);
}