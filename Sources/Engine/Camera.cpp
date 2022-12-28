#include "Camera.h"

Kore::mat3 Engine::Camera::getTransform() {
    return Kore::mat3::Translation(Kore::System::windowWidth() / 2 / zoom - pos.x(), Kore::System::windowHeight() / 2 / zoom - pos.y()) * Kore::mat3::Scale(zoom);
}

Kore::vec2 Engine::Camera::getWorldPos(Kore::vec2 screenPos) {
    return Kore::vec2{
        (screenPos.x() - Kore::System::windowWidth() / 2) / zoom + pos.x(),
        (screenPos.y() - Kore::System::windowHeight() / 2) / zoom + pos.y()};
}