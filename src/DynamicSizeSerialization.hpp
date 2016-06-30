#pragma once

#include <stdint.h>
#include <limits>

/**
 * Deserializes a uint64 size value with adaptive size.
 */
template<class Archive>
void loadSizeValue(Archive &ar, uint64_t& size)
{
    uint8_t size_u8 = 0;
    uint16_t size_u16 = 0;
    uint32_t size_u32 = 0;
    ar >> size_u8;
    if (size_u8 == std::numeric_limits<uint8_t>::max())
    {
        ar >> size_u16;
        if (size_u16 == std::numeric_limits<uint16_t>::max())
        {
            ar >> size_u32;
            if (size_u32 == std::numeric_limits<uint32_t>::max())
            {
                ar >> size;
            }
            else
                size = size_u32;
        }
        else
            size = size_u16;
    }
    else
        size = size_u8;
}

/**
 * Serializes a uint64 size value with adaptive size.
 */
template<class Archive>
void saveSizeValue(Archive &ar, const uint64_t& size)
{
    uint8_t size_u8 = std::numeric_limits<uint8_t>::max();
    uint16_t size_u16 = std::numeric_limits<uint16_t>::max();
    uint32_t size_u32 = std::numeric_limits<uint32_t>::max();
    if (size < size_u8)
    {
        size_u8 = (uint8_t)size;
        ar << size_u8;
    }
    else if (size < size_u16)
    {
        size_u16 = (uint16_t)size;
        ar << size_u8;
        ar << size_u16;
    }
    else if (size < size_u32)
    {
        size_u32 = (uint32_t)size;
        ar << size_u8;
        ar << size_u16;
        ar << size_u32;
    }
    else
    {
        ar << size_u8;
        ar << size_u16;
        ar << size_u32;
        ar << size;
    }
}