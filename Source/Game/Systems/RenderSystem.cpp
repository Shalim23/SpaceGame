#include "RenderSystem.h"
#include "../World.h"
#include "../SystemsManager.h"
#include "../Types/Exceptions.h"
#include "SDL_image.h"
#include <fstream>
#include <sstream>

void RenderSystem::init(World& w, SystemsManager& sm)
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

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if (!m_renderer)
    {
        showMessageBox(__FUNCTION__, "Failed to create renderer!");
        throw SystemInitException{};
    }

    initTexturesDescriptors();
}

void RenderSystem::update(World& w)
{

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
    const auto& new_texture{ m_textures.emplace_back(Texture{type, texture, texture_size}) };

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
