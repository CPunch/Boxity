#include "core/Service.hpp"

Service::Service(): Object() {
    
}

// ==================================== [[ GETTERS ]] ====================================

SRVICETYPE Service::getServiceType() {
    return srvType;
}