#include "DataSystem.h"
#include "../../World.h"
#include "../../SystemsManager.h"
#include "FileHandler.h"

#ifndef NDEBUG
#include "imgui.h"
#endif

void DataSystem::update(World& world, const double deltaTime)
{
    std::get<TextureDataHandler>(dataHandlers_).clearDynamicTextures(world);
}

void DataSystem::shutdown()
{
    std::apply([](auto&&... handler) {((handler.shutdown()), ...); }, dataHandlers_);
}

void DataSystem::init(World& world, SystemsManager& systemsManager)
{
    if (dataDescriptors_.size() > 0)
    {
        return;
    }

    initDataDescriptors();

    std::apply([this, &systemsManager](auto&&... handler)
        {((handler.init(systemsManager, dataDescriptors_)), ...); }, dataHandlers_);

#ifndef NDEBUG
    systemsManager.getSystem<RenderSystem>().registerDebugUICallback([this]{debugUI();});
#endif
}

const TextureInfo& DataSystem::getTexture(const TextureType type)
{
    return std::get<TextureDataHandler>(dataHandlers_).getTexture(type);
}

const TextureInfo& DataSystem::getText(const TextType type) const
{
    return std::get<TextDataHandler>(dataHandlers_).getText(type);
}

TextureInfo DataSystem::createDynamicText(const Entity entity, std::string_view text)
{
    TextureInfo data{ std::get<TextDataHandler>(dataHandlers_).createDynamicText(text) };
    std::get<TextureDataHandler>(dataHandlers_).addDynamicTexture(entity, data.texture);
    return data;
}

TextureInfo DataSystem::createDynamicTexture(const TextureType type, const Entity entity)
{
    return std::get<TextureDataHandler>(dataHandlers_).createDynamicTexture(type, entity);
}

SDL_Surface* DataSystem::createSurface(const TextureType type) const
{
    return std::get<TextureDataHandler>(dataHandlers_).createSurface(type);
}

void DataSystem::initDataDescriptors()
{
    FileHandler dataFile{ utils::getDataFilePath() };

    uint32_t dataTypesCount{};
    if (dataFile.read(reinterpret_cast<char*>(&dataTypesCount), sizeof(uint32_t)))
    {
        const size_t dataFileOffset{ sizeof(uint32_t) + (sizeof(DataDescriptor) * dataTypesCount) };
        for (size_t i{ 0 }; i < dataTypesCount; ++i)
        {
            DataDescriptor desc{};
            if (dataFile.read(reinterpret_cast<char*>(&desc), sizeof(DataDescriptor)))
            {
                desc.position += dataFileOffset;
                dataDescriptors_.emplace_back(desc);
            }
            else
            {
                utils::showMessageBox(__FUNCTION__, "data.bin is corrupted!");
                throw std::exception{};
            }
        }
    }
    else
    {
        utils::showMessageBox(__FUNCTION__, "data.bin is corrupted!");
        throw std::exception{};
    }
}

#ifndef NDEBUG
void DataSystem::debugUI()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Gameplay parameters"))
        {
            showDebugUI_ = true;
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if (showDebugUI_) 
    {
        ImGui::Begin("Gameplay parameters", &showDebugUI_);
        ImGui::Text("Hello World");
        ImGui::End();
    }
}
#endif
