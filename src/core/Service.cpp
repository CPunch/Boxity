#include "core/Service.hpp"

Service::Service(ObjectPtr r): root(r) {
    
}

// ==================================== [[ GETTERS ]] ====================================

SRVICETYPE Service::getServiceType() {
    return srvType;
}

void Service::tick() {}