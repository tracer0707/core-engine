#pragma once

#include "../Renderer/Program.h"
#include "../Shared/String.h"

#include "Asset.h"

namespace Core
{
    class Renderer;

    class Shader : public Asset
    {
        friend class AssetManager;

      private:
        Shader(Renderer* renderer, String vertexSrc, String fragmentSrc);
        ~Shader();

        Renderer* _renderer = nullptr;
        Program* _nativeId = nullptr;

      public:
        void bind();
    };
} // namespace Core