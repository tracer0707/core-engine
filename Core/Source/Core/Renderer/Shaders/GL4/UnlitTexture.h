#pragma once

namespace Core
{
	namespace Shaders
	{
		class UnlitTexture
		{
			public:
				static const char* getVertexSource();
				static const char* getFragmentSource();
		};

		const char* Core::Shaders::UnlitTexture::getVertexSource()
		{
			return R"(
                #version 400 core

                layout (location = 0) in vec3 position;
                layout (location = 1) in vec2 uv0;
                layout (location = 2) in vec4 color0;
                
                out vec2 f_uv0;
                out vec4 f_color0;

                uniform mat4 _u_viewMtx;
                uniform mat4 _u_projMtx;
                uniform mat4 _u_modelMtx;

                void main() {
                    f_uv0 = uv0;
                    f_color0 = color0;
                    gl_Position = _u_projMtx * _u_viewMtx * _u_modelMtx * vec4(position, 1.0);
                }
            )";
		}

		const char* Core::Shaders::UnlitTexture::getFragmentSource()
		{
			return R"(
                #version 400 core

                in vec2 f_uv0;
		        in vec4 f_color0;
		        
                out vec4 frag_color;

                uniform vec4 u_color;
                uniform sampler2D u_texture;

		        void main() {
                    frag_color = f_color0 * u_color * texture(u_texture, f_uv0);
                }
            )";
		}
	} // namespace Shaders
} // namespace Core