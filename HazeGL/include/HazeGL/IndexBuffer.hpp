#pragma once

#include <glad/glad.h>

#include <stdint.h>
#include <vector>

namespace HazeGL {
    class IndexBuffer {
    public:
        IndexBuffer(uint32_t* data, uint32_t count, uint32_t usage=GL_STATIC_DRAW);
        IndexBuffer(std::vector<uint32_t> data, uint32_t usage=GL_STATIC_DRAW);
        ~IndexBuffer();
        void updateBuffer(uint32_t* data, uint32_t size, uint32_t offset=0);
        void updateBuffer(std::vector<uint32_t> data, uint32_t offset=0);
        void bind() const;
        void unbind() const;
        uint32_t getCount() const;
    private:
        uint32_t mRendererID;
        uint32_t mCount;
    };
}