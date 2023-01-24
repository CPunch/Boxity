#pragma once

#include "core/Singleton.hpp"

class Service {
public:
    Service();
    ~Service();

    virtual void onFrame();
};