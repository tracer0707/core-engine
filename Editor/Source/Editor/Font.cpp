#include "Font.h"

#include <imgui.h>
#include <imgui_impl_opengl3.h>

namespace Editor
{
	Font::Font(const fs::path& path, float size)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImFontGlyphRangesBuilder ranges;
		ranges.AddRanges(io.Fonts->GetGlyphRangesDefault());
		ranges.AddRanges(io.Fonts->GetGlyphRangesCyrillic());
		ranges.AddRanges(io.Fonts->GetGlyphRangesGreek());
		ranges.AddRanges(io.Fonts->GetGlyphRangesVietnamese());
		ranges.AddRanges(io.Fonts->GetGlyphRangesThai());
		ranges.AddRanges(io.Fonts->GetGlyphRangesJapanese());
		ranges.AddRanges(io.Fonts->GetGlyphRangesChineseFull());
		ranges.AddRanges(io.Fonts->GetGlyphRangesKorean());
		ImVector<ImWchar> glyphRanges;
		ranges.BuildRanges(&glyphRanges);

		const Core::String utf8Path = Core::Path::toUtf8(path);
		_font = io.Fonts->AddFontFromFileTTF(utf8Path.c_str(), size, nullptr, glyphRanges.Data);
	}

	Font::~Font()
	{
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
}