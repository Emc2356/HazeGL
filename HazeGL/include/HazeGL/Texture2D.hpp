#pragma once

#include <glad/glad.h>
#include <string>

#include <stdint.h>


namespace HazeGL {
    class Texture2D {
    public:
        Texture2D(const std::string& filepath, bool flipped=false, int internalformat=GL_RGBA8, unsigned format=GL_RGBA, uint32_t type=GL_UNSIGNED_BYTE);
        Texture2D(const int width, const int height, unsigned char* buffer=nullptr, int internalformat=GL_RGBA8, unsigned format=GL_RGBA, uint32_t type=GL_UNSIGNED_BYTE);
        Texture2D();
        ~Texture2D();

        void updateData(unsigned char* buffer, int internalformat=GL_RGBA8, unsigned format=GL_RGBA, uint32_t type=GL_UNSIGNED_BYTE);
        void updateData(const std::string& filepath, bool flipped=false, int internalformat=GL_RGBA8, unsigned format=GL_RGBA, uint32_t type=GL_UNSIGNED_BYTE);

        int32_t bind(uint32_t slot=0) const;
        void unbind() const;

        int32_t bindToImage(uint32_t slot=0, bool read=true, bool write=true, int32_t level=0, int32_t format=0) const;
    
        void setFilters(int min, int mag);
        void setRepeatX(bool v);
        void setRepeatY(bool v);

        int getWidth() const;
        int getHeight() const;
        int getBPP() const;

    private:
        uint32_t mRendererID;
        std::string mFilepath;
        uint8_t* mLocalBuffer;
        int mWidth;
        int mHeight;
        // bits per pixel
        int mBPP;
        int mInternalFormat;
        uint32_t mFormat;
        uint32_t mType;
    };
}
