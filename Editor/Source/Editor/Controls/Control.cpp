#include "Control.h"

#include <imgui.h>

#include <Core/Shared/Uuid.h>

namespace Editor
{
	Control::Control() : Container()
	{
		_id = Core::Uuid::create().toString();
		_dragDropSourceData = new DragDropData();
	}

	Control::~Control()
	{
		delete _dragDropSourceData;

		_dragDropSourceData = nullptr;
		_parent = nullptr;
	}

	float Control::getWidth() const
	{
		return _width == 0.0f ? _actualWidth : _width;
	}

	float Control::getHeight() const
	{
		return _height == 0.0f ? _actualHeight : _height;
	}

	void Control::setSize(float width, float height)
	{
		_width = width;
		_height = height;
	}

	void Control::setPosition(float x, float y)
	{
		_x = x;
		_y = y;
	}

	void Control::setObjectTag(int key, void* value)
	{
		_objectTags[key] = value;
	}

	void* Control::getObjectTag(int key) const
	{
		if (_objectTags.find(key) == _objectTags.end())
		{
			return nullptr;
		}

		return _objectTags.at(key);
	}

	void Control::setStringTag(int key, Core::String value)
	{
		_stringTags[key] = value;
	}

	Core::String Control::getStringTag(int key) const
	{
		if (_stringTags.find(key) == _stringTags.end())
		{
			return Core::String::Empty;
		}

		return _stringTags.at(key);
	}

	void Control::setDragDropSource(bool value, Core::String key)
	{
		_isDragDropSource = value;
		_dragDropSourceKey = key;
	}

	void Control::setDragDropSourceData(DragDropData data)
	{
		_dragDropSourceData->key = data.key;
		_dragDropSourceData->value = data.value;
	}

	void Control::setDragDropTarget(bool value, Core::String key)
	{
		_isDragDropTarget = value;
		_dragDropTargetKey = key;
	}

	void Control::updateDragDropSource()
	{
		if (!_isDragDropSource) return;

		if (ImGui::BeginDragDropSource())
		{
			void* data = reinterpret_cast<void*>(_dragDropSourceData);
			ImGui::SetDragDropPayload(_dragDropSourceKey.std_str().c_str(), &data, sizeof(void*));
			ImGui::Text(_dragDropSourceLabel.std_str().c_str());
			ImGui::EndDragDropSource();
		}
	}

	void Control::updateDragDropTarget()
	{
		if (!_isDragDropTarget) return;

		DragDropData* dataGlobal = nullptr;
		const ImGuiPayload* payloadGlobal = ImGui::GetDragDropPayload();
		if (payloadGlobal != nullptr)
		{
			dataGlobal = reinterpret_cast<DragDropData*>(*(void**)payloadGlobal->Data);
		}

		if (ImGui::BeginDragDropTarget())
		{
			ImVec2 mousePosAbs = ImGui::GetMousePos();
			ImVec2 elementPosAbs = ImGui::GetItemRectMin();
			ImVec2 mousePosRelative = ImVec2(mousePosAbs.x - elementPosAbs.x, mousePosAbs.y - elementPosAbs.y);

			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
			{
				if (!_isDragDropEnter)
				{
					_isDragDropEnter = true;
					if (_onDragEnter != nullptr && dataGlobal != nullptr)
					{
						_onDragEnter(dataGlobal, (int)mousePosRelative.x, (int)mousePosRelative.y);
					}
				}
				else
				{
					if (_onDragOver != nullptr && dataGlobal != nullptr)
					{
						_onDragOver(dataGlobal, (int)mousePosRelative.x, (int)mousePosRelative.y);
					}
				}
			}

			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(_dragDropTargetKey.std_str().c_str());
			if (payload != nullptr)
			{
				if (_onDragDrop != nullptr)
				{
					DragDropData* data = reinterpret_cast<DragDropData*>(*(void**)payload->Data);
					_onDragDrop(data, (int)mousePosRelative.x, (int)mousePosRelative.y);
				}
			}
			ImGui::EndDragDropTarget();
		}
		else
		{
			if (_isDragDropEnter)
			{
				_isDragDropEnter = false;
				if (_onDragExit != nullptr && dataGlobal != nullptr)
				{
					_onDragExit(dataGlobal);
				}
			}
		}
	}
} // namespace Editor