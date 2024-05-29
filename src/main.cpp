#include <HazeGL/HazeGL>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <vector>


std::string readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("failed to open file: " + path);
    }
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    std::string buffer(size, ' ');
    file.seekg(0);
    file.read(&buffer[0], size);
    return buffer;
}


int main(void) {
    HazeGL::Window window(1280, 720, "Hello World");

    window.setDebug(true);
    window.initialize(450, GLFW_OPENGL_CORE_PROFILE);

    glfwSetWindowSizeCallback(window.getWindow(), [](GLFWwindow* window, int width, int height) {
        (void) window;
        glViewport(0, 0, width, height);
    });

    window.enableBlending();

    glm::mat4 projection = glm::ortho(
        0.0f, (float)window.getWidth(),
        (float)window.getHeight(), 0.0f,
        -1.0f, 1.0f
    );

    (void) projection;

    std::vector<float> verticies = {
        // TODO: fix coordinates so the image apears correctly
        0.0f  , 500.0f, 0.0f, 0.0f, // bottom left
        0.0f  , 0.0f  , 0.0f, 1.0f, // top left
        500.0f, 0.0f  , 1.0f, 1.0f, // top right
        500.0f, 500.0f, 1.0f, 0.0f, // bottom right
        0.0f  , 500.0f, 0.0f, 0.0f, // bottom left
        0.0f  , 0.0f  , 0.0f, 1.0f, // top left
        500.0f, 0.0f  , 1.0f, 1.0f, // top right
        500.0f, 500.0f, 1.0f, 0.0f, // bottom right
    };

    std::vector<uint32_t> indicies = {
        0, 1, 2,
        2, 3, 0,
        4, 5, 6,
        6, 7, 4,
    };

    HazeGL::VertexArray vao;
    HazeGL::VertexBuffer vbo(verticies, GL_DYNAMIC_DRAW);
    HazeGL::IndexBuffer ibo(indicies);

    HazeGL::VertexBufferLayout layout;
    layout.pushFloat(2); // xy
    layout.pushFloat(2); // uv

    vao.addBuffer(vbo, layout);

    HazeGL::Shader shader(readFile("./src/glsl/main.vert"), readFile("./src/glsl/main.frag"));
    HazeGL::ComputeShader computeShader(readFile("./src/glsl/main.comp"));

    HazeGL::Texture2D texture(512, 512, nullptr);

    std::vector<glm::vec3> some_data = {
        glm::vec3(5.5f, 1.5f, 5.0f)
    };

    HazeGL::ShaderStorageBuffer<glm::vec3> ssbo(some_data);

    HazeGL::Renderer renderer;

    /* Loop until the user closes the window */
    while (!window.shouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT);
        computeShader.bind();
        computeShader.setUniform("uTexture", texture.bindToImage(1, false, true));
        computeShader.dispatch(texture.getWidth() / 16, texture.getHeight() / 16, 1);


        vao.bind();
        ibo.bind();
        shader.bind();

        float x, y;
        window.getMousePosition(&x, &y);

        std::vector<float> update_verticies = verticies;
        update_verticies[0] += x;
        update_verticies[1] += y;
        update_verticies[4] += x;
        update_verticies[5] += y;
        update_verticies[8] += x;
        update_verticies[9] += y;
        update_verticies[12] += x;
        update_verticies[13] += y;

        vbo.updateBuffer(update_verticies);

        ssbo.BindToStorageBuffer(0);

        shader.setUniform("uMVP", projection);
        shader.setUniform("uTexture", texture.bind(0));

        renderer.draw(vao, ibo, shader);

        window.swapBuffers();
        
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}