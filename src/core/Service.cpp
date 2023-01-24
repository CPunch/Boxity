#include "core/Root.hpp"
#include "core/Service.hpp"

Service::Service() {
    Root::getSingleton().registerService(this);
}

Service::~Service() {
    Root::getSingleton().unregisterService(this);
}

// ==================================== [[ EVENTS ]] ====================================

// all of these are stubbed, inherited classes should inherit the virtual functions

void Service::onFrame() {}