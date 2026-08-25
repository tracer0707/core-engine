#include "Image.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <Core/Content/Texture2D.h>

namespace Editor
{
	Image::Image() : Control() {}

	Image::Image(int width, int height) : Control()
	{
		_width = (float)width;
		_height = (float)height;
	}

	Image::~Image() {}

	void Image::measure() const
	{
		_actualWidth = _width == 0 ? ImGui::GetContentRegionAvail().x : _width;
		_actualHeight = _height == 0 ? ImGui::GetContentRegionAvail().y : _height;
	}

	void Image::update()
	{
		if (!_visible) return;

		unsigned int texId = nativeTextureId;
		if (texture != nullptr) texId = texture->getNativeId();

		ImGui::Image((ImTextureID)(intptr_t)texId, ImVec2(getWidth(), getHeight()), ImVec2(0, 1), ImVec2(1, 0));

		updateDragDropSource();
		updateDragDropTarget();
	}
} // namespace Editor