#include "BitSet.h"

#include <algorithm>

namespace Core
{
	const int BitSet::MAX_BITS = 32;

	bool BitSet::getBit(unsigned long long layer)
	{
		return bitSet.test(layer);
	}

	void BitSet::setBit(unsigned long long layer, bool enable)
	{
		bitSet.set(layer, enable);
	}

	unsigned long BitSet::toULong()
	{
		return bitSet.to_ulong();
	}

	void BitSet::fromULong(unsigned long value)
	{
		bitSet = std::bitset<MAX_BITS>(value);
	}
}