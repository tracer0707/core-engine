#pragma once

#include "../Renderer/Program.h"
#include "../Shared/String.h"

#include "Content.h"

namespace Core
{
    class Renderer;

    class Shader : public Content
    {
        friend class ContentManager;

      private:
        Shader(Renderer* renderer, String vertexSrc, String fragmentSrc);
        ~Shader();

        Renderer* _renderer = nullptr;
        Program* _nativeId = nullptr;

      public:
        void bind();
    };
} // namespace Core