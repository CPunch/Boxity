#pragma once

#include "core/Object.hpp"

enum SRVICETYPE {
    RENDERSRV,
    PHYSICSRV,
    TASKSRV,
    MAXSRV
};

class Root;
class Service {
protected:
    SRVICETYPE srvType;
    ObjectPtr root;

public:
    Service(ObjectPtr root);

    SRVICETYPE getServiceType();
    virtual void tick();
};