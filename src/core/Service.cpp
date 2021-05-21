#include "core/Service.hpp"

Service::Service(Object *p) : Object(p) {
    
}

SRVICETYPE Service::getServiceType() {
    return srvType;
}