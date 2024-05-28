#include "HazeGL/VertexBuffer.hpp"


HazeGL::VertexBuffer::VertexBuffer(float* data, uint32_t count, uint32_t usage/*=GL_STATIC_DRAW*/) {
    glGenBuffers(1, &mRendererID);
    glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), (void*)data, usage);

    mSize = count * sizeof(float);
}

HazeGL::VertexBuffer::VertexBuffer(std::vector<float> data, uint32_t usage/*=GL_STATIC_DRAW*/) {
    glGenBuffers(1, &mRendererID);
    glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), (void*)data.data(), usage);

    mSize = data.size() * sizeof(float);
}

uint32_t HazeGL::VertexBuffer::getSize() const {
    return mSize;
}

HazeGL::VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &mRendererID);
}

void HazeGL::VertexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
}

void HazeGL::VertexBuffer::unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void HazeGL::VertexBuffer::updateBuffer(float* data, uint32_t size, uint32_t offset/*=0*/) { 
    bind();
    glBufferSubData(GL_ARRAY_BUFFER, offset, size * sizeof(float), data);    
}

void HazeGL::VertexBuffer::updateBuffer(std::vector<float> data, uint32_t offset/*=0*/) { 
    bind(); 
    glBufferSubData(GL_ARRAY_BUFFER, offset, data.size() * sizeof(float), data.data());
}