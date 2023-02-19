#include "Shader.h"

#include "../config.h"

Shader::~Shader()
{
    if (isLoaded())
        unload();
}

const void Shader::load()
{
    Asset::load();
}

const void Shader::unload()
{
    CURRENT_RENDERER->deleteProgram(nativeId);

    Asset::unload();
}

const void Shader::loadFromString(UString vertexSrc, UString fragmentSrc)
{
    nativeId = CURRENT_RENDERER->createProgram(vertexSrc, fragmentSrc);

    load();
}

const void Shader::bind()
{
    CURRENT_RENDERER->bindProgram(nativeId);
}

const void Shader::unbind()
{
    CURRENT_RENDERER->bindProgram(0);
}
