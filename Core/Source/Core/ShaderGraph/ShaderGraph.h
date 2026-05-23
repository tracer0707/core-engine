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

			ShaderNode* _vertexOutNode;
			ShaderNode* _fragmentOutNode;

		public:
			ShaderGraph(Renderer* renderer);
			~ShaderGraph();

			virtual int getContentType();

			const ShaderNode* getVertexOutNode() const { return _vertexOutNode; }
			const ShaderNode* getFragmentOutNode() const { return _fragmentOutNode; }

			void compile();
			void bind();
	};
} // namespace Core