#pragma once

#include <glad/glad.h>

#include <stdint.h>
#include <vector>

namespace HazeGL {
    class VertexBuffer {
    public:
        VertexBuffer(float* data, uint32_t count, uint32_t usage=GL_STATIC_DRAW);
        VertexBuffer(std::vector<float> data, uint32_t usage=GL_STATIC_DRAW);
        ~VertexBuffer();
        void updateBuffer(float* data, uint32_t size, uint32_t offset=0);
        void updateBuffer(std::vector<float> data, uint32_t offset=0);
        void bind() const;
        void unbind() const;
        uint32_t getSize() const;
    private:
        uint32_t mRendererID;
        uint32_t mSize;
    };
}