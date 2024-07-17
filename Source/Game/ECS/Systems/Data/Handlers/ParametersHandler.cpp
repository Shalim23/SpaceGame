#include "ParametersHandler.h"
#include "../../../../Utils.h"
#include <filesystem>

#ifndef NDEBUG
#include "imgui.h"
#endif

namespace
{
#ifndef NDEBUG
    constexpr size_t newParamInputBufferSize{64};
#endif
}

void ParametersHandler::init(SystemsManager& systemsManager, const std::vector<DataDescriptor>& dataDescriptors)
{
#ifndef NDEBUG
    assert(IM_ARRAYSIZE(newParamTypes) == typeDefaultValues.size());
    newParamInputBuffer.reserve(newParamInputBufferSize);
    //#TODO read from .json SOURCES_PATH
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
            //#TODO dump to .json
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
        const bool isButtonDisabled{ input.empty() || params_.contains(input)};
        
        if (isButtonDisabled)
        {
            ImGui::BeginDisabled();
        }
        
        if (ImGui::Button("Add parameter"))
        {
            params_.emplace(input, typeDefaultValues[newParamSelectedTypeIndex]);
            newParamInputBuffer.clear();
        }

        if (isButtonDisabled)
        {
            ImGui::EndDisabled();
        }

        ImGui::End();
    }
}
#endif
