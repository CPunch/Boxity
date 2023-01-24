#include "core/Root.hpp"
#include "core/Object.hpp"
#include "core/Entity.hpp"

#include "services/PhysicsService.hpp"
#include "services/RenderService.hpp"
#include "services/TaskService.hpp"
#include "services/ScriptService.hpp"

template <> Root *Singleton<Root>::singleton = nullptr;

Root::Root(): Object() {
    name = "Root";
    classType = OBJ_ROOT;

    // set our object feature flags
    addFlag(typeFlags, RENDEROBJ);
    addFlag(typeFlags, ENTITYOBJ);
}

Root::~Root() {
    // free all of our loaded services
    for (Service *srvc : services) {
        delete srvc;
    }
}

void Root::tick_task(void *ud, uint64_t currTime, uint64_t realDelta) {
    // if we have a physics service loaded, we need to simulate the physics before calling tick()
    if (PhysicsService::isAlive())
        PhysicsService::getSingleton().pTick(realDelta);

    // tick our children
    for (ObjectPtr child : Root::getSingleton().children) {
        child->tick(currTime);
    }
}

void Root::preFrame() {
    // tick all our services
    for (Service *srvc : services)
        srvc->onFrame();
}

void Root::init() {
    // schedule our tick timer
    if (!TaskService::isAlive()) {
        new TaskService();
        TaskService::getSingleton().pushTask(Root::tick_task, nullptr, ROOTTICKTMER, true);
    }

    if (!RenderService::isAlive()) {
        new RenderService();
    }

    if (!PhysicsService::isAlive()) {
        new PhysicsService();
    }

    if (!ScriptService::isAlive()) {
        new ScriptService();
    }
}

void Root::serializeDoc(pugi::xml_document &doc) {
    pugi::xml_node rootNode = doc.append_child("root");
    Object::serialize(rootNode);
}

void Root::deserializeDoc(pugi::xml_document &doc) {
    pugi::xml_node rootNode = doc.first_child();
    Object::deserialize(rootNode);
}

// ==================================== [[ GETTERS ]] ====================================

void Root::registerService(Service *srvc) {
    services.insert(srvc);
}

void Root::unregisterService(Service *srvc) {
    services.erase(srvc);
}

ObjectPtr Root::getRoot() {
    return shared_from_this();
}