#include "RendererGL4.h"

#include <GL/glew.h>
#include <cassert>
#include <iostream>

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

    const Program* RendererGL4::createProgram(UString vertexSrc, UString fragmentSrc)
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

        const char* errorsVs = checkProgramErrors(vs);
        const char* errorsFs = checkProgramErrors(fs);

        if (errorsVs != nullptr) std::cout << errorsVs;
        if (errorsFs != nullptr) std::cout << errorsFs;

        if (errorsVs != nullptr || errorsFs != nullptr)
        {
            return nullptr;
        }

        GLuint programId = glCreateProgram();
        glAttachShader(programId, fs);
        glAttachShader(programId, vs);
        glLinkProgram(programId);

        Program* program = new Program();
        program->program = programId;
        program->vertexShader = vs;
        program->fragmentShader = fs;

        shaderPrograms.push_back(program);

        return program;
    }

    const void RendererGL4::deleteProgram(const Program* program)
    {
        const auto& it = std::find_if(
            shaderPrograms.begin(),
            shaderPrograms.end(),
            [=](Program* a) -> bool {
                return program->program == a->program;
            }
        );

        if (it == shaderPrograms.end())
            throw std::invalid_argument("Попытка удалить несуществующую программу");

        if ((*it)->vertexShader > 0)
            glDeleteShader((*it)->vertexShader);

        if ((*it)->fragmentShader > 0)
            glDeleteShader((*it)->fragmentShader);

        if ((*it)->geometryShader > 0)
            glDeleteShader((*it)->geometryShader);

        if ((*it)->computeShader > 0)
            glDeleteShader((*it)->computeShader);

        shaderPrograms.erase(it);

        delete program;
    }

    const void RendererGL4::bindProgram(const Program* program)
    {
        if (program != nullptr)
        {
            currentProgram = program;
            glUseProgram(program->program);
        }
        else
        {
            currentProgram = defaultProgram;
            glUseProgram(defaultProgram->program);
        }
    }

    const char* RendererGL4::checkProgramErrors(UInt32 program)
    {
        GLint isCompiled = 0;
        glGetShaderiv(program, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            char* result = new char[maxLength];
            glGetShaderInfoLog(program, maxLength, &maxLength, result);

            glDeleteShader(program);
            return result;
        }

        return nullptr;
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

        VertexBuffer* buffer = new VertexBuffer { vbo, ibo, vertexArray, vertexArraySize, indexArray, indexArraySize };

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        
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
     
        UInt32 position = glGetAttribLocation(currentProgram->program, "position");
        UInt32 uv0 = glGetAttribLocation(currentProgram->program, "uv0");
        UInt32 color0 = glGetAttribLocation(currentProgram->program, "color0");

        glEnableVertexAttribArray(position);
        glEnableVertexAttribArray(uv0);
        glEnableVertexAttribArray(color0);

#if DOUBLE_PRECISION == 1
        glVertexAttribPointer(position, 3, GL_DOUBLE, GL_FALSE, sizeof(Vertex), (void*)(0));
        glVertexAttribPointer(uv0, 2, GL_DOUBLE, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(Real)));
        glVertexAttribPointer(color0, 4, GL_DOUBLE, GL_FALSE, sizeof(Vertex), (void*)(5 * sizeof(Real)));
#else
        glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0));
        glVertexAttribPointer(uv0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(Real)));
        glVertexAttribPointer(color0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(5 * sizeof(Real)));
#endif

        if (buffer->indexArray != nullptr)
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->ibo);
    }

    const void RendererGL4::drawBuffer(const VertexBuffer* buffer, int primitiveType, UInt32 flags, glm::mat4& view, glm::mat4& proj, glm::mat4& model)
    {
        GLuint viewMtxId = glGetUniformLocation(currentProgram->program, "u_viewMtx");
        GLuint projMtxId = glGetUniformLocation(currentProgram->program, "u_projMtx");
        GLuint modelMtxId = glGetUniformLocation(currentProgram->program, "u_modelMtx");

        glUniformMatrix4fv(viewMtxId, 1, false, glm::value_ptr(view));
        glUniformMatrix4fv(projMtxId, 1, false, glm::value_ptr(proj));
        glUniformMatrix4fv(modelMtxId, 1, false, glm::value_ptr(model));

        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_FALSE);

        if (flags & C_CW) glFrontFace(GL_CW);
        if (flags & C_CCW) glFrontFace(GL_CCW);
        if (flags & C_CULL_BACK) glCullFace(GL_BACK);
        if (flags & C_CULL_FRONT) glCullFace(GL_FRONT);
        if (flags & C_ENABLE_CULL_FACE) glEnable(GL_CULL_FACE);
        if (flags & C_ENABLE_DEPTH_TEST) glEnable(GL_DEPTH_TEST);
        if (flags & C_ENABLE_DEPTH_WRITE) glDepthMask(GL_TRUE);
        if (flags & C_DEPTH_ALWAYS) glDepthFunc(GL_ALWAYS);
        if (flags & C_DEPTH_EQUAL) glDepthFunc(GL_EQUAL);
        if (flags & C_DEPTH_GEQUAL) glDepthFunc(GL_GEQUAL);
        if (flags & C_DEPTH_GREATER) glDepthFunc(GL_GREATER);
        if (flags & C_DEPTH_LEQUAL) glDepthFunc(GL_LEQUAL);
        if (flags & C_DEPTH_LESS) glDepthFunc(GL_LESS);
        if (flags & C_DEPTH_NEVER) glDepthFunc(GL_NEVER);
        if (flags & C_DEPTH_NOTEQUAL) glDepthFunc(GL_NOTEQUAL);

        if (buffer->indexArray != nullptr)
        {
            glDrawElements(
                primitiveType,
                buffer->indexArraySize,
                GL_UNSIGNED_INT,
                0
            );
        }
        else
        {
            glDrawArrays(primitiveType, 0, buffer->vertexArraySize);
        }
    }

    const UInt32 RendererGL4::createTexture(unsigned char* data, UInt32 width, UInt32 height, UInt32 size, UInt32 format)
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
        GLuint texId = glGetUniformLocation(currentProgram->program, name);

        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, id);
        glUniform1i(texId, slot);
    }

    const void RendererGL4::deleteTexture(UInt32 id)
    {
        glDeleteTextures(1, &id);
    }

    const void RendererGL4::clear(UInt32 flags)
    {
        glClearDepth(1.0f);
        glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
        glClear(flags);
    }
}