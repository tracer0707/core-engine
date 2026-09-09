#include "ThumbManager.h"

#include <vector>

#include <FreeImage.h>
#include <glm/gtc/matrix_transform.hpp>

#include <Core/Shared/Uuid.h>
#include <Core/Shared/Path.h>
#include <Core/Shared/Hash.h>
#include <Core/System/Application.h>
#include <Core/Renderer/Renderer.h>
#include <Core/Renderer/FrameBuffer.h>
#include <Core/Renderer/Program.h>
#include <Core/Renderer/Primitives.h>
#include <Core/Content/ContentDatabase.h>
#include <Core/Content/ContentManager.h>
#include <Core/Content/Texture2D.h>
#include <Core/Content/Mesh.h>
#include <Core/Content/Material.h>

#include "../Utils/TextureUtils.h"

namespace Editor
{
	namespace
	{
		constexpr unsigned int ThumbnailSize = 256u;
	}

	ThumbManager::ThumbManager(Core::Application* application, Core::ContentManager* contentManager)
	{
		_application = application;
		_contentManager = contentManager;
	}

	fs::path ThumbManager::getThumbPath(const fs::path& sourcePath)
	{
		Core::Uuid uuid = Core::ContentDatabase::singleton()->getUuid(sourcePath);
		fs::path thumbPath = _application->getRootPath() / fs::path("Thumbnails") / (fs::path(uuid.toString() + ".png"));
		Core::String ext = Core::Path::toUtf8(sourcePath.extension()).toLower();

		if (!fs::exists(thumbPath))
		{
			fs::create_directories(thumbPath.parent_path());
			if (ext == ".texture")
			{
				renderTextureThumbnail(sourcePath, thumbPath);
			}
			else if (ext == ".mesh")
			{
				renderMeshThumbnail(sourcePath, thumbPath);
			}
			else if (ext == ".material")
			{
				renderMaterialThumbnail(sourcePath, thumbPath);
			}
		}

		return thumbPath;
	}

	void ThumbManager::renderTextureThumbnail(const fs::path& sourcePath, const fs::path& thumbPath)
	{
		if (_contentManager == nullptr)
		{
			return;
		}

		Core::Renderer* renderer = _contentManager->getRenderer();
		Core::Texture2D* texture = _contentManager->loadTexture2DFromFile(sourcePath);
		
		if (renderer == nullptr || texture == nullptr)
		{
			return;
		}

		const Core::FrameBuffer* frameBuffer = beginThumbnail(renderer);
		
		Core::Vertex vertices[] = {
			Core::Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f), Core::Color::WHITE),
			Core::Vertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f), Core::Color::WHITE),
			Core::Vertex(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f), Core::Color::WHITE),
			Core::Vertex(glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f), Core::Color::WHITE)};

		unsigned int indices[] = {0, 1, 2, 0, 2, 3};
		
		Core::VertexBuffer* buffer = renderer->createBuffer(vertices, 4, indices, 6);
		Core::Program* program = renderer->getFullScreenQuadProgram();
		
		renderer->bindProgram(program);
		renderer->setUniform(program->getUniformLocation(Hash("u_color")), glm::vec4(1.0f));
		renderer->setUniform(program->getUniformLocation(Hash("u_texture")), 0);
		texture->bind(0);
		
		glm::mat4 identity(1.0f);
		
		renderer->bindBuffer(buffer, 0, identity, identity, identity);
		renderer->drawBufferIndexed(Core::PrimitiveType::Triangle, 0, 6);
		renderer->deleteBuffer(buffer);
		
		finishThumbnail(renderer, frameBuffer, thumbPath);
	}

	void ThumbManager::renderMeshThumbnail(const fs::path& sourcePath, const fs::path& thumbPath)
	{
		if (_contentManager == nullptr)
		{
			return;
		}

		Core::Renderer* renderer = _contentManager->getRenderer();
		Core::Mesh* mesh = _contentManager->loadMeshFromFile(sourcePath);
		
		if (renderer == nullptr || mesh == nullptr || mesh->getVertexBuffer() == nullptr)
		{
			return;
		}

		renderPreviewMesh(renderer, mesh, nullptr, thumbPath);
	}

	void ThumbManager::renderMaterialThumbnail(const fs::path& sourcePath, const fs::path& thumbPath)
	{
		if (_contentManager == nullptr)
		{
			return;
		}

		Core::Renderer* renderer = _contentManager->getRenderer();
		Core::Material* material = _contentManager->loadMaterialFromFile(sourcePath);
		
		if (renderer == nullptr || material == nullptr)
		{
			return;
		}

		const Core::FrameBuffer* frameBuffer = beginThumbnail(renderer);
		
		glm::mat4 view;
		glm::mat4 projection;
		glm::mat4 model;
		
		getPreviewMatrices(Core::AxisAlignedBox(glm::vec3(-1.0f), glm::vec3(1.0f)), view, projection, model);

		if (material->getProgram() != nullptr)
		{
			material->bind();
		}
		else
		{
			renderer->bindProgram(renderer->getDefaultProgram());
		}

		Core::VertexBuffer* sphere = renderer->createBuffer(nullptr, 4096, nullptr, 4096);
		Core::Primitives::sphere(renderer, sphere, view, projection, model, 32, 16,
			C_CCW | C_CULL_BACK | C_ENABLE_DEPTH_TEST | C_ENABLE_DEPTH_WRITE | C_ENABLE_CULL_FACE | C_DEPTH_LEQUAL);
		
		renderer->deleteBuffer(sphere);
		
		finishThumbnail(renderer, frameBuffer, thumbPath);
	}

	const Core::FrameBuffer* ThumbManager::beginThumbnail(Core::Renderer* renderer)
	{
		const Core::FrameBuffer* frameBuffer = renderer->createFrameBuffer(ThumbnailSize, ThumbnailSize);
		renderer->bindFrameBuffer(frameBuffer);
		renderer->setViewportSize(ThumbnailSize, ThumbnailSize);
		renderer->clear(C_CLEAR_COLOR | C_CLEAR_DEPTH, Core::Color(0.08f, 0.08f, 0.08f, 1.0f));
		return frameBuffer;
	}

	void ThumbManager::finishThumbnail(Core::Renderer* renderer, const Core::FrameBuffer* frameBuffer, const fs::path& fileName)
	{
		TextureUtils::saveFrameBuffer(renderer, frameBuffer, fileName);
		renderer->bindFrameBuffer(nullptr);
		renderer->deleteFrameBuffer(frameBuffer);
	}

	void ThumbManager::getPreviewMatrices(const Core::AxisAlignedBox& box, glm::mat4& view, glm::mat4& projection, glm::mat4& model)
	{
		glm::vec3 center(0.0f);
		float radius = 1.0f;
		if (box.isFinite())
		{
			center = box.getCenter();
			radius = glm::max(glm::length(box.getHalfSize()), 0.01f);
		}

		glm::vec3 eye = center + glm::vec3(radius * 1.8f, radius * 1.2f, radius * 1.8f);
		view = glm::lookAt(eye, center, glm::vec3(0.0f, 1.0f, 0.0f));
		projection = glm::perspective(glm::radians(35.0f), 1.0f, 0.01f, radius * 10.0f);
		model = glm::mat4(1.0f);
	}

	void ThumbManager::renderPreviewMesh(Core::Renderer* renderer, Core::Mesh* mesh, Core::Material* material, const fs::path& fileName)
	{
		const Core::FrameBuffer* frameBuffer = beginThumbnail(renderer);

		glm::mat4 view;
		glm::mat4 projection;
		glm::mat4 model;

		getPreviewMatrices(mesh->getBoundingBox(), view, projection, model);

		if (material != nullptr && material->getProgram() != nullptr)
		{
			material->bind();
		}
		else
		{
			renderer->bindProgram(renderer->getUnlitColorProgram());
			renderer->setUniform(renderer->getUnlitColorProgram()->getUniformLocation(Hash("u_color")), glm::vec4(0.85f, 0.85f, 0.85f, 1.0f));
		}

		Core::VertexBuffer* buffer = mesh->getVertexBuffer();
		for (int i = 0; i < mesh->getSubMeshCount(); ++i)
		{
			Core::SubMesh& subMesh = mesh->getSubMesh(i);
			renderer->bindBuffer(buffer, C_CCW | C_CULL_BACK | C_ENABLE_DEPTH_TEST | C_ENABLE_DEPTH_WRITE | C_ENABLE_CULL_FACE | C_DEPTH_LEQUAL, view,
								 projection, model);

			if (buffer->getIndexArraySize() > 0)
			{
				renderer->drawBufferIndexed(Core::PrimitiveType::Triangle, subMesh.getIndexOffset(), subMesh.getIndexCount());
			}
			else
			{
				renderer->drawBufferArray(Core::PrimitiveType::Triangle, subMesh.getIndexOffset(), subMesh.getIndexCount());
			}
		}

		finishThumbnail(renderer, frameBuffer, fileName);
	}
} // namespace Editor