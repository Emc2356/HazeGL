#pragma once 
#include <glad/glad.h>
#include <stdint.h>
#include <stdio.h>
#include <vector>


namespace HazeGL {
    template<class T>
    class ShaderStorageBuffer {
    public:
        ShaderStorageBuffer(T* data, uint32_t count, uint32_t usage=GL_DYNAMIC_DRAW) {
            glGenBuffers(1, &mRendererID);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, mRendererID);
            glBufferData(GL_SHADER_STORAGE_BUFFER, count * sizeof(T), (void*)data, usage);

            mSize = count * sizeof(T);
        }

        ShaderStorageBuffer(uint32_t reserve, uint32_t usage=GL_DYNAMIC_DRAW) {
            glGenBuffers(1, &mRendererID);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, mRendererID);
            glBufferData(GL_SHADER_STORAGE_BUFFER, reserve * sizeof(T), nullptr, usage);

            mSize = reserve * sizeof(T);
        }

        ShaderStorageBuffer(std::vector<T> data, uint32_t usage=GL_DYNAMIC_DRAW) {
            glGenBuffers(1, &mRendererID);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, mRendererID);
            glBufferData(GL_SHADER_STORAGE_BUFFER, data.size() * sizeof(T), (void*)data.data(), usage);

            mSize = data.size() * sizeof(T);
        }

        ShaderStorageBuffer() {
            mRendererID = 0;
            mSize = 0;
        }
        
        ~ShaderStorageBuffer() {
            if (mRendererID != 0) {
                glDeleteBuffers(1, &mRendererID);
            }
        }

        void BindToStorageBuffer(int binding, int offset=0, int size=-1) const {
            if (size == -1) {
                size = mSize - offset;
            }

            glBindBufferRange(GL_SHADER_STORAGE_BUFFER, binding, mRendererID, offset, size);
        }

        uint32_t getSize() const {
            return mSize;
        }

        void bind() const {
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, mRendererID);
        }

        void unbind() const {
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
        }

        void updateBuffer(T* data, uint32_t size, uint32_t offset=0) { 
            bind();
            glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, size * sizeof(T), data);    
        }

        void updateBuffer(std::vector<T> data, uint32_t offset=0) { 
            updateBuffer(data.data(), data.size(), offset);
        }

    private:
        uint32_t mRendererID;
        uint32_t mSize;
    };
}
