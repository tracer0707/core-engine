#pragma once

#include "../Renderer/Program.h"
#include "../Content/Content.h"

namespace Core
{
	class Renderer;
	class ShaderNode;

	class Shader : public Content
	{
		private:
			Renderer* _renderer = nullptr;
			Program* _nativeId = nullptr;

			ShaderNode* _vertexNode;
			ShaderNode* _fragmentNode;

		public:
			Shader(Renderer* renderer);
			~Shader();

			virtual int getContentType();

			const ShaderNode* getVertexNode() const { return _vertexNode; }
			const ShaderNode* getFragmentNode() const { return _fragmentNode; }

			void compile();
			void bind();
	};
} // namespace Core