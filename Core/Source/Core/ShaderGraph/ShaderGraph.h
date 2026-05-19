#pragma once

#include "../Renderer/Program.h"
#include "../Content/Content.h"

namespace Core
{
	class Renderer;
	class ShaderNode;

	class ShaderGraph : public Content
	{
		private:
			Renderer* _renderer = nullptr;
			Program* _nativeId = nullptr;

			ShaderNode* _vertexNode;
			ShaderNode* _fragmentNode;

		public:
			ShaderGraph(Renderer* renderer);
			~ShaderGraph();

			virtual int getContentType();

			const ShaderNode* getVertexNode() const { return _vertexNode; }
			const ShaderNode* getFragmentNode() const { return _fragmentNode; }

			void compile();
			void bind();
	};
} // namespace Core