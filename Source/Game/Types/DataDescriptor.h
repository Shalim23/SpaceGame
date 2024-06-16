#pragma once
#include <cstdint>
#include <string>

struct DataDescriptor
{
    uint32_t id;
    uint32_t position;
    uint32_t size;
};

struct TextDescriptor
{
    uint32_t id;
    std::string text;
};
