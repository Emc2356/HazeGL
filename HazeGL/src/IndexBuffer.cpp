#include "HazeGL/IndexBuffer.hpp"


HazeGL::IndexBuffer::IndexBuffer(uint32_t* data, uint32_t count, uint32_t usage/*=GL_STATIC_DRAW*/) {
    glGenBuffers(1, &mRendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), (void*)data, usage);

    mCount = count;
}

HazeGL::IndexBuffer::IndexBuffer(std::vector<uint32_t> data, uint32_t usage/*=GL_STATIC_DRAW*/) {
    glGenBuffers(1, &mRendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(uint32_t), (void*)data.data(), usage);

    mCount = data.size();
}

uint32_t HazeGL::IndexBuffer::getCount() const {
    return mCount;
}

HazeGL::IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &mRendererID);
}

void HazeGL::IndexBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
}

void HazeGL::IndexBuffer::unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void HazeGL::IndexBuffer::updateBuffer(uint32_t* data, uint32_t size, uint32_t offset/*=0*/) { 
    bind();
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size * sizeof(uint32_t), data);    
}

void HazeGL::IndexBuffer::updateBuffer(std::vector<uint32_t> data, uint32_t offset/*=0*/) { 
    bind(); 
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, data.size() * sizeof(uint32_t), data.data());
}