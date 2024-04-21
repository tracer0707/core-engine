#include "Guid.h"

#define UUID_SYSTEM_GENERATOR
#include "../Classes/uuid.h"

namespace GX
{
	std::string Guid::genGuid()
	{
		return uuids::to_string(uuids::uuid_system_generator{}());;
	}
}