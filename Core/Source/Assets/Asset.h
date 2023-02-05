#pragma once

class Asset
{
public:
	virtual const void load() = 0;
	virtual const void unload() = 0;
};