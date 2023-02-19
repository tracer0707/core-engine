#include "RendererGL4.h"

#include <GL/glew.h>

namespace Core
{
    static RendererGL4 singletonInstance;

    const unsigned int RendererGL4::createProgram(UString vertexSrc, UString fragmentSrc)
    {
        std::string vertexSrcUtf8 = String::toStdString(vertexSrc);
        std::string fragmentSrcUtf8 = String::toStdString(fragmentSrc);

        const char* _vertexSrcUtf8 = vertexSrcUtf8.c_str();
        const char* _fragmentSrcUtf8 = fragmentSrcUtf8.c_str();

        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &_vertexSrcUtf8, NULL);
        glCompileShader(vs);

        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &_fragmentSrcUtf8, NULL);
        glCompileShader(fs);

        GLuint programId = glCreateProgram();
        glAttachShader(programId, fs);
        glAttachShader(programId, vs);
        glLinkProgram(programId);

        shaderPrograms.push_back({ programId, vs, fs, 0, 0 });

        return programId;
    }

    const void RendererGL4::deleteProgram(unsigned int programId)
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
        else
        {
            throw std::invalid_argument("Попытка удалить несуществующую программу");
        }
    }

    const void RendererGL4::bindProgram(unsigned int programId)
    {
        glUseProgram(programId);
    }

    RendererGL4* RendererGL4::singleton()
    {
        return &singletonInstance;
    }
}