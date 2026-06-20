#pragma once

#include <unordered_map>
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

#include "Content.h"

namespace Core
{
	class Renderer;
	class Program;
	class Texture2D;

	class Material : public Content
	{
			friend class Renderer;
			friend class ContentManager;

		private:
			Renderer* _renderer = nullptr;
			Program* _program = nullptr;

			std::unordered_map<std::string, int> _intValues;
			std::unordered_map<std::string, float> _floatValues;
			std::unordered_map<std::string, glm::vec2> _vec2Values;
			std::unordered_map<std::string, glm::vec3> _vec3Values;
			std::unordered_map<std::string, glm::vec4> _vec4Values;
			std::unordered_map<std::string, glm::mat3> _mat3Values;
			std::unordered_map<std::string, glm::mat4> _mat4Values;
			std::unordered_map<std::string, Texture2D*> _textureValues;

			Material(Renderer* renderer);
			virtual ~Material();

		public:
			virtual int getContentType();

			const Program* getProgram() const { return _program; }
			void setProgram(Program* value) { _program = value; }

			int getInt(std::string name) const;
			void setInt(std::string name, int value) { _intValues[name] = value; }

			float getFloat(std::string name) const;
			void setFloat(std::string name, float value) { _floatValues[name] = value; }

			glm::vec2 getVec2(std::string name) const;
			void setVec2(std::string name, glm::vec2 value) { _vec2Values[name] = value; }

			glm::vec3 getVec3(std::string name) const;
			void setVec3(std::string name, glm::vec3 value) { _vec3Values[name] = value; }

			glm::vec4 getVec4(std::string name) const;
			void setVec4(std::string name, glm::vec4 value) { _vec4Values[name] = value; }

			glm::mat3 getMat3(std::string name) const;
			void setMat3(std::string name, glm::mat3 value) { _mat3Values[name] = value; }

			glm::mat4 getMat4(std::string name) const;
			void setMat4(std::string name, glm::mat4 value) { _mat4Values[name] = value; }

			Texture2D* getTexture2D(std::string name) const;
			void setTexture2D(std::string name, Texture2D* value) { _textureValues[name] = value; }

			void bind();
	};
} // namespace Core