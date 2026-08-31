#include "Image.h"

#include <imgui.h>
#include <imgui_internal.h>

#include "../../Resources/Texture.h"

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
		if (_texture != nullptr)
		{
			_actualWidth = _width == 0 ? _texture->getWidth() : _width;
			_actualHeight = _height == 0 ? _texture->getHeight() : _height;
		}
		else
		{
			_actualWidth = _width;
			_actualHeight = _height;
		}
	}

	void Image::update()
	{
		if (!_visible) return;

		unsigned int texId = _nativeTextureId;
		if (_texture != nullptr) texId = _texture->getNativeId();

		ImGui::Image((ImTextureID)(intptr_t)texId, ImVec2(getWidth(), getHeight()), ImVec2(0, 1), ImVec2(1, 0));

		updateDragDropSource();
		updateDragDropTarget();
	}
} // namespace Editor