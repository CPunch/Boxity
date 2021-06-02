#include "core/Root.hpp"

#include "services/RenderService.hpp"
#include "objects/Box.hpp"
#include "objects/Ball.hpp"
#include "objects/Script.hpp"

#include <iostream>

int main() {
    std::shared_ptr<Root> root = std::make_shared<Root>();
    root->init();

    RenderService *rSrvc = (RenderService*)root->getService(RENDERSRV);

    std::shared_ptr<Box> ground = std::make_shared<Box>(Vec2(400, 50), Vec2(200, 400));
    ground->setAnchored(true);
    ground->setParent(root);

    std::shared_ptr<Ball> b = std::make_shared<Ball>(10.0f, Vec2(200, 100));
    b->setAnchored(false);
    b->setParent(root);

    std::shared_ptr<Script> scrpt = std::make_shared<Script>();
    scrpt->setParent(root);
    scrpt->run("print(root.Box.position.x)");

    while (rSrvc->isOpen()) {
        rSrvc->pollEvents();

        // tick
        root->tick();

        // render everything
        rSrvc->render();
    }

    root->remove();
    std::cout << "root removed!" << std::endl;
    return 0;
}
