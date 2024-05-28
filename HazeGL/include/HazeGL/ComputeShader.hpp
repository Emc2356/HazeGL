#pragma once
#include "HazeGL/internal/UniformData.hpp"

#include <string>
#include <unordered_map>
#include <iostream>

#include <stdint.h>


namespace HazeGL {
    class ComputeShader {
    public:
        ComputeShader(const std::string& source);
        ~ComputeShader(); 
        void bind() const;
        void unbind() const;
        void dispatch(uint32_t x, uint32_t y=1, uint32_t z=1, uint32_t barrier=GL_ALL_BARRIER_BITS) const;
        
        template<typename T>
        bool setUniform(const std::string& key, const T& value, const bool debug=false) {
            if (!(mUniforms.find(key) != mUniforms.end())) {
                if (debug) {
                    std::cerr << "[ERROR]: Uniform `" << key << "` doesnt exist!\n";
                }
                return false;
            }
            bind();
            mUniforms[key].setUniform(value);

            return true;
        }
    private:
        std::unordered_map<std::string, HazeGL::internal::Uniform> mUniforms;
        int mRendererID;
    };
}
