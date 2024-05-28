#include "HazeGL/Shader.hpp"
#include "HazeGL/internal/Shader.hpp"


#include <stdio.h>
#include <stdexcept>
#include <vector>



static uint32_t mCompileSimpleShaderProgram(const std::string& vertex_source, const std::string& fragment_source) {
    uint32_t vs = HazeGL::internal::compileSingleShader(GL_VERTEX_SHADER, vertex_source);
    uint32_t fs = HazeGL::internal::compileSingleShader(GL_FRAGMENT_SHADER, fragment_source);

    if (!vs || !fs) {
        throw std::runtime_error("failed while creating the shader");
    }

    uint32_t id = glCreateProgram();

    glAttachShader(id, vs);
    glAttachShader(id, fs);

    glLinkProgram(id);

    int linked;
    glGetProgramiv(id, GL_LINK_STATUS, &linked);
    if (linked == GL_FALSE) {
        int length;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> message(length * sizeof(char));
        glGetProgramInfoLog(id, length, &length, message.data());

        glDeleteShader(fs);
        glDeleteShader(vs);
        glDeleteProgram(id);

        throw std::runtime_error((std::string)"failed while linking the program\n==========\n" + (std::string)message.data());
    }

    glValidateProgram(id);

    int valid;
    glGetProgramiv(id, GL_LINK_STATUS, &valid);
    if (valid == GL_FALSE) {
        int length;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> message(length * sizeof(char));
        glGetProgramInfoLog(id, length, &length, message.data());

        glDeleteShader(fs);
        glDeleteShader(vs);
        glDeleteProgram(id);
        
        throw std::runtime_error((std::string)"program can not run\n==========\n" + (std::string)message.data());
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return id;
}

HazeGL::Shader::Shader(const std::string& vertex_source, const std::string& fragment_source) {
    mRendererID = mCompileSimpleShaderProgram(vertex_source, fragment_source);

    int uniform_count;
    glGetProgramiv(mRendererID, GL_ACTIVE_UNIFORMS, &uniform_count);
    std::vector<char> uniform_name(1024);
    int size;
    uint32_t type;
    for (int loc = 0; loc < uniform_count; loc++) {
        glGetActiveUniform(mRendererID, loc, uniform_name.size(), nullptr, &size, &type, uniform_name.data());

        mUniforms[(std::string)uniform_name.data()] = HazeGL::internal::Uniform(
            (std::string)uniform_name.data(), 
            loc,
            type
        );
    }
}

HazeGL::Shader::~Shader() {
    glDeleteProgram(mRendererID);
}

void HazeGL::Shader::bind() const {
    glUseProgram(mRendererID);
}

void HazeGL::Shader::unbind() const {
    glUseProgram(0);
}