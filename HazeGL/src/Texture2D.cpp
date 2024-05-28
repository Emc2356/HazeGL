#include "HazeGL/Texture2D.hpp"

#include <stb/stb_image.h>

#include <stdexcept>
#include <string.h>


HazeGL::Texture2D::Texture2D() { 
    mRendererID = 0;
    mFilepath = "";
    mLocalBuffer = nullptr;
    mWidth = 0;
    mHeight = 0;
    mBPP = 0;
    mInternalFormat = 0;
    mFormat = 0;
    mType = 0;
}

HazeGL::Texture2D::Texture2D(const std::string& filepath, bool flipped/*=false*/, int internalformat/*=GL_RGBA8*/, unsigned format/*=GL_RGBA*/, uint32_t type/*=GL_UNSIGNED_BYTE*/) {
    stbi_set_flip_vertically_on_load(flipped ? 1 : 0);

    if (!(mLocalBuffer = stbi_load(filepath.c_str(), &mWidth, &mHeight, &mBPP, 4))) {
        std::string failure_reason = (std::string)stbi_failure_reason();
        throw std::runtime_error("failed to load image `" + filepath + "` for the Texture2D: " + failure_reason + "\n");
    }

    glGenTextures(1, &mRendererID);
    glBindTexture(GL_TEXTURE_2D, mRendererID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, internalformat, mWidth, mHeight, 0, format, type, mLocalBuffer);

    mInternalFormat = internalformat;
    mFormat = format;
    mType = type;

    stbi_image_free(mLocalBuffer);
}

HazeGL::Texture2D::Texture2D(const int width, const int height, unsigned char* buffer/*=nullptr*/, int internalformat/*=GL_RGBA8*/, unsigned format/*=GL_RGBA*/, uint32_t type/*=GL_UNSIGNED_BYTE*/) {
    mInternalFormat = internalformat;
    mWidth = width;
    mHeight = height;
    mFormat = format;
    mType = type;
    mLocalBuffer = buffer;

    glGenTextures(1, &mRendererID);
    glBindTexture(GL_TEXTURE_2D, mRendererID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, mInternalFormat, mWidth, mHeight, 0, mFormat, mType, mLocalBuffer);
}

void HazeGL::Texture2D::updateData(unsigned char* buffer, int internalformat/*=GL_RGBA8*/, unsigned format/*=GL_RGBA*/, uint32_t type/*=GL_UNSIGNED_BYTE*/) {
    mInternalFormat = internalformat;
    mFormat = format;
    mType = type;
    mLocalBuffer = buffer;

    bind();

    glTexImage2D(GL_TEXTURE_2D, 0, mInternalFormat, mWidth, mHeight, 0, mFormat, mType, mLocalBuffer);
}

void HazeGL::Texture2D::updateData(const std::string& filepath, bool flipped/*=false*/, int internalformat/*=GL_RGBA8*/, unsigned format/*=GL_RGBA*/, uint32_t type/*=GL_UNSIGNED_BYTE*/) {
    mInternalFormat = internalformat;
    mFormat = format;
    mType = type;
    mFilepath = filepath;

    bind();

    stbi_set_flip_vertically_on_load(flipped ? 1 : 0);

    if (!(mLocalBuffer = stbi_load(filepath.c_str(), &mWidth, &mHeight, &mBPP, 4))) {
        std::string failure_reason = (std::string)stbi_failure_reason();
        throw std::runtime_error("failed to load image `" + filepath + "` for the Texture2D: " + failure_reason + "\n");
    }

    glTexImage2D(GL_TEXTURE_2D, 0, mInternalFormat, mWidth, mHeight, 0, mFormat, mType, mLocalBuffer);
    
    stbi_image_free(mLocalBuffer);
}

HazeGL::Texture2D::~Texture2D() {
    glDeleteTextures(1, &mRendererID);
}

void HazeGL::Texture2D::setFilters(int mag, int min) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
}

void HazeGL::Texture2D::setRepeatX(bool v) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, v ? GL_REPEAT : GL_CLAMP_TO_EDGE);
}

void HazeGL::Texture2D::setRepeatY(bool v) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, v ? GL_REPEAT : GL_CLAMP_TO_EDGE);
}

int32_t HazeGL::Texture2D::bindToImage(uint32_t slot, bool read, bool write, int32_t level, int32_t format) const {
    int access = GL_READ_WRITE;
    if (read && !write) access = GL_READ_ONLY;
    else if (!read && write) access = GL_WRITE_ONLY;
    else if (!read && !write) {
        throw std::runtime_error("read or write must be set to true");
    }

    int frmt = format ? format : mInternalFormat;

    glBindImageTexture(slot, mRendererID, level, 0, 0, access, frmt);
    return slot;
}

int32_t HazeGL::Texture2D::bind(uint32_t slot /*=0*/) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, mRendererID);
    return (int32_t)slot;
}

void HazeGL::Texture2D::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

int HazeGL::Texture2D::getWidth() const { return mWidth; }
int HazeGL::Texture2D::getHeight() const { return mHeight; }
int HazeGL::Texture2D::getBPP() const { return mBPP; }