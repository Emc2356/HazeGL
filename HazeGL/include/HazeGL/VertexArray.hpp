#pragma once

#include <glad/glad.h>

#include "HazeGL/VertexBuffer.hpp"
#include "HazeGL/VertexBufferLayout.hpp"

#include <vector>
#include <stdexcept>


namespace HazeGL {
    class VertexArray {
    public:
        VertexArray();
        ~VertexArray();

        void addBuffer(const VertexBuffer&, const VertexBufferLayout&);

        void bind() const;
        void unbind() const;
    private:
        uint32_t mRendererID;
    };
}