#include "Image.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <Core/Content/Texture.h>

#include "ControlList.h"

namespace Editor
{
    Image::Image() : Control() {}

    Image::Image(int width, int height) : Control()
    {
        _width = width;
        _height = height;
    }

    Image::~Image() {}

    void Image::update()
    {
        if (!_visible) return;

        float w = _width;
        float h = _height;

        if (w == 0) w = ImGui::GetContentRegionAvail().x;
        if (h == 0) h = ImGui::GetContentRegionAvail().y;

        unsigned int texId = nativeTextureId;
        if (texture != nullptr) texId = texture->getNativeId();

        ImGui::Image((ImTextureID)texId, ImVec2(w, h), ImVec2(0, 1), ImVec2(1, 0));
    }

    int Image::getControlType()
    {
        return CONTROL_IMAGE;
    }
} // namespace Editor