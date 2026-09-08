#include "Behavior.h"

namespace Core
{
	Behavior::Behavior(Object* owner) : Component(owner) {}
	Behavior::~Behavior() {}

	void Behavior::init() 
	{
	}

	void Behavior::update(float& dt)
	{
	}
} // namespace Core