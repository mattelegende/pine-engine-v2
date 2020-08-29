#include "Texture2D.hpp"
#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "../../Core/Log/Log.hpp"

Pine::Texture2D::Texture2D() {
    m_Type = EAssetType::Texture;
}

unsigned int Pine::Texture2D::GetId() const {
    return m_Id;
}

int Pine::Texture2D::GetWidth() const {
    return m_Width;
}

int Pine::Texture2D::GetHeight() const {
    return m_Height;
}

const int Pine::Texture2D::GetAssetPreview() const {
    return m_Id;
}

bool Pine::Texture2D::LoadFromFile() {
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

bool Pine::Texture2D::SaveToFile() {
    return false;
}

void Pine::Texture2D::Dispose() {
    glDeleteTextures(1, &m_Id);
}

void Pine::Texture2D::GenerateAssetPreview() {
    // No need to forgive me for this, it's C++!
    m_PreviewFrameBuffer = reinterpret_cast<FrameBuffer*>(0x1);
}