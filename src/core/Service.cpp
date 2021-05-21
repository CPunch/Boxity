#include "core/Service.hpp"

Service::Service(Object *p) : Object(p) {
    
}

// ==================================== [[ GETTERS ]] ====================================

SRVICETYPE Service::getServiceType() {
    return srvType;
}