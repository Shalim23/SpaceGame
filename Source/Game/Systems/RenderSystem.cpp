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

    m_window = SDL_CreateWindow("Space Game", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, 800, 600, 0/*SDL_WINDOW_FULLSCREEN_DESKTOP*/);
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
            RenderData render_data{gatherRenderData(w, ent)};
            for (const auto& layer : render_data)
            {
                for (const auto& data : layer)
                {
                    SDL_RenderCopyExF(m_renderer, data->texture,
                        &data->src, &data->dst, data->rotation, nullptr, SDL_FLIP_NONE);
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

SDL_Point RenderSystem::getScreenSize() const
{
    SDL_Point s;
    SDL_GetRendererOutputSize(m_renderer, &s.x, &s.y);
    return s;
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

RenderSystem::RenderData RenderSystem::gatherRenderData(World& w, const Entity player_ent) const
{
    RenderData render_data{ static_cast<size_t>(RenderLayer::COUNT) };

    const SDL_Point screen_size{ getScreenSize() };
    const SDL_FPoint screen_size_f{ 
        .x = static_cast<float>(screen_size.x),
        .y = static_cast<float>(screen_size.y)
        };

    const SDL_FPoint half_screen_size{ 
        .x = screen_size_f.x / 2.0f,
        .y = screen_size_f.y / 2.0f
    };
    const auto& player_transform{ *w.tryGetComponent<TransformComponent>(player_ent) };

    const SDL_FRect camera_rect{
        .x = player_transform.location.x - half_screen_size.x,
        .y = player_transform.location.y - half_screen_size.y,
        .w = screen_size_f.x,
        .h = screen_size_f.y
    };

    processPlayerData(w, render_data, half_screen_size, player_ent, player_transform);

    w.forEach<RenderComponent>(
        [this, &render_data, &w, &player_transform, &half_screen_size, &camera_rect, player_ent]
            (const Entity render_ent, RenderComponent& render_comp)
        {
            if (render_ent == player_ent)
            {
                return;
            }

            const SDL_FPoint texture_size{ 
                .x = static_cast<float>(render_comp.texture_size.x),
                .y = static_cast<float>(render_comp.texture_size.y)
                };

            const auto& render_obj_transform{ *w.tryGetComponent<TransformComponent>(render_ent) };
            const SDL_FRect render_obj_rect{
                .x = render_obj_transform.location.x - texture_size.x / 2.0f,
                .y = render_obj_transform.location.y - texture_size.y / 2.0f,
                .w = texture_size.x + 1.0f,
                .h = texture_size.y + 1.0f
            };

            SDL_FRect intersect_rect{};
            if (!SDL_IntersectFRect(&camera_rect, &render_obj_rect, &intersect_rect))
            {
                return;
            }

            auto& layer_data{ render_data[static_cast<size_t>(render_comp.layer)] };
            layer_data.emplace_back(&render_comp);
            render_comp.rotation = render_obj_transform.rotation;
            render_comp.src = {
                .x = static_cast<int>(std::abs(render_obj_rect.x - intersect_rect.x)),
                .y = static_cast<int>(std::abs(render_obj_rect.y - intersect_rect.y)),
                .w = static_cast<int>(intersect_rect.w),
                .h = static_cast<int>(intersect_rect.h)
            };

            render_comp.dst = {
                .x = intersect_rect.x + half_screen_size.x - player_transform.location.x,
                .y = intersect_rect.y + half_screen_size.y - player_transform.location.y,
                .w = static_cast<float>(render_comp.src.w),
                .h = static_cast<float>(render_comp.src.h)
            };
        });

    return render_data;
}

void RenderSystem::processPlayerData(World& w, RenderData& render_data,
    const SDL_FPoint& half_screen_size, const Entity player_ent, const TransformComponent& player_transform) const
{
    auto& render_comp{ *w.tryGetComponent<RenderComponent>(player_ent) };
    render_comp.rotation = player_transform.rotation;
    
    auto& layer_data{ render_data[static_cast<size_t>(render_comp.layer)] };
    layer_data.emplace_back(&render_comp);

    render_comp.src = {
        .x = 0,
        .y = 0,
        .w = render_comp.texture_size.x,
        .h = render_comp.texture_size.y
    };

    render_comp.dst = {
        .x = half_screen_size.x - render_comp.texture_size.x / 2,
        .y = half_screen_size.y - render_comp.texture_size.y / 2,
        .w = static_cast<float>(render_comp.src.w),
        .h = static_cast<float>(render_comp.src.h)
    };
}
