#include "Font.h"

#include <imgui.h>
#include <imgui_impl_opengl3.h>

namespace Editor
{
	Font::Font(Core::String path, float size)
	{
		ImGuiIO& io = ImGui::GetIO();
		_font = io.Fonts->AddFontFromFileTTF(path.std_str().c_str(), size, nullptr, io.Fonts->GetGlyphRangesCyrillic());
		io.Fonts->Build();

		ImGui_ImplOpenGL3_DestroyFontsTexture();
		ImGui_ImplOpenGL3_CreateFontsTexture();
	}

	Font::~Font()
	{
		_font = nullptr;
	}

	void Font::setDefault()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.FontDefault = _font;
	}
}