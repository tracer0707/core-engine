#include "RendererGL4.h"

#include <GL/glew.h>
#include <cassert>

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Components/Camera.h"

namespace Core
{
    RendererGL4::RendererGL4(DeviceContext* ctx): Renderer(ctx)
    {
    }

    const void RendererGL4::setViewportSize(int w, int h)
    {
        width = w;
        height = h;

        glViewport(0, 0, w, h);
    }

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
        currentProgram = program;
        glUseProgram(program.program);
    }

    const VertexBuffer* RendererGL4::createBuffer(Vertex* vertexArray, UInt32 vertexArraySize, UInt32* indexArray, UInt32 indexArraySize)
    {
        assert(vertexArray != nullptr && vertexArraySize > 0);

        GLuint vbo = 0;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertexArraySize * sizeof(Vertex), vertexArray, GL_STATIC_DRAW);

        GLuint ibo = 0;
        if (indexArray != nullptr)
        {
            assert(indexArraySize > 0);

            glGenBuffers(1, &ibo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArraySize * sizeof(UInt32), indexArray, GL_STATIC_DRAW);
        }

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        VertexBuffer* buffer = new VertexBuffer { vbo, ibo, vertexArray, vertexArraySize, indexArray, indexArraySize };
        
        return buffer;
    }

    const void RendererGL4::deleteBuffer(const VertexBuffer* buffer)
    {
        glDeleteBuffers(1, &buffer->vbo);

        if (buffer->indexArray != nullptr)
            glDeleteBuffers(1, &buffer->ibo);

        delete buffer;
    }

    const void RendererGL4::bindBuffer(const VertexBuffer* buffer)
    {
        glBindBuffer(GL_ARRAY_BUFFER, buffer->vbo);
        
        if (buffer->indexArray != nullptr)
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->ibo);

#if DOUBLE_PRECISION == 1
        glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, sizeof(Vertex), 0);
        glVertexAttribPointer(1, 2, GL_DOUBLE, GL_FALSE, sizeof(Vertex), (const GLvoid*)(3 * sizeof(Real)));
#else
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(3 * sizeof(Real)));
#endif
    }

    const void RendererGL4::drawBuffer(const VertexBuffer* buffer, glm::mat4& view, glm::mat4& proj, glm::mat4& model)
    {
        GLuint viewMtxId = glGetUniformLocation(currentProgram.program, "u_viewMtx");
        GLuint projMtxId = glGetUniformLocation(currentProgram.program, "u_projMtx");
        GLuint modelMtxId = glGetUniformLocation(currentProgram.program, "u_modelMtx");

        glUniformMatrix4fv(viewMtxId, 1, false, glm::value_ptr(view));
        glUniformMatrix4fv(projMtxId, 1, false, glm::value_ptr(proj));
        glUniformMatrix4fv(modelMtxId, 1, false, glm::value_ptr(model));

        if (buffer->indexArray != nullptr)
        {
            glDrawElements(
                GL_TRIANGLES,
                buffer->indexArraySize,
                GL_UNSIGNED_INT,
                0
            );
        }
        else
        {
            glDrawArrays(GL_TRIANGLES, 0, buffer->vertexArraySize);
        }
    }

    const UInt32 RendererGL4::createTexture(unsigned char* data, UInt32 width, UInt32 height, UInt32 size, GLenum format)
    {
        GLuint tex;
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        if (format == GL_RGBA8)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
        else if (format == GL_COMPRESSED_RGBA_BPTC_UNORM)
            glCompressedTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, size, data);

        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        return tex;
    }

    const void RendererGL4::bindTexture(UInt32 id, const char* name, UInt32 slot)
    {
        GLuint texId = glGetUniformLocation(currentProgram.program, name);

        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, id);
        glUniform1i(texId, slot);
    }

    const void RendererGL4::deleteTexture(UInt32 id)
    {
        glDeleteTextures(1, &id);
    }
}