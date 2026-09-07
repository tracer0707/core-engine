#include "Font.h"

#include <imgui.h>
#include <imgui_impl_opengl3.h>

namespace Editor
{
	Font::Font(const fs::path& path, float size)
	{
		_glyphRanges = new ImVector<ImWchar>();

		ImGuiIO& io = ImGui::GetIO();
		ImFontGlyphRangesBuilder ranges;
		ranges.AddRanges(io.Fonts->GetGlyphRangesDefault());
		ranges.AddRanges(io.Fonts->GetGlyphRangesCyrillic());
		ranges.BuildRanges(_glyphRanges);

		const Core::String utf8Path = Core::Path::toUtf8(path);
		_font = io.Fonts->AddFontFromFileTTF(utf8Path.c_str(), size, nullptr, _glyphRanges->Data);
	}

	Font::~Font()
	{
		delete _glyphRanges;
		_glyphRanges = nullptr;
		_font = nullptr;
	}

	void Font::rebuildFonts()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->Build();

		ImGui_ImplOpenGL3_DestroyFontsTexture();
		ImGui_ImplOpenGL3_CreateFontsTexture();
	}

	void Font::setDefault()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.FontDefault = _font;
	}
} // namespace Editor