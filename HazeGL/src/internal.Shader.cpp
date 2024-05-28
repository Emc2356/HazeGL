#include "HazeGL/internal/Shader.hpp"

#include <vector>


uint32_t HazeGL::internal::compileSingleShader(uint32_t type, const std::string& source) {
    std::string type_str;
    switch (type) {
    case GL_VERTEX_SHADER:
        type_str = "vertex";
        break;
    case GL_FRAGMENT_SHADER:
        type_str = "fragment";
        break;
    case GL_COMPUTE_SHADER:
        type_str = "compute";
        break;
    default:
        std::cerr << "unknown/unsuported shader type: " << type << "\n";
        return 0;
    }
    uint32_t id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> message(1024);
        glGetShaderInfoLog(id, length, &length, message.data());

        std::cerr << "failed while compiling " << type_str << " shader!\n" << message.data();

        glDeleteShader(id);
        return 0;
    }

    return id;
}