#pragma once

#include "core/Object.hpp"

enum SRVICETYPE {
    RENDERSRV,
    PHYSICSRV,
    MAXSRV
};

class Service : public Object {
protected:
    SRVICETYPE srvType;

public:
    Service();

    SRVICETYPE getServiceType();
};