#include "SceneWindow.h"

#include "../Controls/Image.h"
#include "../Controls/Button.h"

namespace Editor
{
	SceneWindow::SceneWindow(UInt32 renderTextureId) : Window("Scene")
	{
		this->renderTextureId = renderTextureId;

		_style.paddingX = 0;
		_style.paddingY = 0;

		image = new Image();
		image->setNativeTextureId(renderTextureId);

		addControl(image);
	}

	SceneWindow::~SceneWindow()
	{
	}
}