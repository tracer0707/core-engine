#pragma once

namespace Core
{
	namespace Shaders
	{
		class Default
		{
			public:
				static const char* getVertexSource();
				static const char* getFragmentSource();
		};

		const char* Core::Shaders::Default::getVertexSource()
		{
			return R"(
                #version 400 core

                layout (location = 0) in vec3 position;

                uniform mat4 u_viewMtx;
                uniform mat4 u_projMtx;
                uniform mat4 u_modelMtx;

                void main() {
                    gl_Position = u_projMtx * u_viewMtx * u_modelMtx * vec4(position, 1.0);
                }
            )";
		}

		const char* Core::Shaders::Default::getFragmentSource()
		{
			return R"(
                #version 400 core

                out vec4 frag_color;

		        void main() {
                    frag_color = vec4(1.0, 0.0, 1.0, 1.0);
                }
            )";
		}
	} // namespace Shaders
} // namespace Core