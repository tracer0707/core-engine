#pragma once

namespace Core
{
	namespace Shaders
	{
		class UnlitColor
		{
			public:
				static const char* getVertexSource();
				static const char* getFragmentSource();
		};

		const char* Core::Shaders::UnlitColor::getVertexSource()
		{
			return R"(
                #version 400 core

                layout (location = 0) in vec3 position;
                layout (location = 6) in vec4 color0;
                
                out vec4 f_color0;

                uniform mat4 _u_viewMtx;
                uniform mat4 _u_projMtx;
                uniform mat4 _u_modelMtx;

                void main() {
                    f_color0 = color0;
                    gl_Position = _u_projMtx * _u_viewMtx * _u_modelMtx * vec4(position, 1.0);
                }
            )";
		}

		const char* Core::Shaders::UnlitColor::getFragmentSource()
		{
			return R"(
                #version 400 core

		        in vec4 f_color0;

                uniform vec4 u_color;
		        
                out vec4 frag_color;

		        void main() {
                    frag_color = f_color0 * u_color;
                }
            )";
		}
	} // namespace Shaders
} // namespace Core