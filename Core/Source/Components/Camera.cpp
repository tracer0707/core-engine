#include "Camera.h"

#include "../Renderer/Renderer.h"

namespace Core
{
    const void Camera::frame(Renderer* renderer)
    {
        renderer->frame(this);
    }
}
