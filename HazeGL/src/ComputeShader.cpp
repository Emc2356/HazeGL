#include "HazeGL/ComputeShader.hpp"
#include "HazeGL/internal/Shader.hpp"


static uint32_t mCompileComputeShaderProgram(const std::string& source) {
    uint32_t cs = HazeGL::internal::compileSingleShader(GL_COMPUTE_SHADER, source);

    if (!cs) {
        throw std::runtime_error("failed while creating the shader");
    }

    int id = glCreateProgram();

    glAttachShader(id, cs);

    glLinkProgram(id);

    int linked;
    glGetProgramiv(id, GL_LINK_STATUS, &linked);
    if (linked == GL_FALSE) {
        int length;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> message(length * sizeof(char));
        glGetProgramInfoLog(id, length, &length, message.data());

        glDeleteShader(cs);
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

        glDeleteShader(cs);
        glDeleteProgram(id);
        
        throw std::runtime_error((std::string)"program can not run\n==========\n" + (std::string)message.data());
    }

    glDeleteShader(cs);

    return id;
}


HazeGL::ComputeShader::ComputeShader(const std::string& source) {
    mRendererID = mCompileComputeShaderProgram(source);
    
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

HazeGL::ComputeShader::~ComputeShader() {
    if (mRendererID != 0) {
        glDeleteProgram(mRendererID);
    }    
}

void HazeGL::ComputeShader::bind() const {
    glUseProgram(mRendererID);
}

void HazeGL::ComputeShader::unbind() const {
    glUseProgram(0);
}

void HazeGL::ComputeShader::dispatch(uint32_t x, uint32_t y/*=1*/, uint32_t z/*=1*/, uint32_t barrier/*=GL_ALL_BARRIER_BITS*/) const { 
    bind();
    glDispatchCompute(x, y, z);
    if (barrier) {
        glMemoryBarrier(barrier);
    }
}
