#pragma once

#include <string>

namespace Core
{
	struct ShaderDefaultUnlitTexture
	{
			static std::string vertex;
			static std::string fragment;
	};

	std::string ShaderDefaultUnlitTexture::vertex = R"(
		#version 400
		layout (location = 0) in vec3 position;
		layout (location = 1) in vec2 uv0;
		layout (location = 2) in vec4 color0;
		out vec2 f_uv0;
		out vec4 f_color0;
		uniform mat4 u_viewMtx;
		uniform mat4 u_projMtx;
		uniform mat4 u_modelMtx;
		void main() {
			f_uv0 = uv0;
			f_color0 = color0;
			gl_Position = u_projMtx * u_viewMtx * u_modelMtx * vec4(position, 1.0);
		}
		)";

	std::string ShaderDefaultUnlitTexture::fragment = R"(
		#version 400
		in vec2 f_uv0;
		in vec4 f_color0;
		out vec4 frag_color;
		uniform sampler2D u_diffuseTex;
		void main() {
			vec4 out_color = texture2D(u_diffuseTex, f_uv0);
			frag_color = vec4(out_color.rgb * f_color0.rgb, 1.0);
		}
		)";
} // namespace Core