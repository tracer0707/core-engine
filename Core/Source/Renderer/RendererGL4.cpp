#include "RendererGL4.h"

#include <GL/glew.h>
#include <cassert>

namespace Core
{
    const Program RendererGL4::createProgram(UString vertexSrc, UString fragmentSrc)
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

        Program program = { programId, vs, fs, 0, 0 };
        shaderPrograms.push_back(program);

        return program;
    }

    const void RendererGL4::deleteProgram(const Program& program)
    {
        const auto& it = std::find_if(
            shaderPrograms.begin(),
            shaderPrograms.end(),
            [=](Program& a) -> bool {
                return program == a;
            }
        );

        if (it == shaderPrograms.end())
            throw std::invalid_argument("Попытка удалить несуществующую программу");

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

    const void RendererGL4::bindProgram(const Program& program)
    {
        glUseProgram(program.program);
    }

    const Buffer RendererGL4::createBuffer(real* vertexArray, uint vertexArraySize, uint* indexArray, uint indexArraySize)
    {
        assert(vertexArray != nullptr && vertexArraySize > 0);

        GLuint vbo = 0;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertexArraySize * sizeof(real), vertexArray, GL_STATIC_DRAW);

        GLuint ibo = 0;
        if (indexArray != nullptr)
        {
            assert(indexArraySize > 0);

            glGenBuffers(1, &ibo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArraySize * sizeof(uint), indexArray, GL_STATIC_DRAW);
        }

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        Buffer buffer = { vbo, ibo, vertexArray, vertexArraySize, indexArray, indexArraySize };
        buffers.push_back(buffer);

        return buffer;
    }

    const void RendererGL4::deleteBuffer(const Buffer& buffer)
    {
        const auto& it = std::find(
            buffers.begin(),
            buffers.end(),
            buffer
        );

        if (it == buffers.end())
            throw std::invalid_argument("Попытка удалить несуществующий буфер");

        glDeleteBuffers(1, &buffer.vbo);

        if (buffer.indexArray != nullptr)
            glDeleteBuffers(1, &buffer.ibo);
    }

    const void RendererGL4::bindBuffer(const Buffer& buffer)
    {
        glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
        
        if (buffer.indexArray != nullptr)
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.ibo);

#if DOUBLE_PRECISION == 1
        glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, 0);
#elif
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
#endif
    }

    const void RendererGL4::drawBuffer(const Buffer& buffer)
    {
        if (buffer.indexArray != nullptr)
        {
            glDrawElements(
                GL_TRIANGLES,
                buffer.indexArraySize,
                GL_UNSIGNED_INT,
                0
            );
        }
        else
        {
            glDrawArrays(GL_TRIANGLES, 0, buffer.vertexArraySize);
        }
    }
}