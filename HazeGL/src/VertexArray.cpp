#include <glad/glad.h>

#include "HazeGL/VertexArray.hpp"


uint32_t HazeGL::getSizeOfGLType(uint32_t type) {
    switch (type) {
    case GL_FLOAT: 
        return sizeof(float);
    case GL_UNSIGNED_INT: 
        return sizeof(uint32_t);
    case GL_UNSIGNED_BYTE: 
        return sizeof(uint8_t);
    default: {
        std::string str = "Unknown type: ";
        str += std::to_string(type);
        throw std::runtime_error(str);
        return 0;
    }
    }
}

HazeGL::VertexArray::VertexArray() {
    glGenVertexArrays(1, &mRendererID);
}

HazeGL::VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &mRendererID);
}

void HazeGL::VertexArray::bind() const {
    glBindVertexArray(mRendererID);
}

void HazeGL::VertexArray::unbind() const {
    glBindVertexArray(0);
}

void HazeGL::VertexArray::addBuffer(const VertexBuffer& vbo, const HazeGL::VertexBufferLayout& layout) {
    vbo.bind();
    bind();

    const std::vector<HazeGL::VertexBufferElement>& elements = layout.getElements();

    uint64_t offset = 0;

    for (uint32_t i = 0; i < elements.size(); i++) {
        const VertexBufferElement& element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const uint64_t*)offset);

        offset += element.count * HazeGL::getSizeOfGLType(element.type);
    }
}