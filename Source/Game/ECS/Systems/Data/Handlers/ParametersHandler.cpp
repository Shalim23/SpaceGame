#include "ParametersHandler.h"
#include "../../../../Utils.h"
#include <filesystem>

#ifndef NDEBUG
#include "imgui.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#endif

namespace
{
#ifndef NDEBUG
    constexpr size_t newParamInputBufferSize{64};
    const std::string paramsJsonPath{std::format("{}{}", SOURCES_PATH, "/Data/params.json")};
#endif
}

void ParametersHandler::init(SystemsManager& systemsManager, const std::vector<DataDescriptor>& dataDescriptors)
{
#ifndef NDEBUG
    assert(IM_ARRAYSIZE(newParamTypes) == typeDefaultValues.size());
    newParamInputBuffer.reserve(newParamInputBufferSize);
    std::ifstream is(paramsJsonPath);
    nlohmann::json paramsJson;
    is >> paramsJson;
    for (const auto& [key, value] : paramsJson.items())
    {
        if(value.is_number_float())
        {
            params_[key] = value.get<float>();
        }
        else if (value.is_number_integer())
        {
            params_[key] = value.get<int>();
        }
    }
#else
    //#TODO read from data.bin file
#endif
}

#ifndef NDEBUG
void ParametersHandler::showParametersUI()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Gameplay parameters"))
        {
            if (ImGui::MenuItem("Update parameters"))
            {
                showUpdateParamsUI_ = true;
            }
            if (ImGui::MenuItem("New parameter"))
            {
                showCreateNewParamUI_ = true;
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if (showUpdateParamsUI_)
    {
        ImGui::Begin("Update gameplay parameters", &showUpdateParamsUI_);
        for (auto& [paramName, paramValue] : params_)
        {
            std::visit(overloaded
            {
            [&paramName](int& arg)
                {
                    ImGui::InputInt(paramName.data(), &arg);
                },
            [&paramName](float& arg)
                { 
                    ImGui::InputFloat(paramName.data(), &arg);
                },
            [&paramName](auto arg)
                {
                    std::stringstream ss;
                    ss << "Unknown param " << paramName;
                    utils::showMessageBox("Update gameplay parameters", ss.str());
                }
            },
             paramValue);
        }

        ImGui::NewLine();
        if (ImGui::Button("Save Params"))
        {
            nlohmann::json paramsJson;

            for (auto& [paramName, paramValue] : params_)
            {
                std::visit(overloaded
                    {
                    [&paramName, &paramsJson](auto arg)
                        {
                            paramsJson[paramName] = arg;
                        }
                    },
                    paramValue);
            }

            std::ofstream os(paramsJsonPath);
            os << paramsJson.dump(4);
        }
        ImGui::End();
    }

    if (showCreateNewParamUI_)
    {
        ImGui::Begin("Add new gameplay parameters", &showCreateNewParamUI_);

        ImGui::InputText("Parameter Name", newParamInputBuffer.data(), newParamInputBufferSize);
        ImGui::Spacing();
        ImGui::Combo("Parameter Type", &newParamSelectedTypeIndex, newParamTypes, IM_ARRAYSIZE(newParamTypes));
        ImGui::NewLine();

        std::string input{ newParamInputBuffer.data() };
        const bool isSaveDisabled{ input.empty() || params_.contains(input)};
        const bool isRemoveDisabled{ input.empty() || !params_.contains(input)};
        
        if (isSaveDisabled)
        {
            ImGui::BeginDisabled();
        }
        
        if (ImGui::Button("Add parameter"))
        {
            params_.emplace(input, typeDefaultValues[newParamSelectedTypeIndex]);
            newParamInputBuffer.clear();
        }

        if (isSaveDisabled)
        {
            ImGui::EndDisabled();
        }

        if (isRemoveDisabled)
        {
            ImGui::BeginDisabled();
        }

        if (ImGui::Button("Remove parameter"))
        {
            params_.erase(input);
            newParamInputBuffer.clear();
        }

        if (isRemoveDisabled)
        {
            ImGui::EndDisabled();
        }

        ImGui::End();
    }
}
#endif
