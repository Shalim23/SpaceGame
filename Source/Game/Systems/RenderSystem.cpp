#include "RenderSystem.h"
#include "../World.h"
#include "../SystemsManager.h"
#include "../Types/Exceptions.h"
#include "SDL_image.h"
#include <fstream>
#include <sstream>

void RenderSystem::preinit(World& w, SystemsManager& sm)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        showMessageBox(__FUNCTION__, "Failed to init SDL!");
        throw SystemInitException{};
    }

    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        showMessageBox(__FUNCTION__, "Failed to init SDL_Image!");
        throw SystemInitException{};
    }

    constexpr Uint32 fullscreen_flag{0};
    //constexpr Uint32 fullscreen_flag{ SDL_WINDOW_FULLSCREEN_DESKTOP };

    m_window = SDL_CreateWindow("Space Game", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, 800, 600, fullscreen_flag);
    if (!m_window)
    {
        showMessageBox(__FUNCTION__, "Failed to create window!");
        throw SystemInitException{};
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!m_renderer)
    {
        showMessageBox(__FUNCTION__, "Failed to create renderer!");
        throw SystemInitException{};
    }

    initTexturesDescriptors();
}

void RenderSystem::update(World& w)
{

    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(m_renderer);

    w.forEach<PlayerComponent>([this, &w](const Entity ent, PlayerComponent& comp)
        {
            processRenderData(w, ent);

            for (const auto& layer : m_sprites_to_render)
            {
                for (const auto& sprite : layer)
                {
                    SDL_RenderCopyExF(m_renderer,
                        sprite->render_data.texture,
                        &sprite->render_data.src,
                        &sprite->render_data.dst,
                        sprite->render_data.rotation,
                        nullptr, SDL_FLIP_NONE);
                }
            }
        });

    SDL_RenderPresent(m_renderer);
}

void RenderSystem::shutdown()
{
    for (const auto& t : m_textures)
    {
        SDL_DestroyTexture(t.texture);
    }
    
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    IMG_Quit();
    SDL_Quit();
}

void RenderSystem::showMessageBox(const char* title, const char* message) const
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, message, m_window);
}

const RenderSystem::Texture& RenderSystem::getTexture(const TextureType type)
{
    const auto iter{ std::ranges::find_if(m_textures,
        [type](const Texture& t) { return t.type == type; }) };
    if (iter != m_textures.end())
    {
        return *iter;
    }

    const auto& texture_data{ getTextureData(type) };
    SDL_Texture* texture{ createTexture(texture_data) };
    const SDL_Point texture_size{ getTextureSize(texture) };
    const auto& new_texture{ m_textures.emplace_back(
        Texture{.type = type, .texture = texture, .size = texture_size}) };

    return new_texture;
}

SDL_Texture* RenderSystem::createTexture(const std::vector<char>& data) const
{
    return IMG_LoadTexture_RW(m_renderer, SDL_RWFromConstMem(data.data(), data.size()), 1);
}

SDL_Point RenderSystem::getTextureSize(SDL_Texture* texture) const
{
    SDL_Point size;
    SDL_QueryTexture(texture, nullptr, nullptr, &size.x, &size.y);
    return size;
}

SDL_FPoint RenderSystem::getTextureSizeF(const SDL_Point& texture_size) const
{
    return SDL_FPoint{ static_cast<float>(texture_size.x),
            static_cast<float>(texture_size.y) };
}

SDL_Point RenderSystem::getScreenSize() const
{
    SDL_Point s;
    SDL_GetRendererOutputSize(m_renderer, &s.x, &s.y);
    return s;
}

SDL_FPoint RenderSystem::getScreenSizeF() const
{
    const SDL_Point s{getScreenSize()};
    return SDL_FPoint{static_cast<float>(s.x),
        static_cast<float>(s.y)};
}

void RenderSystem::initTexturesDescriptors()
{
    std::ifstream metas_file("textures_descriptors.bin", std::ios::binary);
    if (!metas_file.is_open())
    {
        showMessageBox(__FUNCTION__, "textures_descriptors.bin is missing!");
        throw std::exception{};
    }

    TextureDescriptor desc{};
    while (metas_file.read(reinterpret_cast<char*>(&desc), sizeof(TextureDescriptor)))
    {
        m_texture_descriptors.push_back(desc);
    }

    metas_file.close();
    m_textures.reserve(m_texture_descriptors.size());
}

std::vector<char> RenderSystem::getTextureData(const TextureType type) const
{
    const auto texture_type_int{ static_cast<uint32_t>(type) };
    const auto iter{ std::ranges::find_if(m_texture_descriptors,
        [texture_type_int](const TextureDescriptor& meta) { return meta.id == texture_type_int; }) };
    if (iter == m_texture_descriptors.end())
    {
        std::stringstream ss;
        ss << "Unknown TextureType " << texture_type_int << "!";
        showMessageBox(__FUNCTION__, ss.str().c_str());
        throw std::exception{};
    }

    std::ifstream textures_file("textures.bin", std::ios::binary);
    if (!textures_file.is_open())
    {
        showMessageBox(__FUNCTION__, "textures.bin is missing!");
        throw std::exception{};
    }

    textures_file.seekg(iter->position, std::ios::beg);
    if (!textures_file)
    {
        showMessageBox(__FUNCTION__, "textures.bin data is corrupted!");
        throw std::exception{};
    }

    std::vector<char> buffer(iter->size);
    textures_file.read(buffer.data(), iter->size);
    if (!textures_file)
    {
        showMessageBox(__FUNCTION__, "textures.bin data is corrupted!");
        throw std::exception{};
    }

    textures_file.close();
    return buffer;
}

void RenderSystem::processRenderData(World& w, const Entity player_ent)
{
    for (auto& layer : m_sprites_to_render)
    {
        layer.clear();
    }

    const SDL_FPoint screen_size{getScreenSizeF()};

    const SDL_FPoint half_screen_size{ 
        .x = screen_size.x / 2.0f,
        .y = screen_size.y / 2.0f
    };
    const auto& player_transform{ *w.tryGetComponent<TransformComponent>(player_ent) };

    const SDL_FRect camera_rect{
        .x = player_transform.location.x - half_screen_size.x,
        .y = player_transform.location.y - half_screen_size.y,
        .w = screen_size.x,
        .h = screen_size.y
    };

    processPlayerData(w, half_screen_size, player_ent, player_transform);

    w.forEach<SpriteComponent>(
        [this, &w, &player_transform, &half_screen_size, &camera_rect, player_ent]
            (const Entity render_ent, SpriteComponent& sprite)
        {
            if (render_ent == player_ent)
            {
                return;
            }

            const SDL_FPoint texture_size{ 
                getTextureSizeF(sprite.render_data.texture_size)
                };

            const auto& render_obj_transform{ *w.tryGetComponent<TransformComponent>(render_ent) };
            const SDL_FRect render_obj_rect{
                .x = render_obj_transform.location.x - texture_size.x / 2.0f,
                .y = render_obj_transform.location.y - texture_size.y / 2.0f,
                .w = texture_size.x,
                .h = texture_size.y
            };

            SDL_FRect intersect_rect{};
            if (!SDL_IntersectFRect(&camera_rect, &render_obj_rect, &intersect_rect))
            {
                return;
            }

            auto& layer_data{ m_sprites_to_render[static_cast<size_t>(sprite.layer)] };
            layer_data.emplace_back(&sprite);
            sprite.render_data.rotation = render_obj_transform.rotation;

            sprite.render_data.src = createSourceRect(
                std::abs(render_obj_rect.x - intersect_rect.x),
                std::abs(render_obj_rect.y - intersect_rect.y),
                intersect_rect.w, intersect_rect.h
            );

            sprite.render_data.dst = createDestinationRect(
                intersect_rect.x + half_screen_size.x - player_transform.location.x,
                intersect_rect.y + half_screen_size.y - player_transform.location.y,
                intersect_rect.w, intersect_rect.h
            );
        });
}

void RenderSystem::processPlayerData(World& w, const SDL_FPoint& half_screen_size,
    const Entity player_ent, const TransformComponent& player_transform)
{
    auto& sprite{ *w.tryGetComponent<SpriteComponent>(player_ent) };
    sprite.render_data.rotation = player_transform.rotation;
    
    auto& layer_data{ m_sprites_to_render[static_cast<size_t>(sprite.layer)] };
    layer_data.emplace_back(&sprite);

    sprite.render_data.src = createSourceRect(
        0, 0,
        sprite.render_data.texture_size.x,
        sprite.render_data.texture_size.y
        );

    sprite.render_data.dst = createDestinationRect(
        half_screen_size.x - sprite.render_data.texture_size.x / 2,
        half_screen_size.y - sprite.render_data.texture_size.y / 2,
        sprite.render_data.src.w,
        sprite.render_data.src.h
        );
}

SDL_Rect RenderSystem::createSourceRect(const int x, const int y, const int w, const int h)
{
    return SDL_Rect{ .x = x, .y = y, .w = w, .h = h };
}

SDL_FRect RenderSystem::createDestinationRect(const float x, const float y, const float w, const float h)
{
    return SDL_FRect{ .x = roundf(x), .y = ceilf(y),
        .w = ceilf(w), .h = ceilf(h) };
}
