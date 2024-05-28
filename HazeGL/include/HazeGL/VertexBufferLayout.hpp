#pragma once

#include <glad/glad.h>

#include <vector>

#include <stdint.h>


namespace HazeGL {
    uint32_t getSizeOfGLType(uint32_t type);

    struct VertexBufferElement {
        uint32_t count;
        uint32_t type;
        uint32_t normalized;
    };

    struct VertexBufferLayout {
        VertexBufferLayout() : mStride(0) {}

        void pushFloat(uint32_t count, uint32_t normalized=false) {
            mElements.push_back({count, GL_FLOAT, normalized});
            mStride += getSizeOfGLType(GL_FLOAT) * count;
        }
        void pushUint32(uint32_t count, uint32_t normalized=false) {
            mElements.push_back({count, GL_UNSIGNED_INT, normalized});
            mStride += getSizeOfGLType(GL_UNSIGNED_INT) * count;
        }
        void pushUint8(uint32_t count, uint32_t normalized=false) {
            mElements.push_back({count, GL_UNSIGNED_BYTE, normalized});
            mStride += getSizeOfGLType(GL_UNSIGNED_BYTE) * count;
        }

        uint32_t getStride() const { return mStride; }
        const std::vector<VertexBufferElement>& getElements() const { return mElements; }
    private:
        std::vector<VertexBufferElement> mElements;
        uint32_t mStride;
    };
}
