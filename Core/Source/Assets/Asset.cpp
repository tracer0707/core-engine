#include "Asset.h"

namespace Core
{
    const void Asset::load()
    {
        loaded = true;
    }

    const void Asset::unload()
    {
        loaded = false;
    }

    const bool Asset::isLoaded()
    {
        return loaded;
    }
}