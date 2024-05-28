#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "HazeGL/VertexArray.hpp"
#include "HazeGL/IndexBuffer.hpp"
#include "HazeGL/Shader.hpp"

namespace HazeGL {
    class Renderer {
    public:
    void draw(const HazeGL::VertexArray& vao, const HazeGL::IndexBuffer& ibo, const HazeGL::Shader& shader) const;
    
    private:
    };
}