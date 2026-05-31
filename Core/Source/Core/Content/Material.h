#pragma once

#include "Content.h"

namespace Core
{
	class Renderer;
	class Program;

	class Material : public Content
	{
			friend class Renderer;
			friend class ContentManager;

		private:
			Renderer* _renderer = nullptr;
			Program* _program = nullptr;

			Material(Renderer* renderer);
			virtual ~Material();

		public:
			virtual int getContentType();

			const Program* getProgram() const { return _program; }
			void setProgram(Program* value) { _program = value; }

			void bind();
	};
} // namespace Core