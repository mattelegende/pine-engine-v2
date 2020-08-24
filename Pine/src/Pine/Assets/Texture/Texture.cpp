#include "Texture.hpp"
#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "../../Core/Log/Log.hpp"

Pine::Texture::Texture() {
    m_Type = EAssetType::Texture;
}

unsigned int Pine::Texture::GetId() const {
    return m_Id;
}

int Pine::Texture::GetWidth() const {
    return m_Width;
}

int Pine::Texture::GetHeight() const {
    return m_Height;
}

bool Pine::Texture::LoadFromFile() {
    int width, height, channels;

    const auto data = stbi_load(m_FilePath.string().c_str(), &width, &height, &channels, 0);
    
    if (!data) {
		stbi_image_free(data);

        Log::Error("Failed to load texture, " + m_FilePath.string());

        return false;
    }

    glGenTextures(1, &m_Id);
    glBindTexture(GL_TEXTURE_2D, m_Id);
    
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    const int format = channels == 4 ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    m_Width = width;
    m_Height = height;

    stbi_image_free(data);

    return true;
}

bool Pine::Texture::SaveToFile() {
    return false;
}

void Pine::Texture::Dispose() {
    glDeleteTextures(1, &m_Id);
}