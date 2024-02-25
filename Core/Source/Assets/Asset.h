#pragma once

namespace Core
{
	class Asset
	{
	private:
		bool loaded = false;

	public:
		virtual const void load();
		virtual const void unload();

		const bool isLoaded();
	};
}