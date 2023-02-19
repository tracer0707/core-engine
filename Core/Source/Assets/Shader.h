#pragma once

#include "Asset.h"
#include "../Shared/String.h"

class Shader : public Asset
{
private:
	unsigned int nativeId = 0;

public:
	~Shader();

	virtual const void load();
	virtual const void unload();
	virtual const void loadFromString(UString vertexSrc, UString fragmentSrc);
	virtual const void bind();
	virtual const void unbind();
};