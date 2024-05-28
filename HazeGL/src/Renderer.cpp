#include "HazeGL/Renderer.hpp"

void HazeGL::Renderer::draw(const HazeGL::VertexArray& vao, const HazeGL::IndexBuffer& ibo, const HazeGL::Shader& shader) const {
    shader.bind();
    vao.bind();
    ibo.bind();
    glDrawElements(GL_TRIANGLES, ibo.getCount(), GL_UNSIGNED_INT, nullptr);
}