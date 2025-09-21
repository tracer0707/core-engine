#pragma once

#include <bitset>
#include <string>
#include <vector>

namespace Core
{
    class BitSet
    {
    private:
        std::bitset<32> bitSet = std::bitset<32>(0b0000000000000000'0000000000000000);

    public:
        static const int MAX_BITS;

        bool getBit(unsigned long long layer);
        void setBit(unsigned long long layer, bool enable);

        unsigned long toULong();
        void fromULong(unsigned long value);

        std::bitset<32>& getBitSet() { return bitSet; }
    };
}