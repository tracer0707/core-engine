#include "RendererGL4.h"

#include <GL/glew.h>

const unsigned int Core::RendererGL4::createProgram(const char* vertexSrc, const char* fragmentSrc)
{
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexSrc, NULL);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentSrc, NULL);
    glCompileShader(fs);

    GLuint programId = glCreateProgram();
    glAttachShader(programId, fs);
    glAttachShader(programId, vs);
    glLinkProgram(programId);

    shaderPrograms.push_back({ programId, vs, fs, 0, 0 });

	return programId;
}

const void Core::RendererGL4::deleteProgram(unsigned int programId)
{
    const auto& it = std::find_if(
        shaderPrograms.begin(),
        shaderPrograms.end(),
        [=](Program& p) -> bool
        {
            return p.program == programId;
        }
    );

    if (it != shaderPrograms.end())
    {
        if (it->vertexShader > 0)
            glDeleteShader(it->vertexShader);

        if (it->fragmentShader > 0)
            glDeleteShader(it->fragmentShader);

        if (it->geometryShader > 0)
            glDeleteShader(it->geometryShader);

        if (it->computeShader > 0)
            glDeleteShader(it->computeShader);

        shaderPrograms.erase(it);
    }
}