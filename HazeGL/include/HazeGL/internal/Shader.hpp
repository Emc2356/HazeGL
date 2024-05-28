#include <glad/glad.h>

#include <stdexcept>
#include <iostream>


namespace HazeGL::internal {
    uint32_t compileSingleShader(uint32_t type, const std::string& source);
}
