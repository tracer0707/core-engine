#include "RendererGL4.h"

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <cassert>
#include <iostream>
#include <functional>

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include "Program.h"
#include "../Components/Camera.h"

namespace Core
{
    void debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
    {
        if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
        {
            return;
        }

        if (severity == GL_DEBUG_SEVERITY_HIGH || severity == GL_DEBUG_SEVERITY_MEDIUM || severity == GL_DEBUG_SEVERITY_LOW)
        {
            std::cout << "OpenGL [" << severity << "]: " << message << " (" << id << ")" << std::endl;
        }
    }

    RendererGL4::RendererGL4(void* windowCtx) : Renderer(windowCtx)
    {
        _renderCtx = SDL_GL_CreateContext((SDL_Window*)_windowCtx);

        SDL_GL_SetSwapInterval(1);

        makeCurrent();
        swapBuffers();

        GLenum err = glewInit();
        if (err != GLEW_OK)
        {
            fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        }

        _imguiCtx = ImGui::CreateContext();
        ImGui::SetCurrentContext(_imguiCtx);

        ImGui_ImplSDL2_InitForOpenGL((SDL_Window*)_windowCtx, (SDL_GLContext)_renderCtx);
        ImGui_ImplOpenGL3_Init("#version 130");

        glEnable(GL_MULTISAMPLE);

        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(debugMessageCallback, nullptr);
    }

    RendererGL4::~RendererGL4()
    {
        makeCurrent();

        glDisable(GL_MULTISAMPLE);

        ImGui::SetCurrentContext(_imguiCtx);
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();

        for (auto* p : _shaderPrograms)
        {
            if (p != nullptr) deleteProgram(p);
        }
        _shaderPrograms.clear();

        SDL_GL_DeleteContext(_renderCtx);

        _imguiCtx = nullptr;
        _renderCtx = nullptr;
    }

    void RendererGL4::processEvents(void* event)
    {
        ImGui::SetCurrentContext(_imguiCtx);
        ImGui_ImplSDL2_ProcessEvent((SDL_Event*)event);
    }

    void RendererGL4::setViewportSize(int w, int h)
    {
        _width = w;
        _height = h;

        glViewport(0, 0, w, h);
    }

    void RendererGL4::beginUI()
    {
        ImGui::SetCurrentContext(_imguiCtx);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
    }

    void RendererGL4::endUI()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void RendererGL4::makeCurrent()
    {
        SDL_GL_MakeCurrent((SDL_Window*)_windowCtx, (SDL_GLContext)_renderCtx);
    }

    void RendererGL4::swapBuffers()
    {
        SDL_GL_SwapWindow((SDL_Window*)_windowCtx);
    }

    Program* RendererGL4::createProgram(String vertexSrc, String fragmentSrc)
    {
        std::string vertexSrcUtf8 = vertexSrc.std_str();
        std::string fragmentSrcUtf8 = fragmentSrc.std_str();

        const char* _vertexSrcUtf8 = vertexSrcUtf8.c_str();
        const char* _fragmentSrcUtf8 = fragmentSrcUtf8.c_str();

        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &_vertexSrcUtf8, NULL);
        glCompileShader(vs);

        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &_fragmentSrcUtf8, NULL);
        glCompileShader(fs);

        std::string errorsVs = checkProgramErrors(vs);
        std::string errorsFs = checkProgramErrors(fs);

        if (!errorsVs.empty()) std::cout << errorsVs;
        if (!errorsFs.empty()) std::cout << errorsFs;

        if (!errorsVs.empty() || !errorsFs.empty())
        {
            return nullptr;
        }

        GLuint programId = glCreateProgram();
        glAttachShader(programId, vs);
        glAttachShader(programId, fs);
        glLinkProgram(programId);

        GLint linkStatus;
        glGetProgramiv(programId, GL_LINK_STATUS, &linkStatus);
        if (linkStatus == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<char> infoLog(maxLength);
            glGetProgramInfoLog(programId, maxLength, &maxLength, &infoLog[0]);
            std::cerr << "Program link error:\n" << infoLog.data() << std::endl;
            glDeleteProgram(programId);
            return nullptr;
        }

        glDetachShader(programId, vs);
        glDetachShader(programId, fs);

        Program* program = new Program();
        program->program = programId;
        program->vertexShader = vs;
        program->fragmentShader = fs;

        _shaderPrograms.push_back(program);

        return program;
    }

    void RendererGL4::deleteProgram(Program* program)
    {
        const auto& it =
            std::find_if(_shaderPrograms.begin(), _shaderPrograms.end(), [=](Program* a) -> bool { return program->program == a->program; });

        if (it == _shaderPrograms.end()) throw std::invalid_argument("Попытка удалить несуществующую программу");

        if ((*it)->vertexShader > 0) glDeleteShader((*it)->vertexShader);
        if ((*it)->fragmentShader > 0) glDeleteShader((*it)->fragmentShader);
        if ((*it)->geometryShader > 0) glDeleteShader((*it)->geometryShader);
        if ((*it)->computeShader > 0) glDeleteShader((*it)->computeShader);

        _shaderPrograms.erase(it);

        delete program;
    }

    void RendererGL4::bindProgram(Program* program)
    {
        if (program == nullptr)
        {
            _currentProgram = _defaultProgram;
            glUseProgram(_defaultProgram->program);

            return;
        }

        _currentProgram = program;
        glUseProgram(program->program);
    }

    std::string RendererGL4::checkProgramErrors(unsigned int program)
    {
        GLint isCompiled = 0;
        glGetShaderiv(program, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            char* result = new char[maxLength];
            glGetShaderInfoLog(program, maxLength, &maxLength, result);

            std::string log(result);
            delete[] result;

            glDeleteShader(program);
            return log;
        }

        return "";
    }

    VertexBuffer* RendererGL4::createBuffer(unsigned int maxVertexSize, unsigned int maxIndexSize)
    {
        assert(maxVertexSize > 0);

        GLuint vao = 0, vbo = 0, ibo = 0;

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, maxVertexSize * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

        if (maxIndexSize > 0)
        {
            glGenBuffers(1, &ibo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, maxIndexSize * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW);
        }

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);

        VertexBuffer* buffer = new VertexBuffer{vao, vbo, ibo, nullptr, maxVertexSize, nullptr, maxIndexSize};

        return buffer;
    }

    VertexBuffer* RendererGL4::createBuffer(Vertex* vertexArray, unsigned int vertexArraySize, unsigned int* indexArray, unsigned int indexArraySize)
    {
        assert(vertexArray != nullptr && vertexArraySize > 0);

        GLuint vao = 0, vbo = 0, ibo = 0;

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertexArraySize * sizeof(Vertex), vertexArray, GL_STATIC_DRAW);

        if (indexArray != nullptr)
        {
            assert(indexArraySize > 0);

            glGenBuffers(1, &ibo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArraySize * sizeof(unsigned int), indexArray, GL_STATIC_DRAW);
        }

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);

        VertexBuffer* buffer = new VertexBuffer{vao, vbo, ibo, vertexArray, vertexArraySize, indexArray, indexArraySize};

        return buffer;
    }

    void RendererGL4::deleteBuffer(VertexBuffer* buffer)
    {
        GLint currentVAO;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVAO);

        if (currentVAO == (GLint)buffer->vao)
        {
            glBindVertexArray(0);
        }

        glDeleteVertexArrays(1, &buffer->vao);
        glDeleteBuffers(1, &buffer->vbo);

        if (buffer->ibo != 0) glDeleteBuffers(1, &buffer->ibo);

        delete buffer;
    }

    void RendererGL4::drawBuffer(VertexBuffer* buffer, PrimitiveType primitiveType, unsigned int flags, glm::mat4& view, glm::mat4& proj,
                                 glm::mat4& model)
    {
        GLuint viewMtxId = glGetUniformLocation(_currentProgram->program, "u_viewMtx");
        GLuint projMtxId = glGetUniformLocation(_currentProgram->program, "u_projMtx");
        GLuint modelMtxId = glGetUniformLocation(_currentProgram->program, "u_modelMtx");

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

        int _primitiveType = GL_TRIANGLES;

        switch (primitiveType)
        {
        case PrimitiveType::Triangle:
            _primitiveType = GL_TRIANGLES;
            break;
        case PrimitiveType::Line:
            _primitiveType = GL_LINES;
            break;
        default:
            throw std::runtime_error("Unknown primitive type");
            break;
        }

        glBindVertexArray(buffer->vao);

        if (buffer->indexArray != nullptr)
        {
            glDrawElements(_primitiveType, buffer->indexArraySize, GL_UNSIGNED_INT, nullptr);
        }
        else if (buffer->vertexArray != nullptr)
        {
            glDrawArrays(_primitiveType, 0, buffer->vertexArraySize);
        }
    }

    void RendererGL4::updateBuffer(VertexBuffer* buffer, Vertex* vertexArray, unsigned int vertexArraySize, unsigned int* indexArray,
                                   unsigned int indexArraySize)
    {
        assert(vertexArraySize <= buffer->vertexArraySize);

        glBindBuffer(GL_ARRAY_BUFFER, buffer->vbo);
        void* vptr = glMapBufferRange(GL_ARRAY_BUFFER, 0, vertexArraySize * sizeof(Vertex), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

        if (vptr != nullptr)
        {
            memcpy(vptr, vertexArray, vertexArraySize * sizeof(Vertex));
            glUnmapBuffer(GL_ARRAY_BUFFER);
        }

        if (indexArray != nullptr)
        {
            assert(indexArraySize > 0);
            assert(indexArraySize <= buffer->indexArraySize);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->ibo);
            void* iptr =
                glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, indexArraySize * sizeof(unsigned int), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

            if (iptr != nullptr)
            {
                memcpy(iptr, indexArray, indexArraySize * sizeof(unsigned int));
                glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
            }
        }
    }

    const FrameBuffer* RendererGL4::createFrameBuffer(unsigned int width, unsigned int height)
    {
        FrameBuffer* fb = new FrameBuffer();

        glGenFramebuffers(1, &fb->frameBuffer);
        glGenTextures(1, &fb->colorBuffer);
        glGenRenderbuffers(1, &fb->depthBuffer);

        glBindFramebuffer(GL_FRAMEBUFFER, fb->frameBuffer);

        glBindTexture(GL_TEXTURE_2D, fb->colorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb->colorBuffer, 0);

        glBindRenderbuffer(GL_RENDERBUFFER, fb->depthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
        glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fb->depthBuffer);

        return fb;
    }

    void RendererGL4::deleteFrameBuffer(const FrameBuffer* buffer)
    {
        glDeleteFramebuffers(1, &buffer->frameBuffer);
        glDeleteTextures(1, &buffer->colorBuffer);
        glDeleteRenderbuffers(1, &buffer->depthBuffer);

        delete buffer;
    }

    void RendererGL4::bindFrameBuffer(const FrameBuffer* buffer)
    {
        if (buffer == nullptr)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            return;
        }

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, buffer->frameBuffer);
    }

    const unsigned int RendererGL4::createTexture(unsigned char* data, unsigned int width, unsigned int height, unsigned int size,
                                                  TextureFormat format)
    {
        GLuint tex;
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (format == TextureFormat::RGBA8)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
        }
        else if (format == TextureFormat::BC7)
        {
            glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_BPTC_UNORM, width, height, 0, size, data);
        }
        else
        {
            throw std::runtime_error("Texture format is unsupported");
        }

        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        return tex;
    }

    void RendererGL4::bindTexture(unsigned int id, const char* name, unsigned int slot)
    {
        GLuint texId = glGetUniformLocation(_currentProgram->program, name);

        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, id);
        if (texId >= 0) glUniform1i(texId, slot);
    }

    void RendererGL4::deleteTexture(unsigned int id)
    {
        glDeleteTextures(1, &id);
    }

    void RendererGL4::clear(unsigned int flags, Color color)
    {
        unsigned int _flags = 0;

        if (flags & C_CLEAR_DEPTH)
        {
            _flags |= GL_DEPTH_BUFFER_BIT;
            glClearDepth(1.0f);
        }

        if (flags & C_CLEAR_COLOR)
        {
            _flags |= GL_COLOR_BUFFER_BIT;
            glClearColor(color.r, color.g, color.b, color.a);
        }

        glClear(_flags);
    }
} // namespace Core