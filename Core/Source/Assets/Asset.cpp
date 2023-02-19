#include "Asset.h"

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
