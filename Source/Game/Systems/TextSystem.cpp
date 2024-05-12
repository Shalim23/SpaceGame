#include "TextSystem.h"
#include "../World.h"
#include "../SystemsManager.h"
#include "../Types/Exceptions.h"
#include <fstream>

void TextSystem::init(World& world, SystemsManager& systemsManager)
{
    renderSystem_ = &systemsManager.getSystem<RenderSystem>();

    if (TTF_Init() != 0)
    {
        renderSystem_->showMessageBox(__FUNCTION__, "Failed to init SDL TTF!");
        throw SystemInitException{};
    }

    const std::vector<char> fontRawData{ getFontRawData() };
    font_ = loadFontFromRawData(fontRawData, 40);

    //#TODO cache static text
}

void TextSystem::update(World& world)
{
    std::vector<Entity> aliveWidgetEntities;
    world.forEach<WidgetComponent>([&aliveWidgetEntities](const Entity widgetEntity, WidgetComponent&)
        {
            aliveWidgetEntities.push_back(widgetEntity);
        });

    std::vector<DynamicTextsToEntity*> dynamicTextsToRemove;
    for (auto& texts : dynamicTexts_)
    {
        const auto iter{std::ranges::find(aliveWidgetEntities, texts.entity)};
        if (iter == aliveWidgetEntities.end())
        {
            dynamicTextsToRemove.push_back(&texts);
        }
    }

    for (const auto* textsToRemove : dynamicTextsToRemove)
    {
        std::erase_if(dynamicTexts_, [textsToRemove](const DynamicTextsToEntity& texts)
            {
                return textsToRemove->entity == texts.entity;
            });
    }
}

void TextSystem::shutdown()
{
    TTF_CloseFont(font_);

    TTF_Quit();
}

RenderData TextSystem::createText(const Entity entity, std::string_view text, const SDL_Color& color)
{
    SDL_Surface* textSurface{TTF_RenderText_Solid(font_, text.data(), color)};
    if (!textSurface)
    {
        auto a = SDL_GetError();
        renderSystem_->showMessageBox(__FUNCTION__, "Failed to create text surface!");
        throw std::exception{};
    }

    SDL_Texture* texture{renderSystem_->createTextureFromSurface(textSurface)};
    if (!texture)
    {
        renderSystem_->showMessageBox(__FUNCTION__, "Failed to create texture from text surface!");
        throw std::exception{};
    }

    RenderData renderData{};
    renderData.texture = texture;
    renderData.textureSize.x = textSurface->w;
    renderData.textureSize.y = textSurface->h;

    SDL_FreeSurface(textSurface);

    addDynanicText(entity, texture);

    return renderData;
}

std::vector<char> TextSystem::getFontRawData() const
{
    std::ifstream fontFile("Data/gameFont.bin", std::ios::binary);
    if (!fontFile.is_open())
    {
        renderSystem_->showMessageBox(__FUNCTION__, "gameFont.bin is missing!");
        throw SystemInitException{};
    }

    std::vector<char> fontRawData(std::istreambuf_iterator<char>(fontFile), {});

    fontFile.close();

    return fontRawData;
}

TTF_Font* TextSystem::loadFontFromRawData(const std::vector<char>& rawData, const int fontSize) const
{
    SDL_RWops* rw{SDL_RWFromConstMem(rawData.data(), rawData.size())};
    if (!rw)
    {
        renderSystem_->showMessageBox(__FUNCTION__, "Failed to create RWops!");
        throw SystemInitException{};
    }

    TTF_Font* font{TTF_OpenFontRW(rw, 1, fontSize)};
    if (!font)
    {
        renderSystem_->showMessageBox(__FUNCTION__, "Failed to load font!");
        throw SystemInitException{};
        SDL_RWclose(rw);
    }

    return font;
}

void TextSystem::addDynanicText(const Entity entity, SDL_Texture* texture)
{
    auto iter{std::ranges::find_if(dynamicTexts_, [entity](const DynamicTextsToEntity& texts)
        { return texts.entity == entity; })};
    DynamicTextsToEntity* texts{ iter == dynamicTexts_.end() ?
        &dynamicTexts_.emplace_back(DynamicTextsToEntity{ .entity = entity }) :
        &(*iter)};

    texts->textTextures.push_back(texture);
}
