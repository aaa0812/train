#include "textures.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "tools/stb_image.h"

GLBI_Texture metalTexture;
GLBI_Texture lightMetalTexture;
GLBI_Texture groundTexture;
GLBI_Texture goldTexture;
GLBI_Texture dirtyWoodTexture;
GLBI_Texture rustedWoodTexture;
GLBI_Texture plankWoodTexture;
GLBI_Texture crateTexture;

void loadTextures()
{
    // Load texture image from file **METAL TEXTURE**
    int tex_w = 0, tex_h = 0, tex_channels = 0;
    unsigned char *pixels = stbi_load("../assets/textures/metal_texture.png", &tex_w, &tex_h, &tex_channels, 0);
    if (pixels != nullptr)
    {
        metalTexture.createTexture();
        metalTexture.attachTexture();
        metalTexture.loadImage((unsigned int)tex_w, (unsigned int)tex_h, (unsigned int)tex_channels, pixels);
        metalTexture.setParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        metalTexture.setParameters(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        metalTexture.detachTexture();
        stbi_image_free(pixels);
    }
    else
    {
        std::cerr << "failed to load metal_texture.png: " << stbi_failure_reason() << std::endl;
    }

    // Load texture image from file **GOLD TEXTURE**
    pixels = stbi_load("../assets/textures/gold_texture.png", &tex_w, &tex_h, &tex_channels, 0);
    if (pixels != nullptr)
    {
        goldTexture.createTexture();
        goldTexture.attachTexture();
        goldTexture.loadImage((unsigned int)tex_w, (unsigned int)tex_h, (unsigned int)tex_channels, pixels);
        goldTexture.setParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        goldTexture.setParameters(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        goldTexture.detachTexture();
        stbi_image_free(pixels);
    }
    else
    {
        std::cerr << "failed to load gold_texture.png: " << stbi_failure_reason() << std::endl;
    }

    // Load texture image from file **LIGHT METAL TEXTURE**
    pixels = stbi_load("../assets/textures/light_metal_texture.png", &tex_w, &tex_h, &tex_channels, 0);
    if (pixels != nullptr)
    {
        lightMetalTexture.createTexture();
        lightMetalTexture.attachTexture();
        lightMetalTexture.loadImage((unsigned int)tex_w, (unsigned int)tex_h, (unsigned int)tex_channels, pixels);
        lightMetalTexture.setParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        lightMetalTexture.setParameters(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        lightMetalTexture.detachTexture();
        stbi_image_free(pixels);
    }
    else
    {
        std::cerr << "failed to load light_metal_texture.png: " << stbi_failure_reason() << std::endl;
    }

    // Load texture image from file **DIRTY WOOD TEXTURE**
    pixels = stbi_load("../assets/textures/dirty_wood_texture.png", &tex_w, &tex_h, &tex_channels, 0);
    if (pixels != nullptr)
    {
        dirtyWoodTexture.createTexture();
        dirtyWoodTexture.attachTexture();
        dirtyWoodTexture.loadImage((unsigned int)tex_w, (unsigned int)tex_h, (unsigned int)tex_channels, pixels);
        dirtyWoodTexture.setParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        dirtyWoodTexture.setParameters(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        dirtyWoodTexture.detachTexture();
        stbi_image_free(pixels);
    }
    else
    {
        std::cerr << "failed to load dirty_wood_texture.png: " << stbi_failure_reason() << std::endl;
    }

    // Load texture image from file **RUSTED WOOD TEXTURE**
    pixels = stbi_load("../assets/textures/rusted_wood_texture.png", &tex_w, &tex_h, &tex_channels, 0);
    if (pixels != nullptr)
    {
        rustedWoodTexture.createTexture();
        rustedWoodTexture.attachTexture();
        rustedWoodTexture.loadImage((unsigned int)tex_w, (unsigned int)tex_h, (unsigned int)tex_channels, pixels);
        rustedWoodTexture.setParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        rustedWoodTexture.setParameters(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        rustedWoodTexture.detachTexture();
        stbi_image_free(pixels);
    }
    else
    {
        std::cerr << "failed to load rusted_wood_texture.png: " << stbi_failure_reason() << std::endl;
    }

    // Load texture image from file **PLANK WOOD TEXTURE**
    pixels = stbi_load("../assets/textures/plank_wood_texture.png", &tex_w, &tex_h, &tex_channels, 0);
    if (pixels != nullptr)
    {
        plankWoodTexture.createTexture();
        plankWoodTexture.attachTexture();
        plankWoodTexture.loadImage((unsigned int)tex_w, (unsigned int)tex_h, (unsigned int)tex_channels, pixels);
        plankWoodTexture.setParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        plankWoodTexture.setParameters(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        plankWoodTexture.detachTexture();
        stbi_image_free(pixels);
    }
    else
    {
        std::cerr << "failed to load plank_wood_texture.png: " << stbi_failure_reason() << std::endl;
    }

    // Load texture image from file **GROUND TEXTURE**
    pixels = stbi_load("../assets/textures/ground_texture.png", &tex_w, &tex_h, &tex_channels, 0);
    if (pixels != nullptr)
    {
        groundTexture.createTexture();
        groundTexture.attachTexture();
        groundTexture.loadImage((unsigned int)tex_w, (unsigned int)tex_h, (unsigned int)tex_channels, pixels);
        groundTexture.setParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        groundTexture.setParameters(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        groundTexture.detachTexture();
        stbi_image_free(pixels);
    }
    else
    {
        std::cerr << "failed to load ground_texture.png: " << stbi_failure_reason() << std::endl;
    }

     // Load texture image from file **crate TEXTURE**
    pixels = stbi_load("../assets/textures/crate_texture.png", &tex_w, &tex_h, &tex_channels, 0);
    if (pixels != nullptr)
    {
        crateTexture.createTexture();
        crateTexture.attachTexture();
        crateTexture.loadImage((unsigned int)tex_w, (unsigned int)tex_h, (unsigned int)tex_channels, pixels);
        crateTexture.setParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        crateTexture.setParameters(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        crateTexture.detachTexture();
        stbi_image_free(pixels);
    }
    else
    {
        std::cerr << "failed to load crate_texture.png: " << stbi_failure_reason() << std::endl;
    }
}