#include "RendererGL4.h"

#include <glad/gl.h>
#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <cassert>
#include <iostream>
#include <functional>

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include "Program.h"
#include "../Shared/Hash.h"
#include "../Components/Camera.h"

#include "Shaders/GL4/Default.h"
#include "Shaders/GL4/UnlitColor.h"
#include "Shaders/GL4/UnlitTexture.h"

namespace Core
{
	bool RendererGL4::isGladLoaded = false;
	void* RendererGL4::currentContext = nullptr;

	static void debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
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

		if (!isGladLoaded)
		{
			int version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
			printf("Renderer: GL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
			isGladLoaded = true;
		}

		glEnable(GL_MULTISAMPLE);

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(debugMessageCallback, nullptr);

		_imguiCtx = ImGui::CreateContext();
		ImGui::SetCurrentContext(_imguiCtx);

		ImGui_ImplSDL2_InitForOpenGL((SDL_Window*)_windowCtx, (SDL_GLContext)_renderCtx);
		ImGui_ImplOpenGL3_Init("#version 130");

		_defaultProgram = createProgram("Builtin/Default", Shaders::Default::getVertexSource(), Shaders::Default::getFragmentSource());
		_unlitColorProgram = createProgram("Builtin/UnlitColor", Shaders::UnlitColor::getVertexSource(), Shaders::UnlitColor::getFragmentSource());
		_unlitTextureProgram = createProgram("Builtin/UnlitTexture", Shaders::UnlitTexture::getVertexSource(), Shaders::UnlitTexture::getFragmentSource());
	}

	RendererGL4::~RendererGL4()
	{
		makeCurrent();

		glDisable(GL_MULTISAMPLE);

		ImGui::SetCurrentContext(_imguiCtx);
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();

		while (_shaderPrograms.size() > 0)
		{
			deleteProgram(_shaderPrograms.begin()->second);
		}

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
		if (_renderCtx == currentContext) return;
		currentContext = _renderCtx;
		SDL_GL_MakeCurrent((SDL_Window*)_windowCtx, (SDL_GLContext)_renderCtx);
	}

	void RendererGL4::swapBuffers()
	{
		SDL_GL_SwapWindow((SDL_Window*)_windowCtx);
	}

	Program* RendererGL4::createProgram(String name, String vertexSrc, String fragmentSrc)
	{
		if (_shaderPrograms.contains(name))
		{
			throw std::runtime_error("A program with the name \"" + name.std_str() + "\" already exists");
		}
		
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
		glDeleteShader(vs);
		glDeleteShader(fs);

		Program* program = new Program();
		program->name = name;
		program->program = programId;

		program->u_viewMtxLocation = glGetUniformLocation(programId, "_u_viewMtx");
		program->u_projMtxLocation = glGetUniformLocation(programId, "_u_projMtx");
		program->u_modelMtxLocation = glGetUniformLocation(programId, "_u_modelMtx");

		GLint uniformCount = 0;
		glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &uniformCount);

		for (GLint i = 0; i < uniformCount; i++)
		{
			GLchar u_name[256];
			GLsizei u_length;
			GLint u_size;
			GLenum u_type;

			glGetActiveUniform(programId, i, sizeof(u_name), &u_length, &u_size, &u_type, u_name);

			if (String(u_name).startsWith("_")) continue;

			GLint location = glGetUniformLocation(programId, u_name);

			UniformType uniformType;
			switch (u_type)
			{
			case GL_FLOAT:
				uniformType = UniformType::Float;
				break;
			case GL_INT:
				uniformType = UniformType::Int;
				break;
			case GL_FLOAT_VEC2:
				uniformType = UniformType::Vec2;
				break;
			case GL_FLOAT_VEC3:
				uniformType = UniformType::Vec3;
				break;
			case GL_FLOAT_VEC4:
				uniformType = UniformType::Vec4;
				break;
			case GL_FLOAT_MAT3:
				uniformType = UniformType::Mat3;
				break;
			case GL_FLOAT_MAT4:
				uniformType = UniformType::Mat4;
				break;
			case GL_SAMPLER_2D:
				uniformType = UniformType::Sampler2D;
				break;
			case GL_SAMPLER_CUBE:
				uniformType = UniformType::SamplerCube;
				break;
			default:
				uniformType = UniformType::Unknown;
				break;
			}

			UniformInfo info;
			info.name = u_name;
			info.nameHash = Hash(info.name.std_str());
			info.type = uniformType;
			info.location = location;
			info.size = u_size;
			program->uniforms.add(info);
		}

		_shaderPrograms[name] = program;

		return program;
	}

	void RendererGL4::deleteProgram(Program* program)
	{
		auto it = std::find_if(_shaderPrograms.begin(), _shaderPrograms.end(), [program](const auto& pair)
		{
			return pair.second == program;
		});

		if (program == nullptr || it == _shaderPrograms.end())
			throw std::invalid_argument("An attempt was made to delete a program that does not exist");

		if (program->program > 0) glDeleteProgram(program->program);

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

	VertexBuffer* RendererGL4::createBuffer(Vertex* vertexArray, unsigned int vertexArraySize, unsigned int* indexArray, unsigned int indexArraySize)
	{
		assert(vertexArraySize > 0);

		GLuint vao = 0, vbo = 0, ibo = 0;

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertexArraySize * sizeof(Vertex), vertexArray, vertexArray != nullptr ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);

		if (indexArray != nullptr)
		{
			assert(indexArraySize > 0);

			glGenBuffers(1, &ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArraySize * sizeof(unsigned int), indexArray, GL_STATIC_DRAW);
		}
		else if (indexArraySize > 0)
		{
			glGenBuffers(1, &ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArraySize * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW);
		}

		// Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0));
		glEnableVertexAttribArray(0);

		// Normal
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		
		// Tangent
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
		
		// Bitangent
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(9 * sizeof(float)));
		glEnableVertexAttribArray(3);

		// UV0
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(12 * sizeof(float)));
		glEnableVertexAttribArray(4);

		// UV1
		glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(14 * sizeof(float)));
		glEnableVertexAttribArray(5);

		// Color
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(16 * sizeof(float)));
		glEnableVertexAttribArray(6);

		// Blend Weight
		glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(20 * sizeof(float)));
		glEnableVertexAttribArray(7);

		// Blend Indices
		glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(24 * sizeof(float)));
		glEnableVertexAttribArray(8);

		glBindVertexArray(0);

		VertexBuffer* buffer = new VertexBuffer(vao, vbo, ibo, vertexArray != nullptr ? VertexBufferType::Static : VertexBufferType::Dynamic, vertexArray, vertexArraySize, indexArray, indexArraySize);

		return buffer;
	}

	void RendererGL4::updateBuffer(VertexBuffer* buffer, Vertex* vertexArray, unsigned int vertexArraySize, unsigned int* indexArray,
								   unsigned int indexArraySize)
	{
		assert(buffer->type == VertexBufferType::Dynamic && "Only dynamic vertex buffers can be updated");
		assert(vertexArraySize <= buffer->getMaxVertexArraySize() && "Vertex array size exceeds maximum");

		glBindBuffer(GL_ARRAY_BUFFER, buffer->getVbo());
		void* vptr = glMapBufferRange(GL_ARRAY_BUFFER, 0, vertexArraySize * sizeof(Vertex), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

		if (vptr != nullptr)
		{
			memcpy(vptr, vertexArray, vertexArraySize * sizeof(Vertex));
			glUnmapBuffer(GL_ARRAY_BUFFER);

			buffer->updateVertexArray(vertexArray, vertexArraySize);
		}

		if (indexArray != nullptr)
		{
			assert(indexArraySize > 0);
			assert(indexArraySize <= buffer->getMaxIndexArraySize() && "Index array size exceeds maximum");

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->getIbo());
			void* iptr =
				glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, indexArraySize * sizeof(unsigned int), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

			if (iptr != nullptr)
			{
				memcpy(iptr, indexArray, indexArraySize * sizeof(unsigned int));
				glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

				buffer->updateIndexArray(indexArray, indexArraySize);
			}
		}
	}

	void RendererGL4::deleteBuffer(VertexBuffer* buffer)
	{
		GLint currentVAO;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVAO);

		if (currentVAO == (GLint)buffer->getVao())
		{
			glBindVertexArray(0);
		}

		uint32_t vao = buffer->getVao();
		glDeleteVertexArrays(1, &vao);

		uint32_t vbo = buffer->getVbo();
		glDeleteBuffers(1, &vbo);

		uint32_t ibo = buffer->getIbo();
		if (ibo != 0) glDeleteBuffers(1, &ibo);

		delete buffer;
	}

	void RendererGL4::bindBuffer(VertexBuffer* buffer, unsigned int flags, glm::mat4& view, glm::mat4& proj, glm::mat4& model)
	{
		glUniformMatrix4fv(_currentProgram->u_viewMtxLocation, 1, false, glm::value_ptr(view));
		glUniformMatrix4fv(_currentProgram->u_projMtxLocation, 1, false, glm::value_ptr(proj));
		glUniformMatrix4fv(_currentProgram->u_modelMtxLocation, 1, false, glm::value_ptr(model));

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

		if (buffer->getVao() > 0)
		{
			glBindVertexArray(buffer->getVao());
		}
		else
		{
			glBindVertexArray(0);
		}
	}

	void RendererGL4::drawBufferArray(PrimitiveType primitiveType, unsigned int offset, unsigned int count)
	{
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

		if (offset >= 0 && count > 0)
		{
			glDrawArrays(_primitiveType, offset, count);
		}
	}

	void RendererGL4::drawBufferIndexed(PrimitiveType primitiveType, unsigned int offset, unsigned int count)
	{
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

		if (offset >= 0 && count > 0)
		{
			glDrawElements(_primitiveType, count, GL_UNSIGNED_INT, reinterpret_cast<void*>(offset * sizeof(uint32_t)));
		}
	}

	const FrameBuffer* RendererGL4::createFrameBuffer(unsigned int width, unsigned int height)
	{
		FrameBuffer* fb = new FrameBuffer();

		fb->width = width;
		fb->height = height;

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
			throw std::runtime_error("Texture format is not supported");
		}

		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		return tex;
	}

	void RendererGL4::bindTexture(unsigned int id, unsigned int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, id);
	}

	void RendererGL4::deleteTexture(unsigned int id)
	{
		glDeleteTextures(1, &id);
	}

	void RendererGL4::setUniform(int location, int value)
	{
		glUniform1i(location, value);
	}

	void RendererGL4::setUniform(int location, float value)
	{
		glUniform1f(location, value);
	}

	void RendererGL4::setUniform(int location, glm::vec2 value)
	{
		glUniform2fv(location, 1, glm::value_ptr(value));
	}

	void RendererGL4::setUniform(int location, glm::vec3 value)
	{
		glUniform3fv(location, 1, glm::value_ptr(value));
	}

	void RendererGL4::setUniform(int location, glm::vec4 value)
	{
		glUniform4fv(location, 1, glm::value_ptr(value));
	}

	void RendererGL4::setUniform(int location, glm::mat3 value)
	{
		glUniformMatrix3fv(location, 1, false, glm::value_ptr(value));
	}

	void RendererGL4::setUniform(int location, glm::mat4 value)
	{
		glUniformMatrix4fv(location, 1, false, glm::value_ptr(value));
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