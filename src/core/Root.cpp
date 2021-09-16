#include "core/Root.hpp"
#include "core/Object.hpp"
#include "core/Entity.hpp"

#include "services/PhysicsService.hpp"
#include "services/RenderService.hpp"
#include "services/TaskService.hpp"
#include "services/ScriptService.hpp"

Root::Root(): Object() {
    name = "Root";
    classType = OBJ_ROOT;

    // set our object feature flags
    addFlag(typeFlags, RENDEROBJ);
    addFlag(typeFlags, ENTITYOBJ);

    // initalize the loaded services table
    for (int i = 0; i < MAXSRV; i++)
        loadedServices[i] = nullptr;
}

Root::~Root() {
    // free all of our loaded services
    for (int i = 0; i < MAXSRV; i++) {
        if (loadedServices[i] == nullptr)
            continue;

        delete loadedServices[i];
    }
}

void Root::tick_task(void* ud, uint64_t currTime, uint64_t realDelta) {
    Root *root = (Root*)ud;
    PhysicsService *pSrvc = (PhysicsService*)root->loadedServices[PHYSICSRV];

    // if we have a physics service loaded, we need to simulate the physics before calling tick()
    if (pSrvc != nullptr)
        pSrvc->pTick(realDelta);

    // tick our children
    for (ObjectPtr child : root->children) {
        child->tick(currTime);
    }
}

void Root::tick() {
    // tick all our services
    for (int i = 0; i < MAXSRV; i++)
        if (loadedServices[i] != nullptr)
            loadedServices[i]->tick();
}

void Root::init() {
    // schedule our tick timer
    ((TaskService*)getService(TASKSRV))->pushTask(Root::tick_task, (void*)this, ROOTTICKTMER, true);
}

void Root::serializeDoc(pugi::xml_document &doc) {
    pugi::xml_node rootNode = doc.append_child("root");
    Object::serialize(rootNode);
}

// ==================================== [[ GETTERS ]] ====================================

ObjectPtr Root::getRoot() {
    return shared_from_this();
}

Service* Root::getService(SRVICETYPE srvc) {
    if (loadedServices[srvc] != nullptr)
        return loadedServices[srvc];

    // service isn't loaded, load it
    Service *newSrvc;
    switch(srvc) {
        case RENDERSRV: // stubbed
            newSrvc = new RenderService(shared_from_this());
            break;
        case PHYSICSRV:
            newSrvc = new PhysicsService(shared_from_this());
            break;
        case TASKSRV:
            newSrvc = new TaskService(shared_from_this());
            break;
        case SCRIPTSRV:
            newSrvc = new ScriptService(shared_from_this());
            break;
        default:
            return nullptr;
    }

    loadedServices[srvc] = newSrvc;
    return newSrvc;
}