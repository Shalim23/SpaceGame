#pragma once
#include <vector>
#include <unordered_map>
#include <variant>
#include "../DataDescriptor.h"

class SystemsManager;

class ParametersHandler
{
template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };

public:
    void init(SystemsManager& systemsManager,
        const std::vector<DataDescriptor>& dataDescriptors);
    void shutdown() {}

#ifndef NDEBUG
    void showParametersUI();
#endif

private:
#ifndef NDEBUG
    std::unordered_map<std::string, std::variant<int, float>> params_;
    bool showCreateNewParamUI_{ false };
    bool showUpdateParamsUI_{ false };

    std::string newParamInputBuffer{};
    const char* newParamTypes[2]{"int", "float"};
    const std::vector<std::variant<int, float>> typeDefaultValues{{0}, {0.0f}};
    int newParamSelectedTypeIndex{};
#else
    std::map<uint32_t, std::variant<int, float>> params_;
#endif
};
