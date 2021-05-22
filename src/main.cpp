#include "core/Root.hpp"

#include "services/RenderService.hpp"
#include "objects/Box.hpp"

#include <vector>

int main() {
    std::shared_ptr<Root> root = std::make_shared<Root>();
    RenderService *rSrvc = (RenderService*)root->getService(RENDERSRV);

    std::shared_ptr<Box> ground = std::make_shared<Box>(Vec2(400, 50), Vec2(200, 400));
    ground->setAnchored(true);
    ground->setParent(root);

    while (rSrvc->isOpen()) {
        rSrvc->pollEvents();

        // tick
        root->tick();

        // render everything
        rSrvc->render();
    }

    return 0;
}
