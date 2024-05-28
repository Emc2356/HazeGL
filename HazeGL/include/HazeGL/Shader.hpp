#pragma once

#include <string>
#include <unordered_map>
#include <glad/glad.h>

#include "HazeGL/internal/UniformData.hpp"

#include <iostream>


namespace HazeGL {
    class Shader {
    public:
        Shader(const std::string& vertex_source, const std::string& fragment_source);
        ~Shader(); 
        void bind() const;
        void unbind() const;
        
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