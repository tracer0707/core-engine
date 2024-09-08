#pragma once

#include <Shared/String.h>

namespace Editor
{
	class Modifier
	{
	private:
		UString _name = "";

	public:
		Modifier(UString name);
		virtual ~Modifier();

		UString getName() { return _name; }
	};
}